#include "Spell_ValithriaDreamwalker.h"

void SpellDreamwalkerManaVoidAuraScript::PeriodicTick(AuraEffect const* aurEff)
{
    // first 3 ticks have amplitude 1 second
    // remaining tick every 500ms
    if (aurEff->GetTickNumber() <= 5)
        if (!(aurEff->GetTickNumber() & 1))
            PreventDefaultAction();
}

void SpellDreamwalkerManaVoidAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellDreamwalkerManaVoidAuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}