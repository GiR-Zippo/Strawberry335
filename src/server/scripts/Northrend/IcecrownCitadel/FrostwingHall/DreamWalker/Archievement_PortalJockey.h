#ifndef __ACHIEVEMENT_PORTALJOCKEY_H_
#define __ACHIEVEMENT_PORTALJOCKEY_H_

class AchievementPortalJockey : public AchievementCriteriaScript
{
    public:
        AchievementPortalJockey() : AchievementCriteriaScript("achievement_portal_jockey") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            return target && !target->GetAI()->GetData(MISSED_PORTALS);
        }
};

#endif