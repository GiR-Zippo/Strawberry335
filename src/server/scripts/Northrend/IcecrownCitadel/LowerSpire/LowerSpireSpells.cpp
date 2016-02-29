#include "LowerSpireSpells.h"

bool SpellFrostGiantDeathPlagueSpellScript::Load()
{
    _failed = false;
    return true;
}

void SpellFrostGiantDeathPlagueSpellScript::CountTargets(std::list< WorldObject* >& targets)
{
    targets.remove(GetCaster());
    _failed = targets.empty();
}

void SpellFrostGiantDeathPlagueSpellScript::FilterTargets(std::list< WorldObject* >& targets)
{
    // Select valid targets for jump
    targets.remove_if(DeathPlagueTargetSelector(GetCaster()));
    if (!targets.empty())
    {
        WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
        targets.clear();
        targets.push_back(target);
    }

    targets.push_back(GetCaster());
}

void SpellFrostGiantDeathPlagueSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (GetHitUnit() != GetCaster())
        GetCaster()->CastSpell(GetHitUnit(), SPELL_DEATH_PLAGUE_AURA, true);
    else if (_failed)
        GetCaster()->CastSpell(GetCaster(), SPELL_DEATH_PLAGUE_KILL, true);
}

void SpellFrostGiantDeathPlagueSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellFrostGiantDeathPlagueSpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellFrostGiantDeathPlagueSpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
    OnEffectHitTarget += SpellEffectFn(SpellFrostGiantDeathPlagueSpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
}