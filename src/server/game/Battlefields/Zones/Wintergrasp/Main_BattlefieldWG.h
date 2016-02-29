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
 *
 * @section DESCRIPTION
 *
 * The class BattlefieldWG handles the zone Wintergrasp
 * This class is inherited by class Battlefield
 */

#ifndef _MAIN_BATTLEFIELDWG_H
#define _MAIN_BATTLEFIELDWG_H

#include "Data_BattlefieldWG.h"
#include "Misc_BattlefieldWG.h"
#include "Workshop_BattlefieldWG.h"
#include "ObjectAccessor.h"
#include "WorldPacket.h"
#include "World.h"
#include "SpellAuras.h"
#include "Group.h"
#include "GroupMgr.h"


#define WINTERGRASP_PROMOTE_PLAYER_RANGE                40.0f
#define WINTERGRASP_BUILDING_QUEST_OR_ACHIEVEMENT_RANGE 60.0f
#define WINTERGRASP_MAX_WIN_IN_A_ROW                    3
#define WINTERGRASP_MAX_FORTRESS_HEIGHT                 3.0f

enum WintergraspData
{
    BATTLEFIELD_WG_ZONEID = 4197,           // Wintergrasp
    BATTLEFIELD_WG_MAPID = 571,             // Northrend
    BATTLEFIELD_WG_NOTIFICATION_LEVEL = 75, // Required level for world notifications
};

enum WintergraspWorldStates
{
    BATTLEFIELD_WG_WORLD_STATE_VEHICLE_H = 3490,
    BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_H = 3491,
    BATTLEFIELD_WG_WORLD_STATE_VEHICLE_A = 3680,
    BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_A = 3681,
    BATTLEFIELD_WG_WORLD_STATE_ACTIVE = 3801,
    BATTLEFIELD_WG_WORLD_STATE_DEFENDER = 3802,
    BATTLEFIELD_WG_WORLD_STATE_ATTACKER = 3803,
    BATTLEFIELD_WG_WORLD_STATE_SHOW_WORLDSTATE = 3710
};

enum WintergraspAreaIds
{
    AREA_WINTERGRASP_FORTRESS = 4575,
    AREA_THE_SUNKEN_RING = 4538,
    AREA_THE_BROKEN_TEMPLATE = 4539,
    AREA_WESTPARK_WORKSHOP = 4611,
    AREA_EASTPARK_WORKSHOP = 4612,
    AREA_WINTERGRASP = 4197,
    AREA_THE_CHILLED_QUAGMIRE = 4589
};

enum WintergraspGraveyardId
{
    BATTLEFIELD_WG_GY_WORKSHOP_NE,
    BATTLEFIELD_WG_GY_WORKSHOP_NW,
    BATTLEFIELD_WG_GY_WORKSHOP_SE,
    BATTLEFIELD_WG_GY_WORKSHOP_SW,
    BATTLEFIELD_WG_GY_KEEP,
    BATTLEFIELD_WG_GY_HORDE,
    BATTLEFIELD_WG_GY_ALLIANCE,
    BATTLEFIELD_WG_GRAVEYARD_MAX
};

enum WintergraspTimer
{
    TIMER_SAVE                      = 60 * IN_MILLISECONDS,
    TIMER_GRAB_PASSENGER_DELAY      = 250,
    TIMER_PLAYER_TELEPORT_DELAY     = 250,
    TIMER_DECREASE_TOWER_DEFENDED   = 10 * MINUTE * IN_MILLISECONDS,
    TIMER_CHECK_FORTRESS            = 10 * IN_MILLISECONDS,
    TIMER_ACHIEVEMENT_WIN_WG        = 10 * MINUTE * IN_MILLISECONDS
};

struct SavedRankObject
{
    uint32 auraId;
    uint8 stackAmount;
};

struct BfWGCoordGY
{
    float x;
    float y;
    float z;
    float o;
    uint32 gyid;
    uint8 type;
    int32 textid; // for gossip menu
    TeamId startcontrol;
};

// 7 in sql, 7 in header
BfWGCoordGY const WGGraveYard[BATTLEFIELD_WG_GRAVEYARD_MAX] =
{ 
    { 5104.750f, 2300.940f, 368.579f, 0.733038f, 1329, BATTLEFIELD_WG_GY_WORKSHOP_NE, LANG_WG_SPIRIT_GUIDE_SUNKENRING_GY, TEAM_NEUTRAL      },
    { 5099.120f, 3466.036f, 368.484f, 5.317802f, 1330, BATTLEFIELD_WG_GY_WORKSHOP_NW, LANG_WG_SPIRIT_GUIDE_BROKENTEMPLE_GY, TEAM_NEUTRAL    },
    { 4314.648f, 2408.522f, 392.642f, 6.268125f, 1333, BATTLEFIELD_WG_GY_WORKSHOP_SE, LANG_WG_SPIRIT_GUIDE_EASTPARK_GY, TEAM_NEUTRAL        },
    { 4331.716f, 3235.695f, 390.251f, 0.008500f, 1334, BATTLEFIELD_WG_GY_WORKSHOP_SW, LANG_WG_SPIRIT_GUIDE_WESTPARK_GY, TEAM_NEUTRAL        },
    { 5537.986f, 2897.493f, 517.057f, 4.819249f, 1285, BATTLEFIELD_WG_GY_KEEP, LANG_WG_SPIRIT_GUIDE_FORTRESS_GY, TEAM_NEUTRAL               },
    { 5032.454f, 3711.382f, 372.468f, 3.971623f, 1331, BATTLEFIELD_WG_GY_HORDE, LANG_WG_SPIRIT_GUIDE_HORDELANDING_GY, TEAM_HORDE            },
    { 5140.790f, 2179.120f, 390.950f, 1.972220f, 1332, BATTLEFIELD_WG_GY_ALLIANCE, LANG_WG_SPIRIT_GUIDE_ALLIANCELANDING_GY, TEAM_ALLIANCE   },
};

