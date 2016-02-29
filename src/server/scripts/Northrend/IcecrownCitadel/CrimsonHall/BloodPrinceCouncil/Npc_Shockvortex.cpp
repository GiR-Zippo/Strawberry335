#include "Npc_Shockvortex.h"

void NpcShockvortexAI::Reset()
{
    me->SetReactState(REACT_DEFENSIVE);
    me->CastSpell(me, SPELL_SHOCK_VORTEX_DUMMY, true);
    _events.ScheduleEvent(EVENT_SHOCK_VORTEX_ACTIVATE, 6000);
}

void NpcShockvortexAI::EnterCombat(Unit* who)
{

}

void NpcShockvortexAI::UpdateAI(const uint32 diff)
{
    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SHOCK_VORTEX_ACTIVATE:
                me->CastSpell(me, SPELL_SHOCK_VORTEX_PERIODIC, true);
                break;
            default:
                break;
        }
    }
}