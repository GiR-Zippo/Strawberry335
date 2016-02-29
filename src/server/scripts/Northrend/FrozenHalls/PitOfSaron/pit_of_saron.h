/*
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

#ifndef DEF_PIT_OF_SARON_H
#define DEF_PIT_OF_SARON_H

#define PoSScriptName "instance_pit_of_saron"
#define MAX_ENCOUNTER 3

enum DataTypes
{
    // Encounter states and GUIDs
    DATA_GARFROST           = 0,
    DATA_ICK                = 1,
    DATA_TYRANNUS           = 2,

    // GUIDs
    DATA_RIMEFANG           = 3,
    DATA_KRICK              = 4,
    DATA_JAINA_SYLVANAS_1   = 5,    // GUID of either Jaina or Sylvanas part 1, depending on team, as it's the same spawn.
    DATA_JAINA_SYLVANAS_2   = 6,    // GUID of either Jaina or Sylvanas part 2, depending on team, as it's the same spawn.
    DATA_TYRANNUS_EVENT     = 7,
    DATA_TEAM_IN_INSTANCE   = 8,
};

enum Yells
{
    SAY_AMBUSH_1                    = -1658050,
    SAY_AMBUSH_2                    = -1658051,
    SAY_GAUNTLET_START              = -1658052,
    SAY_TYRANNUS_INTRO_1            = -1658053,
    SAY_GORKUN_INTRO_2              = -1658054,
    SAY_TYRANNUS_INTRO_3            = -1658055,

    SAY_AGGRO                       = -1658056,
    SAY_SLAY_1                      = -1658057,
    SAY_SLAY_2                      = -1658058,
    SAY_DEATH                       = -1658059,
    SAY_MARK_RIMEFANG_1             = -1658060,
    SAY_MARK_RIMEFANG_2             = -1658061,
    SAY_DARK_MIGHT_1                = -1658062,
    SAY_DARK_MIGHT_2                = -1658063,

    SAY_GORKUN_OUTRO_1              = -1658064,
    SAY_GORKUN_OUTRO_2              = -1658065,
    SAY_JAYNA_OUTRO_3               = -1658066,
    SAY_SYLVANAS_OUTRO_3            = -1658067,
    SAY_JAYNA_OUTRO_4               = -1658068,
    SAY_SYLVANAS_OUTRO_4            = -1658069,
    SAY_JAYNA_OUTRO_5               = -1658070,

    // Krick
    SAY_KRICK_AGGRO                 = -1658010,
    SAY_KRICK_SLAY_1                = -1658011,
    SAY_KRICK_SLAY_2                = -1658012,
    SAY_KRICK_BARRAGE_1             = -1658013,
    SAY_KRICK_BARRAGE_2             = -1658014,
    SAY_KRICK_POISON_NOVA           = -1658015,
    SAY_KRICK_CHASE_1               = -1658016,
    SAY_KRICK_CHASE_2               = -1658017,
    SAY_KRICK_CHASE_3               = -1658018,

    // Ick
    SAY_ICK_POISON_NOVA             = -1658020,
    SAY_ICK_CHASE_1                 = -1658021,

    // OUTRO
    SAY_KRICK_OUTRO_1               = -1658030,
    SAY_JAYNA_OUTRO_2               = -1658031,
    SAY_SYLVANAS_OUTRO_2            = -1658032,
    SAY_KRICK_OUTRO_3               = -1658033,
    SAY_JAYNA_OUTRO_6               = -1658034,
    SAY_SYLVANAS_OUTRO_6            = -1658035,
    SAY_KRICK_OUTRO_5               = -1658036,
    SAY_TYRANNUS_OUTRO_7            = -1658037,
    SAY_KRICK_OUTRO_8               = -1658038,
    SAY_TYRANNUS_OUTRO_9            = -1658039,
    SAY_JAYNA_OUTRO_10              = -1658040,
    SAY_SYLVANAS_OUTRO_10           = -1658041,

    SAY_GARFROST_AGGRO              = 0,
    SAY_PHASE2                      = 1,
    SAY_PHASE3                      = 2,
    SAY_GARFROST_DEATH              = 3,
    SAY_SLAY                        = 4,
    SAY_THROW_SARONITE              = 5,
    SAY_CAST_DEEP_FREEZE            = 6,

    SAY_TYRANNUS_DEATH              = -1658007 // todo
};

enum CreatureIds
{
    NPC_GARFROST                                = 36494,
    NPC_KRICK                                   = 36477,
    NPC_ICK                                     = 36476,
    NPC_TYRANNUS                                = 36658,
    NPC_RIMEFANG                                = 36661,

    NPC_TYRANNUS_EVENTS                         = 36794,
    NPC_SYLVANAS_PART1                          = 36990,
    NPC_SYLVANAS_PART2                          = 38189,
    NPC_JAINA_PART1                             = 36993,
    NPC_JAINA_PART2                             = 38188,
    NPC_KILARA                                  = 37583,
    NPC_ELANDRA                                 = 37774,
    NPC_KORALEN                                 = 37779,
    NPC_KORLAEN                                 = 37582,
    NPC_CHAMPION_1_HORDE                        = 37584,
    NPC_CHAMPION_2_HORDE                        = 37587,
    NPC_CHAMPION_3_HORDE                        = 37588,
    NPC_CHAMPION_1_ALLIANCE                     = 37496,
    NPC_CHAMPION_2_ALLIANCE                     = 37497,

    NPC_HORDE_SLAVE_1                           = 36770,
    NPC_HORDE_SLAVE_2                           = 36771,
    NPC_HORDE_SLAVE_3                           = 36772,
    NPC_HORDE_SLAVE_4                           = 36773,
    NPC_ALLIANCE_SLAVE_1                        = 36764,
    NPC_ALLIANCE_SLAVE_2                        = 36765,
    NPC_ALLIANCE_SLAVE_3                        = 36766,
    NPC_ALLIANCE_SLAVE_4                        = 36767,
    NPC_FREED_SLAVE_1_ALLIANCE                  = 37575,
    NPC_FREED_SLAVE_2_ALLIANCE                  = 37572,
    NPC_FREED_SLAVE_3_ALLIANCE                  = 37576,
    NPC_FREED_SLAVE_1_HORDE                     = 37579,
    NPC_FREED_SLAVE_2_HORDE                     = 37578,
    NPC_FREED_SLAVE_3_HORDE                     = 37577,
    NPC_RESCUED_SLAVE_ALLIANCE                  = 36888,
    NPC_RESCUED_SLAVE_HORDE                     = 36889,
    NPC_MARTIN_VICTUS_1                         = 37591,
    NPC_MARTIN_VICTUS_2                         = 37580,
    NPC_GORKUN_IRONSKULL_1                      = 37581,
    NPC_GORKUN_IRONSKULL_2                      = 37592,
    NPC_SINDRAGOSA_END                          = 37755,

    NPC_FORGEMASTER_STALKER                     = 36495,
    NPC_EXPLODING_ORB                           = 36610,
    NPC_YMIRJAR_DEATHBRINGER                    = 36892,
    NPC_ICY_BLAST                               = 36731,

    NPC_ICICLE_TRIGGER                          = 32780
};

enum GameObjectIds
{
    GO_SARONITE_ROCK                            = 196485,
    GO_ICE_WALL                                 = 201885,
    GO_HALLS_OF_REFLECTION_PORTCULLIS           = 201848,
};

enum SharedFactions
{
	FACTION_FRIENDLY_FOR_ALL = 35,
};

#endif
