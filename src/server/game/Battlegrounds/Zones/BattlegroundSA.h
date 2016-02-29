/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 27.07.2014
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
 * @section DESCRIPTION
 *
 * The class BattlegroundSA handles the battleground Strand of the Ancients
 * This class is inherited by class Battleground
 */

#ifndef __BATTLEGROUNDSA_H
#define __BATTLEGROUNDSA_H

#include "Battleground.h"

#pragma warning (disable:4305) // double to float convert

#define GETBOOLSTATE(x)                 boolStateMap.at(x)
#define GETUINT32STATE(x)               uint32StateMap.at(x)

#define BG_SA_FRONT_GATE_MAX            5
#define BG_SA_FLAG_AMOUNT               3
#define BG_SA_DEMOLISHER_AMOUNT         4
#define BG_SA_INVISIBLE_WALL_F_SCALE    1.5f
#define BG_SA_INVISIBLE_WALL_SCALE      1.35f


enum BG_SA_Status
{
    BG_SA_NOTSTARTED                    = 0,
    BG_SA_WARMUP                        = 1,
    BG_SA_ROUND_ONE                     = 2,
    BG_SA_SECOND_WARMUP                 = 3,
    BG_SA_ROUND_TWO                     = 4,
    BG_SA_BONUS_ROUND                   = 5
};

enum BG_SA_GateState
{
    BG_SA_GATE_OK                       = 1,
    BG_SA_GATE_DAMAGED                  = 2,
    BG_SA_GATE_DESTROYED                = 3
};

enum BG_SA_EventIdGate
{
    BG_SA_EVENT_BLUE_GATE_DAMAGED       = 19040,
    BG_SA_EVENT_BLUE_GATE_DESTROYED     = 19045,

    BG_SA_EVENT_GREEN_GATE_DAMAGED      = 19041,
    BG_SA_EVENT_GREEN_GATE_DESTROYED    = 19046,

    BG_SA_EVENT_RED_GATE_DAMAGED        = 19042,
    BG_SA_EVENT_RED_GATE_DESTROYED      = 19047,

    BG_SA_EVENT_PURPLE_GATE_DAMAGED     = 19043,
    BG_SA_EVENT_PURPLE_GATE_DESTROYED   = 19048,

    BG_SA_EVENT_YELLOW_GATE_DAMAGED     = 19044,
    BG_SA_EVENT_YELLOW_GATE_DESTROYED   = 19049,

    BG_SA_EVENT_ANCIENT_GATE_DAMAGED    = 19836,
    BG_SA_EVENT_ANCIENT_GATE_DESTROYED  = 19837
};

enum BG_SA_Timers
{
    BG_SA_BOAT_START                    =  60 * IN_MILLISECONDS,
    BG_SA_WARMUPLENGTH                  = 120 * IN_MILLISECONDS,
    BG_SA_ROUNDLENGTH                   = 600 * IN_MILLISECONDS,
    BG_SA_RESPAWN_TIME_DEMOLISHER       =  30 * IN_MILLISECONDS,
    BG_SA_UPDATE_TIME_DEMOLISHER        =   1 * IN_MILLISECONDS,
    BG_SA_UPDATE_TIME_SEAFORIUM_BOMB    =  15 * IN_MILLISECONDS
};

enum BG_SA_WorldStates
{
    BG_SA_TIMER_MINS                    = 3559,
    BG_SA_TIMER_SEC_TENS                = 3560,
    BG_SA_TIMER_SEC_DECS                = 3561,
    BG_SA_ALLY_ATTACKS                  = 4352,
    BG_SA_HORDE_ATTACKS                 = 4353,
    BG_SA_PURPLE_GATEWS                 = 3614,
    BG_SA_RED_GATEWS                    = 3617,
    BG_SA_BLUE_GATEWS                   = 3620,
    BG_SA_GREEN_GATEWS                  = 3623,
    BG_SA_YELLOW_GATEWS                 = 3638,
    BG_SA_ANCIENT_GATEWS                = 3849,
    BG_SA_WEST_GY_ALLIANCE              = 3635,
    BG_SA_EAST_GY_ALLIANCE              = 3636,
    BG_SA_CENTER_GY_ALLIANCE            = 3637,
    BG_SA_RIGHT_ATT_TOKEN_ALL           = 3627,
    BG_SA_LEFT_ATT_TOKEN_ALL            = 3626,
    BG_SA_LEFT_ATT_TOKEN_HRD            = 3629,
    BG_SA_RIGHT_ATT_TOKEN_HRD           = 3628,
    BG_SA_HORDE_DEFENCE_TOKEN           = 3631,
    BG_SA_ALLIANCE_DEFENCE_TOKEN        = 3630,
    BG_SA_EAST_GY_HORDE                 = 3632,
    BG_SA_WEST_GY_HORDE                 = 3633,
    BG_SA_CENTER_GY_HORDE               = 3634,
    BG_SA_BONUS_TIMER                   = 0xdf3,
    BG_SA_ENABLE_TIMER                  = 3564
};

