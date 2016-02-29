#include "Achievement_Festergut.h"

///////////////////////
// Flu Shot Shortage //
///////////////////////

bool AchievementFluShotShortage::OnCheck(Player* /*source*/, Unit* target)
{
    if (target && target->GetTypeId() == TYPEID_UNIT)
        return target->ToCreature()->AI()->GetData(DATA_INOCULATED_STACK) < 3;

    return false;
}
