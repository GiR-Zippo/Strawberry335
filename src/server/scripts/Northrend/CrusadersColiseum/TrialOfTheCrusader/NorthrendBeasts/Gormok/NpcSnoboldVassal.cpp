#include "../NorthrendBeasts.h"
#include "Vehicle.h"

enum
{
    // Spells
    SPELL_SNOBOLLED                         = 66406,
    SPELL_BATTER                            = 66408,
    SPELL_FIRE_BOMB                         = 66313,
    SPELL_HEAD_CRACK                        = 66407,

    SPELL_RISING_ANGER                      = 66636,

    EVENT_BATTER                            = 1,
    EVENT_HEAD_CRACK                        = 2,
    EVENT_FIRE_BOMB                         = 3,
    EVENT_ENTER_TARGET                      = 4,

    EVENTS_GORMOK                           = 1,
    EVENTS_PLAYER                           = 2
};

class NpcSnoboldVassalAI : public ScriptedAI
{
    public:
        NpcSnoboldVassalAI(Creature* creature) : ScriptedAI(creature), m_TargetGUID(0)
        {
            m_Instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }

        // ToDo: Only prevent DIRECT damage from the target (not damage cleaving of another target)
        void DamageTaken(Unit* DoneBy, uint32 &Damage)
        {
            if (DoneBy->GetGUID() == m_TargetGUID)
                Damage = 0;
        }

        void MovementInform(uint32 type, uint32 pointId)
        {
            if (type != EFFECT_MOTION_TYPE)
                return;
            if (pointId == 0)
                m_EventMap.ScheduleEvent(EVENT_ENTER_TARGET, 100);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case SNOBOLD_INIT:
                    SetGormokState();
                    break;
                case SNOBOLD_EXIT_GORMOK:
                    ExitGormok();
                    if (FindTarget())
                        JumpToPlayer();
                    else
                        me->DespawnOrUnsummon();
                    break;
                case SNOBOLD_PLAYER_STATE:
                    SetPlayerState();
                    break;
                case SNOBOLD_PLAYER_DIED:
                    if (Unit *gormok = Unit::GetCreature(*me, m_Instance->GetData64(NPC_GORMOK)))
                    {
                        JumpToGormok();
                        SetGormokState();
                    }
                    else if (FindTarget())
                        JumpToPlayer();
                    else
                        me->DespawnOrUnsummon();
                    break;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            m_EventMap.Update(diff);

            while (uint32 eventId = m_EventMap.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_FIRE_BOMB:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -15, true))
                            me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_FIRE_BOMB, true);
                        else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 15, true))
                            me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_FIRE_BOMB, true);

                        m_EventMap.ScheduleEvent(EVENT_FIRE_BOMB, urand(10000, 25000), EVENTS_GORMOK);
                        return;
                    }
                    case EVENT_HEAD_CRACK:
                    {
                        if (Player *target = Unit::GetPlayer(*me, m_TargetGUID))
                            me->AddAura(SPELL_HEAD_CRACK, target);
                
                        m_EventMap.ScheduleEvent(EVENT_HEAD_CRACK, 10000, EVENTS_PLAYER);
                        return;
                    }
                    case EVENT_BATTER:
                    {
                        if (Player *target = Unit::GetPlayer(*me, m_TargetGUID))
                        {
                            if (target->HasUnitState(UNIT_STATE_CASTING))
                            {
                                sLog->outError("Interrupted");
                                me->CastSpell(target, SPELL_BATTER, true);
                                m_EventMap.ScheduleEvent(EVENT_BATTER, 6000, EVENTS_PLAYER);
                            }
                            else
                                m_EventMap.ScheduleEvent(EVENT_BATTER, 200, EVENTS_PLAYER);
                        }
                        return;
                    }
                    case EVENT_ENTER_TARGET:
                    {
                        me->AddAura(SPELL_SNOBOLLED,ObjectAccessor::FindPlayer(m_TargetGUID));
                        return;
                    }
                }

            // Attack the player
            if (Unit *unit = me->GetVehicleBase())
            {
                if (unit->GetEntry() == NPC_GORMOK)
                    return;

                me->SetTarget(m_TargetGUID);
                DoMeleeAttackIfReady();
            }
        }

    private:
        EventMap m_EventMap;
        InstanceScript* m_Instance;
        uint64 m_TargetGUID;

        void SetPlayerState()
        {
            m_EventMap.ScheduleEvent(EVENT_BATTER, 5000, EVENTS_PLAYER);
            m_EventMap.ScheduleEvent(EVENT_HEAD_CRACK, 10000, EVENTS_PLAYER);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }

        void SetGormokState()
        {
            m_EventMap.ScheduleEvent(EVENT_FIRE_BOMB, urand(5000, 20000), EVENTS_GORMOK);
        }

        void JumpToGormok()
        {
            m_EventMap.CancelEventGroup(EVENTS_PLAYER);
            Unit* gormok = me->GetMap()->GetCreature(m_Instance->GetData64(NPC_GORMOK));
            if (gormok && gormok->isAlive())
                for (uint8 i = 0; i < SEAT_GORMOK; i++)
                    if (!gormok->GetVehicleKit()->GetPassenger(i))
                    {
                        m_TargetGUID = 0;
                        me->EnterVehicle(gormok, i);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        return;
                    }
        }

        void JumpToPlayer()
        {
            if (Unit *target = ObjectAccessor::FindPlayer(m_TargetGUID))
                me->GetMotionMaster()->MoveJump(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 15.0f, 15.0f);
        }

        void ExitGormok()
        {
            m_EventMap.CancelEventGroup(EVENTS_GORMOK);
            me->ExitVehicle();
            me->AddAura(SPELL_RISING_ANGER, Unit::GetCreature(*me, m_Instance->GetData64(NPC_GORMOK)));
        }

        bool FindTarget()
        {
            Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true, -SPELL_SNOBOLLED);
            if (!target)
            {
                m_TargetGUID = 0;
                return false;
            }

            m_TargetGUID = target->GetGUID();
            return true;
        }
};

class NpcSnoboldVassal : public CreatureScript
{
    public:
        NpcSnoboldVassal() : CreatureScript("mob_snobold_vassal") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new NpcSnoboldVassalAI(creature);
        }

};

void AddNpcSnoboldVassal()
{
    new NpcSnoboldVassal();
}
