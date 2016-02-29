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

#ifndef _DATA_BATTLEFIELDWG_H
#define _DATA_BATTLEFIELDWG_H


#define WINTERGRASP_ATTACKER_TOWER_MAX      3
#define WINTERGRASP_DEFENDER_TOWER_MAX      4

enum WintergraspVehicles
{
    BATTLEFIELD_WG_SIEGE_ENGINE_ALLIANCE               = 28312,
    BATTLEFIELD_WG_SIEGE_ENGINE_CANNON_ALLIANCE        = 28319,
    BATTLEFIELD_WG_SIEGE_ENGINE_HORDE                  = 32627,
    BATTLEFIELD_WG_SIEGE_ENGINE_CANNON_HORDE           = 32629,
    BATTLEFIELD_WG_CATAPULT                            = 27881,
    BATTLEFIELD_WG_DEMOLISHER                          = 28094,
    BATTLEFIELD_WG_TOWER_CANNON                        = 28366
};

enum WintergraspSpells
{
    // Wartime auras
    SPELL_RECRUIT                                      = 37795,
    SPELL_CORPORAL                                     = 33280,
    SPELL_LIEUTENANT                                   = 55629,
    SPELL_TENACITY                                     = 58549,
    SPELL_TENACITY_VEHICLE                             = 59911,
    SPELL_TOWER_CONTROL                                = 62064,
    SPELL_GREAT_HONOR                                  = 58555,
    SPELL_GREATER_HONOR                                = 58556,
    SPELL_GREATEST_HONOR                               = 58557,
    SPELL_ALLIANCE_FLAG                                = 14268,
    SPELL_HORDE_FLAG                                   = 14267,
    SPELL_GRAB_PASSENGER                               = 61178,

    // Reward spells
    SPELL_VICTORY_REWARD                               = 56902,
    SPELL_DEFEAT_REWARD                                = 58494,
    SPELL_DAMAGED_TOWER                                = 59135,
    SPELL_DESTROYED_TOWER                              = 59136,
    SPELL_DAMAGED_BUILDING                             = 59201,
    SPELL_INTACT_BUILDING                              = 59203,

    SPELL_TELEPORT_BRIDGE                              = 59096,
    SPELL_TELEPORT_FORTRESS                            = 60035,
    SPELL_TELEPORT_CAMP_HORDE                          = 58632,
    SPELL_TELEPORT_CAMP_ALLIANCE                       = 58633,
    SPELL_TELEPORT_DALARAN_TO_WG                       = 49759,

    SPELL_TELEPORT_DALARAN                             = 53360,
    SPELL_VICTORY_AURA                                 = 60044,

    // Other spells
    SPELL_WINTERGRASP_WATER                            = 36444,
    SPELL_ESSENCE_OF_WINTERGRASP                       = 58045,
    SPELL_WINTERGRASP_RESTRICTED_FLIGHT_AREA           = 58730,
    SPELL_WINTERGRASP_SPECTRAL_GRYPHON                 = 55164,
    SPELL_WINTERGRASP_VEHICLE_TELEPORT                 = 49759,
    SPELL_ARGENTUM_TOURNAMENT_ARMISTICE                = 64373,

    // Phasing spells
    SPELL_HORDE_CONTROLS_FACTORY_PHASE_SHIFT           = 56618, // ADDS PHASE 16
    SPELL_ALLIANCE_CONTROLS_FACTORY_PHASE_SHIFT        = 56617, // ADDS PHASE 32

    SPELL_HORDE_CONTROL_PHASE_SHIFT                    = 55773, // ADDS PHASE 64
    SPELL_ALLIANCE_CONTROL_PHASE_SHIFT                 = 55774  // ADDS PHASE 128
};

enum WintergraspMiscData
{
    BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF,
    BATTLEFIELD_WG_DATA_BROKEN_TOWER_DEF,
    BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT,
    BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT,
    BATTLEFIELD_WG_DATA_MAX_VEHICLE_A,
    BATTLEFIELD_WG_DATA_MAX_VEHICLE_H,
    BATTLEFIELD_WG_DATA_VEHICLE_A,
    BATTLEFIELD_WG_DATA_VEHICLE_H,
    BATTLEFIELD_WG_DATA_MAX
};

