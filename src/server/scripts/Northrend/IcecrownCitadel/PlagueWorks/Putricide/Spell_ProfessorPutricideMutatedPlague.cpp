#include "Spell_ProfessorPutricide.h"

/////////////////////////
// Mutated Plague Aura //
/////////////////////////

void AuraScriptPutricideMutatedPlague::HandleTriggerSpell(AuraEffect const* aurEff)
{
    PreventDefaultAction();
    Unit* caster = GetCaster();
    if (!caster)
        return;

    uint32 triggerSpell = GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell;
    SpellInfo const* spell = sSpellMgr->GetSpellInfo(triggerSpell);
    spell = sSpellMgr->GetSpellForDifficultyFromSpell(spell, caster);

    int32 damage = spell->Effects[EFFECT_0].CalcValue(caster);
    float multiplier = 2.0f;
    if (GetTarget()->GetMap()->GetSpawnMode() & 1)
        multiplier = 3.0f;

    damage *= int32(pow(multiplier, GetStackAmount()));
    damage = int32(damage * 1.5f);

    GetTarget()->CastCustomSpell(triggerSpell, SPELLVALUE_BASE_POINT0, damage, GetTarget(), TRIGGERED_FULL_MASK, NULL, aurEff, GetCasterGUID());
}

void AuraScriptPutricideMutatedPlague::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    uint32 healSpell = uint32(GetSpellInfo()->Effects[EFFECT_0].BasePoints);
    SpellInfo const* healSpellInfo = sSpellMgr->GetSpellInfo(healSpell);

    if (!healSpellInfo)
        return;

    int32 heal = healSpellInfo->Effects[0].BasePoints * GetStackAmount();
    GetTarget()->CastCustomSpell(healSpell, SPELLVALUE_BASE_POINT0, heal, GetTarget(), TRIGGERED_FULL_MASK, NULL, NULL, GetCasterGUID());
}

void AuraScriptPutricideMutatedPlague::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(AuraScriptPutricideMutatedPlague::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    AfterEffectRemove += AuraEffectRemoveFn(AuraScriptPutricideMutatedPlague::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
}
