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

#ifndef _HALLS_OF_STONE_H
#define _HALLS_OF_STONE_H


enum Data
{
    DATA_KRYSTALLUS_EVENT               = 0,
    DATA_MAIDEN_OF_GRIEF_EVENT,
    DATA_BRANN_EVENT,
    DATA_SJONNIR_EVENT
};

enum Data64
{
    DATA_KRYSTALLUS                     = 0,
    DATA_MAIDEN_OF_GRIEF,
    DATA_SJONNIR,
    DATA_KADDRAK_FIRST,
    DATA_KADDRAK_SECOND,
    DATA_MARNAK_FIRST,
    DATA_MARNAK_SECOND,
    DATA_ABEDNEUM,
    DATA_GO_TRIBUNAL_CONSOLE,
    DATA_GO_KADDRAK,
    DATA_GO_MARNAK,
    DATA_GO_ABEDNEUM,
    DATA_GO_SKY_FLOOR,
    DATA_SJONNIR_DOOR,
    DATA_MAIDEN_DOOR,
    DATA_TRIBUNAL_CONTROLLER,
    DATA_ESCORT_BRANN,
    DATA_LIGHTNING_TRIGGER_LEFT,
    DATA_LIGHTNING_TRIGGER_RIGHT,
    DATA_BRANN_FINAL,
    DATA_TRIGGER_ENTRANCE
};

enum Creatures
{
    CREATURE_MAIDEN                     = 27975,
    CREATURE_KRYSTALLUS                 = 27977,
    CREATURE_SJONNIR                    = 27978,
    CREATURE_MARNAK                     = 30897,
    CREATURE_KADDRAK                    = 30898,
    CREATURE_ABEDNEUM                   = 30899,
    CREATURE_BRANN                      = 28070,
    CREATURE_TRIBUNAL_CONTROLLER        = 28234,
    CREATURE_LIGHTNING_TRIGGER          = 28130,
    CREATURE_SHADOW_ORB                 = 599999,
    CREATURE_BRANN_FINAL                = 28071,
    CREATURE_TRIGGER_ENTRANCE           = 28073
};

enum GameObjects
{
    GO_ABEDNEUM                         = 191669,
    GO_MARNAK                           = 191670,
    GO_KADDRAK                          = 191671,
    GO_MAIDEN_DOOR                      = 191292,
    GO_BRANN_DOOR                       = 191295,
    GO_SJONNIR_DOOR                     = 191296,
    GO_TRIBUNAL_CONSOLE                 = 193907,
    GO_TRIBUNAL_CHEST                   = 190586,
    GO_TRIBUNAL_CHEST_HERO              = 193996
};

enum Debug
{
    BOSS_KADDRAK_ACTIVE                 = 0x00000001,
    BOSS_MARNAK_ACTIVE                  = 0x00000002,
    BOSS_ABEDNEUM_ACTIVE                = 0x00000004
};


#endif // _HALLS_OF_STONE_H