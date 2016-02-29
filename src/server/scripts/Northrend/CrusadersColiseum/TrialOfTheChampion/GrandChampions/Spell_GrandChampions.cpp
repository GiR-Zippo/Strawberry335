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

#include "Spell_GrandChampions.h"
#include "Boss_GrandChampions.h"


typedef Spell_GrandChampions_RideMount::Spell_GrandChampions_RideMountSpellScript   RideMountSpellScript;
typedef Spell_GrandChampions_RideMount::Spell_GrandChampions_RideMountAuraScript    RideMountAuraScript;
typedef Spell_GrandChampions_Defend::Spell_GrandChampions_DefendAuraScript          DefendScript;
typedef Spell_GrandChampions_HexMending::Spell_GrandChampions_HexMendingAuraScript  HexMendingScript;


SpellCastResult RideMountSpellScript::CheckRequirement()
{
    if (GetCaster()->GetUInt32Value(PLAYER_VISIBLE_ITEM_16_ENTRYID) == 46106
        || GetCaster()->GetUInt32Value(PLAYER_VISIBLE_ITEM_16_ENTRYID) == 46069
        || GetCaster()->GetUInt32Value(PLAYER_VISIBLE_ITEM_16_ENTRYID) == 46070)
    {
        GetCaster()->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
        return SPELL_CAST_OK;
    }
    else
    {
        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_HAVE_LANCE_EQUIPPED);
        return SPELL_FAILED_CUSTOM_ERROR;
    }
}

void RideMountSpellScript::Register()
{
    OnCheckCast += SpellCheckCastFn(RideMountSpellScript::CheckRequirement);
}

void RideMountAuraScript::HandleEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (Unit* target = GetTarget())
        target->RemoveAurasDueToSpell(SPELL_DEFEND);

    if (Unit* caster = GetCaster())
    {
        caster->RemoveAurasDueToSpell(SPELL_DEFEND);
        for (uint8 i=0; i<3; i++)
            caster->RemoveAurasDueToSpell(SPELL_VISUAL_SHIELD_1+i);
    }
}

void RideMountAuraScript::Register()
{
    OnEffectApply += AuraEffectApplyFn(RideMountAuraScript::HandleEffect, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    OnEffectRemove += AuraEffectRemoveFn(RideMountAuraScript::HandleEffect, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
}


bool DefendScript::Validate(SpellInfo const* /*spellEntry*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_VISUAL_SHIELD_1))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_VISUAL_SHIELD_2))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_VISUAL_SHIELD_3))
        return false;

    return true;
}

void DefendScript::RefreshVisualShields(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    Unit* caster = GetCaster();

    if (!caster)
        return;

    if (Unit* rider = caster->GetCharmer())
    {
        for (uint8 i=0; i < 3; ++i)
            rider->RemoveAurasDueToSpell(SPELL_VISUAL_SHIELD_1 + i);

        if (Aura* defend = caster->GetAura(GetId()))
            rider->CastSpell(rider, SPELL_VISUAL_SHIELD_1 + (defend->GetStackAmount()-1), true);
    }
    else
    {
        for (uint8 i=0; i < 3; ++i)
            caster->RemoveAurasDueToSpell(SPELL_VISUAL_SHIELD_1 + i);

        if (Aura* defend = caster->GetAura(GetId()))
            caster->CastSpell(caster, SPELL_VISUAL_SHIELD_1 + (defend->GetStackAmount()-1), true);
    }
}

void DefendScript::Register()
{
    OnEffectApply += AuraEffectApplyFn(DefendScript::RefreshVisualShields, EFFECT_FIRST_FOUND, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK);
    OnEffectRemove += AuraEffectRemoveFn(DefendScript::RefreshVisualShields, EFFECT_FIRST_FOUND, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK);
}


void Player_GrandChampions_HexMendingAI::HealReceived(Unit* healer, uint32 & addHealth)
{
    PlayerAI::HealReceived(healer, addHealth);
    me->CastCustomSpell(SPELL_HEX_OF_MENDING_HEAL, SPELLVALUE_BASE_POINT0, int32(addHealth*2.0f), me, TRIGGERED_FULL_MASK);
}


void HexMendingScript::OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (GetTarget()->GetTypeId() != TYPEID_PLAYER)
        return;

    oldAI = GetTarget()->GetAI();
    GetTarget()->SetAI(new Player_GrandChampions_HexMendingAI(GetTarget()->ToPlayer()));
    oldAIState = GetTarget()->IsAIEnabled;
    GetTarget()->IsAIEnabled = true;
}

void HexMendingScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (GetTarget()->GetTypeId() != TYPEID_PLAYER)
        return;

    delete GetTarget()->GetAI();
    GetTarget()->SetAI(oldAI);
    GetTarget()->IsAIEnabled = oldAIState;
}

void HexMendingScript::Register()
{
    AfterEffectApply += AuraEffectApplyFn(HexMendingScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    AfterEffectRemove += AuraEffectRemoveFn(HexMendingScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
}