enum BG_SA_NpcEntry
{
    NPC_ANTI_PERSONNAL_CANNON           = 27894,
    NPC_DEMOLISHER_SA                   = 28781,
    NPC_RIGGER_SPARKLIGHT               = 29260,
    NPC_GORGRIL_RIGSPARK                = 29262,
    NPC_SA_TRIGGER                      = 23472
};

enum BG_SA_NpcType
{
    BG_SA_GUN_1                         = 0,
    BG_SA_GUN_2,
    BG_SA_GUN_3,
    BG_SA_GUN_4,
    BG_SA_GUN_5,
    BG_SA_GUN_6,
    BG_SA_GUN_7,
    BG_SA_GUN_8,
    BG_SA_GUN_9,
    BG_SA_GUN_10,
    BG_SA_NPC_TRIGGER_1,
    BG_SA_NPC_TRIGGER_2,
    BG_SA_NPC_TRIGGER_3,
    BG_SA_NPC_TRIGGER_4,
    BG_SA_NPC_TRIGGER_5,
    BG_SA_BEACH_DEMOLISHER_1,
    BG_SA_BEACH_DEMOLISHER_2,
    BG_SA_BEACH_DEMOLISHER_3,
    BG_SA_BEACH_DEMOLISHER_4,
    BG_SA_NPC_SPARKLIGHT_EAST,
    BG_SA_NPC_RIGSPARK_WEST,
    BG_SA_DEMOLISHER_1,
    BG_SA_DEMOLISHER_2,
    BG_SA_DEMOLISHER_3,
    BG_SA_DEMOLISHER_4,
    BG_SA_MAXNPC
};

enum BG_SA_ObjectEntry
{
    OBJECT_TITAN_RELIC                  = 192834,
    OBJECT_INVISIBLE_WALL               = 180322,
    OBJECT_PORTAL_DEFENDER              = 192819,
    OBJECT_SEAFORIUM_BOMB               = 190753
};

enum BG_SA_ObjectType
{
    BG_SA_GREEN_GATE                    = 0,
    BG_SA_YELLOW_GATE,
    BG_SA_BLUE_GATE,
    BG_SA_RED_GATE,
    BG_SA_PURPLE_GATE,
    BG_SA_ANCIENT_GATE,
    BG_SA_TITAN_RELIC,
    BG_SA_BOAT_ONE,
    BG_SA_BOAT_TWO,
    BG_SA_SIGIL_1,
    BG_SA_SIGIL_2,
    BG_SA_SIGIL_3,
    BG_SA_SIGIL_4,
    BG_SA_SIGIL_5,
    BG_SA_EAST_FLAGPOLE,
    BG_SA_WEST_FLAGPOLE,
    BG_SA_CENTRAL_FLAGPOLE,
    BG_SA_EAST_FLAG,
    BG_SA_WEST_FLAG,
    BG_SA_CENTRAL_FLAG,
    BG_SA_PORTAL_DEFFENDER_BLUE,
    BG_SA_PORTAL_DEFFENDER_GREEN,
    BG_SA_PORTAL_DEFFENDER_YELLOW,
    BG_SA_PORTAL_DEFFENDER_PURPLE,
    BG_SA_PORTAL_DEFFENDER_RED,
    BG_SA_BOMB,
    // 67 Bombs
    BG_SA_INVISIBLE_WALL_EAST_F         = 93,
    BG_SA_INVISIBLE_WALL_EAST_1,
    BG_SA_INVISIBLE_WALL_EAST_2,
    BG_SA_MAXOBJ
};

enum BG_SA_Boat
{
    BG_SA_BOAT_ONE_A                    = 193182,
    BG_SA_BOAT_TWO_H                    = 193183,
    BG_SA_BOAT_ONE_H                    = 193184,
    BG_SA_BOAT_TWO_A                    = 193185
};

enum BG_SA_BoolState
{
    STATE_SHIPS_STARTED                 = 0,
    STATE_TIMER_ENABLED                 = 1,
    STATE_SIGNALED_ROUND_TWO            = 2,
    STATE_SIGNALED_ROUND_TWO_HALF_MIN   = 3,
    STATE_INIT_SECOND_ROUND             = 4,
    STATE_ACHIEVMENT_DOTA               = 5,
    STATE_INVISIBLE_WALLS               = 6
};

enum BG_SA_UInt32State
{
    STATE_TOTAL_TIME                    = 0,
    STATE_END_ROUND_TIMER               = 1,
    STATE_UPDATE_WAIT_TIMER             = 2,
    STATE_TIMER_DEMOLISHER_RESPAWN      = 3
};

enum BG_SA_Spells
{
    BG_SA_SPELL_PARACHUTE               = 44795,
    BG_SA_SPELL_DISABLE_HEALTH_REGEN    = 52455,
    BG_SA_SPELL_END_OF_ROUND            = 52459
};

