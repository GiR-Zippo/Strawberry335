#include "Spell_LadyDeathwhisper.h"

///////////////////////
// Mana Barrier Aura //
///////////////////////

void AuraScriptDeathwhisperManaBarrier::HandlePeriodicTick(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    if (Unit* caster = GetCaster())
    {
        int32 missingHealth = int32(caster->GetMaxHealth() - caster->GetHealth());
        caster->ModifyHealth(missingHealth);
        caster->ModifyPower(POWER_MANA, -missingHealth);
    }
}

void AuraScriptDeathwhisperManaBarrier::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(AuraScriptDeathwhisperManaBarrier::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}

