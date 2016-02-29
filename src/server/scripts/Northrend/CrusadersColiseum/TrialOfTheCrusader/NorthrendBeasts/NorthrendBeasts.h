#ifndef _NORTHREND_BEASTS_H
#define _NORTHREND_BEASTS_H

#include "../TrialCrusader.h"
#include "../Encounter.h"

enum
{
    // Misc
    FORCED_SPAWN_TIME                       = 180000,
    
    // Gormok Specific
    NPC_SNOBOLD_VASSAL                      = 34800,
    VEHICLE_GORMOK                          = 442,
    VEHICLE_PLAYER                          = 443,
    SEAT_GORMOK                             = 4,
    SEAT_PLAYER                             = 0,

    SNOBOLD_EXIT_GORMOK                     = 1,
    SNOBOLD_PLAYER_STATE                    = 2,
    SNOBOLD_PLAYER_DIED                     = 3,
    SNOBOLD_INIT                            = 4,

    // Jormungar Specific
    MODEL_UNDERGROUND                       = 11686,

    // Events
    BEASTS_INTRODUCE_INSTANCE               = 1,    // Entrypoint
    BEASTS_INTRODUCE_GORMOK                 = 2,
    BEASTS_GARROSH_MOCK                     = 3,
    BEASTS_VARIAN_MOCK                      = 4,
    BEASTS_SPAWN_GORMOK                     = 5,
    BEASTS_INTRODUCE_JORMUNGAR              = 6,    // Entrypoint
    BEASTS_SPAWN_JORMUNGAR                  = 7,
    BEASTS_INTRODUCE_ICEHOWL                = 8,    // Entrypoint
    BEASTS_SPAWN_ICEHOWL                    = 9,
    BEASTS_DONE                             = 10,   // Entrypoint
    BEASTS_FAIL                             = 11,   // Entrypoint

    // Achievements
    SPELL_WORMS_KILLED_IN_10_SECONDS        = 68523,
    UPPER_BACK_PAIN_10_PLAYER               = 11779,
    UPPER_BACK_PAIN_10_PLAYER_HEROIC        = 11802,
    UPPER_BACK_PAIN_25_PLAYER               = 11780,
    UPPER_BACK_PAIN_25_PLAYER_HEROIC        = 11801,

};

const Position NorthrendBeastLoc[] =
{
    {563.822f,  223.296f,   394.479f,   4.730f}, // SPAWNPOINT
    {563.822f,  178.659f,   394.479f,   4.730f}, // GORMOK & ICEHOWL WAYPOINT
    {563.822f,  188.659f,   394.479f,   4.730f}, // DREADSCALE WAYPOINT 1
    {582.909f,  168.698f,   394.730f,   4.730f}, // DREADSCALE WAYPOINT 2
    {546.709f,  168.698f,   394.730f,   4.730f}, // ACIDMAW SPAWNPOINT
};

class NorthrendBeasts : public Encounter
{
    public:
        NorthrendBeasts(InstanceScript *Instance) : Encounter(Instance),
            m_SnoboldCount(0),
            m_AchievementTimer(0),
            m_JormungarStarted(false),
            m_IcehowlStarted(false),
            m_JormungarCount(0)
        {
            m_EventMap.ScheduleEvent(BEASTS_INTRODUCE_INSTANCE, 100);
        }

        void HandleEvent(uint32 EventId);

        void OnStart()
        {
            m_AchievementTimer = 0;
        }

        void OnDone()
        {
            m_EventMap.ScheduleEvent(BEASTS_DONE, 100);
        }

        void OnFail()
        {
            m_EventMap.ScheduleEvent(BEASTS_FAIL, 100);
        }

        void Update(uint32 diff)
        {
            Encounter::Update(diff);

            if (m_AchievementTimer <= diff)
                 m_AchievementTimer = 0;
             else
                 m_AchievementTimer -= diff;
        }

        void OnCreatureCreate(Creature* creature)
        {
            if (creature->GetEntry() == NPC_SNOBOLD_VASSAL)
                m_SnoboldCount++;
        }

        void OnUnitDeath(Unit *unit)
        {
            switch (unit->GetEntry())
            {
                case NPC_SNOBOLD_VASSAL:
                    m_SnoboldCount--;
                    break;
                case NPC_GORMOK:
                    m_EventMap.ScheduleEvent(BEASTS_INTRODUCE_JORMUNGAR, 100);
                    break;
                case NPC_ACIDMAW:
                case NPC_DREADSCALE:
                {
                    m_JormungarCount++;
                    if (m_JormungarCount == 1)
                        m_AchievementTimer = 10000;
                    else
                    {
                        if (m_AchievementTimer > 0)
                            m_Instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_WORMS_KILLED_IN_10_SECONDS);

                        m_EventMap.ScheduleEvent(BEASTS_INTRODUCE_ICEHOWL, 100);
                    }
                    break;
                }
            }
        }

        bool CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* source, Unit const* target, uint32 miscvalue1)
        {
            switch (criteria_id)
            {
                case UPPER_BACK_PAIN_10_PLAYER:
                case UPPER_BACK_PAIN_10_PLAYER_HEROIC:
                    return m_SnoboldCount >= 2;
                case UPPER_BACK_PAIN_25_PLAYER:
                case UPPER_BACK_PAIN_25_PLAYER_HEROIC:
                    return m_SnoboldCount >= 4;
            }

            return false;
        }

    private:
        uint32 m_JormungarCount;
        uint32 m_AchievementTimer;
        uint32 m_SnoboldCount;
        bool m_JormungarStarted;
        bool m_IcehowlStarted;
};

#endif
