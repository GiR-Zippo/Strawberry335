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
#include "Achievements_Wintergrasp.h"
#include "Npc_Wintergrasp.h"
#include "Spell_Wintergrasp.h"


void AddSC_wintergrasp()
{
    // NPCs
    new npc_wg_queue();
    new npc_wg_spirit_guide();
    new npc_wg_demolisher_engineer();

    // Achievements
    new achievement_wg_didnt_stand_a_chance();

    // Spells
    new SpellWGCannon();
    new SpellWGVehicleRanged();
    new SpellWGVehicleMelee();
    //new SpellWGCatapultPlagueSlime();
    new SpellWGCatapultFlameBreath();
    new SpellWGForceBuilding();
    new SpellWGGrabPassenger();
    new SpellWGDefenderTeleport();
    new SpellWGPortalTeleport();
    new SpellWGVehicleTeleport();
    new SpellWGDriveVehicle();
    new SpellWGRocketLauncher();
}
