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

/* Script Data Start
SDName: Boss Krystallus
SD%Complete: 90%
SDComment: heroic spell trigger is missing
SDCategory: Ulduar/HallsOfStone
Script Data End */

#ifndef _DATA_KRYSTALLUS_H
#define _DATA_KRYSTALLUS_H

#include "../HallsOfStone.h"


#define BOUNDARY_POSITION_N              1018.0f
#define BOUNDARY_POSITION_S              955.0f
#define BOUNDARY_POSITION_W              783.0f

enum Spells
{
    SPELL_BOULDER_TOSS                 = 50843,
    H_SPELL_BOULDER_TOSS               = 59742,
    SPELL_GROUND_SPIKE                 = 59750,
    SPELL_GROUND_SLAM                  = 50827,
    SPELL_SHATTER                      = 50810,
    H_SPELL_SHATTER                    = 61546,
    SPELL_SHATTER_EFFECT               = 50811,
    H_SPELL_SHATTER_EFFECT             = 61547,
    SPELL_STONED                       = 50812,
    SPELL_STOMP                        = 48131,
    H_SPELL_STOMP                      = 59744
};

enum Yells
{
    SAY_AGGRO                          = -1599007,
    SAY_KILL                           = -1599008,
    SAY_DEATH                          = -1599009,
    SAY_SHATTER                        = -1599010
};

enum Events
{
    EVENT_SPELL_BOULDER                = 1,
    EVENT_SPELL_GROUND_SPIKE           = 2,
    EVENT_SPELL_GROUND_SLAM            = 3,
    EVENT_SPELL_SHATTER                = 4,
    EVENT_SPELL_STOMP                  = 5,
    EVENT_CHECK_IN_ROOM                = 6
};


#endif // _DATA_KRYSTALLUS_H
