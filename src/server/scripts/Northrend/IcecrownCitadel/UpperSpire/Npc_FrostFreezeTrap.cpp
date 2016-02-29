#include "Npc_FrostFreezeTrap.h"

void Npc_FrostFreezeTrapAI::DoAction(const int32 action)
{
    switch (action)
    {
        case 1000:
        case 11000:
            _events.ScheduleEvent(EVENT_ACTIVATE_TRAP, uint32(action));
            break;
        default:
            break;
    }
}

void Npc_FrostFreezeTrapAI::UpdateAI(const uint32 diff)
{
    _events.Update(diff);

    if (_events.ExecuteEvent() == EVENT_ACTIVATE_TRAP)
    {
        DoCast(me, SPELL_COLDFLAME_JETS);
        _events.ScheduleEvent(EVENT_ACTIVATE_TRAP, 22000);
    }
}
