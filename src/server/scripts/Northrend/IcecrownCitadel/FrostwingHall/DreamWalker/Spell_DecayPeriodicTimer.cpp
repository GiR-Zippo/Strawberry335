#include "Spell_ValithriaDreamwalker.h"

bool SpellDreamwalkerDecayPeriodicTimerAuraScript::Load()
{
    _decayRate = GetId() != SPELL_TIMER_BLAZING_SKELETON ? 1000 : 5000;
    return true;
}

void SpellDreamwalkerDecayPeriodicTimerAuraScript::DecayPeriodicTimer(AuraEffect* aurEff)
{
    int32 timer = aurEff->GetPeriodicTimer();
    if (timer <= 5)
        return;

    aurEff->SetPeriodicTimer(timer - _decayRate);
}

void SpellDreamwalkerDecayPeriodicTimerAuraScript::Register()
{
    OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(SpellDreamwalkerDecayPeriodicTimerAuraScript::DecayPeriodicTimer, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}