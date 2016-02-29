#include "Spell_TheLichKing.h"

class ValkyrSearchTargetFilter
{
public:
    bool operator()(WorldObject* target) const
    {
        if (Unit* unit = target->ToUnit())
        {
            if (unit->HasAura(SPELL_VALKYR_TARGET_SEARCH))
                return true;

            if (unit->IsFalling())
                return true;
        }
        return false;
    }
};

bool SpellTheLichKingValkyrTargetSearchSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_VALKYR_TARGET_SEARCH))
        return false;
    return true;
}

bool SpellTheLichKingValkyrTargetSearchSpellScript::Load()
{
    _target = NULL;
    return true;
}

void SpellTheLichKingValkyrTargetSearchSpellScript::SelectTarget(std::list<WorldObject*>& targets)
{
    if (targets.empty())
        return;

    targets.remove_if(ValkyrSearchTargetFilter());
    if (targets.empty())
        return;

    _target = Trinity::Containers::SelectRandomContainerElement(targets);
    targets.clear();
    targets.push_back(_target);
    GetCaster()->GetAI()->SetGUID(_target->GetGUID());
}

void SpellTheLichKingValkyrTargetSearchSpellScript::ReplaceTarget(std::list<WorldObject*>& targets)
{
    targets.clear();
    if (_target)
        targets.push_back(_target);
}

void SpellTheLichKingValkyrTargetSearchSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetCaster()->CastSpell(GetHitUnit(), SPELL_CHARGE, true);
}

void SpellTheLichKingValkyrTargetSearchSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingValkyrTargetSearchSpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingValkyrTargetSearchSpellScript::ReplaceTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingValkyrTargetSearchSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}

bool SpellTheLichKingEjectAllPassengersSpellScript::Load()
{
    return GetCaster()->IsVehicle();
}

void SpellTheLichKingEjectAllPassengersSpellScript::HandleDummy(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetCaster()->GetVehicleKit()->RemoveAllPassengers();
}

void SpellTheLichKingEjectAllPassengersSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingEjectAllPassengersSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}