#include "AreaTrigger_PlagueWorks.h"

//////////////////////
// PlagueWorks Trap //
//////////////////////

bool AreaTriggerPlagueWorksTrap::OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
{
    if (player->isGameMaster())
        return false;

    InstanceScript* instance = player->GetInstanceScript();
    if (!instance || instance->GetBossState(DATA_PROFESSOR_PUTRICIDE) != DONE)
    {
        if (Creature* trap = player->FindNearestCreature(NPC_PUTRICIDE_TRAP, 20.0f))
        {
            trap->AI()->DoAction(ACTION_START_EVENT);
            return true;
        }
    }

    return false;
}
