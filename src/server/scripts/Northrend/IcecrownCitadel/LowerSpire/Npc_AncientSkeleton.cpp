#include "Npc_AncientSkeleton.h"

void NpcAncientSkeletonAI::Reset()
{
    events.Reset();
}

void NpcAncientSkeletonAI::EnterCombat(Unit*)
{
//     if (Creature* nerubar = me->FindNearestCreature(NPC_NERUBAR_BROODKEEPER, 50.0f, true))
//         nerubar->AI()->DoAction(ACTION_START_MOVE);
    events.ScheduleEvent(EVENT_CAST_SHIELD_BASH, urand(5000, 10000));
}

void NpcAncientSkeletonAI::UpdateAI(const uint32 diff)
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
            case EVENT_CAST_SHIELD_BASH:
                DoCast(SPELL_SHIELD_BASH);
                events.ScheduleEvent(EVENT_CAST_SHIELD_BASH, urand(5000, 10000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}