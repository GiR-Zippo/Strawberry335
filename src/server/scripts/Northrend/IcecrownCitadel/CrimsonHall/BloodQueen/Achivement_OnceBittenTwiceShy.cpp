#include "Achivement_OnceBittenTwiceShy.h"

bool AchievementOnceBittenTwiceShyN::OnCheck(Player* source, Unit* target)
{
    if (!target)
        return false;

    if (BossBloodQueenLanaThelAI* lanaThelAI = CAST_AI(BossBloodQueenLanaThelAI, target->GetAI()))
        return !lanaThelAI->WasVampire(source->GetGUID());
    return false;
}

bool AchievementOnceBittenTwiceShyV::OnCheck(Player* source, Unit* target)
{
    if (!target)
        return false;

    if (BossBloodQueenLanaThelAI* lanaThelAI = CAST_AI(BossBloodQueenLanaThelAI, target->GetAI()))
        return lanaThelAI->WasVampire(source->GetGUID());
    return false;
}