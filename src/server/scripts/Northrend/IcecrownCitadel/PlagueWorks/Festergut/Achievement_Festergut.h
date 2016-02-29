#ifndef __ACHIEVEMENT_FESTERGUT_H
#define __ACHIEVEMENT_FESTERGUT_H

#include "Festergut.h"

class AchievementFluShotShortage : public AchievementCriteriaScript
{
public:
    AchievementFluShotShortage() : AchievementCriteriaScript("achievement_flu_shot_shortage") { }

    bool OnCheck(Player* /*source*/, Unit* target);
};

#endif