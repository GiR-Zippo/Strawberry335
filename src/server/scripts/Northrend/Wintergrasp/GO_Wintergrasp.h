/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 29.07.2014
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

#ifndef _GO_WINTERGRASP_H
#define _GO_WINTERGRASP_H

/*#include "BattlefieldMgr.h"
#include "Main_BattlefieldWG.h"
#include "Battlefield.h"
#include "GameObjectAI.h"

#define WG_VEHICLE_TELEPORT_UPDATE  2500


enum Spells
{
    SPELL_VEHICLE_TELEPORT                        = 49759,
};

enum CreatureIds
{
    NPC_WORLD_TRIGGER_LARGE_AOI_NOT_IMMUNE_PC_NPC = 23472,
};

uint8 const MAX_WINTERGRASP_VEHICLES              = 4;

uint32 const vehiclesList[MAX_WINTERGRASP_VEHICLES] =
{
    NPC_WINTERGRASP_CATAPULT,
    NPC_WINTERGRASP_DEMOLISHER,
    NPC_WINTERGRASP_SIEGE_ENGINE_ALLIANCE,
    NPC_WINTERGRASP_SIEGE_ENGINE_HORDE
};

/** Vehicle teleport inside wintergrasp fortress *//*
class go_wg_vehicle_teleporter : public GameObjectScript
{
    public:

        go_wg_vehicle_teleporter() : GameObjectScript("go_wg_vehicle_teleporter") { }

        struct go_wg_vehicle_teleporterAI : public GameObjectAI
        {
                    go_wg_vehicle_teleporterAI(GameObject* gameObject) : GameObjectAI(gameObject), _checkTimer(WG_VEHICLE_TELEPORT_UPDATE) { }

                void UpdateAI(uint32 diff);

            private:
                uint32 _checkTimer;
        };

        GameObjectAI* GetAI(GameObject* go) const
        {
            return new go_wg_vehicle_teleporterAI(go);
        }
};*/


#endif // _GO_WINTERGRASP_H
