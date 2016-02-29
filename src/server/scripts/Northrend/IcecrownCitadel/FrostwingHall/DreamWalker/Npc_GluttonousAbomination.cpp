#include "Npc_GluttonousAbomination.h"

void NpcGluttonousAbominationAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_GUT_SPRAY, urand(10000, 13000));
}

void NpcGluttonousAbominationAI::JustDied(Unit* )
{
    DoCast(me, SPELL_ROT_WORM_SPAWNER, true);
}

void NpcGluttonousAbominationAI::UpdateAI(const uint32 diff)
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
            case EVENT_GUT_SPRAY:
                DoCast(me, SPELL_GUT_SPRAY);
                _events.ScheduleEvent(EVENT_GUT_SPRAY, urand(10000, 13000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}