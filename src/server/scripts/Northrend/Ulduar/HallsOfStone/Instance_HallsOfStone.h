/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _INSTANCE_HALLSOFSTONE_H
#define _INSTANCE_HALLSOFSTONE_H

#include "HallsOfStone.h"

#define MAX_ENCOUNTER 4

/* Halls of Stone encounters:
0- Krystallus
1- Maiden of Grief
2- Escort Event
3- Sjonnir The Ironshaper
*/


class instance_halls_of_stone : public InstanceMapScript
{
public:
    instance_halls_of_stone() : InstanceMapScript("instance_halls_of_stone", 599) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_halls_of_stone_InstanceMapScript(map);
    }

    struct instance_halls_of_stone_InstanceMapScript : public InstanceScript
    {
        instance_halls_of_stone_InstanceMapScript(Map* map) : InstanceScript(map) {}

        uint64 uiMaidenOfGrief;
        uint64 uiKrystallus;
        uint64 uiSjonnir;

        uint64 uiLightningTrigger[2];
        uint64 uiKaddrak[2];
        uint64 uiMarnak[2];
        uint64 uiAbedneum;
        uint64 uiBrann;
        uint64 uiTribunalController;
        uint64 uiBrannFinal;
        uint64 uiTriggerEntrance;

        uint64 uiMaidenOfGriefDoor;
        uint64 uiSjonnirDoor;
        uint64 uiBrannDoor;
        uint64 uiTribunalConsole;
        uint64 uiTribunalChest;
        uint64 uiTribunalSkyFloor;
        uint64 uiKaddrakGo;
        uint64 uiAbedneumGo;
        uint64 uiMarnakGo;

        uint32 m_auiEncounter[MAX_ENCOUNTER];

        std::string str_data;

        void Initialize();

        void OnCreatureCreate(Creature* creature);
        void OnGameObjectCreate(GameObject* go);

        void SetData(uint32 type, uint32 data);
        uint32 GetData(uint32 type);
        uint64 GetData64(uint32 identifier);

        std::string GetSaveData();
        void Load(const char* in);
    };
};

#endif // _INSTANCE_HALLSOFSTONE_H
