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
 * The class WintergraspCapturePoint handles capturale points
 * This class is inherited by class BfCapturePoint
 * There are many other classes which are needed to control the capturable point
 */

#ifndef _WORKSHOP_BATTLEFIELDWG_H
#define _WORKSHOP_BATTLEFIELDWG_H

#include "Battlefield.h"
#include "Data_BattlefieldWG.h"

class BattlefieldWG;


enum WintergraspFactoryBannerArtKit
{
    GO_ARTKIT_FACTORY_HORDE            = 1,
    GO_ARTKIT_FACTORY_ALLIANCE         = 2,
    GO_ARTKIT_FACTORY_NEUTRAL          = 21
};

enum WintergraspWorldstates
{
    WORLDSTATE_WORKSHOP_NE             = 3701,
    WORLDSTATE_WORKSHOP_NW             = 3700,
    WORLDSTATE_WORKSHOP_SE             = 3703,
    WORLDSTATE_WORKSHOP_SW             = 3702,
    WORLDSTATE_WORKSHOP_K_W            = 3698,
    WORLDSTATE_WORKSHOP_K_E            = 3699
};

class WintergraspWorkshopData
{
    public:

        WintergraspWorkshopData(BattlefieldWG* WG, uint8 workshopId);

        uint8 GetId() const                 { return m_workshopId; }
        uint32 GetTeam() const              { return m_TeamControl; }
        uint32 GetWorldState() const        { return m_WorldState; }
        uint32 GetState() const             { return m_State; }
        uint32 GetCapturePointEntry() const { return m_FactoryEntry; }

        /** Spawning associate creature and store them */
        void AddCreature(const WintergraspObjectPositionData& obj);

        /** Spawning associate gameobject and store them */
        void AddGameObject(const WintergraspObjectPositionData& obj);

        /** Spawning associate rocket launcher and store them */
        void AddRocketLauncher(const WintergraspObjectPositionData& obj);

        /** Initialize the workshop */
        void Init(uint8 team);

        /** Called on change faction in CapturePoint class */
        void GiveControlTo(uint8 team);

        /** Called at battle-start from BattlefieldWG */
        void OnBattleStart();

        /** Called at battle-end from BattlefieldWG after UpdateTeam() and before Save() */
        void OnBattleEnd();

        void UpdateGraveyard();
        void Save();

    private:

        BattlefieldWG* m_WG; /** Pointer to wintergrasp */
        uint8 m_workshopId; /** Id of the workshop, useful to retrieve data of the WorkshopsData array */
        uint32 m_FactoryEntry; /** Entry of the factory banner (capture point) */
        uint64 m_FactoryGUID; /** GUID of the factory banner */
        uint32 m_WorldState; /** worldstate Id */
        uint32 m_State; /** current State */
        uint32 m_TeamControl; /** Team witch control the workshop */
        uint64 m_Engineer[2]; /** Contains the mechanic engineers */
        GuidSet m_CreatureOnPoint[2]; /** Contain all Creature associate to this point */
        GuidSet m_GameObjectOnPoint[2]; /** Contain all Gameobject associate to this point */
        GuidSet m_RocketLauncherOnPoint; /** Contain all Rocket Launchers associate to this point */
        uint32 m_NameId; /** Id of trinity_string witch contain name of this node, using for alert message */
};

/**
 * WintergraspCapturePoint 
 */
class WintergraspCapturePoint : public BfCapturePoint
{
    public:

        WintergraspCapturePoint(BattlefieldWG* battlefield, TeamId teamInControl);
        ~WintergraspCapturePoint() { DeleteMe(); };

        void LinkToWorkshop(WintergraspWorkshopData* workshop) { m_Workshop = workshop; }
        WintergraspWorkshopData* GetWorkshop() const { return m_Workshop; }

        void ChangeTeam(TeamId oldteam);
        TeamId GetTeam() const { return m_team; }

    protected:

        WintergraspWorkshopData* m_Workshop;

    private:

        void DeleteMe() { DelCapturePoint(); }
};

struct WintergraspFortressWorkshopData
{
    uint8 workshopEntry; /** Workshop Entry to name it in the battlefield script */
    uint32 worldState; /** Workshop Worldstate (Area POI etc.) */

    WintergraspObjectPositionData Workshop;
    WintergraspObjectPositionData Engineer;
    WintergraspObjectPositionData SpiritGuide; /** Spirit Guide Horde/Alliance */
};

