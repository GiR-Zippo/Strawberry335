#include "Npc_ServantOfTheThrone.h"

void NpcServantOfTheThroneAI::Reset()
{
    events.Reset();
}

void NpcServantOfTheThroneAI::EnterCombat(Unit*)
{
//     if (Creature* nerubar = me->FindNearestCreature(NPC_NERUBAR_BROODKEEPER, 50.0f, true))
//         nerubar->AI()->DoAction(ACTION_START_MOVE);
    me->CallForHelp(10.0f);
    events.ScheduleEvent(EVENT_CAST_GLACIAL_BLAST, 1000);
}

void NpcServantOfTheThroneAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);

    while (uint32 id = events.ExecuteEvent())
    {
        switch (id)
        {
            case EVENT_CAST_GLACIAL_BLAST:
                DoCast(SPELL_GLACIAL_BLAST);
                events.ScheduleEvent(EVENT_CAST_GLACIAL_BLAST, urand(3000, 6000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}
