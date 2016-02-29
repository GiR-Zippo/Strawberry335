#include "Spell_DeathbringerSaurfang.h"

////////////////
// Blood Nova //
////////////////

bool SpellScriptDeathbringerBloodNova::Validate(SpellInfo const* /*spellInfo*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_BLOOD_LINK_DUMMY))
        return false;
    return true;
}

void SpellScriptDeathbringerBloodNova::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);  // make this the default handler
    if (GetCaster()->GetPower(POWER_ENERGY) != GetCaster()->GetMaxPower(POWER_ENERGY))
        GetHitUnit()->CastCustomSpell(SPELL_BLOOD_LINK_DUMMY, SPELLVALUE_BASE_POINT0, 2, GetCaster(), TRIGGERED_FULL_MASK);
}

void SpellScriptDeathbringerBloodNova::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptDeathbringerBloodNova::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
}



/////////////////////////
// Blood Nova Targeting//
/////////////////////////

void SpellScriptDeathbringerBloodNovaTargeting::FilterTargetsInitial(std::list<WorldObject*>& targets)
{
    if (targets.empty())
        return;

    // select one random target, with preference of ranged targets
    uint32 targetsAtRange = 0;
    uint32 const minTargets = uint32(GetCaster()->GetMap()->GetSpawnMode() & 1 ? 10 : 4);
    targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster(), false));

    // get target count at range
    for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr, ++targetsAtRange)
        if ((*itr)->GetDistance(GetCaster()) < 12.0f)
            break;

    // set the upper cap
    if (targetsAtRange < minTargets)
        targetsAtRange = std::min<uint32>(targets.size() - 1, minTargets);

    std::list<WorldObject*>::const_iterator itr = targets.begin();
    std::advance(itr, urand(0, targetsAtRange));
    target = *itr;
    targets.clear();
    targets.push_back(target);
}

void SpellScriptDeathbringerBloodNovaTargeting::FilterTargetsSubsequent(std::list<WorldObject*>& unitList)
{
    if (!target)
        return;

    unitList.clear();
    unitList.push_back(target);
}

void SpellScriptDeathbringerBloodNovaTargeting::HandleForceCast(SpellEffIndex /*effIndex*/)
{
    GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), TRIGGERED_FULL_MASK);
}

void SpellScriptDeathbringerBloodNovaTargeting::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptDeathbringerBloodNovaTargeting::FilterTargetsInitial, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptDeathbringerBloodNovaTargeting::FilterTargetsSubsequent, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
    OnEffectHitTarget += SpellEffectFn(SpellScriptDeathbringerBloodNovaTargeting::HandleForceCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
}
