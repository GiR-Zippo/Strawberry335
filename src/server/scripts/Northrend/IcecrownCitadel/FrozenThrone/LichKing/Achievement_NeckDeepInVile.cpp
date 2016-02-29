#include "Achievement_TheLichKing.h"

bool AchievementNeckDeepInVile::OnCheck(Player* /*source*/, Unit* target)
{
    if (!target)
        return false;

    return !target->GetAI()->GetData(DATA_VILE);
}