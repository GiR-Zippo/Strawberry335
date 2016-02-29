#include "Spell_DeathbringerSaurfang.h"

///////////////////
// Rune Of Blood //
///////////////////

bool SpellScriptDeathbringerRuneOfBlood::Validate(SpellInfo const* /*spellInfo*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_BLOOD_LINK_DUMMY))
        return false;
    return true;
}

void SpellScriptDeathbringerRuneOfBlood::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);  // make this the default handler
    if (GetCaster()->GetPower(POWER_ENERGY) != GetCaster()->GetMaxPower(POWER_ENERGY))
        GetHitUnit()->CastCustomSpell(SPELL_BLOOD_LINK_DUMMY, SPELLVALUE_BASE_POINT0, 1, GetCaster(), TRIGGERED_FULL_MASK);
}

void SpellScriptDeathbringerRuneOfBlood::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptDeathbringerRuneOfBlood::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
}
