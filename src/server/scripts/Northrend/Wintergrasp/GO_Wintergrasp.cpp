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

#include "GO_Wintergrasp.h"

/*typedef go_wg_vehicle_teleporter::go_wg_vehicle_teleporterAI WGVehicleTeleporterAI;


void WGVehicleTeleporterAI::UpdateAI(uint32 diff)
{
    if (_checkTimer <= diff)
    {
        if (Battlefield* wg = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
        {
            for (uint8 i = 0; i < MAX_WINTERGRASP_VEHICLES; i++)
            {
                if (Creature* vehicleCreature = go->FindNearestCreature(vehiclesList[i], 3.0f, true))
                {
                    if (!vehicleCreature->HasAura(SPELL_VEHICLE_TELEPORT) && vehicleCreature->GetCharmer()->ToPlayer()->GetTeamId() == wg->GetDefenderTeam())
                    {
                        std::list<Creature*> pCreatureList;
                        go->GetCreatureListWithEntryInGrid(pCreatureList, NPC_WORLD_TRIGGER_LARGE_AOI_NOT_IMMUNE_PC_NPC, 100.0f);
                        if(pCreatureList.empty())
                            return;

                        for (std::list<Creature*>::const_iterator itr = pCreatureList.begin(); itr != pCreatureList.end(); ++itr)
                        {
                            if ((*itr))
                            {
                                if ((*itr)->GetDistance2d(go) >= 60.0f)
                                    (*itr)->CastSpell(vehicleCreature, SPELL_VEHICLE_TELEPORT, true);
                            }
                        }
                    }
                }
            }
        }
        _checkTimer = WG_VEHICLE_TELEPORT_UPDATE;
    }
    else
        _checkTimer -= diff;
}*/