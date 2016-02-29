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

#ifndef _MISC_BATTLEFIELDWG_H
#define _MISC_BATTLEFIELDWG_H

#include "Battlefield.h"
#include "Data_BattlefieldWG.h"

const uint8 WG_MAX_ATTACKTOWERS = 3;
const uint8 WG_MAX_TOWER_CANNON = 7;

class BattlefieldWG;


/**
 * Graveyards
 */
class BfGraveyardWG : public BfGraveyard
{
    public:

        BfGraveyardWG(BattlefieldWG* Bf);

        void SetTextId(int32 textid) { m_GossipTextId = textid; }
        int32 GetTextId() { return m_GossipTextId; }

    protected:

        int32 m_GossipTextId;
};


/**
 * Tower Element
 *
 * -Creatures
 * -GameObjects
 */
struct WintergraspTowerData
{
    uint32 towerEntry; /** Gameobject id of tower */
    uint8 nbObject; /** Number of gameobjects spawned on this point */
    WintergraspObjectPositionData GameObject[6]; /** Gameobject position and entry (Horde/Alliance) */

    /**
     * Creature: Turrets and Guard
     * @todo: Killed on Tower destruction ? Tower damage ? Requires confirming
     */
    uint8 nbCreatureBottom;
    WintergraspObjectPositionData CreatureBottom[9];
    uint8 nbCreatureTop;
    WintergraspObjectPositionData CreatureTop[5];
};

