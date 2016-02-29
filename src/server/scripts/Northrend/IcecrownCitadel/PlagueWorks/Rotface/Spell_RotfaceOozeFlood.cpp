#include "Spell_Rotface.h"

////////////////
// Ooze Flood //
////////////////

void SpellScriptRotfaceOozeFlood::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (!GetHitUnit())
        return;

    std::list<Creature*> triggers;
    GetHitUnit()->GetCreatureListWithEntryInGrid(triggers, NPC_PUDDLE_STALKER_TOP, 20.0f);

    if (triggers.empty())
        return;

    triggers.sort(Trinity::ObjectDistanceOrderPred(GetHitUnit()));
    triggers.back()->CastSpell(triggers.back(), uint32(GetEffectValue()), true, NULL, NULL, GetOriginalCaster() ? GetOriginalCaster()->GetGUID() : 0);
}

void SpellScriptRotfaceOozeFlood::FilterTargets(std::list<WorldObject*>& targets)
{
    // get 2 targets except 2 nearest
    targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));

    // .resize() runs pop_back();
    if (targets.size() > 2)
        targets.resize(2);
}

void SpellScriptRotfaceOozeFlood::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptRotfaceOozeFlood::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptRotfaceOozeFlood::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
}
