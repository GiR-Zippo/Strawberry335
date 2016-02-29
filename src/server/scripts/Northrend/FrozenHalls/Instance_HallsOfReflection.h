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

    enum EncounterWaves
    {
        ENCOUNTER_WAVE_MERCENARY    = 6,
        ENCOUNTER_WAVE_FOOTMAN      = 10,
        ENCOUNTER_WAVE_RIFLEMAN     = 6,
        ENCOUNTER_WAVE_PRIEST       = 6,
        ENCOUNTER_WAVE_MAGE         = 6
    };

    enum InstanceEvents
    {
        EVENT_NEXT_WAVE             = 1,
        EVENT_START_LICH_KING       = 2,
        EVENT_WIPE_CHECK            = 3
    };

    const Position PriestSpawnPos[ENCOUNTER_WAVE_PRIEST] =
    {
        { 5277.74f, 2016.88f, 707.778f, 5.96903f },
        { 5295.88f, 2040.34f, 707.778f, 5.07891f },
        { 5320.37f, 1980.13f, 707.778f, 2.00713f },
        { 5280.51f, 1997.84f, 707.778f, 0.296706f },
        { 5302.45f, 2042.22f, 707.778f, 4.90438f },
        { 5306.57f, 1977.47f, 707.778f, 1.50098f },
    };

    const Position MageSpawnPos[ENCOUNTER_WAVE_MAGE] =
    {
        { 5312.75f, 2037.12f, 707.778f, 4.59022f },
        { 5309.58f, 2042.67f, 707.778f, 4.69494f },
        { 5275.08f, 2008.72f, 707.778f, 6.21337f },
        { 5279.65f, 2004.66f, 707.778f, 0.069813f },
        { 5275.48f, 2001.14f, 707.778f, 0.174533f },
        { 5316.7f, 2041.55f, 707.778f, 4.50295f },
    };

    const Position MercenarySpawnPos[ENCOUNTER_WAVE_MERCENARY] =
    {
        { 5302.25f, 1972.41f, 707.778f, 1.37881f },
        { 5311.03f, 1972.23f, 707.778f, 1.64061f },
        { 5277.36f, 1993.23f, 707.778f, 0.401426f },
        { 5318.7f, 2036.11f, 707.778f, 4.2237f },
        { 5335.72f, 1996.86f, 707.778f, 2.74017f },
        { 5299.43f, 1979.01f, 707.778f, 1.23918f },
    };

    const Position FootmenSpawnPos[ENCOUNTER_WAVE_FOOTMAN] =
    {
        { 5306.06f, 2037, 707.778f, 4.81711f },
        { 5344.15f, 2007.17f, 707.778f, 3.15905f },
        { 5337.83f, 2010.06f, 707.778f, 3.22886f },
        { 5343.29f, 1999.38f, 707.778f, 2.9147f },
        { 5340.84f, 1992.46f, 707.778f, 2.75762f },
        { 5325.07f, 1977.6f, 707.778f, 2.07694f },
        { 5336.6f, 2017.28f, 707.778f, 3.47321f },
        { 5313.82f, 1978.15f, 707.778f, 1.74533f },
        { 5280.63f, 2012.16f, 707.778f, 6.05629f },
        { 5322.96f, 2040.29f, 707.778f, 4.34587f },
    };

    const Position RiflemanSpawnPos[ENCOUNTER_WAVE_RIFLEMAN] =
    {
        { 5343.47f, 2015.95f, 707.778f, 3.49066f },
        { 5337.86f, 2003.4f, 707.778f, 2.98451f },
        { 5319.16f, 1974, 707.778f, 1.91986f },
        { 5299.25f, 2036, 707.778f, 5.02655f },
        { 5295.64f, 1973.76f, 707.778f, 1.18682f },
        { 5282.9f, 2019.6f, 707.778f, 5.88176f },
    };
} // INSTANCE_HALLSOFREFLECTION

class Instance_HallsOfReflection : public InstanceMapScript
{
    public:

    Instance_HallsOfReflection() : InstanceMapScript("instance_halls_of_reflection", 668) { }

    struct Instance_HallsOfReflection_InstanceMapScript : public InstanceScript
    {
        public:

            Instance_HallsOfReflection_InstanceMapScript(Map* pMap) : InstanceScript(pMap) {};

            typedef std::set<uint64> TempSummonSet;
            typedef std::map<uint32, uint32> DataMap;
            typedef std::map<uint32, uint64> Data64Map;

            void Initialize() override;

            void OnPlayerEnter(Player*) override;
            void OnCreatureCreate(Creature*) override;
            void OnGameObjectCreate(GameObject*) override;

            void SetData(uint32, uint32) override;
            uint32 GetData(uint32) override;
            uint64 GetData64(uint32) override;

            std::string GetSaveData() override;
            void Load(const char*) override;

            void Update(uint32) override;

        private:

            // Wipe has been detected. Perform cleanup and reset.
            void doWipeCheck();

            // spawn a wave on behalf of the summoner.
            void spawnWave(Creature*);
            void setFlags(TempSummon*);
            void setWaveInComabat();
            void despawnWave();

            void openDoor(uint64);
            void closeDoor(uint64);

            EventMap eventMap;

            DataMap dataMap;
            Data64Map data64Map;

            TempSummonSet tempSummonSet;

            uint64 uiFalric;
            uint64 uiMarwyn;
            uint64 uiLichKing;
            uint64 uiJainaPart1;
            uint64 uiSylvanasPart1;
            uint64 uiLider;

            uint64 uiGunship;
            uint64 uiChest;
            uint64 uiPortal;

            uint64 uiFrostmourne;
            uint64 uiFrontDoor;
            uint64 uiFrostwornDoor;
            uint64 uiArthasDoor;
            uint64 uiRunDoor;
            uint64 uiWall[4];
            uint64 uiWallID[4];
            uint64 uiCaveDoor;

            uint32 encounter[MAX_ENCOUNTER];

            uint32 uiTeamInInstance;
            uint32 uiWaveCount;
            uint32 uiIntroDone;
            uint32 uiSummons;
            uint32 uiDataPhase;

            bool waveInit;
    };

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new Instance_HallsOfReflection_InstanceMapScript(pMap);
    }
};


#endif // _INSTANCE_HALLS_OF_REFLECTION_H
