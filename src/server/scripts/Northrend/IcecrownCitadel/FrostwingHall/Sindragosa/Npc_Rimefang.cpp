#include "Npc_Rimefang.h"

void NpcRimefangAI::InitializeAI()
{
    // Increase add count
    if (!me->isDead())
    {
        _instance->SetData(DATA_SINDRAGOSA_FROSTWYRMS, me->GetDBTableGUIDLow());  // this cannot be in Reset because reset also happens on evade
        Reset();
    }
}

void NpcRimefangAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_FROST_BREATH_RIMEFANG, urand(12000, 15000));
    _events.ScheduleEvent(EVENT_ICY_BLAST, urand(30000, 35000));
    me->SetReactState(REACT_DEFENSIVE);
    _icyBlastCounter = 0;

    if (!_summoned)
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);
    }
}

void NpcRimefangAI::JustRespawned()
{
    ScriptedAI::JustRespawned();
    _instance->SetData(DATA_SINDRAGOSA_FROSTWYRMS, me->GetDBTableGUIDLow());  // this cannot be in Reset because reset also happens on evade
}

void NpcRimefangAI::JustDied(Unit* )
{
    _events.Reset();
}

void NpcRimefangAI::DoAction(const int32 action)
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
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        float moveTime = me->GetExactDist(&RimefangFlyPos) / (me->GetSpeed(MOVE_FLIGHT) * 0.001f);
        me->m_Events.AddEvent(new FrostwyrmLandEvent(*me, RimefangLandPos), me->m_Events.CalculateTime(uint64(moveTime) + 250));
        me->SetDefaultMovementType(IDLE_MOTION_TYPE);
        me->GetMotionMaster()->MoveIdle();
        me->StopMoving();
        me->GetMotionMaster()->MovePoint(POINT_FROSTWYRM_FLY_IN, RimefangFlyPos);
    }
}

void NpcRimefangAI::MovementInform(uint32 type, uint32 point)
{
    if (type != EFFECT_MOTION_TYPE || point != POINT_FROSTWYRM_LAND)
        return;

    me->setActive(false);
    me->SetCanFly(false);
    me->SetDisableGravity(false);
    me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
    me->SetHomePosition(RimefangLandPos);
    me->SetFacingTo(RimefangLandPos.GetOrientation());
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
}

void NpcRimefangAI::EnterCombat(Unit* )
{
    DoCast(me, SPELL_FROST_AURA_RIMEFANG, true);
}

void NpcRimefangAI::UpdateAI(const uint32 diff)
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
            case EVENT_FROST_BREATH_RIMEFANG:
                DoCast(me, SPELL_FROST_BREATH);
                _events.ScheduleEvent(EVENT_FROST_BREATH_RIMEFANG, urand(35000, 40000));
                break;
            case EVENT_ICY_BLAST:
            {
                _icyBlastCounter = RAID_MODE<uint8>(5, 7, 6, 8);
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                me->SetCanFly(true);
                me->GetMotionMaster()->MovePoint(POINT_FROSTWYRM_FLY_IN, RimefangFlyPos);
                float moveTime = me->GetExactDist(&RimefangFlyPos)/(me->GetSpeed(MOVE_FLIGHT)*0.001f);
                _events.ScheduleEvent(EVENT_ICY_BLAST, uint64(moveTime) + urand(60000, 70000));
                _events.ScheduleEvent(EVENT_ICY_BLAST_CAST, uint64(moveTime) + 250);
                break;
            }
            case EVENT_ICY_BLAST_CAST:
                if (--_icyBlastCounter)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    {
                        me->SetFacingToObject(target);
                        DoCast(target, SPELL_ICY_BLAST);
                    }
                    _events.ScheduleEvent(EVENT_ICY_BLAST_CAST, 3000);
                }
                else if (Unit* victim = me->SelectVictim())
                {
                    me->SetReactState(REACT_DEFENSIVE);
                    AttackStart(victim);
                    me->SetCanFly(false);
                }
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}