enum WintergraspAchievements
{
    BATTLEFIELD_WG_ACHIEVEMENT_WIN_WG                   = 1717,
    BATTLEFIELD_WG_ACHIEVEMENT_WIN_WG_100               = 1718, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_WG_GNOMESLAUGHTER        = 1723, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_WG_TOWER_DESTROY         = 1727,
    BATTLEFIELD_WG_ACHIEVEMENT_DESTRUCTION_DERBY_A      = 1737, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_WG_TOWER_CANNON_KILL     = 1751, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_WG_MASTER_A              = 1752, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_WIN_WG_TIMER_10          = 1755,
    BATTLEFIELD_WG_ACHIEVEMENT_STONE_KEEPER_50          = 2085, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_STONE_KEEPER_100         = 2086, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_STONE_KEEPER_250         = 2087, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_STONE_KEEPER_500         = 2088, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_STONE_KEEPER_1000        = 2089, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_WG_RANGER                = 2199, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_DESTRUCTION_DERBY_H      = 2476, /** @todo: Has to be implemented */
    BATTLEFIELD_WG_ACHIEVEMENT_WG_MASTER_H              = 2776  /** @todo: Has to be implemented */
};

enum WintergraspNpcs
{
    BATTLEFIELD_WG_NPC_GUARD_H                          = 30739,
    BATTLEFIELD_WG_NPC_GUARD_A                          = 30740,

    BATTLEFIELD_WG_NPC_VIERON_BLAZEFEATHER              = 31102,
    BATTLEFIELD_WG_NPC_STONE_GUARD_MUKAR                = 32296, // <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_HOODOO_MASTER_FU_JIN             = 31101, // <MASTER HEXXER>
    BATTLEFIELD_WG_NPC_CHAMPION_ROS_SLAI                = 39173, // <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_COMMANDER_DARDOSH                = 31091,
    BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_KILRATH         = 31151,
    BATTLEFIELD_WG_NPC_SIEGESMITH_STRONGHOOF            = 31106,
    BATTLEFIELD_WG_NPC_PRIMALIST_MULFORT                = 31053,
    BATTLEFIELD_WG_NPC_LIEUTENANT_MURP                  = 31107,

    BATTLEFIELD_WG_NPC_BOWYER_RANDOLPH                  = 31052,
    BATTLEFIELD_WG_NPC_KNIGHT_DAMERON                   = 32294, // <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_SORCERESS_KAYLANA                = 31051, // <ENCHANTRESS>
    BATTLEFIELD_WG_NPC_MARSHAL_MAGRUDER                 = 39172, // <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_COMMANDER_ZANNETH                = 31036,
    BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_AHBRAMIS        = 31153,
    BATTLEFIELD_WG_NPC_SIEGE_MASTER_STOUTHANDLE         = 31108,
    BATTLEFIELD_WG_NPC_ANCHORITE_TESSA                  = 31054,
    BATTLEFIELD_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO      = 31109,

    BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER         = 30400, // Horde mechanic engineer for Wintergrasp
    BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER        = 30499, // Alliance mechanic engineer for Wintergrasp
    BATTLEFIELD_WG_NPC_CONTROL_ARMS                     = 27852,
    BATTLEFIELD_WG_NPC_DISCOVER_UNIT                    = 27869,

    BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER                  = 30560
};

enum WintergraspCommonNpcs
{
    NPC_FLIGHT_MASTER_ALLIANCE                          = 30869,
    NPC_FLIGHT_MASTER_HORDE                             = 30870,
    NPC_SPIRIT_KEEPER                                   = 6491,  // Universal Spirit Keeper - hidden while war
    NPC_TAUNKA_SPIRIT_GUIDE                             = 31841, // Horde spirit guide for Wintergrasp
    NPC_DWARVEN_SPIRIT_GUIDE                            = 31842, // Alliance spirit guide for Wintergrasp
    NPC_WORLD_TRIGGER_LARGE_AOI_NOT_IMMUNE_PC_NPC       = 23472
};

