#include "Spell_DeathbringerSaurfang.h"

////////////////
// Blood Link //
////////////////

bool SpellScriptDeathbringerBloodLink::Validate(SpellInfo const* /*spellInfo*/) {
    if (!sSpellMgr->GetSpellInfo(SPELL_BLOOD_LINK_POWER))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_BLOOD_POWER))
        return false;
    return true;
}

void SpellScriptDeathbringerBloodLink::HandleDummy(SpellEffIndex /*effIndex*/) {
    GetHitUnit()->CastCustomSpell(SPELL_BLOOD_LINK_POWER, SPELLVALUE_BASE_POINT0, GetEffectValue(), GetHitUnit(), TRIGGERED_FULL_MASK);
    if (Aura* bloodPower = GetHitUnit()->GetAura(SPELL_BLOOD_POWER))
        bloodPower->RecalculateAmountOfEffects();
    PreventHitDefaultEffect(EFFECT_0);
}

void SpellScriptDeathbringerBloodLink::Register() {
    OnEffectHitTarget += SpellEffectFn(SpellScriptDeathbringerBloodLink::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}



/////////////////////
// Blood Link Aura //
/////////////////////

bool AuraScriptDeathbringerBloodLinkAura::Validate(SpellInfo const* /*spellInfo*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_MARK_OF_THE_FALLEN_CHAMPION))
        return false;
    return true;
}

void AuraScriptDeathbringerBloodLinkAura::HandlePeriodicTick(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    if (GetUnitOwner()->getPowerType() == POWER_ENERGY && GetUnitOwner()->GetPower(POWER_ENERGY) == GetUnitOwner()->GetMaxPower(POWER_ENERGY))
        if (Creature* saurfang = GetUnitOwner()->ToCreature())
            saurfang->AI()->DoAction(ACTION_MARK_OF_THE_FALLEN_CHAMPION);
}


void AuraScriptDeathbringerBloodLinkAura::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(AuraScriptDeathbringerBloodLinkAura::HandlePeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
}