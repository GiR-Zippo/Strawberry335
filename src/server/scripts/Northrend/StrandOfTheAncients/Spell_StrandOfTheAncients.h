/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 17.08.2014
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

#include "ScriptMgr.h"


/** SotA Carrying Seaforium - Aura - ID: 52418 */
enum SotACarryingSeaforium
{
    ITEM_MASSIVE_SEAFORIUM_CHARGE   = 39213,
};

struct SpellSotACarryingSeaforiumAuraScript : public AuraScript
{
    PrepareAuraScript(SpellSotACarryingSeaforiumAuraScript);

    void HandleAfterEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register() override;
};

class SpellSotACarryingSeaforium : public SpellScriptLoader
{
    public:

    SpellSotACarryingSeaforium() : SpellScriptLoader("spell_sota_carrying_seaforium") { }

    AuraScript* GetAuraScript() const override
    {
        return new SpellSotACarryingSeaforiumAuraScript();
    };
};

/** Strand Of The Ancients Vehicle - Spell/Aura Drive Vehicle - ID: 60683 */
enum eSpellSotADriveVehicle
{
    SOTA_SPELL_VEHICLE_SCALING = 65636
};

struct SpellSotADriveVehicleAuraScript : public AuraScript
{
    PrepareAuraScript(SpellSotADriveVehicleAuraScript);

    void HandleAfterEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register() override;
};

class SpellSotADriveVehicle : public SpellScriptLoader
{
    public:

    SpellSotADriveVehicle() : SpellScriptLoader("spell_sota_drive_vehicle") { }

    AuraScript* GetAuraScript() const override
    {
        return new SpellSotADriveVehicleAuraScript();
    };
};


/** Strand Of The Ancients - Spell End of Round - ID: 52459  */
struct SpellSotAEndOfRoundSpellScript : public SpellScript
{
    PrepareSpellScript(SpellSotAEndOfRoundSpellScript);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/);
    void Register() override;
};

class SpellSotAEndOfRound : public SpellScriptLoader
{
    public:

    SpellSotAEndOfRound() : SpellScriptLoader("spell_sota_end_of_round") { }

    SpellScript* GetSpellScript() const override
    {
        return new SpellSotAEndOfRoundSpellScript();
    };
};