uint8 const WG_MAX_OUTSIDE_NPC = 14;
uint8 const WG_OUTSIDE_ALLIANCE_NPC = 7;

typedef std::set<GameObject*> GameObjectSet;
typedef std::set<Group*> GroupSet;
//typedef std::set<WintergraspCapturePoint *> CapturePointSet; unused ?

const WintergraspBuildingSpawnData WGGameObjectBuilding[] =
{
    // Wall (Not spawned in db)
    // Entry WS X Y Z O type NameID
    { 190219, 3749, 5371.46f, 3047.47f, 407.571f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 190220, 3750, 5331.26f, 3047.1f, 407.923f, 0.052359f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191795, 3764, 5385.84f, 2909.49f, 409.713f, 0.00872f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191796, 3772, 5384.45f, 2771.84f, 410.27f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191799, 3762, 5371.44f, 2630.61f, 408.816f, 3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191800, 3766, 5301.84f, 2909.09f, 409.866f, 0.008724f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191801, 3770, 5301.06f, 2771.41f, 409.901f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191802, 3751, 5280.2f, 2995.58f, 408.825f, 1.61443f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191803, 3752, 5279.14f, 2956.02f, 408.604f, 1.5708f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191804, 3767, 5278.69f, 2882.51f, 409.539f, 1.5708f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191806, 3769, 5279.5f, 2798.94f, 409.998f, 1.5708f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191807, 3759, 5279.94f, 2724.77f, 409.945f, 1.56207f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191808, 3760, 5279.6f, 2683.79f, 409.849f, 1.55334f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191809, 3761, 5330.96f, 2630.78f, 409.283f, 3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 190369, 3753, 5256.08f, 2933.96f, 409.357f, 3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 190370, 3758, 5257.46f, 2747.33f, 409.743f, -3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 190371, 3754, 5214.96f, 2934.09f, 409.19f, -0.008724f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 190372, 3757, 5215.82f, 2747.57f, 409.188f, -3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 190374, 3755, 5162.27f, 2883.04f, 410.256f, 1.57952f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 190376, 3756, 5163.72f, 2799.84f, 409.227f, 1.57952f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },

    // Tower of keep (Not spawned in db)
    { 190221, 3711, 5281.15f, 3044.59f, 407.843f, 3.11539f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, LANG_WG_NORTH_WEST_KEEP_TOWER },
    { 190373, 3713, 5163.76f, 2932.23f, 409.19f, 3.12412f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, LANG_WG_SOUTH_WEST_KEEP_TOWER },
    { 190377, 3714, 5166.4f, 2748.37f, 409.188f, -1.5708f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, LANG_WG_SOUTH_EAST_KEEP_TOWER },
    { 190378, 3712, 5281.19f, 2632.48f, 409.099f, -1.58825f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, LANG_WG_NORTH_EAST_KEEP_TOWER },

    // Wall (with passage) (Not spawned in db)
    { 191797, 3765, 5343.29f, 2908.86f, 409.576f, 0.008724f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191798, 3771, 5342.72f, 2771.39f, 409.625f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },
    { 191805, 3768, 5279.13f, 2840.8f, 409.783f, 1.57952f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0 },

    // South tower (Not spawned in db)
    { 190356, 3704, 4557.17f, 3623.94f, 395.883f, 1.67552f, BATTLEFIELD_WG_OBJECTTYPE_TOWER, LANG_WG_SHADOWSIGHT_TOWER },
    { 190357, 3705, 4398.17f, 2822.5f, 405.627f, -3.12412f, BATTLEFIELD_WG_OBJECTTYPE_TOWER, LANG_WG_WINTERS_EDGE_TOWER },
    { 190358, 3706, 4459.1f, 1944.33f, 434.991f, -2.00276f, BATTLEFIELD_WG_OBJECTTYPE_TOWER, LANG_WG_FLAMEWATCH_TOWER },

    // Door of forteress (Not spawned in db)
    { BATTLEFIELD_WG_GO_FORTRESS_GATE, 3763, 5162.99f, 2841.23f, 410.162f, -3.13286f, BATTLEFIELD_WG_OBJECTTYPE_DOOR, 0 },

    // Last door (Not spawned in db)
    { BATTLEFIELD_WG_GO_VAULT_GATE, 3773, 5397.11f, 2841.54f, 425.899f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST, 0 },
};

