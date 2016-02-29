#ifndef __ACHIEVEMENT_DEATHBRINGER_SAURFANG_H
#define __ACHIEVEMENT_DEATHBRINGER_SAURFANG_H

#include "DeathbringerSaurfang.h"

class AchievementIveGoneAndMadeAMess : public AchievementCriteriaScript
{
public:
    AchievementIveGoneAndMadeAMess() : AchievementCriteriaScript("achievement_ive_gone_and_made_a_mess") { }

    bool OnCheck(Player* /*source*/, Unit* target);
};

#endif