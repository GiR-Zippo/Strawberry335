#include "Spell_TheLichKing.h"

void SpellTheLichKingJumpSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetHitUnit()->RemoveAurasDueToSpell(SPELL_RAISE_DEAD);
    GetHitUnit()->CastSpell((Unit*)NULL, SPELL_JUMP_2, true);
    if (Creature* creature = GetHitCreature())
        creature->AI()->DoAction(ACTION_BREAK_FROSTMOURNE);
}

void SpellTheLichKingJumpSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingJumpSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}

void SpellTheLichKingJumpRemoveAuraSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
}

void SpellTheLichKingJumpRemoveAuraSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingJumpRemoveAuraSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}