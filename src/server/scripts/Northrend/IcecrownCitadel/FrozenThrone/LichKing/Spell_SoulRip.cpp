#include "Spell_TheLichKing.h"

void SpellTheLichKingSoulRipAuraScript::OnPeriodic(AuraEffect const* aurEff)
{
    PreventDefaultAction();
    // shouldn't be needed, this is channeled
    if (Unit* caster = GetCaster())
        caster->CastCustomSpell(SPELL_SOUL_RIP_DAMAGE, SPELLVALUE_BASE_POINT0, 5000 * aurEff->GetTickNumber(), GetTarget(), TRIGGERED_FULL_MASK, NULL, aurEff, GetCasterGUID());
}

void SpellTheLichKingSoulRipAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellTheLichKingSoulRipAuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
}