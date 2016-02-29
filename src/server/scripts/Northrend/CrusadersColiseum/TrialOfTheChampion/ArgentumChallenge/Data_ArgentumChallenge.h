/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 26.08.2014
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

#ifndef _DATA_ARGENTUM_CHALLENGE_H
#define _DATA_ARGENTUM_CHALLENGE_H


enum ArgentumChallenge_Says
{
    SAY_COMMON_AGGRO                = 1,
    SAY_COMMON_KILL                 = 3,
    SAY_COMMON_DEATH                = 4,

    SAY_EADRIC_HAMMER = 2,
    SAY_EADRIC_RADIANCE_WARNING = 5,
    SAY_EADRIC_HAMMER_WARNING = -1999913,

    SAY_PALETRESS_SUMMON_MEMORY = 2,
    SAY_PALETRESS_MEMORY_DIES = 5,
    SAY_PALETRESS_NIGHTMARE_WARNING = -1999912,  // Convert to script_texts
};


#endif // _DATA_ARGENTUM_CHALLENGE_H
