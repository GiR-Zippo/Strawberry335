#include "Spell_TheLichKing.h"

void SpellTheLichKingSummonSpiritBombSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetHitUnit()->CastSpell((Unit*)NULL, uint32(GetEffectValue()), true);
}

void SpellTheLichKingSummonSpiritBombSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingSummonSpiritBombSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}