const Position WGTurret[] =
{
    { 5391.19f, 3060.8f, 419.616f, 1.69557f },
    { 5266.75f, 2976.5f, 421.067f, 3.20354f },
    { 5234.86f, 2948.8f, 420.88f, 1.61311f },
    { 5323.05f, 2923.7f, 421.645f, 1.5817f },
    { 5363.82f, 2923.87f, 421.709f, 1.60527f },
    { 5264.04f, 2861.34f, 421.587f, 3.21142f },
    { 5264.68f, 2819.78f, 421.656f, 3.15645f },
    { 5322.16f, 2756.69f, 421.646f, 4.69978f },
    { 5363.78f, 2756.77f, 421.629f, 4.78226f },
    { 5236.2f, 2732.68f, 421.649f, 4.72336f },
    { 5265.02f, 2704.63f, 421.7f, 3.12507f },
    { 5350.87f, 2616.03f, 421.243f, 4.72729f },
    { 5390.95f, 2615.5f, 421.126f, 4.6409f },
    { 5148.8f, 2820.24f, 421.621f, 3.16043f },
    { 5147.98f, 2861.93f, 421.63f, 3.18792f },
    { 5352.19f, 3061.25f, 421.10f, 1.62505f }
};

// Portal position in dalaran
const Position WGPortals[]
{
    { 5687.03f, 773.185f, 647.751f, 5.60934f },
    { 5924.11f, 570.403f, 661.087f, 4.43208f }
};

// Rocket launcher position in wg keep
const Position WGKeepRocketLauncher[] =
{
    { 5363.01f, 2741.6f,  409.211f, 3.22886f },
    { 5363.03f, 2740.53f, 409.203f, 3.21141f },
    { 5363.06f, 2737.27f, 409.18f,  3.19395f },
    { 5363.1f,  2739.42f, 409.196f, 3.21141f },
    { 5363.12f, 2738.27f, 409.188f, 3.21141f },
    { 5363.77f, 3006.14f, 409.275f, 3.31613f },
    { 5363.87f, 3007.53f, 409.275f, 3.29867f },
    { 5364.06f, 3008.92f, 409.275f, 3.14159f },
    { 5364.25f, 3010.51f, 409.275f, 3.31613f },
    { 5364.4f,  3011.7f,  409.275f, 3.26377f }
};

