/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#ifndef _BLOODFURNACE_H
#define _BLOODFURNACE_H

#define ENTRY_SEWER1                     181823
#define ENTRY_SEWER2                     181766
#define MAX_ENCOUNTER                    3


enum Data
{
    DATA_THE_MAKER                     = 1,
    DATA_BROGGOK                       = 2,
    DATA_KELIDAN_THE_BREAKER           = 3,
    DATA_BROGGOK_EVENT                 = 4
};

enum Data64
{
    DATA64_THE_MAKER                   = 1,
    DATA64_BROGGOK                     = 2,
    DATA64_BROGGOK_CONTROLLER          = 3,
    DATA64_KELIDAN_THE_MAKER           = 4,
    DATA64_FINAL_EXIT_DOOR             = 5,
    DATA64_THE_MAKER_FRONT_DOOR        = 6,
    DATA64_THE_MAKER_REAR_DOOR         = 7,
    DATA64_BROGGOK_FRONT_DOOR          = 8,
    DATA64_BROGGOK_REAR_DOOR           = 9,
    DATA64_KELIDAN_EXIT_DOOR           = 10,

    DATA64_THE_MAKER_PRISON_CELL_FR    = 11, // FR = front right
    DATA64_THE_MAKER_PRISON_CELL_BR    = 12, // BR = back right
    DATA64_THE_MAKER_PRISON_CELL_FL    = 13, // FL = front left
    DATA64_THE_MAKER_PRISON_CELL_BL    = 14, // BL = back left
    DATA64_BROGGOK_PRISON_CELL_FR      = 15,
    DATA64_BROGGOK_PRISON_CELL_BR      = 16,
    DATA64_BROGGOK_PRISON_CELL_FL      = 17,
    DATA64_BROGGOK_PRISON_CELL_BL      = 18,
    DATA64_BROGGOK_PREEVENT_LEVER      = 19
};

enum Creatures
{
    CREATURE_THE_MAKER                 = 17381,
    CREATURE_BROGGOK                   = 17380,
    CREATURE_BROGGOK_CONTROLLER        = 45000,
    CREATURE_BROGGOK_EVENT             = 17398,
    CREATURE_KELIDAN_THE_BREAKER       = 17377
};

enum GameObjects
{
    GO_FINAL_EXIT_DOOR                 = 181766,
    GO_THE_MAKER_FRONT_DOOR            = 181811,
    GO_THE_MAKER_REAR_DOOR             = 181812,
    GO_BROGGOK_FRONT_DOOR              = 181822,
    GO_BROGGOK_REAR_DOOR               = 181819,
    GO_KELIDAN_EXIT_DOOR               = 181823,

    GO_THE_MAKER_PRISON_CELL_FR        = 181813, // FR = front right
    GO_THE_MAKER_PRISON_CELL_BR        = 181814, // BR = back right
    GO_THE_MAKER_PRISON_CELL_FL        = 181816, // FL = front left
    GO_THE_MAKER_PRISON_CELL_BL        = 181815, // BL = back left
    GO_BROGGOK_PRISON_CELL_FR          = 181818,
    GO_BROGGOK_PRISON_CELL_BR          = 181821,
    GO_BROGGOK_PRISON_CELL_FL          = 181817,
    GO_BROGGOK_PRISON_CELL_BL          = 181820,
    GO_BROGGOK_PREEVENT_LEVER          = 181982
};


#endif // _BLOOD_FURNACE_H
