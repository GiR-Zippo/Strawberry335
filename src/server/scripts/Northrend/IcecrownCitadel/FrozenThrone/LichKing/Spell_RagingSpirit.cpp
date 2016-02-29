#include "Spell_TheLichKing.h"

bool SpellTheLichKingRagingSpiritSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_RAGING_SPIRIT))
        return false;
    return true;
}

void SpellTheLichKingRagingSpiritSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetHitUnit()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
}

void SpellTheLichKingRagingSpiritSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingRagingSpiritSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}