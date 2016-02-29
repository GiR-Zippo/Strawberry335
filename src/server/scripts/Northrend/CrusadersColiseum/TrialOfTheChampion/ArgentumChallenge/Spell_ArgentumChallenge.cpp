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

#include "Spell_ArgentumChallenge.h"
#include "Boss_ArgentumChallenge.h"


typedef Spell_Eadric_Radiance::Spell_Eadric_Radiance_SpellScript                    RadianceScript;
typedef Spell_Eadric_HammerOfRighteous::Spell_Eadric_HammerOfRighteousSpellScript   HammerOfRighteousScript;
typedef Spell_Paletress_Shield::Spell_Paletress_ShieldAuraScript                    ShieldScript;

void RadianceScript::FilterTargets(std::list<WorldObject*>& unitList)
{
    unitList.remove_if(OrientationCheck(GetCaster()));
}

void RadianceScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(RadianceScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(RadianceScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
}


void HammerOfRighteousScript::HandleBeforeHit()
{
    Unit* caster = GetCaster();
    Unit* target = GetExplTargetUnit();

    if (!caster || !target)
        return;

    Aura* hammerJustice = target->GetAura(SPELL_HAMMER_JUSTICE_STUN);

    // Target isn't affected by HoJ so is able to pickup the Hammer
    if (!hammerJustice)
    {
        caster->CastSpell(target, SPELL_HAMMER_RIGHTEOUS_P);
        PreventHitDamage();
    }
}

void HammerOfRighteousScript::Register()
{
    BeforeHit += SpellHitFn(HammerOfRighteousScript::HandleBeforeHit);
}


void ShieldScript::HandleOnEffectAbsorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
{
    Unit* caster = GetCaster();
    Unit* attacker = dmgInfo.GetAttacker();

    if (!caster || !attacker)
        return;

    absorbAmount = dmgInfo.GetDamage();

    // Deal the damage and show it on caster's log
    caster->DealDamage(attacker, (uint32)(absorbAmount*0.25f), NULL, dmgInfo.GetDamageType(), dmgInfo.GetSchoolMask());
    caster->SendSpellNonMeleeDamageLog(attacker, GetSpellInfo()->Id, (uint32) (absorbAmount*0.25f),  dmgInfo.GetSchoolMask(), 0, 0, true, 0);
}

void ShieldScript::Register()
{
    OnEffectAbsorb += AuraEffectAbsorbFn(ShieldScript::HandleOnEffectAbsorb, EFFECT_0);
}
