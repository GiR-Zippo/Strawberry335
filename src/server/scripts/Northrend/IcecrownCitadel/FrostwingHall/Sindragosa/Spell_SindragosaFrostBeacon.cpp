#include "Spell_Sindragosa.h"

bool SpellSindragosaFrostBeaconAuraScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_ICE_TOMB_DAMAGE))
        return false;
    return true;
}

void SpellSindragosaFrostBeaconAuraScript::PeriodicTick(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    if (Unit* caster = GetCaster())
        caster->CastSpell(GetTarget(), SPELL_ICE_TOMB_DAMAGE, true);
}

void SpellSindragosaFrostBeaconAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellSindragosaFrostBeaconAuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}