// Here there is all npc keeper spawn point
const WintergraspObjectPositionData WGKeepNPC[] =
{
    // X Y Z O horde alliance
    // North East
    { 5326.203125f, 2660.026367f, 409.100891f, 2.543383f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Roaming Guard
    { 5298.430176f, 2738.760010f, 409.316010f, 3.971740f, BATTLEFIELD_WG_NPC_VIERON_BLAZEFEATHER, BATTLEFIELD_WG_NPC_BOWYER_RANDOLPH }, // Vieron Blazefeather
    { 5335.310059f, 2764.110107f, 409.274994f, 4.834560f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5349.810059f, 2763.629883f, 409.333008f, 4.660030f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    // North
    { 5373.470215f, 2789.060059f, 409.322998f, 2.600540f, BATTLEFIELD_WG_NPC_STONE_GUARD_MUKAR, BATTLEFIELD_WG_NPC_KNIGHT_DAMERON }, // Stone Guard Mukar
    { 5296.560059f, 2789.870117f, 409.274994f, 0.733038f, BATTLEFIELD_WG_NPC_HOODOO_MASTER_FU_JIN, BATTLEFIELD_WG_NPC_SORCERESS_KAYLANA }, // Voodoo Master Fu'jin
    { 5372.670000f, 2786.740000f, 409.442000f, 2.809980f, BATTLEFIELD_WG_NPC_CHAMPION_ROS_SLAI, BATTLEFIELD_WG_NPC_MARSHAL_MAGRUDER }, // Wintergrasp Quartermaster
    { 5368.709961f, 2856.360107f, 409.322998f, 2.949610f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5367.910156f, 2826.520020f, 409.322998f, 3.333580f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5389.270020f, 2847.370117f, 418.759003f, 3.106690f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5388.560059f, 2834.770020f, 418.759003f, 3.071780f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5359.129883f, 2837.989990f, 409.364014f, 4.698930f, BATTLEFIELD_WG_NPC_COMMANDER_DARDOSH, BATTLEFIELD_WG_NPC_COMMANDER_ZANNETH }, // Commander Dardosh
    { 5366.129883f, 2833.399902f, 409.322998f, 3.141590f, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_KILRATH, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_AHBRAMIS }, // Tactical Officer Kilrath
    // X Y Z O horde alliance
    // North West
    { 5350.680176f, 2917.010010f, 409.274994f, 1.466080f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5335.120117f, 2916.800049f, 409.444000f, 1.500980f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5295.560059f, 2926.669922f, 409.274994f, 0.872665f, BATTLEFIELD_WG_NPC_SIEGESMITH_STRONGHOOF, BATTLEFIELD_WG_NPC_SIEGE_MASTER_STOUTHANDLE }, // Stronghoof
    { 5371.399902f, 3026.510010f, 409.205994f, 3.250030f, BATTLEFIELD_WG_NPC_PRIMALIST_MULFORT, BATTLEFIELD_WG_NPC_ANCHORITE_TESSA }, // Primalist Mulfort
    { 5392.123535f, 3031.110352f, 409.187683f, 3.677212f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Roaming Guard
    // South
    { 5270.060059f, 2847.550049f, 409.274994f, 3.071780f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5270.160156f, 2833.479980f, 409.274994f, 3.124140f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5179.109863f, 2837.129883f, 409.274994f, 3.211410f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5179.669922f, 2846.600098f, 409.274994f, 3.089230f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5234.970215f, 2883.399902f, 409.274994f, 4.293510f, BATTLEFIELD_WG_NPC_LIEUTENANT_MURP, BATTLEFIELD_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO }, // Lieutenant Murp
    // X Y Z O horde alliance
    // Portal guards (from around the fortress)
    { 5319.209473f, 3055.947754f, 409.176636f, 1.020201f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5311.612305f, 3061.207275f, 408.734161f, 0.965223f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5264.713379f, 3017.283447f, 408.479706f, 3.482424f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5269.096191f, 3008.315918f, 408.826294f, 3.843706f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5201.414551f, 2945.096924f, 409.190735f, 0.945592f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5193.386230f, 2949.617188f, 409.190735f, 1.145859f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5148.116211f, 2904.761963f, 409.193756f, 3.368532f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5153.355957f, 2895.501465f, 409.199310f, 3.549174f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5154.353027f, 2787.349365f, 409.250183f, 2.555644f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5150.066406f, 2777.876953f, 409.343903f, 2.708797f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5193.706543f, 2732.882812f, 409.189514f, 4.845073f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5202.126953f, 2737.570557f, 409.189514f, 5.375215f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5269.181152f, 2671.174072f, 409.098999f, 2.457459f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5264.960938f, 2662.332520f, 409.098999f, 2.598828f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5307.111816f, 2616.006836f, 409.095734f, 5.355575f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
    { 5316.770996f, 2619.430176f, 409.027740f, 5.363431f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A } // Standing Guard
};

const WintergraspObjectPositionData WGOutsideNPC[WG_MAX_OUTSIDE_NPC] =
{
    { 5032.04f, 3681.79f, 362.980f, 4.210f, BATTLEFIELD_WG_NPC_VIERON_BLAZEFEATHER, 0 },
    { 5020.71f, 3626.19f, 360.150f, 4.640f, BATTLEFIELD_WG_NPC_HOODOO_MASTER_FU_JIN, 0 },
    { 4994.85f, 3660.51f, 359.150f, 2.260f, BATTLEFIELD_WG_NPC_COMMANDER_DARDOSH, 0 },
    { 5015.46f, 3677.11f, 362.970f, 6.009f, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_KILRATH, 0 },
    { 5031.12f, 3663.77f, 363.500f, 3.110f, BATTLEFIELD_WG_NPC_SIEGESMITH_STRONGHOOF, 0 },
    { 5042.74f, 3675.82f, 363.060f, 3.358f, BATTLEFIELD_WG_NPC_PRIMALIST_MULFORT, 0 },
    { 5014.45f, 3640.87f, 361.390f, 3.280f, BATTLEFIELD_WG_NPC_LIEUTENANT_MURP, 0 },
    { 5100.07f, 2168.89f, 365.779f, 1.972f, 0, BATTLEFIELD_WG_NPC_BOWYER_RANDOLPH },
    { 5081.70f, 2173.73f, 365.878f, 0.855f, 0, BATTLEFIELD_WG_NPC_SORCERESS_KAYLANA },
    { 5078.28f, 2183.70f, 365.029f, 1.466f, 0, BATTLEFIELD_WG_NPC_COMMANDER_ZANNETH },
    { 5088.49f, 2188.18f, 365.647f, 5.253f, 0, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_AHBRAMIS },
    { 5095.67f, 2193.28f, 365.924f, 4.939f, 0, BATTLEFIELD_WG_NPC_SIEGE_MASTER_STOUTHANDLE },
    { 5088.61f, 2167.66f, 365.689f, 0.680f, 0, BATTLEFIELD_WG_NPC_ANCHORITE_TESSA },
    { 5080.40f, 2199.00f, 359.489f, 2.967f, 0, BATTLEFIELD_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO },
};

// All Npc which are everytime spawned without Team dependancy
const WintergraspObjectPositionData WGNeutralNPC[] =
{
    { 4773.361328f, 3307.161865f, 366.080078f, 6.070840f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - West Bridge W
    { 4778.620605f, 3306.083252f, 365.697174f, 3.015641f, 0, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard Alliance - West Bridge W
    { 4782.251465f, 3274.343506f, 366.213928f, 3.113819f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - West Bridge E
    { 4776.038086f, 3274.541504f, 365.758392f, 6.251484f, 0, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard Alliance - West Bridge E

    { 4688.712402f, 2876.954102f, 387.468872f, 3.376920f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - Central Bridge W
    { 4683.003418f, 2875.795166f, 388.084259f, 0.172494f, 0, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard Alliance - Central Bridge W
    { 4689.535645f, 2848.611328f, 388.882050f, 0.196045f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - Central Bridge E
    { 4694.271484f, 2849.620117f, 387.854767f, 3.357272f, 0, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard Alliance - Central Bridge E

    { 4865.705566f, 2901.550537f, 374.376648f, 6.255386f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - Central Bridge Way Fortress W
    { 4872.727051f, 2901.195801f, 375.729492f, 3.137358f, 0, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard Alliance - Central Bridge Way Fortress W
    { 4873.042969f, 2879.910156f, 375.936310f, 3.129495f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - Central Bridge Way Fortress E
    { 4866.134766f, 2879.842529f, 374.467468f, 0.101785f, 0, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard Alliance - Central Bridge Way Fortress E

    { 4673.962891f, 2413.096924f, 370.352936f, 0.419881f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - East Bridge W
    { 4679.133789f, 2415.582520f, 370.335663f, 3.592890f, 0, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard Alliance - East Bridge W
    { 4690.031738f, 2387.352783f, 370.115570f, 3.482934f, BATTLEFIELD_WG_NPC_GUARD_H, 0 }, // Standing Guard Horde - East Bridge E
    { 4685.353027f, 2385.263916f, 370.425507f, 0.427734f, 0, BATTLEFIELD_WG_NPC_GUARD_A }  // Standing Guard Alliance - East Bridge E
};

const WintergraspObjectPositionData WGKeepBanner[] =
{
    { 5154.668945f, 2853.399170f, 409.184845f, 3.164180f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner Ground MainGate W
    { 5155.230957f, 2828.531250f, 409.189697f, 3.164180f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner Ground MainGate E
    { 5373.129395f, 2862.495605f, 409.547394f, 3.140547f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner Ground FinalGate W
    { 5371.840332f, 2820.819336f, 409.532257f, 3.140547f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner Ground FinalGate E

    { 5280.767578f, 3065.175293f, 431.180023f, 1.637540f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner W - Tower SW Back
    { 5262.301270f, 3047.611328f, 431.247803f, 3.133718f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - Tower SW Back
    { 5163.222656f, 2952.812744f, 432.513092f, 1.649297f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner W - Tower SW Front
    { 5144.895996f, 2935.129639f, 432.255707f, 3.121908f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - Tower SW Front
    { 5158.339844f, 2883.280273f, 431.573761f, 3.152412f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - Wall W MainGate
    { 5159.965332f, 2800.189697f, 430.380402f, 3.113133f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - Wall E MainGate
    { 5145.836914f, 2747.494873f, 432.620911f, 3.227920f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - Tower SE Front
    { 5163.847168f, 2729.452393f, 432.583313f, 4.720170f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner E - Tower SE Front
    { 5260.611328f, 2631.940186f, 432.469879f, 3.094393f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - Tower SE Back
    { 5278.328125f, 2613.608887f, 432.366760f, 4.692673f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner E - Tower SE Back
    { 5397.280273f, 2873.097412f, 456.726929f, 3.132679f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - FinalGate W
    { 5396.891113f, 2809.286865f, 456.526947f, 3.140535f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Banner S - FinalGate E

    { 5391.467285f, 3055.887451f, 433.744507f, 1.656218f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Wall Back Outside
    { 5352.264648f, 3055.913818f, 435.312164f, 1.538402f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Wall Back Outside
    { 5271.624512f, 2976.369141f, 435.061920f, 3.168087f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Back Outside
    { 5235.066406f, 2943.183594f, 435.115143f, 1.664059f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Wall Front Outside
    { 5153.476562f, 2861.976807f, 435.799438f, 3.191635f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Front Outside
    { 5154.363770f, 2820.485107f, 435.716766f, 3.211267f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Front Outside
    { 5236.347656f, 2738.348389f, 435.627594f, 4.621050f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Wall Front Outside
    { 5270.738281f, 2704.668701f, 435.828979f, 3.171977f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Back Outside
    { 5350.843750f, 2621.592285f, 435.317657f, 4.778117f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Wall Back Outside
    { 5391.243652f, 2621.033203f, 435.200043f, 4.601404f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Wall Back Outside
    { 5392.671875f, 3036.686523f, 433.730713f, 4.703549f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Wall Back Inside
    { 5352.520508f, 3036.697754f, 435.177673f, 4.734964f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Wall Back Inside
    { 5290.870605f, 2976.704834f, 435.249603f, 0.002929f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Back Inside
    { 5235.269043f, 2923.875000f, 435.061218f, 4.719253f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Wall Front Inside
    { 5172.701172f, 2862.584717f, 435.752686f, 0.002932f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Front Inside
    { 5173.594727f, 2820.985596f, 435.776001f, 0.002911f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Front Inside
    { 5237.136230f, 2757.569824f, 435.714661f, 1.538358f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Wall Front Inside
    { 5289.969727f, 2704.623047f, 435.907257f, 0.002891f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Wall Back Inside
    { 5350.992188f, 2640.850342f, 435.374054f, 1.664015f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Wall Back Inside
    { 5392.172852f, 2640.248779f, 435.199493f, 1.534430f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Wall Back Inside

    { 5364.017090f, 2918.256836f, 435.895203f, 1.518760f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Inner Wall Back Outside
    { 5322.913086f, 2918.083252f, 435.893005f, 1.510900f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Inner Wall Back Outside
    { 5269.711914f, 2861.580078f, 435.627808f, 3.097358f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Inner Wall Back Outside
    { 5270.255371f, 2819.903809f, 435.790894f, 3.219090f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Inner Wall Back Outside
    { 5322.045898f, 2762.376953f, 435.851440f, 4.711365f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Inner Wall Back Outside
    { 5363.599121f, 2762.355713f, 435.729187f, 4.723150f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Inner Wall Back Outside
    { 5364.298828f, 2899.028809f, 435.755493f, 4.774177f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Inner Wall Back Inside
    { 5322.250488f, 2898.867920f, 435.818939f, 4.664219f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner W - Inner Wall Back Inside
    { 5288.941895f, 2861.800781f, 435.683777f, 0.061803f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Inner Wall Back Inside
    { 5289.484375f, 2820.181641f, 435.929260f, 0.050023f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner S - Inner Wall Back Inside
    { 5322.025879f, 2781.608643f, 435.702698f, 1.624751f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Inner Wall Back Inside
    { 5363.406738f, 2781.585938f, 435.761383f, 1.683654f, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_H, BATTLEFIELD_WG_GO_BANNER_MIN_FLAG_A }, // Banner E - Inner Wall Back Inside
};

const WintergraspObjectPositionData WGAttackerBaseSpiritKeeper[] =
{
    { 5141.20996f, 2178.36011f, 390.99200f, 1.96583f, 0, NPC_DWARVEN_SPIRIT_GUIDE }, // Alliance attacker base
    { 5032.43018f, 3711.60010f, 372.47699f, 3.82329f, NPC_TAUNKA_SPIRIT_GUIDE, 0  }, // Horde attacker base
};

const WintergraspTeleporterData WGPortalDefenderData[] =
{
    // Player teleporter
    { 190763, 5153.41f, 2901.35f, 409.191f, -0.069f },
    { 190763, 5268.70f, 2666.42f, 409.099f, -0.715f },
    { 190763, 5197.05f, 2944.81f, 409.191f, 2.3387f },
    { 190763, 5196.67f, 2737.34f, 409.189f, -2.932f },
    { 190763, 5314.58f, 3055.85f, 408.862f, 0.5410f },
    { 190763, 5391.28f, 2828.09f, 418.675f, -2.164f },
    { 190763, 5153.93f, 2781.67f, 409.246f, 1.6580f },
    { 190763, 5311.44f, 2618.93f, 409.092f, -2.373f },
    { 190763, 5269.21f, 3013.84f, 408.828f, -1.762f },
    { 190763, 5401.62f, 2853.66f, 418.674f, 2.6354f },
    // Vehicle teleporter
    { 192951, 5314.51f, 2703.69f, 408.550f, -0.890f },
    { 192951, 5316.25f, 2977.04f, 408.539f, -0.820f },
};

const float WGFortressInside[4][4]
{   /*  PositionX       PositionY      */
    {   5147.69677f,    2604.44726f     }, // Fortress MIN
    {   5521.16601f,    3055.30297f     }, // Fortress MAX
    {   5392.0f,        2800.0f         }, // Orbroom MIN
    {   5500.0f,        2880.0f         }  // Orbroom MAX
};

const Position WGDalaranPortPosition = { 5807.77832f, 588.3255f, 661.93927f, 1.68709f };

class BattlefieldWG : public Battlefield
{
    public:

        typedef std::map<uint64, SavedRankObject> SavedPlayerRanks;

        ~BattlefieldWG();

        /**
         * Called when the battle start
         *
         * +Spawn relic and turret
         * +Rebuild tower and wall
         * +Invite player to war
         */
        void OnBattleStart();

        /**
         * Called when battle end
         *
         * +Remove relic and turret
         * +Change banner/npc in keep if it needed
         * +Saving battlestate
         * +Reward honor/mark to player
         * +Remove vehicle
         * @param endByTimer : true if battle ended when timer is at 00:00, false if battle ended by clicking on relic
         */
        void OnBattleEnd(bool endByTimer);

        /**
         * Called when grouping starts (15 minutes before battlestart)
         *
         * +Invite all player in zone to join queue
         */
        void OnStartGrouping();

        /**
         * Called when player accept invite to join battle
         *
         * +Update aura
         * +Teleport if it needed
         * +Update worldstate
         * +Update tenacity
         * @paraam pPlayer : Player who accepted invite
         */
        void OnPlayerJoinWar(Player* pPlayer);

        /**
         * Called when player left the battle
         *
         * +Update player aura
         * @param pPlayer : Player who left the battle
         */
        void OnPlayerLeaveWar(Player* pPlayer);

        /**
         * Called when player left the WG zone
         *
         * @param pPlayer : Player who left the zone
         */
        void OnPlayerLeaveZone(Player* pPlayer);

        /**
         * Called when player enters in WG zone
         *
         * +Update aura
         * +Update worldstate
         * @param pPlayer : Player who enters the zone
         */
        void OnPlayerEnterZone(Player* pPlayer);

        /**
         * Called for update battlefield data
         *
         * +Save battle timer in database every minutes
         * +Update imunity aura from graveyard
         * @param diff : time elapsed since the last call (in ms)
         */
        bool Update(uint32 diff);

        /**
         * Called when a creature is created
         *
         * +Update vehicle count
         * @param pCreature : Creature which has been added to map
         */
        void OnCreatureCreate(Creature* pCreature);

        /**
         * Called when a creature is removed
         *
         * +Update vehicle count
         * @param pCreature : Creature which has been removed from map
         */
        void OnCreatureRemove(Creature* pCreature);

        /**
         * Called when a gameobject is created
         *
         * @param pObj : Gameobject which has been added to map
         */
        void OnGameObjectCreate(GameObject* pObj);

        /**
         * Called when a wall/tower is broken
         *
         * +Update quest
         * @param team : TeamId of the player who executed this event
         * @param guid : GUID of the gameobject which has been broken
         */
        void BrokenWallOrTower(TeamId team, uint64 guid);

        /**
         * Called when a tower is damaged
         *
         * +Update tower count (for reward calcul)
         * @param team : TeamId of the building
         */
        void UpdateDamagedTowerCount(TeamId team);

        /**
         * Called when tower is broken
         *
         * +Update tower buff
         * +check if three south tower is down for remove 10 minutes to wg
         * @param team : TeamId of the player who executed this event
         */
        void UpdatedDestroyedTowerCount(TeamId team);

        void DoCompleteOrIncrementAchievement(uint32 achievement, Player* pPlayer, uint8 incrementNumber = 1);

        void RemoveAurasFromPlayer(Player* pPlayer);

        /** Called when battlefield is setup, at server start */
        bool SetupBattlefield();

        /** Return pointer to relic object */
        GameObject* GetRelic() {
            return GetGameObject(titansRelicGUID);
        }

        /** Define relic object */
        void SetRelic(uint64 relicGUID) {
            titansRelicGUID = relicGUID;
        }

        /** Check if players can interact with the relic (Only if the last door has been broken) */
        bool CanInteractWithRelic() {
            return isRelicInteractible;
        }

        /** Define if player can interact with the relic */
        void SetRelicInteractible(bool allow) {
            isRelicInteractible = allow;
        }

        /** Called to change the Keep to the Defender team */
        void UpdateTeam();

        /** Called to show/hide the neutral + attacker base Spirit Keepers (wartime/no wartime) */
        void UpdateNeutralSpirits();

        /** Called to show/hide all db spawned creatures */
        void UpdateDBSpawnedCreatures();

        void UpdateVehicleCountWG();
        void UpdateCounterVehicle(bool init);

        void SendInitWorldStatesTo(Player* pPlayer);
        void SendInitWorldStatesToAll(bool toServer = false);
        void FillInitialWorldStates(WorldPacket& data);

        void HandleKill(Player* pPlayer, Unit* pUnit);
        void OnUnitDeath(Unit* pUnit);
        void PromotePlayer(Player* pPlayer);

        void UpdateTenacity();
        void ProcessEvent(WorldObject* pWObj, uint32 eventId);

        bool FindAndRemoveVehicleFromList(Unit* pUnit);

        /** returns the graveyardId in the specified area. */
        uint8 GetSpiritGraveyardId(uint32 areaId) const;

        uint32 GetData(uint32 data) const;

        void AddCapturablePoint(WintergraspCapturePoint* cPoint){
            AddCapturePoint(cPoint);
        }

        BfCapturePoint* RemoveCapturePointForWorkshop(WintergraspWorkshopData* workshop);

        /** Sends a trinity_string message to all player who are above the required level */
        void SendWarningToAllWithLevelReq(uint32 entry, uint32 additionalEntry = 0);

        /** Sets the queue in the client ui enabled */
        void SetClientQueueEnabled();

        /**
         * Called to grant bonus honor for tower damaged/destroyed
         *
         * @param team : Team of the tower
         * @param destroyed : Has tower been destroyed
         */
        void GrantHonorForTower(TeamId team, bool destroyed);

    private:

        bool isRelicInteractible;
        bool notificationSent; // 30min before the battle begins

        Workshop workshopsList;
        GameObjectBuilding buildingsInZone;

        GuidSet defenderPortalList;
        GuidSet keepGameObject[2];
        GuidSet vehicles[2];
        GuidSet canonList;
        GuidSet rocketLaunchers;
        GuidSet keepCreatures[2];
        GuidSet outsideCreatures[2];
        GuidSet keepBanner[2];
        GuidSet spiritKeeper;
        GuidSet databaseSpawnedCreatures;

        uint8 tenacityStack;
        uint32 saveTimer;
        uint32 fortressCheckTimer;
        uint32 battleTimer;

        uint64 titansRelicGUID;
        uint64 dalaranPortals[2];

        uint8 winCounter;

        /** Used to save reached player ranks for restoration */
        SavedPlayerRanks savedPlayerRanks;

        /** 
         * Called to show portals 15 minutes before battle starts
         *
         * +Hide static portal in Dalaran
         * +Show alliance and horde portal to wintergrasp
         */
        void setBFStartPortals();

        /** 
         * Called to show portals to wintergrasp after the battle
         *
         * -Depending on battle winner (alliance / horde)
         * +Hide alliance and horde portal to wintergrasp
         * +Show static portal in Dalaran
         */
        void setBFEndPortals();

        /** Called to teleport player */
        void teleportPlayer(Player* pPlayer);

        /** Called to teleport player */
        uint32 getTeleportSpell(Player* pPlayer);

        /**
         * Called to check if position of player is inside the wg fortress
         *
         * @param pPlayer : Player to be checked
         * @return : is pPlayer in fortress
         */
        bool isPlayerInFortress(Player* pPlayer);

        /**
         * Called to check if position of player is inside the orb room
         *
         * @param pPlayer : Player to be checked
         * @return : is pPlayer in orb room
         */
        bool isPlayerInOrbRoom(Player* pPlayer);

        /**
         * Called to remove player (online) from orb room at battlestart
         *
         * +Check pPlayer is in orb room
         * +If true, kick him
         */
        void checkAndKickPlayerInOrbRoom(Player* pPlayer);

        /**
         * Called to remove player (offline) from orb room at battlestart
         *
         * +Check all offline players if position is in orb room
         * +If true, relocate player to dalaran
         */
        void relocateOfflinePlayersInOrbRoom();

        /**
         * Called when a player leaves war and war is in progress
         *
         * -If there is already a saved rank -> overwrite it
         * +Save pPlayers rank to restore if he rejoin battle
         * @param pPlayer : Player who should be saved
         */
        void savePlayerRank(Player* pPlayer);

        /**
         * Called when a player joins war to restore his previous rank
         *
         * +Try to restore pPlayers rank if it's saved
         * @param pPlayer : Player who should be restored
         * @return : true if pPlayers rank could be restored
         */
        bool restorePlayerRank(Player* pPlayer);

        /**
         * Called to check quests/achievements which are related with buildings
         *
         * @param team : Team of the destroyed building
         * @param pObject : GO pointer of the destroyed object
         */
        void checkBuildingPlayerQuestAndAchievements(TeamId team, GameObject* pObject);

        /**
         * Called to increment quest of players
         *
         * -Kill credit is saved in array WintergraspQuestKillCredit
         * @param questType : Type of quest to reward from WintergraspQuestType
         * @param pPlayer : Player who should be proceeded
         */
        void incrementPlayerQuest(WintergraspQuestType questType, Player* pPlayer);

        /**
         * Called to increment quest of players
         *
         * -Kill credit is saved in array WintergraspQuestKillCredit
         * @param questType : Type of quest to reward from WintergraspQuestType
         * @param playerList : Reference to vector of players in range
         */
        void incrementPlayerQuest(WintergraspQuestType questType, std::vector<Player*>& playerList);

        /**
         * Called to complete quest of players
         *
         * -not used
         * @param questId : ID of quest to complete
         * @param playerList : Reference to vector of players in range
         *//*
        void completePlayerQuest(uint32 questId, std::vector<Player*>& playerList);*/

        /**
         * Called to complete achievement of players
         *
         * @param achievement : ID of achievement saved in WintergraspAchievements
         * @param playerList : Reference to vector of players in range
         */
        void completePlayerAchievement(WintergraspAchievements achievement, std::vector<Player*>& playerList);

        /**
         * Called to increase quest and complete achievement of players
         *
         * -Better performance if one condition should handle quest and achievement
         * @param questType : Type of quest to reward from WintergraspQuestType
         * @param achievement : ID of achievement saved in WintergraspAchievements
         * @param playerList : Reference to vector of players in range
         */
        void handlePlayerQuestAndAchievement(WintergraspQuestType questType, WintergraspAchievements achievement, std::vector<Player*>& playerList);

        /**
         * Called to grant bonus honor for team at the end of battle
         *
         * +Base attacker tower
         * +Base defender tower
         * +Base rank
         * +Depending on intact or damaged
         * @param endByTimer : Battle ended by timer or with relic use
         */
        void addBonusHonorAfterBattle(bool endByTimer);

        /** Called to check if player of attacker team is inside fortress */
        void checkFortress();
};


#endif // _MAIN_BATTLEFIELDWG_H