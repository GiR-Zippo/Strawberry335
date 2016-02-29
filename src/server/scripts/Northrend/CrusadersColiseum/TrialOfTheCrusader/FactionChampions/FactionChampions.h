#ifndef _FACTION_CHAMPIONS_H
#define _FACTION_CHAMPIONS_H

#include "../Encounter.h"
#include "../TrialCrusader.h"

enum
{
    // Events
    CHAMPIONS_INTRO                         = 1,    // Entrypoint
    CHAMPIONS_GARROSH_INTERRUPT             = 2,
    CHAMPIONS_VARIAN_INTERRUPT              = 3,
    CHAMPIONS_TIRION_GIVE_IN                = 4,
    CHAMPIONS_VARIAN_CONTINUE               = 5,
    CHAMPIONS_GARROSH_CONTINUE              = 6,
    CHAMPIONS_VARIAN_ANSWER                 = 7,
    CHAMPIONS_GARROSH_ANSWER                = 8,
    CHAMPIONS_SUMMON                        = 9,
    CHAMPIONS_START                         = 10,
    CHAMPIONS_END                           = 11,   // Entrypoint
};

enum
{
	NPC_ALLIANCE_DEATH_KNIGHT = 34461,
	NPC_ALLIANCE_DRUID_BALANCE = 34460,
	NPC_ALLIANCE_DRUID_RESTORATION = 34469,
	NPC_ALLIANCE_HUNTER = 34467,
	NPC_ALLIANCE_MAGE = 34468,
	NPC_ALLIANCE_PALADIN_HOLY = 34465,
	NPC_ALLIANCE_PALADIN_RETRIBUTION = 34471,
	NPC_ALLIANCE_PRIEST_DISCIPLINE = 34466,
	NPC_ALLIANCE_PRIEST_SHADOW = 34473,
	NPC_ALLIANCE_ROGUE = 34472,
	NPC_ALLIANCE_SHAMAN_ENHANCEMENT = 34463,
	NPC_ALLIANCE_SHAMAN_RESTORATION = 34470,
	NPC_ALLIANCE_WARLOCK = 34474,
	NPC_ALLIANCE_WARRIOR = 34475,

	NPC_HORDE_DEATH_KNIGHT = 34458,
	NPC_HORDE_DRUID_BALANCE = 34451,
	NPC_HORDE_DRUID_RESTORATION = 34459,
	NPC_HORDE_HUNTER = 34448,
	NPC_HORDE_MAGE = 34449,
	NPC_HORDE_PALADIN_HOLY = 34445,
	NPC_HORDE_PALADIN_RETRIBUTION = 34456,
	NPC_HORDE_PRIEST_DISCIPLINE = 34447,
	NPC_HORDE_PRIEST_SHADOW = 34441,
	NPC_HORDE_ROGUE = 34454,
	NPC_HORDE_SHAMAN_ENHANCEMENT = 34455,
	NPC_HORDE_SHAMAN_RESTORATION = 34444,
	NPC_HORDE_WARLOCK = 34450,
	NPC_HORDE_WARRIOR = 34453,
};

enum SharedFactions
{
	FACTION_HOSTILE_FOR_ALL		= 16,
	FACTION_FRIENDLY_FOR_ALL	= 35,
};

const Position FactionChampionLoc[]=
{
    {514.231f, 105.569f, 418.234f, 0},          //  0 - Horde Initial Pos 0
    {508.334f, 115.377f, 418.234f, 0},          //  1 - Horde Initial Pos 1
    {506.454f, 126.291f, 418.234f, 0},          //  2 - Horde Initial Pos 2
    {506.243f, 106.596f, 421.592f, 0},          //  3 - Horde Initial Pos 3
    {499.885f, 117.717f, 421.557f, 0},          //  4 - Horde Initial Pos 4

    {613.127f, 100.443f, 419.74f, 0},           //  5 - Ally Initial Pos 0
    {621.126f, 128.042f, 418.231f, 0},          //  6 - Ally Initial Pos 1
    {618.829f, 113.606f, 418.232f, 0},          //  7 - Ally Initial Pos 2
    {625.845f, 112.914f, 421.575f, 0},          //  8 - Ally Initial Pos 3
    {615.566f, 109.653f, 418.234f, 0},          //  9 - Ally Initial Pos 4

    {535.469f, 113.012f, 394.66f, 0},           // 10 - Horde Final Pos 0
    {526.417f, 137.465f, 394.749f, 0},          // 11 - Horde Final Pos 1
    {528.108f, 111.057f, 395.289f, 0},          // 12 - Horde Final Pos 2
    {519.92f, 134.285f, 395.289f, 0},           // 13 - Horde Final Pos 3
    {533.648f, 119.148f, 394.646f, 0},          // 14 - Horde Final Pos 4
    {531.399f, 125.63f, 394.708f, 0},           // 15 - Horde Final Pos 5
    {528.958f, 131.47f, 394.73f, 0},            // 16 - Horde Final Pos 6
    {526.309f, 116.667f, 394.833f, 0},          // 17 - Horde Final Pos 7
    {524.238f, 122.411f, 394.819f, 0},          // 18 - Horde Final Pos 8
    {521.901f, 128.488f, 394.832f, 0},          // 19 - Horde Final Pos 9
};

class FactionChampions : public Encounter
{
    public:
        FactionChampions(InstanceScript *Instance, uint64 CrusadersCache) :
            Encounter(Instance), m_CrusadersCache(CrusadersCache) 
        {
            m_EventMap.ScheduleEvent(CHAMPIONS_INTRO, 100);
        }

        void HandleEvent(uint32 EventId);

        void OnStart()
        {
            m_AchievementTimer = 0;
        }

        void OnDone()
        {
            m_Instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_DEFEAT_FACTION_CHAMPIONS);
            if (m_AchievementTimer > 0)
                m_Instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_CHAMPIONS_KILLED_IN_MINUTE);
            m_Instance->DoRespawnGameObject(m_CrusadersCache, 7 * DAY);
            m_EventMap.ScheduleEvent(CHAMPIONS_END, 100);
        }

        uint32 OnSpecial()
        {
            m_AchievementTimer = 60000;
            return IN_PROGRESS;
        }

        void OnFail()
        {
            m_EventMap.ScheduleEvent(BARRET_RETURN, 1000);
        }

        void Update (uint32 diff)
        {
            Encounter::Update(diff);

            if (m_AchievementTimer <= diff)
                m_AchievementTimer = 0;
            else
                m_AchievementTimer -= diff;
        }

    private:
        uint32 m_AchievementTimer;
        uint64 m_CrusadersCache;
};

#endif /* _FACTION_CHAMPIONS_H */