enum BG_SA_Achievments
{
    BG_SA_ACHIEVMENT_DEFENSE_OF_THE_ANCIENTS    = 0,
    BG_SA_ACHIEVMENT_NOT_EVEN_A_SCRETCH         = 1
};

enum BG_SA_Data
{
    BG_SA_DATA_CAN_DEMOLISHER_BUILD     = 0,
    BG_SA_DATA_ATTACKER_TEAM            = 1,
    BG_SA_DATA_ACHIEVMENT_DOTA          = 2, // Defense of the ancients
    BG_SA_DATA_ACHIEVMENT_NEAS_A        = 3, // Not even a scratch alliance
    BG_SA_DATA_ACHIEVMENT_NEAS_H        = 4, // Not even a scratch horde
    BG_SA_DATA_DEMOLISHER_TIMER         = 5

};

enum BG_SA_Actions
{
    BG_SA_ACTION_BUILD_DEMOLISHER_ALLIANCE      = 0,
    BG_SA_ACTION_BUILD_DEMOLISHER_HORDE         = 1
};

enum BG_SA_Graveyards
{
    BG_SA_BEACH_GY                      = 0,
    BG_SA_DEFENDER_LAST_GY              = 1,
    BG_SA_EAST_CAPTURABLE_GY            = 2,
    BG_SA_WEST_CAPTURABLE_GY            = 3,
    BG_SA_CENTRAL_CAPTURABLE_GY         = 4,
    BG_SA_MAX_GY
};


struct StrandOfTheAncientsSpawnData
{
    uint32 entry;
    float x;
    float y;
    float z;
    float o;
};

struct StrandOfTheAncientsRoundScore
{
    TeamId winner;
    uint32 time;
};

const StrandOfTheAncientsSpawnData StrandOfTheAncientsCreatureSpawns[]
{
    // ENTRY                        PositionX       PositionY       PositionZ       Orientation
    { NPC_ANTI_PERSONNAL_CANNON,    1436.429f,      110.05f,        41.407f,        5.4f        },
    { NPC_ANTI_PERSONNAL_CANNON,    1404.9023f,     84.758f,        41.183f,        5.46f       },
    { NPC_ANTI_PERSONNAL_CANNON,    1068.693f,      -86.951f,       93.81f,         0.02f       },
    { NPC_ANTI_PERSONNAL_CANNON,    1068.83f,       -127.56f,       96.45f,         0.0912f     },
    { NPC_ANTI_PERSONNAL_CANNON,    1422.115f,      -196.433f,      42.1825f,       1.0222f     },
    { NPC_ANTI_PERSONNAL_CANNON,    1454.887f,      -220.454f,      41.956f,        0.9627f     },
    { NPC_ANTI_PERSONNAL_CANNON,    1232.345f,      -187.517f,      66.945f,        0.45f       },
    { NPC_ANTI_PERSONNAL_CANNON,    1249.634f,      -224.189f,      66.72f,         0.635f      },
    { NPC_ANTI_PERSONNAL_CANNON,    1236.213f,      92.287f,        64.965f,        5.751f      },
    { NPC_ANTI_PERSONNAL_CANNON,    1215.11f,       57.772f,        64.739f,        5.78f       },
    { NPC_SA_TRIGGER,               1453.49f,       -250.453f,      30.896f,        4.2883f     },
    { NPC_SA_TRIGGER,               1377.05f,       97.036f,        30.8605f,       2.46539f    },
    { NPC_SA_TRIGGER,               1186.05f,       58.8048f,       56.5491f,       2.75992f    },
    { NPC_SA_TRIGGER,               1042.83f,       -72.839f,       84.8145f,       3.58615f    },
    { NPC_SA_TRIGGER,               1233.62f,       -250.49f,       55.4036f,       3.7016f     }
};

// 4 Demolisher spawn locations at the beach
const StrandOfTheAncientsSpawnData StrandOfTheAncientsBeachDemolisherSpawns[]
{
    // ENTRY                        PositionX       PositionY       PositionZ       Orientation
    { NPC_DEMOLISHER_SA,            1611.597656f,   -117.270073f,   8.719355f,      2.513274f   },
    { NPC_DEMOLISHER_SA,            1575.562500f,   -158.421875f,   5.024450f,      2.129302f   },
    { NPC_DEMOLISHER_SA,            1618.047729f,   61.424641f,     7.248210f,      3.979351f   },
    { NPC_DEMOLISHER_SA,            1575.103149f,   98.873344f,     2.830360f,      3.752458f   }
};

// 2 Demolisher spawn locations at the workshops
const StrandOfTheAncientsSpawnData StrandOfTheAncientsDemolisherSpawns[]
{
    // ENTRY                        PositionX       PositionY       PositionZ       Orientation
    { NPC_DEMOLISHER_SA,            1370.834595f,   -316.619843f,   35.025036f,     2.083169f   }, // East
    { NPC_DEMOLISHER_SA,            1353.183716f,   223.669006f,    35.263783f,     4.510048f   }  // West
};

