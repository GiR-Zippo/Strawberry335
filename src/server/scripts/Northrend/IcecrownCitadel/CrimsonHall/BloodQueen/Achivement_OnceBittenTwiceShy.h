#ifndef __ACHIEVEMENT_ONCEBITTENTWICESHY_H_
#define __ACHIEVEMENT_ONCEBITTENTWICESHY_H_

#include "BloodQueenLanaThel.h"
#include "Boss_BloodQueenLanaThel.h"

class AchievementOnceBittenTwiceShyN : public AchievementCriteriaScript
{
    public:
        AchievementOnceBittenTwiceShyN() : AchievementCriteriaScript("achievement_once_bitten_twice_shy_n") { }

    bool OnCheck(Player* source, Unit* target);
};

class AchievementOnceBittenTwiceShyV : public AchievementCriteriaScript
{
    public:
        AchievementOnceBittenTwiceShyV() : AchievementCriteriaScript("achievement_once_bitten_twice_shy_v") { }

    bool OnCheck(Player* source, Unit* target);
};

#endif