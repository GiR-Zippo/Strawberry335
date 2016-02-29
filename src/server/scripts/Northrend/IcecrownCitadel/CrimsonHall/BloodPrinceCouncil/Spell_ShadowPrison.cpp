#include "Spell_BloodPrinceCouncil.h"

void SpellBloodCouncilShadowPrisonAuraScript::HandleDummyTick(AuraEffect const* aurEff)
{
    if (GetTarget()->isMoving() && GetTarget()->GetTypeId() == TYPEID_PLAYER)
        GetTarget()->CastSpell(GetTarget(), SPELL_SHADOW_PRISON_DAMAGE, true, NULL, aurEff);
}

void SpellBloodCouncilShadowPrisonAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellBloodCouncilShadowPrisonAuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
}

void SpellBloodCouncilShadowPrisonDamageSpellScript::AddExtraDamage()
{
    if (Aura* aur = GetHitUnit()->GetAura(GetSpellInfo()->Id))
        if (AuraEffect const* eff = aur->GetEffect(EFFECT_1))
            SetHitDamage(GetHitDamage() + eff->GetAmount());
}

void SpellBloodCouncilShadowPrisonDamageSpellScript::Register()
{
    OnHit += SpellHitFn(SpellBloodCouncilShadowPrisonDamageSpellScript::AddExtraDamage);
}