const StrandOfTheAncientsSpawnData StrandOfTheAncientsCapturePointSpawns[]
{
    // ENTRY                        PositionX       PositionY       PositionZ       Orientation
    { NPC_RIGGER_SPARKLIGHT,        1348.644165f,   -298.786469f,   31.080130f,     1.710423f   }, // East
    { NPC_GORGRIL_RIGSPARK,         1358.191040f,   195.527786f,    31.018187f,     4.171337f   }  // West
};

const StrandOfTheAncientsSpawnData StrandOfTheAncientsObjectSpawns[]
{
    // ENTRY    PositionX       PositionY       PositionZ       Orientation
    // Gates
    { 190722,   1411.57f,       108.163f,       28.692f,        5.441f      }, // Green
    { 190727,   1055.452f,      -108.1f,        82.134f,        0.034f      }, // Yellow
    { 190724,   1431.3413f,     -219.437f,      30.893f,        0.9736f     }, // Blue
    { 190726,   1227.667f,      -212.555f,      55.372f,        0.5023f     }, // Red
    { 190723,   1214.681f,      81.21f,         53.413f,        5.745f      }, // Purple
    { 192549,   878.555f,       -108.2f,        117.845f,       0.0f        }, // Chamber
    { 192834,   836.5f,         -108.8f,        120.219f,       0.0f        }, // Titan Relic
    // Ships
    { 0,        2679.696777f,   -826.891235f,   3.712860f,      5.78367f    }, // rot2 1 rot3 0.0002f
    { 0,        2574.003662f,   981.261475f,    2.603424f,      0.807696f   },
    // Sigils
    { 192687,   1414.054f,      106.72f,        41.442f,        5.441f      },
    { 192685,   1060.63f,       -107.8f,        94.7f,          0.034f      },
    { 192689,   1433.383f,      -216.4f,        43.642f,        0.9736f     },
    { 192690,   1230.75f,       -210.724f,      67.611f,        0.5023f     },
    { 192691,   1217.8f,        79.532f,        66.58f,         5.745f      },
    // Flagpoles
    { 191311,   1338.863892f,   -153.336533f,   30.895121f,     -2.530723f  },
    { 191311,   1309.124268f,   9.410645f,      30.893402f,     2.918436f   },
    { 191311,   1215.114258f,   -65.711861f,    70.084267f,     -3.124123f  },
    // Flags
    { 191306,   1338.859253f,   -153.327316f,   30.895077f,     -2.530723f  },
    { 191308,   1309.192017f,    9.416233f,     30.893402f,     2.918436f   },
    { 191310,   1215.108032f,   -65.715767f,    70.084267f,     -3.124123f  },
    // Portals
    { 192819,   1468.380005f,   -225.798996f,   30.896200f,     0.0f        },
    { 192819,   1394.270020f,   72.551399f,     31.054300f,     0.0f        },
    { 192819,   1065.260010f,   -89.79501f,     81.073402f,     0.0f        },
    { 192819,   1216.069946f,   47.904301f,     54.278198f,     0.0f        },
    { 192819,   1255.569946f,   -233.548996f,   56.43699f,      0.0f        },
    // Bombs
    { 190753,   1333.45f,       211.354f,       31.0538f,       5.03666f    },
    { 190753,   1334.29f,       209.582f,       31.0532f,       1.28088f    },
    { 190753,   1332.72f,       210.049f,       31.0532f,       1.28088f    },
    { 190753,   1334.28f,       210.78f,        31.0538f,       3.85856f    },
    { 190753,   1332.64f,       211.39f,        31.0532f,       1.29266f    },
    { 190753,   1371.41f,       194.028f,       31.5107f,       0.753095f   },
    { 190753,   1372.39f,       194.951f,       31.4679f,       0.753095f   },
    { 190753,   1371.58f,       196.942f,       30.9349f,       1.01777f    },
    { 190753,   1370.43f,       196.614f,       30.9349f,       0.957299f   },
    { 190753,   1369.46f,       196.877f,       30.9351f,       2.45348f    },
    { 190753,   1370.35f,       197.361f,       30.9349f,       1.08689f    },
    { 190753,   1369.47f,       197.941f,       30.9349f,       0.984787f   },
    { 190753,   1592.49f,       47.5969f,       7.52271f,       4.63218f    },
    { 190753,   1593.91f,       47.8036f,       7.65856f,       4.63218f    },
    { 190753,   1593.13f,       46.8106f,       7.54073f,       4.63218f    },
    { 190753,   1589.22f,       36.3616f,       7.45975f,       4.64396f    },
    { 190753,   1588.24f,       35.5842f,       7.55613f,       4.79564f    },
    { 190753,   1588.14f,       36.7611f,       7.49675f,       4.79564f    },
    { 190753,   1595.74f,       35.5278f,       7.46602f,       4.90246f    },
    { 190753,   1596,           36.6475f,       7.47991f,       4.90246f    },
    { 190753,   1597.03f,       36.2356f,       7.48631f,       4.90246f    },
    { 190753,   1597.93f,       37.1214f,       7.51725f,       4.90246f    },
    { 190753,   1598.16f,       35.888f,        7.50018f,       4.90246f    },
    { 190753,   1579.6f,        -98.0917f,      8.48478f,       1.37996f    },
    { 190753,   1581.2f,        -98.401f,       8.47483f,       1.37996f    },
    { 190753,   1580.38f,       -98.9556f,      8.4772f,        1.38781f    },
    { 190753,   1585.68f,       -104.966f,      8.88551f,       0.493246f   },
    { 190753,   1586.15f,       -106.033f,      9.10616f,       0.493246f   },
    { 190753,   1584.88f,       -105.394f,      8.82985f,       0.493246f   },
    { 190753,   1581.87f,       -100.899f,      8.46164f,       0.929142f   },
    { 190753,   1581.48f,       -99.4657f,      8.46926f,       0.929142f   },
    { 190753,   1583.2f,        -91.2291f,      8.49227f,       1.40038f    },
    { 190753,   1581.94f,       -91.0119f,      8.49977f,       1.40038f    },
    { 190753,   1582.33f,       -91.951f,       8.49353f,       1.1844f     },
    { 190753,   1342.06f,       -304.049f,      30.9532f,       5.59507f    },
    { 190753,   1340.96f,       -304.536f,      30.9458f,       1.28323f    },
    { 190753,   1341.22f,       -303.316f,      30.9413f,       0.486051f   },
    { 190753,   1342.22f,       -302.939f,      30.986f,        4.87643f    },
    { 190753,   1382.16f,       -287.466f,      32.3063f,       4.80968f    },
    { 190753,   1381,           -287.58f,       32.2805f,       4.80968f    },
    { 190753,   1381.55f,       -286.536f,      32.3929f,       2.84225f    },
    { 190753,   1382.75f,       -286.354f,      32.4099f,       1.00442f    },
    { 190753,   1379.92f,       -287.34f,       32.2872f,       3.81615f    },
    { 190753,   1100.52f,       -2.41391f,      70.2984f,       0.131054f   },
    { 190753,   1099.35f,       -2.13851f,      70.3375f,       4.4586f     },
    { 190753,   1099.59f,       -1.00329f,      70.238f,        2.49903f    },
    { 190753,   1097.79f,       0.571316f,      70.159f,        4.00307f    },
    { 190753,   1098.74f,       -7.23252f,      70.7972f,       4.1523f     },
    { 190753,   1098.46f,       -5.91443f,      70.6715f,       4.1523f     },
    { 190753,   1097.53f,       -7.39704f,      70.7959f,       4.1523f     },
    { 190753,   1097.32f,       -6.64233f,      70.7424f,       4.1523f     },
    { 190753,   1096.45f,       -5.96664f,      70.7242f,       4.1523f     },
    { 190753,   971.725f,       0.496763f,      86.8467f,       2.09233f    },
    { 190753,   973.589f,       0.119518f,      86.7985f,       3.17225f    },
    { 190753,   972.524f,       1.25333f,       86.8351f,       5.28497f    },
    { 190753,   971.993f,       2.05668f,       86.8584f,       5.28497f    },
    { 190753,   973.635f,       2.11805f,       86.8197f,       2.36722f    },
    { 190753,   974.791f,       1.74679f,       86.7942f,       1.5936f     },
    { 190753,   974.771f,       3.0445f,        86.8125f,       0.647199f   },
    { 190753,   979.554f,       3.6037f,        86.7923f,       1.69178f    },
    { 190753,   979.758f,       2.57519f,       86.7748f,       1.76639f    },
    { 190753,   980.769f,       3.48904f,       86.7939f,       1.76639f    },
    { 190753,   979.122f,       2.87109f,       86.7794f,       1.76639f    },
    { 190753,   986.167f,       4.85363f,       86.8439f,       1.5779f     },
    { 190753,   986.176f,       3.50367f,       86.8217f,       1.5779f     },
    { 190753,   987.33f,        4.67389f,       86.8486f,       1.5779f     },
    { 190753,   985.23f,        4.65898f,       86.8368f,       1.5779f     },
    { 190753,   984.556f,       3.54097f,       86.8137f,       1.5779f     },
    // Invisible walls
    { 180322,   1592.059f,      -113.983f,      7.0f,           4.117f      }, // Front East
    { 180322,   1605.348f,      -111.979f,      8.2f,           5.69806f    }, // East
    { 180322,   1589.696f,      -101.009f,      8.2f,           2.56432f    }, // East
};

