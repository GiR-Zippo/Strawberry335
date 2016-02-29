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

#include "Achievements_Wintergrasp.h"


bool achievement_wg_didnt_stand_a_chance::OnCheck(Player* source, Unit* target)
{
    if (!target)
        return false;

    if (Player* victim = target->ToPlayer())
    {
        if (!victim->IsMounted())
            return false;

        if (Vehicle* vehicle = source->GetVehicle())
            if (vehicle->GetVehicleInfo()->m_ID == WG_VEHICLE_ID_TOWER_CANNON)
                return true;
    }

    return false;
}
