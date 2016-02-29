#include "Spell_DeathbringerSaurfang.h"

//////////////////
// Remove Marks //
//////////////////

void SpellScriptDeathbringerRemoveMarks::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
}

void SpellScriptDeathbringerRemoveMarks::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptDeathbringerRemoveMarks::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}