// 192414 : 0 in sql, 1 in header
// BATTLEFIELD_WG_GO_BANNER_STAND_A : 0 in sql, 3 in header
const WintergraspTowerData AttackTowers[WG_MAX_ATTACKTOWERS] =
{
    // West tower
    {
        190356,
        6,
        {
            { 4559.109863f, 3606.219971f, 419.998993f, -1.483530f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4539.419922f, 3622.489990f, 420.033997f, -3.071770f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4555.259766f, 3641.649902f, 419.973999f, 1.675510f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4574.870117f, 3625.909912f, 420.079010f, 0.080117f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4433.899902f, 3534.139893f, 360.274994f, -1.850050f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Flag near workshop
            { 4572.930176f, 3475.520020f, 363.009003f, 1.42240f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A } // Flag near bridge
        },
        1,
        {
            { 4418.688477f, 3506.251709f, 358.975494f, 4.293305f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Roaming Guard
        },
        0,
        {
        }
    },

    // South Tower
    {
        190357,
        5,
        {
            { 4416.000000f, 2822.669922f, 429.851013f, -0.017452f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4398.819824f, 2804.699951f, 429.791992f, -1.588250f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4387.620117f, 2719.570068f, 389.934998f, -1.544620f, 192366, 192414 }, // Flag near tower
            { 4464.120117f, 2855.449951f, 406.110992f, 0.829032f, 192366, 192429 }, // Flag near tower
            { 4526.459961f, 2810.179932f, 391.200012f, -2.993220f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Flag near bridge
        },
        6,
        {
            { 4452.859863f, 2808.870117f, 402.604004f, 6.056290f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4455.899902f, 2835.958008f, 401.122559f, 0.034907f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4412.649414f, 2953.792236f, 374.799957f, 0.980838f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Roaming Guard
            { 4362.089844f, 2811.510010f, 407.337006f, 3.193950f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4412.290039f, 2753.790039f, 401.015015f, 5.829400f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4421.939941f, 2773.189941f, 400.894989f, 5.707230f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
        },
        0,
        {
        },
    },

    // East Tower
    {
        190358,
        4,
        {
            { 4466.790039f, 1960.420044f, 459.144012f, 1.151920f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4475.350098f, 1937.030029f, 459.070007f, -0.43633f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4451.759766f, 1928.099976f, 459.075989f, -2.00713f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
            { 4442.990234f, 1951.900024f, 459.092987f, 2.740160f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A }, // Flag on tower
        },
        5,
        {
            { 4501.060059f, 1990.280029f, 431.157013f, 1.029740f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4463.830078f, 2015.180054f, 430.299988f, 1.431170f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4494.580078f, 1943.760010f, 435.627014f, 6.195920f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4450.149902f, 1897.579956f, 435.045013f, 4.398230f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
            { 4428.870117f, 1906.869995f, 432.648010f, 3.996800f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Standing Guard
        },
        0,
        {
        },
    },
};


struct WintergraspTowerCannonData
{
    uint32 towerEntry;
    uint8 nbTowerCannonBottom;
    Position TowerCannonBottom[5];
    uint8 nbTurretTop;
    Position TurretTop[5];
};

const WintergraspTowerCannonData TowerCannon[WG_MAX_TOWER_CANNON] =
{
    {
        190221,
        0,
        {
        },
        2,
        {
            { 5255.88f, 3047.63f, 438.499f, 3.13677f },
            { 5280.9f, 3071.32f, 438.499f, 1.62879f },
        },
    },
    {
        190373,
        0,
        {
        },
        2,
        {
            { 5138.59f, 2935.16f, 439.845f, 3.11723f },
            { 5163.06f, 2959.52f, 439.846f, 1.47258f },
        },
    },
    {
        190377,
        0,
        {
        },
        2,
        {
            { 5163.84f, 2723.74f, 439.844f, 4.75971f },
            { 5139.69f, 2747.4f, 439.844f, 3.17221f },
        },
    },
    {
        190378,
        0,
        {
        },
        2,
        {
            { 5278.21f, 2607.23f, 439.755f, 4.71944f },
            { 5255.01f, 2631.98f, 439.755f, 3.15257f },
        },
    },
    {
        190356,
        3,
        {
            {4537.380371f, 3599.531738f, 402.886993f, 3.998462f},
            {4581.497559f, 3604.087158f, 402.886963f, 5.651723f},
            {4532.318848f, 3643.966064f, 402.886719f, 2.463250f},
        },
        3,
        {
            {4581.895996f, 3626.438477f, 426.539062f, 0.117806f},
            {4559.853516f, 3599.367188f, 426.539978f, 4.811275f},
            {4532.572754f, 3621.335449f, 426.539093f, 3.271900f},
        },
    },
    {
        190357,
        2,
        {
            { 4421.640137f, 2799.935791f, 412.630920f, 5.459298f },
            { 4420.263184f, 2845.340332f, 412.630951f, 0.742197f },
        },
        3,
        {
            { 4423.430664f, 2822.762939f, 436.283142f, 6.223487f },
            { 4397.825684f, 2847.629639f, 436.283325f, 1.579430f },
            { 4398.814941f, 2797.266357f, 436.283051f, 4.703747f },
        },
    },
    {
        190358,
        2,
        {
            { 4448.138184f, 1974.998779f, 441.995911f, 1.967238f },
            { 4488.824219f, 1955.311768f, 441.995636f, 0.346594f },
        },
        3,
        {
            { 4469.448242f, 1966.623779f, 465.647217f, 1.153573f },
            { 4481.996582f, 1933.658325f, 465.647186f, 5.873029f },
            { 4436.447266f, 1954.641968f, 465.647400f, 2.753834f },
        },
    },
};


/**
 * Structs using for Building, Graveyard, Workshop
 *
 * -Buildings which can be destroyed durng battle
 */
struct BfWGGameObjectBuilding
{
    BfWGGameObjectBuilding(BattlefieldWG* WG)
    {
        m_WG = WG;
        m_Team = 0;
        m_BuildGUID = 0;
        m_Type = 0;
        m_WorldState = 0;
        m_State = 0;
        m_NameId = 0;
    }

    /** The team which controls this point */
    uint8 m_Team;

    /** WG object */
    BattlefieldWG* m_WG;

    /** Linked gameobject */
    uint64 m_BuildGUID;

    /** eWGGameObjectBuildingType */
    uint32 m_Type;

    /** WorldState */
    uint32 m_WorldState;

    /** eWGGameObjectState */
    uint32 m_State;

    /** Name id for warning text */
    uint32 m_NameId;

    /** GameObject associations */
    GuidSet m_GameObjectList[2];

    /** Creature associations */
    GuidSet m_CreatureBottomList[2];
    GuidSet m_CreatureTopList[2];
    GuidSet m_TowerCannonBottomList;
    GuidSet m_TurretTopList;

    void Rebuild();

    /** Called when associated gameobject is damaged */
    void Damaged();

    /** Called when associated gameobject is destroyed */
    void Destroyed();

    void Init(GameObject* go, uint32 type, uint32 worldstate, uint32 nameid);
    void UpdateCreatureAndGo();
    void UpdateTurretAttack(bool disable);

    void Save();
};

typedef std::set<BfWGGameObjectBuilding*> GameObjectBuilding;


#endif // _MISC_BATTLEFIELDWG_H