const WintergraspFortressWorkshopData FortressWorkshops[WG_MAX_WORKSHOP - WG_CAPTURABLE_WORKSHOP] =
{
    // Fortress West Workshop
    {
        BATTLEFIELD_WG_WORKSHOP_KEEP_WEST,
        WORLDSTATE_WORKSHOP_K_W,
        { 5389.640137f, 2978.280029f, 409.677000f, 1.53589f, BATTLEFIELD_WG_GO_WORKSHOP_BASE_W, 0 }, // Workshop
        { 5391.949219f, 2976.756592f, 415.140411f, 4.71511f, BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER, BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER }, // Engineer
        { 5537.806641f, 2896.289307f, 517.055664f, 4.79835f, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE } // Spirit Guide
    },

    // Fortress East Workshop
    {
        BATTLEFIELD_WG_WORKSHOP_KEEP_EAST,
        WORLDSTATE_WORKSHOP_K_E,
        { 5387.970215f, 2710.790039f, 409.505005f, 1.53589f, BATTLEFIELD_WG_GO_WORKSHOP_BASE_E, 0 }, // Workshop
        { 5390.283691f, 2709.196045f, 414.967834f, 4.61904f, BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER, BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER }, // Engineer
    }
};

struct WintergraspCapturableWorkshopData
{
    uint8 workshopEntry; /** Workshop Entry to name it in the battlefield script */
    uint32 worldState; /** Workshop Worldstate (Area POI etc.) */
    uint32 nameId; /** Workshop NameId for trinity_string */

    WintergraspObjectPositionData GameObjectMain[2]; /** Workshop and Factory Flag Data */

    WintergraspObjectPositionData Engineer; /** Mechanic Engineer Horde/Alliance */
    WintergraspObjectPositionData SpiritGuide; /** Spirit Guide Horde/Alliance */

    uint8 nbBanner; /** Number of banner spawned on this point */
    WintergraspObjectPositionData GameObjectBanner[6]; /** Gameobject Banner position and entry (Horde/Alliance) */

    uint8 nbCreature; /** Number of creatures spawned on this point */
    WintergraspObjectPositionData Creature[10]; /** Guards */

    uint8 nbRocketLauncher; /** Number of rocket launchers spawned on this point */
    WintergraspObjectPositionData RocketLauncher[5]; /** Useable rocket launchers */
};

