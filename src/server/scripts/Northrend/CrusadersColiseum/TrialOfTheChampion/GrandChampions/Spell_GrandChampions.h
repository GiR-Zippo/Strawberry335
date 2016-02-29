/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
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

#ifndef _SPELL_GRAND_CHAMPIONS_H
#define _SPELL_GRAND_CHAMPIONS_H

#include "Vehicle_GrandChampions.h"


class Spell_GrandChampions_RideMount : public SpellScriptLoader
{
    public:

    Spell_GrandChampions_RideMount() : SpellScriptLoader("spell_toc5_ride_mount") {}

    class Spell_GrandChampions_RideMountSpellScript : public SpellScript
    {
        PrepareSpellScript(Spell_GrandChampions_RideMountSpellScript);

        SpellCastResult CheckRequirement();
        void Register() override;
    };

    class Spell_GrandChampions_RideMountAuraScript : public AuraScript
    {
        PrepareAuraScript(Spell_GrandChampions_RideMountAuraScript);

        void HandleEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
        void Register() override;
    };

    SpellScript* GetSpellScript() const override
    {
        return new Spell_GrandChampions_RideMountSpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new Spell_GrandChampions_RideMountAuraScript();
    }
};

class Spell_GrandChampions_Defend : public SpellScriptLoader
{
    public:

    Spell_GrandChampions_Defend() : SpellScriptLoader("spell_toc5_defend") { }

    class Spell_GrandChampions_DefendAuraScript : public AuraScript
    {
        PrepareAuraScript(Spell_GrandChampions_DefendAuraScript);

        bool Validate(SpellInfo const* /*spellEntry*/);

        void RefreshVisualShields(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
        void Register() override;
    };

    AuraScript* GetAuraScript() const override
    {
        return new Spell_GrandChampions_DefendAuraScript();
    }
};

class Player_GrandChampions_HexMendingAI : public PlayerAI
{
    public:

        Player_GrandChampions_HexMendingAI(Player* player) : PlayerAI(player) {}

        void HealReceived(Unit* healer, uint32& addHealth);
        void UpdateAI(const uint32 /*diff*/) { }

    private:

        uint64 casterGUID;
};

class Spell_GrandChampions_HexMending : public SpellScriptLoader
{
    public:

    Spell_GrandChampions_HexMending() : SpellScriptLoader("spell_toc5_hex_mending") { }

    class Spell_GrandChampions_HexMendingAuraScript : public AuraScript
    {
        public:

            PrepareAuraScript(Spell_GrandChampions_HexMendingAuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
            void Register() override;

        private:

            UnitAI* oldAI;
            bool oldAIState;
    };

    AuraScript* GetAuraScript() const override
    {
        return new Spell_GrandChampions_HexMendingAuraScript();
    }
};


#endif // _SPELL_GRAND_CHAMPIONS_H
