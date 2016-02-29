#ifndef __ACHIEVEMENT_ALLYOUCANEAT_H_
#define __ACHIEVEMENT_ALLYOUCANEAT_H_

#include "Sindragosa.h"

class AchievementAllYouCanEat : public AchievementCriteriaScript
{
    public:
        AchievementAllYouCanEat() : AchievementCriteriaScript("achievement_all_you_can_eat") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (!target)
                return false;
            return target->GetAI()->GetData(DATA_MYSTIC_BUFFET_STACK) <= 5;
        }
};

#endif