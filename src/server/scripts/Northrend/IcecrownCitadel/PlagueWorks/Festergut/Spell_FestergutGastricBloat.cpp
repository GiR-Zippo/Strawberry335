#include "Spell_Festergut.h"

///////////////////
// Gastric Bloat //
///////////////////

bool SpellScriptFestergutGastricBloat::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_GASTRIC_EXPLOSION))
        return false;
    return true;
}

void SpellScriptFestergutGastricBloat::HandleScript(SpellEffIndex /*effIndex*/)
{
    Aura const* aura = GetHitUnit()->GetAura(GetSpellInfo()->Id);
    if (!(aura && aura->GetStackAmount() == 10))
        return;

    GetHitUnit()->RemoveAurasDueToSpell(GetSpellInfo()->Id);
    GetHitUnit()->CastSpell(GetHitUnit(), sSpellMgr->GetSpellIdForDifficulty(SPELL_GASTRIC_EXPLOSION, GetCaster()), true);
}

void SpellScriptFestergutGastricBloat::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptFestergutGastricBloat::HandleScript, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
}