const StrandOfTheAncientsSpawnData StrandOfTheAncientsPlayerSpawns[]
{
    // ENTRY    PositionX       PositionY       PositionZ       Orientation
    {  607,     2684.64355f,    -832.28704f,    20.0f,          1.06618f    }, // Attacker East
    {  607,     2577.31640f,    989.12023f,     20.0f,          2.35030f    }, // Attacker West
    {  607,     1597.53283f,    -105.55210f,    9.37f,          4.1096f     }, // Attacker after ship started
    {  607,     1209.7f,        -65.16f,        70.1f,          0.0f        }  // Defender
};

const uint32 StrandOfTheAncientsFactions[2] =
{
    1732,
    1735,
};

const uint32 StrandOfTheAncientsGYEntries[BG_SA_MAX_GY] =
{
    1350,
    1349, // defender last GY
    1347, // east capturable GY
    1346, // west capturable GY
    1348, // center, capturable
};

const float StrandOfTheAncientsGYOrientation[BG_SA_MAX_GY] =
{
    6.202f,
    6.148f, // defender last GY
    2.172f, // east capturable GY
    4.386f, // west capturable GY
    5.945f, // center, capturable
};


class BattlegroundSAScore : public BattlegroundScore
{
    public:

    BattlegroundSAScore() : demolishers_destroyed(0), gates_destroyed(0) { };

