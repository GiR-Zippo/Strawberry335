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

#ifndef _SPELL_WINTERGRASP_H
#define _SPELL_WINTERGRASP_H

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "Data_BattlefieldWG.h"

#define CALC_BASE_ITEMLVL               200
#define CALC_NON_VEHICLE_DAMAGE_FACTOR  0.5f
#define CALC_DISTANCE_FACTOR            (1 - (0.085 * dist))


/** Wintergrasp Cannon - Spell Fire Cannon - ID: 51673 */
struct SpellWGCannonSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGCannonSpellScript);

    void RecalculateDamage();
    void Register();
};

class SpellWGCannon : public SpellScriptLoader
{
    public:

        SpellWGCannon() : SpellScriptLoader("spell_wg_cannon") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellWGCannonSpellScript();
        };
};


/**
 * Wintergrasp Demolisher   - Spell Hurl Boulder  - ID: 50999
 *
 * Wintergrasp Catapult     - Spell Plague Barrel - ID: 50026
 *
 * Wintergrasp Siege Engine - Spell Fire Cannon   - ID: 51383
 */
struct SpellWGVehicleRangedSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGVehicleRangedSpellScript);

    void RecalculateDamage();
    void Register();
};

class SpellWGVehicleRanged : public SpellScriptLoader
{
    public:

        SpellWGVehicleRanged() : SpellScriptLoader("spell_wg_vehicle_ranged") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellWGVehicleRangedSpellScript();
        };
};


/**
 * Wintergrasp Demolisher   - Spell Ram - ID: 50652
 *
 * Wintergrasp Siege Engine - Spell Ram         - ID: 51678
 *                          - Spell Steam Rush  - ID: 67798
 */
enum eSpellWGVehicleMelee
{
    SPELL_STEAM_RUSH = 67798
};

struct SpellWGVehicleMeleeSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGVehicleMeleeSpellScript);

    void RecalculateDamage();
    void Register();
};

class SpellWGVehicleMelee : public SpellScriptLoader
{
    public:

        SpellWGVehicleMelee() : SpellScriptLoader("spell_wg_vehicle_melee") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellWGVehicleMeleeSpellScript();
        };
};


/** Wintergrasp Catapult - Aura Plague Slime - ID: 61858 */
struct SpellWGCatapultPlagueSlimeAuraScript : public AuraScript
{
    public:

        PrepareAuraScript(SpellWGCatapultPlagueSlimeAuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
        void RecalculateEffectAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated);
        void Register();

    private:

        // Target of the Aura
        Unit* pUnit;
};

class SpellWGCatapultPlagueSlime : public SpellScriptLoader
{
    public:

        SpellWGCatapultPlagueSlime() : SpellScriptLoader("spell_wg_catapult_plague_slime") { }

        AuraScript* GetAuraScript() const
        {
            return new SpellWGCatapultPlagueSlimeAuraScript();
        };
};


/** Wintergrasp Catapult - Spell Flame Breath - ID: 50990 */
struct SpellWGCatapultFlameBreathSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGCatapultFlameBreathSpellScript);

    void RecalculateDamage();
    void Register();
};

class SpellWGCatapultFlameBreath : public SpellScriptLoader
{
    public:

        SpellWGCatapultFlameBreath() : SpellScriptLoader("spell_wg_catapult_flame_breath") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellWGCatapultFlameBreathSpellScript();
        };
};


/** Wintergrasp Engineer - Spell Force Building Vehicles - ID: 56664 / 56659 / 56662 /  61409/ */
enum SpellWGBuilding
{
    SPELL_WINTERGRASP_BUILD_CATAPULT_FORCE                  = 56664,
    SPELL_WINTERGRASP_BUILD_DEMOLISHER_FORCE                = 56659,
    SPELL_WINTERGRASP_BUILD_SIEGE_VEHICLE_FORCE_ALLIANCE    = 56662,
    SPELL_WINTERGRASP_BUILD_SIEGE_VEHICLE_FORCE_HORDE       = 61409,
};

struct SpellWGForceBuildingSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGForceBuildingSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellWGForceBuilding : public SpellScriptLoader
{
    public:

        SpellWGForceBuilding() : SpellScriptLoader("spell_wg_force_building") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellWGForceBuildingSpellScript();
        }
};


