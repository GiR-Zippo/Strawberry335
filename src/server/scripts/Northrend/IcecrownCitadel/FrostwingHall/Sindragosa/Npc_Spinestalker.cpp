#include "Npc_Spinestalker.h"

void NpcSpinestalkerAI::InitializeAI()
{
    // Increase add count
    if (!me->isDead())
    {
        _instance->SetData(DATA_SINDRAGOSA_FROSTWYRMS, me->GetDBTableGUIDLow());  // this cannot be in Reset because reset also happens on evade
        Reset();
    }
}

void NpcSpinestalkerAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_BELLOWING_ROAR, urand(20000, 25000));
    _events.ScheduleEvent(EVENT_CLEAVE_SPINESTALKER, urand(10000, 15000));
    _events.ScheduleEvent(EVENT_TAIL_SWEEP, urand(8000, 12000));
    me->SetReactState(REACT_DEFENSIVE);

    if (!_summoned)
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);
    }
}

void NpcSpinestalkerAI::JustRespawned()
{
    ScriptedAI::JustRespawned();
    _instance->SetData(DATA_SINDRAGOSA_FROSTWYRMS, me->GetDBTableGUIDLow());  // this cannot be in Reset because reset also happens on evade
}

void NpcSpinestalkerAI::JustDied(Unit* )
{
    _events.Reset();
}

void NpcSpinestalkerAI::DoAction(const int32 action)
{
    if (action == ACTION_START_FROSTWYRM)
    {
        if (_summoned)
            return;

        _summoned = true;
        if (me->isDead())
            return;

        me->setActive(true);
        me->SetSpeed(MOVE_FLIGHT, 2.0f);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        float moveTime = me->GetExactDist(&SpinestalkerFlyPos) / (me->GetSpeed(MOVE_FLIGHT) * 0.001f);
        me->m_Events.AddEvent(new FrostwyrmLandEvent(*me, SpinestalkerLandPos), me->m_Events.CalculateTime(uint64(moveTime) + 250));
        me->SetDefaultMovementType(IDLE_MOTION_TYPE);
        me->GetMotionMaster()->MoveIdle();
        me->StopMoving();
        me->GetMotionMaster()->MovePoint(POINT_FROSTWYRM_FLY_IN, SpinestalkerFlyPos);
    }
}

void NpcSpinestalkerAI::MovementInform(uint32 type, uint32 point)
{
    if (type != EFFECT_MOTION_TYPE || point != POINT_FROSTWYRM_LAND)
        return;

    me->setActive(false);
    me->SetCanFly(false);
    me->SetDisableGravity(false);
    me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
    me->SetHomePosition(SpinestalkerLandPos);
    me->SetFacingTo(SpinestalkerLandPos.GetOrientation());
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
}

void NpcSpinestalkerAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_BELLOWING_ROAR:
                DoCast(me, SPELL_BELLOWING_ROAR);
                _events.ScheduleEvent(EVENT_BELLOWING_ROAR, urand(25000, 30000));
                break;
            case EVENT_CLEAVE_SPINESTALKER:
                DoCastVictim(SPELL_CLEAVE_SPINESTALKER);
                _events.ScheduleEvent(EVENT_CLEAVE_SPINESTALKER, urand(10000, 15000));
                break;
            case EVENT_TAIL_SWEEP:
                DoCast(me, SPELL_TAIL_SWEEP);
                _events.ScheduleEvent(EVENT_TAIL_SWEEP, urand(22000, 25000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}