#include "Spell_ProfessorPutricide.h"

/////////////////////////////
// Clear Aura Effect Value //
/////////////////////////////

void SpellScriptPutricideClearAuraEffectValue::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    uint32 auraId = sSpellMgr->GetSpellIdForDifficulty(uint32(GetEffectValue()), GetCaster());
    GetHitUnit()->RemoveAurasDueToSpell(auraId);
}

void SpellScriptPutricideClearAuraEffectValue::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPutricideClearAuraEffectValue::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}