enum WintergraspItems
{
    BATTLEFIELD_WG_ROCKET_LAUNCHER              = 42986
};

// Only quests who needs to be handled are stored here
enum WintergraspQuestType
{
    BATTLEFIELD_WG_QUEST_VICTORY_IN_WINTERGRASP = 0,
    BATTLEFIELD_WG_QUEST_DEFEND_THE_SIEGE       = 1,
    BATTLEFIELD_WG_QUEST_TOPPLING_THE_TOWERS    = 2,
    BATTLEFIELD_WG_QUEST_KILL_THEM_ALL          = 3,
    BATTLEFIELD_WG_QUEST_KILL_THEM_ALL_BASE     = 4,
    BATTLEFIELD_WG_QUEST_MAX
};

uint32 const WintergraspQuest[2][BATTLEFIELD_WG_QUEST_MAX] =
{
    { 13181, 13222, 13538, 13177, 13179 },
    { 13183, 13223, 13539, 13178, 13180 }
};

uint32 const WintergraspQuestKillCredit[2][BATTLEFIELD_WG_QUEST_MAX] =
{
    { 0, 31284, 35074, 39019, 39019 },
    { 0, 31284, 35074, 31086, 31086 }
};

// Areas which should be handled if player has quest
enum WintergraspArea
{
    BATTLEFIELD_WG_AREA_GLACING_FALLS       = 4585,
    BATTLEFIELD_WG_AREA_FOREST_OF_SHADOWS   = 4587,
    BATTLEFIELD_WG_AREA_CAULDRON_OF_FLAMES  = 4584,
    BATTLEFIELD_WG_AREA_STEPPE_OF_LIFE      = 4590
};

enum WintergraspAreaQuestType
{
    BATTLEFIELD_WG_AREAQUEST_GLACING_FALLS      = 0,
    BATTLEFIELD_WG_AREAQUEST_FOREST_OF_SHADOWS  = 1,
    BATTLEFIELD_WG_AREAQUEST_CAULDRON_OF_FLAMES = 2,
    BATTLEFIELD_WG_AREAQUEST_STEPPE_OF_LIFE     = 3,
    BATTLEFIELD_WG_AREAQUEST_MAX
};

uint32 const WintergraspAreaQuestLootID[2][BATTLEFIELD_WG_AREAQUEST_MAX] =
{
    { 40000, 40002, 40003, 40004 },
    { 40001, 40002, 40003, 40005 }
};

uint32 const VehNumWorldState[]                        = { 3680, 3490 };
uint32 const MaxVehNumWorldState[]                     = { 3681, 3491 };
uint32 const ClockWorldState[]                         = { 3781, 4354 };
uint32 const WintergraspFaction[]                      = { 1732, 1735, 35 };

uint8 const WG_MAX_WORKSHOP                            = 6;
uint8 const WG_CAPTURABLE_WORKSHOP                     = 4;

enum WintergraspGameObjectBuildingType
{
    BATTLEFIELD_WG_OBJECTTYPE_DOOR,
    BATTLEFIELD_WG_OBJECTTYPE_TITANRELIC,
    BATTLEFIELD_WG_OBJECTTYPE_WALL,
    BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST,
    BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER,
    BATTLEFIELD_WG_OBJECTTYPE_TOWER
};

enum WintergraspGameObjectState
{
    BATTLEFIELD_WG_OBJECTSTATE_NONE,
    BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_INTACT,
    BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DAMAGE,
    BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DESTROY,
    BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT,
    BATTLEFIELD_WG_OBJECTSTATE_HORDE_DAMAGE,
    BATTLEFIELD_WG_OBJECTSTATE_HORDE_DESTROY,
    BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT,
    BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE,
    BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY
};

