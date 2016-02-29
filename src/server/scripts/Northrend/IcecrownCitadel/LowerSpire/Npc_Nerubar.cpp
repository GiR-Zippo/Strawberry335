#include "Npc_Nerubar.h"

void NpcLowerSpireNerubarAI::Reset()
{
    //events.Reset();
    //if (_StartCounter = 1)
    //    _StartCounter = 60000;
    //else
    //    _StartCounter = 0;

    Position pos(*me);
    pos.m_positionZ = me->GetMap()->GetHeight(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, 500.0f)+ 2.0f;
    me->NearTeleportTo(pos);

    me->SetReactState(REACT_AGGRESSIVE);

    events.ScheduleEvent(EVENT_CAST_CRYPT_SCARABS, 5000);
    events.ScheduleEvent(EVENT_CAST_WEB, 10000);
    events.ScheduleEvent(EVENT_CAST_DARK_MENDING, urand(15000, 20000));
}

void NpcLowerSpireNerubarAI::DoAction(const int32 id)
{
    //if (id == ACTION_START_MOVE)
    //{
    //    Position pos;
    //    pos.Relocate(me);
    //    pos.m_positionZ += 5.0f;
    //    if (Creature *creature = me->SummonCreature(NPC_INVISIBLE_STALKER, pos, TEMPSUMMON_TIMED_DESPAWN, 60000))
    //    {
    //        me->CastSpell(creature, SPELL_WEBROPE);
    //        stalker = creature->GetGUID();
    //    }
    //    pos.m_positionZ = me->GetMap()->GetHeight(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, 500.0f)+17.0f;
    //    me->GetMotionMaster()->MovePoint(POINT_NPC_DOWN, pos);
    //}
}

void NpcLowerSpireNerubarAI::JustDied(Unit* killer)
{
    DoCast(killer, SPELL_SOULFEAST);
}

void NpcLowerSpireNerubarAI::MovementInform(uint32 type, uint32 id)
{
//    if (type != EFFECT_MOTION_TYPE && type != POINT_MOTION_TYPE)
//        return;
//
//    Position pos;
//    std::list<Player*> targets;
//    switch (id)
//    {
//        case POINT_NPC_DOWN:
//            me->GetMotionMaster()->MovementExpired();
//            pos.Relocate(me);
//            pos.m_positionZ = me->GetMap()->GetHeight(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true, 500.0f);
//            me->GetMotionMaster()->MoveLand(POINT_NPC_GROUND, pos);
//            break;
//        case POINT_NPC_GROUND:
//            me->GetMotionMaster()->MovementExpired();
//            if (Creature *creature = ObjectAccessor::GetCreature(*me, stalker))
//                creature->DespawnOrUnsummon();
//
//            me->SetCanFly(false);
//            me->SetDisableGravity(false);
//            me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
//            me->SetReactState(REACT_AGGRESSIVE);
//            break;
//        default:
//            break;
//    }
}

void NpcLowerSpireNerubarAI::UpdateAI(const uint32 diff)
{
    //if (_StartCounter && _StartCounter <= diff)
    //{
    //    DoAction(ACTION_START_MOVE);
    //    _StartCounter = 0;
    //}
    //else
    //    _StartCounter -= diff;

    if (!UpdateVictim())
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_CAST_CRYPT_SCARABS:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                    DoCast(target, SPELL_CRYPT_SCARABS);
                events.ScheduleEvent(EVENT_CAST_CRYPT_SCARABS, 35000);
                break;
            case EVENT_CAST_WEB:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                    DoCast(target, SPELL_WEB);
                events.ScheduleEvent(EVENT_CAST_WEB, urand(21000, 22000));
                break;
            case EVENT_CAST_DARK_MENDING:
                if (Unit* target = DoSelectLowestHpFriendly(50.0f))
                    DoCast(target, SPELL_DARK_MENDING);
                events.ScheduleEvent(EVENT_CAST_DARK_MENDING, urand(15000, 20000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}