    virtual ~BattlegroundSAScore() {};

    uint8 demolishers_destroyed;
    uint8 gates_destroyed;
};

class BattlegroundSA : public Battleground
{
    public:

        typedef std::map<BG_SA_BoolState, bool>             SotABoolStateMap;
        typedef std::map<BG_SA_UInt32State, uint32>         SotAUInt32StateMap;
        typedef std::map<uint32 /*type*/, uint32 /*timer*/> SotADemolisherMap;

        BattlegroundSA();
        ~BattlegroundSA();

        /** Called for ini battleground, after that the first player be entered */
        void FillInitialWorldStates(WorldPacket& data) override;
        bool SetupBattleground() override;

        /** Called every time for update battle data */
        void PostUpdateImpl(uint32 diff) override;

        /** Called when a player join battle */
        void AddPlayer(Player* pPlayer) override;

        /** Called when players join
         *
         * -Not used
         */
        void StartingEventCloseDoors() override;

        /** Called when battle starts
         *
         * -Not used
         */
        void StartingEventOpenDoors() override;

        /**
         * Called when a player deal damage to building (door)
         *
         * @param pPlayer : Player who damaged pObj
         * @param pObj : Gameobject which is damaged
         * @param eventType : Damaged / Destroyed
         */
        void EventPlayerDamagedGO(Player* pPlayer, GameObject* pObj, uint32 eventType) override;

        /**
         * Called when a player kill a unit in bg
         *
         * @param pCreature : Creature which has been killed
         * @param pPlayer : Player who killed pCreature
         */
        void HandleKillUnit(Creature* pCreature, Player* pPlayer) override;

