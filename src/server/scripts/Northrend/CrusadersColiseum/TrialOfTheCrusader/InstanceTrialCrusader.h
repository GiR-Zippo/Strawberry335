#ifndef _INSTANCE_TRIAL_CRUSADER_H
#define _INSTANCE_TRIAL_CRUSADER_H

#include "TrialCrusader.h"
#include "Encounter.h"

enum
{
    ENCOUNTER_NORTHREND_BEASTS              = 0,
    ENCOUNTER_LORD_JARAXXUS                 = 1,
    ENCOUNTER_FACTION_CHAMPIONS             = 2,
    ENCOUNTER_TWIN_VALKYR                   = 3,
    ENCOUNTER_ANUBARAK                      = 4,
    MAX_ENCOUNTERS                          = 5,

    UPDATE_STATE_UI_SHOW                    = 4390,
    UPDATE_STATE_UI_COUNT                   = 4389,

    INSTANCE_COMBAT_CHECK                   = 1,
    INSTANCE_DELAYED_ANUBARAK               = 2,
};

class InstanceTrialCrusaderMapScript : public InstanceScript
{
public:
    InstanceTrialCrusaderMapScript(Map* map) : InstanceScript(map),
        m_EncounterCurrent(ENCOUNTER_NORTHREND_BEASTS),
        m_EncounterStatus(NOT_STARTED),
        m_Encounter(NULL),
        m_Immortality(true),
        m_Insanity(false)
    {
        if (instance->IsHeroic())
            m_TrialCounter = 50;
        else
            m_TrialCounter = 0;

        m_EventMap.ScheduleEvent(INSTANCE_COMBAT_CHECK, 2000);
    }

    void Save();
    std::string GetSaveData();
    void Load(const char* strIn);

    bool IsEncounterInProgress() const;
    void OnPlayerEnter(Player* player);
    void OnCreatureCreate(Creature* creature);
    void OnGameObjectCreate(GameObject* go);
    void Update(uint32 diff);
    void OnUnitDeath(Unit *unit);

    // Instance Data
    void SetData(uint32 type, uint32 data);
    uint64 GetData64(uint32 type);
    uint32 GetData(uint32 type);

    // Achievements
    bool CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* source, Unit const* target, uint32 miscvalue1);

private:
    void OpenDoor(uint64 guid);
    void CloseDoor(uint64 guid);
    void UpdateCounter(Player *player);
    Encounter *GetNewEncounter();

    void EncounterSwitch(uint32 EncounterId);
    void CombatCheck();
    void DestroyChest();

    std::string SaveDataBuffer;

    Encounter *m_Encounter;
    uint32 m_EncounterCurrent;
    uint32 m_EncounterStatus;
    uint32 m_TrialCounter;

    std::map<uint32, uint64> m_GUIDMap;

    uint64 m_CrusadersCacheGUID;

    // Achievement stuff
    bool m_Immortality;
    bool m_Insanity;

    EventMap m_EventMap;
};

class InstanceTrialCrusader : public InstanceMapScript
{
public:
    InstanceTrialCrusader() : InstanceMapScript("instance_trial_of_the_crusader", 649) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new InstanceTrialCrusaderMapScript(map);
    }
};

#endif
