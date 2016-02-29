/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 02.10.2014
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

#ifndef _HALLS_OF_REFLECTION_H
#define _HALLS_OF_REFLECTION_H

#include "Encounter_HallsOfReflection.h"


#define GET_INSTANCESCRIPT_SAFE(x)  x = me->GetInstanceScript() ? me->GetInstanceScript() : NULL
#define ALIVE_CHECK_TIMER           5 * IN_MILLISECONDS
#define ENCOUNTER_VALUE_MAX         3
#define BOSS_REQUIREMENTS           "Es gibt hier kein Weiterkommen. Euch fehlt der Boss %s um fortzufahren."

enum Data
{
    MAX_ENCOUNTER                   = 5,

    DATA_INTRO_EVENT                = 0,
    DATA_FALRIC_EVENT               = 1,
    DATA_MARWYN_EVENT               = 2,
    DATA_FROSWORN_EVENT             = 3,
    DATA_LICHKING_EVENT             = 4,

    DATA_TEAM_IN_INSTANCE           = 6,
    DATA_CURRENT_ENCOUNTER          = 7
};

enum Creatures
{
    NPC_FALRIC                      = 38112,
    NPC_MARWYN                      = 38113,
    NPC_FROSTWORN_GENERAL           = 36723,
    NPC_REFLECTION                  = 37107,
    NPC_LICH_KING_EVENT             = 36954,
    NPC_QUELDELAR                   = 37158,
    BOSS_LICH_KING                  = 37226,

    NPC_ARCHMAGE_KORELN             = 37582,
    NPC_DARK_RANGER_LORALEN         = 37779,
    NPC_UTHER                       = 37225,
    NPC_JAINA_OR_SYLVANAS           = 37221,
    NPC_JAINA_OR_SYLVANAS_O         = 36955,
    NPC_SYLVANAS                    = 37223,
    NPC_SYLVANAS_OUTRO              = 37554,
    NPC_BARTLETT                    = 37182,
    NPC_KORM                        = 37833,

    NPC_WAVE_MERCENARY              = 38177,
    NPC_WAVE_FOOTMAN                = 38173,
    NPC_WAVE_RIFLEMAN               = 38176,
    NPC_WAVE_PRIEST                 = 38175,
    NPC_WAVE_MAGE                   = 38172,
    NPC_WAVE_LOOT_DUMMY             = 37906,

    NPC_ICE_WALL                    = 37014,
    NPC_RAGING_GNOUL                = 36940,
    NPC_RISEN_WITCH_DOCTOR          = 36941,
    NPC_ABON                        = 37069,

    NPC_GUNSHIP_CANNON_HORDE        = 37593,
    NPC_WORLD_TRIGGER               = 23472
};

enum GameObjects
{
    GO_FROSTMOURNE                  = 202302,
    GO_FROSTMOURNE_ALTAR            = 202236,
    GO_FRONT_DOOR                   = 201976,
    GO_FROSTWORN_DOOR               = 197341,
    GO_ARTHAS_DOOR                  = 197342,
    GO_RUN_DOOR                     = 197343,

    GO_ICE_WALL                     = 201385,
    GO_CAVE                         = 201596,

    GO_STAIRS_SKYBREAKER            = 201709,
    GO_SKYBREAKER                   = 201598,
    GO_STAIRS_ORGRIM_HAMMER         = 202211,
    GO_ORGRIM_HAMMER                = 201599,
    GO_PORTAL                       = 202079,
    GO_CHEST_HORDE                  = 202212, //3145
    GO_CHEST_ALLIANCE               = 201710, //30357
    GO_CHEST_HORDE_H                = 202337, //3246
    GO_CHEST_ALLIANCE_H             = 202336  //3333
};

enum Achievements
{
    ACHIEV_HALLS_OF_REFLECTION_N    = 4518,
    ACHIEV_HALLS_OF_REFLECTION_H    = 4521,
    ACHIEV_NOT_RETREATING_EVENT     = 22615,
    SPELL_ACHIEV_CHECK              = 72830,
};

enum HorWorldStates
{
    WORLD_STATE_HOR                 = 4884,
    WORLD_STATE_HOR_WAVE_COUNT      = 4882
};

enum Actions
{
    INSTANCE_ACTION_ENCOUNTER_DONE  = 1,
    INSTANCE_ACTION_RESET_EVENT     = 2,
    INSTANCE_ACTION_START_GUNSHIP   = 3,
    INSTANCE_ACTION_STOP_GUNSHIP    = 4,
    INSTANCE_ACTION_RESTART_GUNSHIP = 5
};

enum Spells
{
    // Falric and Marwyn
    SPELL_BOSS_SPAWN_AURA           = 72712
};

enum InstanceEvents
{
    INSTANCE_EVENT_GUNSHIP_FIRE     = 22709,
    INSTANCE_EVENT_GUNSHIP_ARRIVAL  = 22714
};

enum EncounterEvents
{
    ENCOUNTER_EVENT_FAST_INTRO      = 12,
    ENCOUNTER_EVENT_RESTART_WAVES   = 13
};

enum EncounterActions
{
    // Falric + Marwyn Event
    ENCOUNTER_ACTION_F_M_E_START    = ENCOUNTER_PREFIX + 1,
};


enum EncounterValue
{
    ENCOUNTER_VALUE_F_M_E           = ENCOUNTER_PREFIX - ENCOUNTER_VALUE_MAX,
    ENCOUNTER_VALUE_F_M_E_FAST,
    ENCOUNTER_VALUE_F_M_E_WAVE_RESTART,
    ENCOUNTER_VALUE_LICH_KING_EVENT
};

enum SharedFactions
{
	FACTION_FRIENDLY_FOR_ALL = 35,
};

#endif // _HALLS_OF_REFLECTION_H
