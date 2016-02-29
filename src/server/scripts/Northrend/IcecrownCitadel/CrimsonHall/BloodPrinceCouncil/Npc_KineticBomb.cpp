#include "Npc_KineticBomb.h"

void NpcKineticBombAI::Reset()
{
    _events.Reset();
    me->SetWalk(true);
    me->CastSpell(me, SPELL_UNSTABLE, true);
    me->CastSpell(me, SPELL_KINETIC_BOMB_VISUAL, true);
    me->SetReactState(REACT_PASSIVE);
    me->GetPosition(_x, _y, _groundZ);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    _events.ScheduleEvent(EVENT_BOMB_DESPAWN, 60000);
    _groundZ = me->GetMap()->GetHeight(me->GetPhaseMask(), _x, _y, _groundZ, true, 500.0f);
    DoAction(ACTION_KINETIC_BOMB_DOWN);
}

void NpcKineticBombAI::DoAction(const int32 action)
{
    if (action == SPELL_KINETIC_BOMB_EXPLOSION)
        _events.RescheduleEvent(EVENT_BOMB_DESPAWN, 1000);
    else if (action == ACTION_KINETIC_BOMB_UP)
    {
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveCharge(_x, _y, me->GetHomePosition().GetPositionZ(), me->GetSpeed(MOVE_RUN), POINT_KINETIC_BOMB_MAX_HEIGHT);
    }
    else if (action == ACTION_KINETIC_BOMB_DOWN)
    {
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveCharge(_x, _y, _groundZ, me->GetSpeed(MOVE_WALK), POINT_KINETIC_BOMB_IMPACT);
    }
}

void NpcKineticBombAI::MovementInform(uint32 type, uint32 id)
{
    if (type == POINT_MOTION_TYPE)
        if (id == POINT_KINETIC_BOMB_MAX_HEIGHT)
            DoAction(ACTION_KINETIC_BOMB_DOWN);
}

void NpcKineticBombAI::UpdateAI(const uint32 diff)
{
    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_BOMB_DESPAWN:
                me->SetVisible(false);
                _instance->SetData(DATA_KINETIC_BOMB_COUNT, _instance->GetData(DATA_KINETIC_BOMB_COUNT) - 1);
                me->DespawnOrUnsummon(5000);
                break;
            default:
                break;
        }
    }
}