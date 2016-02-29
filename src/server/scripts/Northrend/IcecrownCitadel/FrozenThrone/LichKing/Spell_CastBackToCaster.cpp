#include "Spell_TheLichKing.h"

void SpellTheLichKingCastBackToCasterSpellScript::HandleScript(SpellEffIndex /*effIndex*/)
{
    GetHitUnit()->CastSpell(GetCaster(), uint32(GetEffectValue()), true);
}

void SpellTheLichKingCastBackToCasterSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingCastBackToCasterSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}