#include "Spell_TheLichKing.h"

void SpellTheLichKingLightsFavorAuraScript::OnPeriodic(AuraEffect const* /*aurEff*/)
{
    if (Unit* caster = GetCaster())
        if (AuraEffect* effect = GetAura()->GetEffect(EFFECT_1))
            effect->RecalculateAmount(caster);
}

void SpellTheLichKingLightsFavorAuraScript::CalculateBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
{
    canBeRecalculated = true;
    amount = 0;
    if (Unit* caster = GetCaster())
        amount = int32(caster->GetHealthPct());
}

void SpellTheLichKingLightsFavorAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellTheLichKingLightsFavorAuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
    DoEffectCalcAmount += AuraEffectCalcAmountFn(SpellTheLichKingLightsFavorAuraScript::CalculateBonus, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
}