#include "../NorthrendBeasts.h"
#include "Vehicle.h"
#include "CreatureAIImpl.h" 
#include "ScriptedCreature.h"
#include "Creature.h"

enum
{
    //Yells
    SAY_SNOBOLLED                           = -1649000,

    //Equipment
    EQUIP_MAIN                              = 50760,
    EQUIP_OFFHAND                           = 48040,
    EQUIP_RANGED                            = 47267,
    EQUIP_DONE                              = EQUIP_NO_CHANGE,

    //Spells
    SPELL_IMPALE                            = 66331,
    SPELL_STAGGERING_STOMP                  = 67648,

    //Events
    EVENT_IMPALE                            = 1,
    EVENT_STAGGERING_STOMP                  = 2,
    EVENT_THROW                             = 3,
    EVENT_THROW2                            = 4,
};

class BossGormokAI : public ScriptedAI
{
    public:
        BossGormokAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = creature->GetInstanceScript();
        }

        void CleanupSnobolds()
        {
            for (int8 i = 0; i < 6; ++i)
                if (Unit *snobold = ObjectAccessor::FindUnit(m_Snobolds[i]))
                {
                    snobold->ExitVehicle();
                    snobold->ToCreature()->DespawnOrUnsummon();
                }
        }

        void JustDied(Unit *)
        {
            CleanupSnobolds();
        }

        void JustReachedHome()
        {
            if (m_Instance)
                m_Instance->SetData(GO_MAIN_GATE_DOOR, CLOSE);

            CleanupSnobolds();
            me->DespawnOrUnsummon();
        }

        void EnterEvadeMode()
        {
            if (m_Instance)
                m_Instance->SetData(GO_MAIN_GATE_DOOR, OPEN);
            ScriptedAI::EnterEvadeMode();
        }

        void MovementInform(uint32 type, uint32 pointId)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (pointId == 0)
            {
                m_Instance->SetData(GO_MAIN_GATE_DOOR, CLOSE);

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetInCombatWithZone();

                m_EventMap.ScheduleEvent(EVENT_IMPALE, urand(8000, 10000));
                m_EventMap.ScheduleEvent(EVENT_STAGGERING_STOMP, 15000);
                m_EventMap.ScheduleEvent(EVENT_THROW, urand(15000, 30000));

                me->CreateVehicleKit(VEHICLE_GORMOK, NPC_GORMOK);

                for (uint8 i = 0; i < SEAT_GORMOK; i++)
                {
                    if (Creature* Snobold = DoSpawnCreature(NPC_SNOBOLD_VASSAL, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
                    {
                        m_Snobolds[i] = Snobold->GetGUID();
                        Snobold->EnterVehicle(me, i);
                        Snobold->SetReactState(REACT_AGGRESSIVE);
                        Snobold->SetInCombatWithZone();
                        Snobold->GetAI()->DoAction(SNOBOLD_INIT);
                    }
                }
            }
        }

        void HandleEvent(uint32 EventId)
        {
            switch (EventId)
            {
                case EVENT_IMPALE:
                {
                    DoCastVictim(SPELL_IMPALE);
                    m_EventMap.ScheduleEvent(EVENT_IMPALE, urand(8000, 10000));
                    return;
                }
                case EVENT_STAGGERING_STOMP:
                {
                    DoCastVictim(SPELL_STAGGERING_STOMP);
                    m_EventMap.ScheduleEvent(EVENT_STAGGERING_STOMP, 150000);
                    return;
                }
                case EVENT_THROW:
                {
                    for (uint8 i = 0; i < SEAT_GORMOK; ++i)
                        if (Unit* Snobold = me->GetVehicleKit()->GetPassenger(i))
                        {
                            Snobold->ChangeSeat(SEAT_GORMOK);
                            break;
                        }

                    m_EventMap.ScheduleEvent(EVENT_THROW2, 500);
                    return;
                }
                case EVENT_THROW2:
                {
                    if (Unit* Snobold = me->GetVehicleKit()->GetPassenger(SEAT_GORMOK))
                    {
                        Snobold->GetAI()->DoAction(SNOBOLD_EXIT_GORMOK);
                        DoScriptText(SAY_SNOBOLLED, me);
                    }

                    m_EventMap.ScheduleEvent(EVENT_THROW, urand(15000, 30000));
                    return;
                }
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            m_EventMap.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 EventId = m_EventMap.ExecuteEvent())
            {
                HandleEvent(EventId);
                return;
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap m_EventMap;
        InstanceScript *m_Instance;
        uint64 m_Snobolds[6];
};

class BossGormok : public CreatureScript
{
    public:
        BossGormok() : CreatureScript("boss_gormok") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new BossGormokAI(creature);
        }
};

void AddBossGormok()
{
    new BossGormok();
}

void AddNpcFireBomb();
void AddSpellFireBomb();
void AddSpellSnobolled();
void AddNpcSnoboldVassal();

void AddEncounterGormok()
{
    AddBossGormok();
    AddNpcFireBomb();
    AddSpellFireBomb();
    AddSpellSnobolled();
    AddNpcSnoboldVassal();
}
