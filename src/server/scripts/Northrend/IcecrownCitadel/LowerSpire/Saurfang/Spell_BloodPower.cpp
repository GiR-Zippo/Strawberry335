#include "Spell_DeathbringerSaurfang.h"

/////////////////
// Blood Power //
/////////////////

void SpellScriptDeathbringerBloodPower::ModAuraValue()
{
    if (Aura* aura = GetHitAura())
                    aura->RecalculateAmountOfEffects();
}

void SpellScriptDeathbringerBloodPower::Register()
{
    AfterHit += SpellHitFn(SpellScriptDeathbringerBloodPower::ModAuraValue);
}



//////////////////////
// Blood Power Aura //
//////////////////////

void AuraScriptDeathbringerBloodPower::RecalculateHook(AuraEffect const* /*aurEffect*/, int32& amount, bool& canBeRecalculated)
{
    amount = int32(GetUnitOwner()->GetPower(POWER_ENERGY));
    canBeRecalculated = true;
}

void AuraScriptDeathbringerBloodPower::Register()
{
    DoEffectCalcAmount += AuraEffectCalcAmountFn(AuraScriptDeathbringerBloodPower::RecalculateHook, EFFECT_0, SPELL_AURA_MOD_SCALE);
    DoEffectCalcAmount += AuraEffectCalcAmountFn(AuraScriptDeathbringerBloodPower::RecalculateHook, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
}

bool AuraScriptDeathbringerBloodPower::Load()
{
    if (GetUnitOwner()->getPowerType() != POWER_ENERGY)
        return false;
    return true;
}