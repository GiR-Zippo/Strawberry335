#include "Npc_BrokenFrostmourne.h"

void NpcBrokenFrostmourneAI::Reset()
{
    _events.Reset();
}

void NpcBrokenFrostmourneAI::IsSummonedBy ( Unit* )
{
    _events.SetPhase(PHASE_OUTRO);
    _events.ScheduleEvent(EVENT_OUTRO_KNOCK_BACK, 3000, 0, PHASE_OUTRO);
}

void NpcBrokenFrostmourneAI::DoAction ( const int32 action )
{
    if (action == ACTION_SUMMON_TERENAS)
        _events.ScheduleEvent(EVENT_OUTRO_SUMMON_TERENAS, 6000, 0, PHASE_OUTRO);
}

void NpcBrokenFrostmourneAI::EnterEvadeMode()
{
}

void NpcBrokenFrostmourneAI::UpdateAI ( const uint32 diff )
{
    UpdateVictim();

    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_OUTRO_KNOCK_BACK:
                DoCastAOE(SPELL_BROKEN_FROSTMOURNE_KNOCK);
                break;
            case EVENT_OUTRO_SUMMON_TERENAS:
                DoCastAOE(SPELL_SUMMON_TERENAS);
                break;
            default:
                break;
        }
    }

    // no melee attacks
}