enum WintergraspWorkshopIds
{
    BATTLEFIELD_WG_WORKSHOP_NE,
    BATTLEFIELD_WG_WORKSHOP_NW,
    BATTLEFIELD_WG_WORKSHOP_SE,
    BATTLEFIELD_WG_WORKSHOP_SW,
    BATTLEFIELD_WG_WORKSHOP_KEEP_WEST,
    BATTLEFIELD_WG_WORKSHOP_KEEP_EAST
};

enum WintergraspTeamControl
{
    BATTLEFIELD_WG_TEAM_ALLIANCE,
    BATTLEFIELD_WG_TEAM_HORDE,
    BATTLEFIELD_WG_TEAM_NEUTRAL
};

enum WintergraspGameObject
{
    BATTLEFIELD_WG_GO_FACTORY_BANNER_NE                = 190475,
    BATTLEFIELD_WG_GO_FACTORY_BANNER_NW                = 190487,
    BATTLEFIELD_WG_GO_FACTORY_BANNER_SE                = 194959,
    BATTLEFIELD_WG_GO_FACTORY_BANNER_SW                = 194962,

    BATTLEFIELD_WG_GO_WORKSHOP_BASE_W                  = 192028,
    BATTLEFIELD_WG_GO_WORKSHOP_BASE_E                  = 192029,
    BATTLEFIELD_WG_GO_WORKSHOP_NW                      = 192030,
    BATTLEFIELD_WG_GO_WORKSHOP_NE                      = 192031,
    BATTLEFIELD_WG_GO_WORKSHOP_SW                      = 192032,
    BATTLEFIELD_WG_GO_WORKSHOP_SE                      = 192033,

    BATTLEFIELD_WG_GO_TITAN_S_RELIC                    = 192829,

    BATTLEFIELD_WG_GO_FORTRESS_TOWER_1                 = 190221,
    BATTLEFIELD_WG_GO_FORTRESS_TOWER_2                 = 190373,
    BATTLEFIELD_WG_GO_FORTRESS_TOWER_3                 = 190377,
    BATTLEFIELD_WG_GO_FORTRESS_TOWER_4                 = 190378,

    BATTLEFIELD_WG_GO_SHADOWSIGHT_TOWER                = 190356,
    BATTLEFIELD_WG_GO_WINTER_S_EDGE_TOWER              = 190357,
    BATTLEFIELD_WG_GO_FLAMEWATCH_TOWER                 = 190358,

    BATTLEFIELD_WG_GO_FORTRESS_GATE                    = 190375,
    BATTLEFIELD_WG_GO_VAULT_GATE                       = 191810,

    BATTLEFIELD_WG_GO_KEEP_COLLISION_WALL              = 194323,

    BATTLEFIELD_WG_GO_BANNER_STAND_H                   = 192269,
    BATTLEFIELD_WG_GO_BANNER_STAND_A                   = 192278,
    BATTLEFIELD_WG_GO_BANNER_FLAG_H                    = 192488,
    BATTLEFIELD_WG_GO_BANNER_FLAG_A                    = 192501,
    BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H                = 192374,
    BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A                = 192326,

    BATTLEFIELD_WG_GO_WINTERGRASP_PORTAL               = 193772
};

struct WintergraspObjectPositionData
{
    float x;
    float y;
    float z;
    float o;
    uint32 entryHorde;
    uint32 entryAlliance;
};

struct WintergraspBuildingSpawnData
{
    uint32 entry;
    uint32 WorldState;
    float x;
    float y;
    float z;
    float o;
    uint32 type;
    uint32 nameId;
};

struct WintergraspRebuildableBuildingData
{
    uint64 Guid;
    uint32 entry;
    uint32 WorldState;
    float x;
    float y;
    float z;
    float o;
    uint32 type;
    uint32 nameId;
};

struct WintergraspTeleporterData
{
    uint32 entry;
    float x;
    float y;
    float z;
    float o;
};


#endif // _DATA_BATTLEFIELDWG_H