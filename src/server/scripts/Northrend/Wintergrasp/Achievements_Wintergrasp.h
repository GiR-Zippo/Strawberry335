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

#ifndef _MISC_WINTERGRASP_H
#define _MISC_WINTERGRASP_H

#include "Vehicle.h"


enum WGAchievmentDidntStandAChance
{
    WG_VEHICLE_ID_TOWER_CANNON    = 244
};

class achievement_wg_didnt_stand_a_chance : public AchievementCriteriaScript
{
    public:
        achievement_wg_didnt_stand_a_chance() : AchievementCriteriaScript("achievement_wg_didnt_stand_a_chance") { }

        bool OnCheck(Player* source, Unit* target);
};


#endif // _MISC_WINTERGRASP_H
