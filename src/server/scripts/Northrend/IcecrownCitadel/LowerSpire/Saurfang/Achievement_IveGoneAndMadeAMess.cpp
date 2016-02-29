#include "Achievement_DeathbringerSaurfang.h"

//////////////////////////////
// Ive Gone And Made A Mess //
//////////////////////////////

bool AchievementIveGoneAndMadeAMess::OnCheck(Player* /*source*/, Unit* target)
{
    if (target)
        if (Creature* saurfang = target->ToCreature())
            if (saurfang->AI()->GetData(DATA_MADE_A_MESS))
                return true;

    return false;
}