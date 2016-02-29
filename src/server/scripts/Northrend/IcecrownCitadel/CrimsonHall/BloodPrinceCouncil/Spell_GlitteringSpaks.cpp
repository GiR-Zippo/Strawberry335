#include "Spell_BloodPrinceCouncil.h"

void SpellTaldaramGlitteringSparksSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetCaster()->CastSpell(GetCaster(), uint32(GetEffectValue()), true);
}

void SpellTaldaramGlitteringSparksSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTaldaramGlitteringSparksSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}