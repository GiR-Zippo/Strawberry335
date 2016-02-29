/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 02.10.2014
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

#ifndef _INSTANCE_HALLS_OF_REFLECTION_H
#define _INSTANCE_HALLS_OF_REFLECTION_H

#include "HallsOfReflection.h"

/* Halls of Reflection encounters:
0- Falric
1- Marwyn
2- Frostworn General
3- The Lich King
*/

namespace INSTANCE_HALLSOFREFLECTION
{
    #define GET_DATA(x)             GetData(x)
    #define GET_DATA64(x)           GetData64(x)
    #define INCREASE_DATA(x, y)     if (dataMap.find(x) != dataMap.end()) dataMap[x] = dataMap.at(x) + y

    enum Spells
    {
        // Gunship
        SPELL_GUNSHIP_CANNON_FIRE   = 70017,
    };

    enum DelayedFails
    {
        DELAYED_EXECUTE             = NULL,
        DELAYED_FAIL_NORMAL         = 1,
        DELAYED_FAIL_LICH_KING      = 2
    };

    struct GunshipSpawnData
    {
        uint32 entry;
        float positionX;
        float positionY;
        float positionZ;
        float orientation;
    };

    const GunshipSpawnData GunshipCreatureSpawnData[2][5] =
    {
        {
            { NPC_BARTLETT, 43.059921f, 0.0f, 25.169128f, 3.158126f },
            { NPC_WORLD_TRIGGER, -27.024689f, 39.527699f, 0.759338f, 1.389564f  },
            { NPC_WORLD_TRIGGER, -6.473418f, 39.773136f, 0.900574f, 1.389564f   },
            { NPC_WORLD_TRIGGER, 3.992048f, 38.328308f, 0.961914f, 1.389564f    }
        },
        {
            { NPC_KORM, 53.626797f, 7.642050f, 30.153259f, 3.283201f },
            { NPC_GUNSHIP_CANNON_HORDE, -10.826968f, 34.170052f, 10.599121f, 1.519418f  },
            { NPC_GUNSHIP_CANNON_HORDE, 2.174323f, 33.170052f, 10.011963f, 1.519418f    },
            { NPC_GUNSHIP_CANNON_HORDE, 14.380422f, 32.170052f, 10.590149f, 1.519418f   },
            { NULL, NULL }
        }
    };

    const GunshipSpawnData GunshipGameObjectSpawnData[2][3] =
    {
        {
            { GO_PORTAL, 5199.254395f, 1606.665649f, 806.427734f, 0.949778f           },
            { GO_CHEST_ALLIANCE, 5185.143555f, 1617.246094f, 806.476929f, 0.376437f   },
            { GO_CHEST_ALLIANCE_H, 5185.143555f, 1617.246094f, 806.476929f, 0.376437f }
        },
        {
            { GO_PORTAL, 5223.814453f, 1568.503906f, 819.686829f, 2.747909f           },
            { GO_CHEST_HORDE, 5211.628906f, 1572.154907f, 818.978027f, 0.513451f      },
            { GO_CHEST_HORDE_H, 5211.628906f, 1572.154907f, 818.978027f, 0.513451f    }
        }
    };

    const Position GunshipCommanderCustomPosition = { 5243.403809f, 1642.011719f, 784.302185f, 0.906616f };
} // INSTANCE_HALLSOFREFLECTION

class Instance_HallsOfReflection : public InstanceMapScript
{
    public:

    Instance_HallsOfReflection() : InstanceMapScript("instance_halls_of_reflection", 668) { }

    struct Instance_HallsOfReflection_InstanceMapScript : public InstanceScript
    {
        public:

            Instance_HallsOfReflection_InstanceMapScript(Map* pMap) : InstanceScript(pMap) { };
            ~Instance_HallsOfReflection_InstanceMapScript()
            {
                if (pCurrentEncounter)
                    delete pCurrentEncounter;
            }

            typedef std::map<uint32, uint32> DataMap;
            typedef std::map<uint32, uint64> Data64Map;
            typedef std::set<uint64> CannonSet;

            void Initialize() override;
            bool IsEncounterInProgress() const override;
            bool SetBossState(uint32 /*bossId*/, EncounterState /*state*/) override;
            bool CheckRequiredBosses(uint32 /*bossId*/, Player const* /*player*/ = NULL) const override;

            void OnPlayerEnter(Player*) override;
            void OnCreatureCreate(Creature*) override;
            void OnGameObjectCreate(GameObject*) override;

            void SetData(uint32, uint32) override;
            uint32 GetData(uint32) override;
            uint64 GetData64(uint32) override;

            void DoAction(int32 action) override;
            void ProcessEvent(WorldObject* /*obj*/, uint32 eventID) override;

            std::string GetSaveData() override;
            void Load(const char*) override;

            void Update(uint32) override;

        private:

            bool arePlayersAlive();

            void prepareLichKingEvent();
            void startGunShip();

            void setupGunship();
            void finishGunship();
            void spawnAfterGunshipFinished();

            Transport* getCurrentGunship();

            bool prepareEvent;
            uint8 delayedFail;
            uint32 aliveCheckTimer;

            DataMap dataMap;
            Data64Map data64Map;
            CannonSet cannonSet;

            Encounter_HallsOfReflection* pCurrentEncounter;
    };

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new Instance_HallsOfReflection_InstanceMapScript(pMap);
    }
};


#endif // _INSTANCE_HALLS_OF_REFLECTION_H