const WintergraspCapturableWorkshopData CapturableWorkshops[WG_CAPTURABLE_WORKSHOP] =
{
    // North-East Workshop
    {
        BATTLEFIELD_WG_WORKSHOP_NE,
        WORLDSTATE_WORKSHOP_NE,
        LANG_WG_SUNKEN_RING_WS,
        {
            { 4942.259766f, 2392.649902f, 320.60700f, -0.20944f, BATTLEFIELD_WG_GO_WORKSHOP_NE, 0       }, // Workshop
            { 4949.339844f, 2432.590088f, 320.17700f,  1.38621f, BATTLEFIELD_WG_GO_FACTORY_BANNER_NE, 0 }, // Factory Banner
        },
            { 4940.168945f, 2390.480957f, 326.07037f, 2.92014f, BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER, BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER }, // Engineer
            { 5104.750000f, 2300.949951f, 368.56799f, 0.73304f, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE }, // Spirit Guide
        6,
        {
            { 5021.772461f, 2531.871338f, 343.905940f, 1.268385f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner W Ground - Way to the fortress
            { 4778.312500f, 2438.041260f, 346.224121f, 3.241296f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner S Ground - Way to the bridge
            { 5006.325195f, 2487.825439f, 358.241882f, 2.177791f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner WN - Way to the fortress
            { 4984.259277f, 2504.999023f, 358.092224f, 5.814188f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner WS - Way to the fortress
            { 4809.707031f, 2442.481689f, 357.884857f, 4.215194f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner SW - Way to the bridge
            { 4803.993652f, 2407.298096f, 357.996368f, 1.741187f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner SE - Way to the bridge
        },
        4,
        {
            { 5002.241211f, 2489.858887f, 332.06564f, 1.01946f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard EN - Way to the fortress
            { 4986.420410f, 2500.727295f, 332.12381f, 0.99590f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard ES - Way to the fortress
            { 4815.763184f, 2433.619629f, 332.38324f, 2.89656f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard SW - Way to the bridge
            { 4810.611328f, 2413.015869f, 333.00952f, 2.92013f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard SE - Way to the bridge
        },
        5,
        {
            { 4925.252930f, 2425.430664f, 320.177490f, 1.529726f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4926.789648f, 2425.330664f, 320.177490f, 1.529726f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4928.252930f, 2425.230664f, 320.177490f, 1.529726f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4929.752930f, 2425.130664f, 320.177490f, 1.529726f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4931.252930f, 2425.030664f, 320.177490f, 1.529726f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 }
        }
    },

    // North-West Workshop
    {
        BATTLEFIELD_WG_WORKSHOP_NW,
        WORLDSTATE_WORKSHOP_NW,
        LANG_WG_BROKEN_TEMPLE_WS,
        {
            { 4961.609863f, 3380.939941f, 377.36401f, 2.78380f, BATTLEFIELD_WG_GO_WORKSHOP_NW, 0       }, // Workshop
            { 4948.520020f, 3342.340088f, 376.87500f, 4.40057f, BATTLEFIELD_WG_GO_FACTORY_BANNER_NW, 0 }, // Factory Banner
        },
            { 4963.868164f, 3382.788818f, 382.82736f, 5.98645f, BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER, BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER }, // Engineer
            { 5103.129883f, 3462.129883f, 368.36799f, 5.27090f, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE }, // Spirit Guide
        4,
        {
            { 5002.000000f, 3282.463867f, 373.071533f, 5.409717f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner E Ground - Way to the fortress
            { 4865.221680f, 3335.911865f, 370.485779f, 3.547886f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner S Ground - Way to the bridge
            { 5040.296387f, 3294.233398f, 381.438538f, 4.658908f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner E - Way to the fortress
            { 4855.409180f, 3296.979980f, 376.207397f, 3.178742f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner S - Way to the fortress
        },
        4,
        {
            { 4862.824707f, 3332.149902f, 370.47000f, 3.47534f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard SW - Way to the bridge
            { 4868.052734f, 3312.697021f, 369.33231f, 3.47534f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard SE - Way to the bridge
            { 5007.072266f, 3282.638184f, 371.83893f, 5.29838f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard ES - Way to the fortress
            { 5022.916992f, 3293.331299f, 370.27155f, 5.29838f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard EN - Way to the fortress
        },
        5,
        {
            { 4973.705566f, 3346.380615f, 376.877136f, 4.196353f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4971.671875f, 3347.534180f, 376.877136f, 4.196353f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4969.585938f, 3348.733154f, 376.877136f, 4.196353f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4967.513672f, 3349.893311f, 376.877136f, 4.196353f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4965.382324f, 3351.102295f, 376.877136f, 4.196353f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 }
        }
    },

    // South-East Workshop
    {
        BATTLEFIELD_WG_WORKSHOP_SE,
        WORLDSTATE_WORKSHOP_SE,
        LANG_WG_EASTPARK_WS,
        {
            { 4360.979980f, 2355.469971f, 376.45999f, -1.56207f, BATTLEFIELD_WG_GO_WORKSHOP_SE, 0       }, // Workshop
            { 4398.080078f, 2356.500000f, 376.19000f,  0.03213f, BATTLEFIELD_WG_GO_FACTORY_BANNER_SE, 0 }, // Factory Banner
        },
            { 4358.702148f, 2356.959717f, 381.92380f, 1.55266f, BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER, BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER }, // Engineer
            { 4318.439941f, 2408.070068f, 392.67600f, 6.23083f, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE }, // Spirit Guide
        5,
        {
            { 4417.556152f, 2323.722900f, 371.822479f, 6.112195f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner N Ground - Way to the bridge
            { 4408.597168f, 2422.910400f, 377.308685f, 1.600078f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner W - Way to the south tower
            { 4416.957520f, 2414.805664f, 377.383850f, 0.001786f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner N - Way to the south tower
            { 4418.065918f, 2300.199219f, 377.377380f, 0.001794f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner N - Way to the bridge
            { 4409.996094f, 2292.041992f, 377.393738f, 4.698477f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner E - Way to the east tower
        },
        9,
        {
            { 4348.913574f, 2410.278809f, 374.66040f, 1.58013f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard W Bottom - Stage to the south tower
            { 4370.211426f, 2410.220459f, 374.66064f, 1.58013f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard W Middle - Stage to the south tower
            { 4392.467773f, 2410.343750f, 374.66040f, 1.58013f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard W Top - Stage to the south tower
            { 4420.467285f, 2382.584717f, 374.70861f, 0.22925f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Stage to the bridge
            { 4416.991699f, 2354.782959f, 372.95700f, 6.02941f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Way to the bridge
            { 4416.156250f, 2331.903076f, 371.54315f, 6.08439f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Way to the bridge
            { 4393.599121f, 2302.676758f, 374.66037f, 4.72958f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard E Top - Stage to the east tower 
            { 4369.760742f, 2302.266602f, 374.66037f, 4.72958f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard E Middle - Stage to the east tower
            { 4348.539551f, 2301.901611f, 374.66037f, 4.72958f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard E Bottom - Stage to the east tower
        },
        5,
        {
            { 4383.031641f, 2337.474854f, 376.319733f, 6.198908f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4382.931641f, 2335.981934f, 376.319733f, 6.198908f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4382.831641f, 2334.481934f, 376.319733f, 6.198908f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4382.731641f, 2332.981934f, 376.319733f, 6.198908f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4382.631641f, 2331.481934f, 376.319733f, 6.198908f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 }
        }
    },

    // South-West Workshop
    {
        BATTLEFIELD_WG_WORKSHOP_SW,
        WORLDSTATE_WORKSHOP_SW,
        LANG_WG_WESTPARK_WS,
        {
            { 4357.520020f, 3309.530029f, 372.50000f, -1.78023f, BATTLEFIELD_WG_GO_WORKSHOP_SW, 0       }, // Workshop
            { 4390.779785f, 3304.090088f, 372.42898f,  6.09702f, BATTLEFIELD_WG_GO_FACTORY_BANNER_SW, 0 }, // Factory Banner
        },
            { 4355.462891f, 3311.499268f, 377.96310f, 1.35293f, BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER, BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER }, // Engineer
            { 4336.250000f, 3235.520020f, 390.33401f, 0.62831f, NPC_TAUNKA_SPIRIT_GUIDE, NPC_DWARVEN_SPIRIT_GUIDE }, // Spirit Guide
        4,
        {
            { 4403.331055f, 3372.827148f, 364.591125f, 1.563761f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner W Ground - Way to the west tower
            { 4422.371094f, 3287.217529f, 372.079834f, 0.005718f, BATTLEFIELD_WG_GO_BANNER_STAND_H, BATTLEFIELD_WG_GO_BANNER_STAND_A }, // Banner N Ground - Way to the bridge
            { 4438.585449f, 3359.450684f, 371.139801f, 6.230000f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner N - Way to the bridge
            { 4448.610840f, 3235.660889f, 369.908813f, 4.729885f, BATTLEFIELD_WG_GO_BANNER_FLAG_H, BATTLEFIELD_WG_GO_BANNER_FLAG_A   }, // Banner E - Way to the east tower
        },
        10,
        {
            { 4378.958496f, 3374.763184f, 365.68685f, 1.57284f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard W - Way to the west tower
            { 4396.651367f, 3374.799316f, 364.70254f, 1.54535f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard W - Way to the west tower
            { 4418.757324f, 3363.620117f, 372.30502f, 1.53842f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard W - Stage to the west tower
            { 4429.996582f, 3346.842773f, 372.30502f, 6.22725f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Stage to the bridge
            { 4417.343262f, 3320.927734f, 371.06497f, 0.00203f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Way to the bridge
            { 4422.546875f, 3301.928711f, 369.61969f, 0.29263f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Way to the bridge
            { 4451.208496f, 3278.143555f, 371.14486f, 0.04225f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Stage to the bridge
            { 4452.298828f, 3248.245361f, 371.14486f, 0.05308f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard N - Stage to the bridge
            { 4390.812500f, 3249.802979f, 371.99728f, 4.67123f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard E - Way to the south tower
            { 4372.213867f, 3249.706055f, 372.68536f, 4.77726f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }, // Guard E - Way to the south tower
        },
        5,
        {
            { 4374.698242f, 3284.169434f, 372.426941f, 5.978111f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4374.155273f, 3282.547363f, 372.426941f, 5.978111f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4373.594238f, 3280.764893f, 372.426941f, 5.978111f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4373.033203f, 3278.982178f, 372.426941f, 5.978111f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 },
            { 4372.549805f, 3277.446777f, 372.426941f, 5.978111f, BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, 0 }
        }
    }
};

typedef std::set<WintergraspWorkshopData*> Workshop;


#endif // _WORKSHOP_BATTLEFIELDWG_H