/** Wintergrasp Vehicle - Spell Grab Passenger - ID: 61178 */
enum SpellWGPassenger
{
    SPELL_RIDE_WG_VEHICLE = 60968,
};

struct SpellWGGrabPassengerSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGGrabPassengerSpellScript);

    void HandleScript(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellWGGrabPassenger : public SpellScriptLoader
{
    public:

        SpellWGGrabPassenger() : SpellScriptLoader("spell_wg_grab_passenger") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellWGGrabPassengerSpellScript();
        }
};


/** Wintergrasp Teleport - Spell Teleport - ID: 54643 */
enum SpellWGTeleport
{
    SPELL_WINTERGRASP_DEFENDER_TELEPORT = 54643,
};

struct SpellWGDefenderTeleportSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGDefenderTeleportSpellScript);

    SpellCastResult CheckCast();
    void HandleDummy(SpellEffIndex /*effindex*/);
    void Register();
};

class SpellWGDefenderTeleport : public SpellScriptLoader
{
    public:

        SpellWGDefenderTeleport() : SpellScriptLoader("spell_wg_defender_teleport") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellWGDefenderTeleportSpellScript();
        }
};


/** Wintergrasp Portal Teleport - Spell Teleport - ID: 58622 */
struct SpellWGPortalTeleportSpellScript : public SpellScript
{
    PrepareSpellScript(SpellWGPortalTeleportSpellScript);

    void HandleScript(SpellEffIndex /*effindex*/);
    void Register();
};

class SpellWGPortalTeleport : public SpellScriptLoader
{
    public:

    SpellWGPortalTeleport() : SpellScriptLoader("spell_wg_portal_teleport") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellWGPortalTeleportSpellScript();
    }
};


/** Wintergrasp Vehicle Teleport - Spell Teleport - ID: 49759 */
struct SpellWGVehicleTeleportSpellScript : public SpellScript
{
    public:

        PrepareSpellScript(SpellWGVehicleTeleportSpellScript);

        SpellCastResult CheckCast();
        void HandleTeleport(SpellEffIndex /*effindex*/);
        void Register();

    private:

        /**
         * Used to teleport vehicle and players
         *
         * +Exit vehicle
         * +Teleport players
         * +Teleport vehicle
         * +Enter vehicle
         * @param pUnit : Player which triggered this spell
         * @param pCreature : World trigger teleport destination
         */
        void ExitVehicleAndTeleport(Vehicle* pVehicle, Creature* pCreature);
};

class SpellWGVehicleTeleport : public SpellScriptLoader
{
    public:

    SpellWGVehicleTeleport() : SpellScriptLoader("spell_wg_vehicle_teleport") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellWGVehicleTeleportSpellScript();
    }
};


/** Wintergrasp Vehicle - Spell/Aura Drive Vehicle - ID: 60968 */
enum eSpellWGDriveVehicle
{
    WG_SPELL_VEHICLE_SCALING    = 65635,
    SOTA_SPELL_VEHICLE_SCALING  = 65636
};

struct SpellWGDriveVehicleAuraScript : public AuraScript
{
    PrepareAuraScript(SpellWGDriveVehicleAuraScript);

    void HandleAfterEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void HandleAfterEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellWGDriveVehicle : public SpellScriptLoader
{
    public:

    SpellWGDriveVehicle() : SpellScriptLoader("spell_wg_drive_vehicle") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellWGDriveVehicleAuraScript();
    };
};


/** Wintergrasp Rocket Launcher - Spell Rocket-Propelled Goblin Grenade - ID: 49761 */
enum eSpellWGRocketLauncher
{
    WG_SPELL_ROCKET_LAUNCHER_TRIGGER = 49769,
};

struct SpellWGRocketLauncherSpellScript : public SpellScript
{
    public:

    PrepareSpellScript(SpellWGRocketLauncherSpellScript);

    void HandleDummy(SpellEffIndex /*effindex*/);
    void Register();
};

class SpellWGRocketLauncher : public SpellScriptLoader
{
    public:

    SpellWGRocketLauncher() : SpellScriptLoader("spell_wg_rocket_launcher") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellWGRocketLauncherSpellScript();
    }
};


#endif // _SPELL_WINTERGRASP_H
