/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#ifndef _INSTANCE_TRIAL_OF_THE_CHAMPION_H
#define _INSTANCE_TRIAL_OF_THE_CHAMPION_H

#include "TrialOfTheChampion.h"


#define MAX_ENCOUNTER       3
#define ALIVE_CHECK_TIMER   5 * IN_MILLISECONDS

class Encounter_TrialOfTheChampion;

class Instance_TrialOfTheChampion : public InstanceMapScript
{
    public:

        Instance_TrialOfTheChampion() : InstanceMapScript("instance_trial_of_the_champion", 650) { }

        struct Instance_TrialOfTheChampion_InstanceMapScript : public InstanceScript
        {
            public:

                Instance_TrialOfTheChampion_InstanceMapScript(Map* pMap)
                    : InstanceScript(pMap), pCurrentEncounter(NULL) {}
                ~Instance_TrialOfTheChampion_InstanceMapScript()
                {
                    if (pCurrentEncounter)
                        delete pCurrentEncounter;
                }

                void Initialize() override;
                bool IsEncounterInProgress() const override;

                void OnCreatureCreate(Creature* pCreature) override;
                void OnGameObjectCreate(GameObject* pObject) override;

                void SetData(uint32 type, uint32 data) override;
                uint32 GetData(uint32 data) override;
                uint64 GetData64(uint32 data) override;

                void DoAction(int32 action) override;

                bool CheckAchievementCriteriaMeet(uint32 criteriaId, Player const* /*source*/, Unit const* /*target*/, uint32 /*miscvalue1*/) override;

                std::string GetSaveData() override;
                void Load(const char* in) override;

                void Update(uint32 diff) override;

            private:

                bool arePlayersAlive();
                uint32 getRelatedCreatureEntry(uint32 criteria_id);

                uint32 encounter[MAX_ENCOUNTER];
                uint32 grandChampionEntry[3];
                uint32 memoryEntry;

                uint32 aliveCheckTimer;

                std::map<uint32, uint64> data64Map;
                std::vector<uint64> vehicleList;

                uint32 teamInInstance;
                bool achievementHadWorse;
                bool announcerPositionUpdate;

                Encounter_TrialOfTheChampion* pCurrentEncounter;
        };

        InstanceScript* GetInstanceScript(InstanceMap* pMap) const
        {
            return new Instance_TrialOfTheChampion_InstanceMapScript(pMap);
        }
};


#endif // _INSTANCE_TRIAL_OF_THE_CHAMPION_H
