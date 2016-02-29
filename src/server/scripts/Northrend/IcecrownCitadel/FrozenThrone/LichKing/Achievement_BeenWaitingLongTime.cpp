#include "Achievement_TheLichKing.h"

bool AchievementBeenWaitingLongTime::OnCheck(Player* /*source*/, Unit* target)
{
    if (!target)
        return false;

    return target->GetAI()->GetData(DATA_PLAGUE_STACK) >= 30;
}