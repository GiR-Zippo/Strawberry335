#include "Spell_Festergut.h"

////////////////////
// Pungent Blight //
////////////////////

bool SpellScriptFestergutPungentBlight::Load()
{
    return GetCaster()->GetTypeId() == TYPEID_UNIT;
}

void SpellScriptFestergutPungentBlight::HandleScript(SpellEffIndex /*effIndex*/)
{
    // Get Inhaled Blight id for our difficulty
    uint32 blightId = sSpellMgr->GetSpellIdForDifficulty(uint32(GetEffectValue()), GetCaster());

    // ...and remove it
    GetCaster()->RemoveAurasDueToSpell(blightId);
    GetCaster()->ToCreature()->AI()->Talk(EMOTE_PUNGENT_BLIGHT);
}

void SpellScriptFestergutPungentBlight::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptFestergutPungentBlight::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
}