        /** Return the nearest graveyard where player can respawn */
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* pPlayer) override;

        /**
         * Called when a player clicked on gameobject (graveyard flag)
         *
         * -After the cast bar has finished
         * @param pPlayer : Player who clicked on pObj
         * @param pObj : Gameobject which has been clicked
         */
        void EventPlayerClickedOnFlag(Player* pPlayer, GameObject* pObj) override;

        /**
         * Called when a player used an gameobject (titan relic)
         *
         * @param pPlayer : Player who used pObj
         * @param pObj : Gameobject which has been used
         */
        void EventPlayerUsedGO(Player* pPlayer, GameObject* pObject) override;

        /** Called to get uint32 data */
        uint32 GetData(uint32 type) override;

        /** Called to do an action */
        void DoAction(uint32 type, uint64 value) override;

        /** Called when an areatrigger has been activated */
        void HandleAreaTrigger(Player* pPlayer, uint32 trigger) override;

        /** Update score board */
        void UpdatePlayerScore(Player* pPlayer, uint32 type, uint32 value, bool doAddHonor = true) override;

    private:

        /** Called indirect for ini battleground, after that the first player be entered */
        void ResetBGSubclass() override;

        /** Called when a player leave battleground
        *
        * +Dismount player (vehicle)
        */
        void RemovePlayer(Player* pPlayer, uint64 guid, uint32 team) override;

        /**
        * Called when a gate is destroyed
        *
        * @param pPlayer : Player who destroyed the gate (score)
        * @param pObj : Gameobject which has been clicked
        */
        void DestroyGate(Player* pPlayer, GameObject* pObj) override;

        /** Called on setup and between the two round */
        bool resetBattleground();

        /** Spawn all creatures */
        bool spawnCreatures();

        /** Despawn all creatures */
        void despawnCreatures();

        /** Spawn all gameobjects */
        bool spawnGameObjects();

        /** Despawn all gameobjects */
        void despawnGameObjects();

        /** Reset intern states / worldstates and transports*/
        void resetStates();

        /**
         * Called to start the second round
         *
         * +Save round timer
         * +Switch attacker team
         * +Set states
         * +Call resetBattleground
         */
        void initSecondRound();

        /** 
         * Called to spawn invisible walls
         *
         * -Used to let player not run into the battleground
         * -Only needed when player will be added when ships are started
         */
        void spawnInvisibleWalls();

        /** Called to despawn invisible walls */
        void despawnInvisibleWalls();

        /**
         * Called to finish and score the battleground
         *
         * +Calculate winnerTeam
         * +Call endBattleground if there is no tie
         */
        void finishBattleground();

        /**
        * Called on battleground ending
        *
        * +Reward honor to winner / loser team
        * +Call Battleground::EndBattleground
        */
        void endBattleground(uint32 winnerTeam);

        /**
         * Called to get gateId from event which has been executed
         *
         * @param id : Enum BG_SA_EventIdGate
         * @return : ObjectId from enum BG_SA_ObjectType
         */
        int32 getGateIdFromDamagedOrDestroyEventId(uint32 id)
        {
            switch (id)
            {
                // Green gate
                case BG_SA_EVENT_GREEN_GATE_DAMAGED:
                case BG_SA_EVENT_GREEN_GATE_DESTROYED:
                    return BG_SA_GREEN_GATE;
                // Blue gate
                case BG_SA_EVENT_BLUE_GATE_DAMAGED:
                case BG_SA_EVENT_BLUE_GATE_DESTROYED:
                    return BG_SA_BLUE_GATE;
                // Red gate
                case BG_SA_EVENT_RED_GATE_DAMAGED:
                case BG_SA_EVENT_RED_GATE_DESTROYED:
                    return BG_SA_RED_GATE;
                // Purple gate
                case BG_SA_EVENT_PURPLE_GATE_DAMAGED:
                case BG_SA_EVENT_PURPLE_GATE_DESTROYED:
                    return BG_SA_PURPLE_GATE;
                // Yellow gate
                case BG_SA_EVENT_YELLOW_GATE_DAMAGED:
                case BG_SA_EVENT_YELLOW_GATE_DESTROYED:
                    return BG_SA_YELLOW_GATE;
                // Ancient gate
                case BG_SA_EVENT_ANCIENT_GATE_DAMAGED:
                case BG_SA_EVENT_ANCIENT_GATE_DESTROYED:
                    return BG_SA_ANCIENT_GATE;
                default:
                    return -1; // not found
                break;
            }
        }

        /**
         * Called to get the worldstate of a gate
         *
         * @param id : Gate from enum BG_SA_ObjectType
         * @return : Worldstate from enum BG_SA_Worldstates
         */
        uint32 getWorldStateFromGateId(uint32 id)
        {
            switch (id)
            {
                case BG_SA_GREEN_GATE:
                    return BG_SA_GREEN_GATEWS;
                case BG_SA_YELLOW_GATE:
                    return BG_SA_YELLOW_GATEWS;
                case BG_SA_BLUE_GATE:
                    return BG_SA_BLUE_GATEWS;
                case BG_SA_RED_GATE:
                    return BG_SA_RED_GATEWS;
                case BG_SA_PURPLE_GATE:
                    return BG_SA_PURPLE_GATEWS;
                case BG_SA_ANCIENT_GATE:
                    return BG_SA_ANCIENT_GATEWS;
                default:
                    break;
            }
            return 0;
        }

        /**
         * Called to teleport players to the right location
         *
         * +Remove aura spirit of redemption
         * +Resurrect player and make him ready for battleground (health, power,...)
         * +Port to correct location (attacker(ship) : defender(south graveyard))
         */
        void teleportPlayers();

        /** Called to port the player to a player who is on the boat
         *
         * @todo Implement
         */
        void teleportPlayerAfterShipStarted(Player* pPlayer);

        /** Set selectable or not demolisher, called on battle start, when ships arrive to dock */
        void demolisherStartState(bool start);

        /**
         * Called to update the visual timer and worldstates
         *
         * +Calculate end of round time
         * +Update worldstates
         */
        void sendTime();

        /**
         * Called when a player click on flag (graveyard flag)
         *
         * @param gId : Graveyard from enum BG_SA_Graveyards
         * @param pPlayer : Player who has captured the graveyard
         */
        void captureGraveyard(BG_SA_Graveyards gId, Player* pPlayer);

        /**
         * Called to relocate dead players when graveyard has been captured
         *
         * +Find players at graveyard gId
         * +Call GetClosestGraveyard to relocate players
         * @param gId : Graveyard from enum BG_SA_Graveyards
         */
        void relocateDeadPlayersAtGraveyard(BG_SA_Graveyards gId);

        /**
         * Called to switch de demolisher engineer of the graveyard
         *
         * +Change visibility of the demolisher engineers (based on array graveyardStatus)
         * @param gId : Graveyard from enum BG_SA_Graveyards
         */
        void switchDemolisherEngineer(BG_SA_Graveyards gId);

        /** Switch on/off timer worldstate */
        void toggleTimer();

        /**
         * Called to respawn dead demolisher
         *
         * +Respawn beach demolisher at home position
         * +Delete demolisher which are spawned by engineers
         */
        void updateDemolisherSpawns();

        /** Called to activate the ships and send a packet to all players to let it go */
        void startBGTransport();

        /**
         * Called to send a packet to all players for creating/deleting the ships
         *
         * @param remove : If true, the ships will be removed (OutOfRange packet)
         */
        void sendBGTransport(bool remove = false);

        /**
         * Called to send a packet to a single player to create the ships
         *
         * +Get updateData from ships
         * +Build worldpacket with updateData
         * @param pPlayer : Player who should receive the packet
         */
        void sendTransportInit(Player* pPlayer);

        /**
         * Called to send a packet to a single player to delete the ships
         *
         * +Get updateData from ships
         * +Build worldpacket with updateData
         * @param pPlayer : Player who should receive the packet
         */
        void sendTransportsRemove(Player* pPlayer);

        /**
         * Called when a gameobject door is damaged
         *
         * +Update worldstates
         * +Send warning to all players
         * @param pObj : Gameobject which has been damaged
         * @param gateId : Id of the gate from enum BG_SA_OBJECTs
         */
        void eventGateDamaged(GameObject* pObj, int32 gateId);

        /**
         * Called when a gameobject door is destroyed
         *
         * +Update worldstates
         * +Send warning to all players
         * @param pObj : Gameobject which has been damaged
         * @param gateId : Id of the gate from enum BG_SA_ObjectType
         */
        void eventGateDestroyed(GameObject* pObj, int32 gateId);

        /**
         * Called to check demolisher count
         *
         * -Used to check if we can build more demolisher by engineer
         * -There are 2 slots for each captured graveyard
         * @return : true if there are free slots to build demolisher
         */
        bool checkDemolisherCount();

        /**
         * Called by creatureScript to spawn a demolisher
         *
         * @param engineerGUID : GUID of the creature (east / west)
         * @param team : Needed to set the faction of the creature based on array BG_SA_Factions
         */
        void spawnDemolisher(uint64 engineerGUID, TeamId team);

        /**
         * Called to spawn a demolisher
         *
         * @param spawnData : Const struct of spawn locations (demolisherSpawns - saved in header)
         * @param team : Needed to set the faction of the creature based on array BG_SA_Factions
         */
        void spawnBattleDemolisher(const StrandOfTheAncientsSpawnData& spawnData, TeamId team);

        template <class Type>
        /**
         * Called indirect from achievmentscripts
         *
         * @param achievment : Id of the achievment from enum BG_SA_Achievments
         * @param addValue : Template type if needed to check the achievment (If you dont want to use it, set it to NULL)
         */
        bool checkAchievmentCriteria(BG_SA_Achievments achievment, Type addValue);

        /** Called to set winner if battle has ended because of less players */
        uint32 getPrematureWinner() override
        {
            if (roundScores[0].time && roundScores[1].time)
            {
                if (roundScores[0].time < roundScores[1].time)
                    return (roundScores[0].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
                else if (roundScores[0].time > roundScores[1].time)
                    return (roundScores[1].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
            }
            return 0;
        }

        /** All bool related states */
        SotABoolStateMap boolStateMap;

        /** All uint32 related states */
        SotAUInt32StateMap uint32StateMap;

        /** status of each gate (Destroy/Damage/Intact) */
        BG_SA_GateState gateStatus[6];

        /** Statu of battle (Start or not, and what round) */
        BG_SA_Status status;

        /** Id of attacker team */
        TeamId attackers;

        /** Team witch conntrol each graveyard */
        TeamId graveyardStatus[BG_SA_MAX_GY];

        /** Score of each round */
        StrandOfTheAncientsRoundScore roundScores[2];

        /** Respawn list of beach demolisher */
        SotADemolisherMap demoliserRespawnList;

        /** Delete list of demolisher */
        SotADemolisherMap demoliserDeleteList;

        /** Achievement: Not Even a Scratch */
        bool notEvenAScratch[BG_TEAMS_COUNT];
};

template <class Type>
bool BattlegroundSA::checkAchievmentCriteria(BG_SA_Achievments achievment, Type addValue)
{
    switch (achievment)
    {
        case BG_SA_ACHIEVMENT_DEFENSE_OF_THE_ANCIENTS:
            return !boolStateMap[STATE_ACHIEVMENT_DOTA];
        case BG_SA_ACHIEVMENT_NOT_EVEN_A_SCRETCH:
            return notEvenAScratch[GetTeamIndexByTeamId(addValue)];
        default:
            return false;
    }
}

#endif
