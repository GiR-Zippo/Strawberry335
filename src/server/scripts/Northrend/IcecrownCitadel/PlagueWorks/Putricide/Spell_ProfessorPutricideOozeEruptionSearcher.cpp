#include "Spell_ProfessorPutricide.h"

////////////////////////////
// Ooze Eruption Searcher //
////////////////////////////

void SpellScriptPutricideOozeEruptionSearcher::HandleDummy(SpellEffIndex /*effIndex*/)
{
    uint32 adhesiveId = sSpellMgr->GetSpellIdForDifficulty(SPELL_VOLATILE_OOZE_ADHESIVE, GetCaster());
    if (GetHitUnit()->HasAura(adhesiveId))
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_OOZE_ERUPTION, true);
        GetHitUnit()->RemoveAurasDueToSpell(adhesiveId, GetCaster()->GetGUID(), 0, AURA_REMOVE_BY_ENEMY_SPELL);
    }
}

void SpellScriptPutricideOozeEruptionSearcher::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPutricideOozeEruptionSearcher::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}
