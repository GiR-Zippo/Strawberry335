#include "Spell_Sindragosa.h"

bool SpellSindragosaIcyGripSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_ICY_GRIP_JUMP))
        return false;
    return true;
}

void SpellSindragosaIcyGripSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (GetCaster()->getVictim() != GetHitUnit()) // Should not affect the tank
        GetHitUnit()->CastSpell(GetCaster(), SPELL_ICY_GRIP_JUMP, true);
}

void SpellSindragosaIcyGripSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellSindragosaIcyGripSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}