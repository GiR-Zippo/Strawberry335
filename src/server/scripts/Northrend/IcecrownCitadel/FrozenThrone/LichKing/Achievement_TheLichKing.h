#ifndef __ACHIEVEMENT_THELICHKING_H_
#define __ACHIEVEMENT_THELICHKING_H_

#include "TheLichKing.h"

class AchievementBeenWaitingLongTime : public AchievementCriteriaScript
{
    public:
        AchievementBeenWaitingLongTime() : AchievementCriteriaScript("achievement_been_waiting_long_time") { }

    bool OnCheck(Player* /*source*/, Unit* target);
};

class AchievementNeckDeepInVile : public AchievementCriteriaScript
{
    public:
        AchievementNeckDeepInVile() : AchievementCriteriaScript("achievement_neck_deep_in_vile") { }

    bool OnCheck(Player* /*source*/, Unit* target);
};

#endif