#include "Spell_ProfessorPutricide.h"

/////////////////////////
// Unstable Experiment //
/////////////////////////

void SpellScriptPutricideUnstableExperiment::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (GetCaster()->GetTypeId() != TYPEID_UNIT)
        return;

    uint32 stage = GetCaster()->ToCreature()->AI()->GetData(DATA_EXPERIMENT_STAGE);
    Creature* target = NULL;
    std::list<Creature*> creList;
    GetCreatureListWithEntryInGrid(creList, GetCaster(), NPC_ABOMINATION_WING_MAD_SCIENTIST_STALKER, 200.0f);
    // 2 of them are spawned at green place - weird trick blizz
    for (std::list<Creature*>::iterator itr = creList.begin(); itr != creList.end(); ++itr)
    {
        target = *itr;
        std::list<Creature*> tmp;
        GetCreatureListWithEntryInGrid(tmp, target, NPC_ABOMINATION_WING_MAD_SCIENTIST_STALKER, 10.0f);
        if ((!stage && tmp.size() > 1) || (stage && tmp.size() == 1))
            break;
    }

    GetCaster()->CastSpell(target, uint32(GetSpellInfo()->Effects[stage].CalcValue()), true, NULL, NULL, GetCaster()->GetGUID());
}

void SpellScriptPutricideUnstableExperiment::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPutricideUnstableExperiment::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}
