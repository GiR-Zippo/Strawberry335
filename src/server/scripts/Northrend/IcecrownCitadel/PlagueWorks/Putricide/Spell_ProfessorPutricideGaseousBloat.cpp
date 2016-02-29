#include "Spell_ProfessorPutricide.h"
#include <GridNotifiers.h>

////////////////////////
// Gaseous Bloat Aura //
////////////////////////

bool SpellScriptPutricideGaseousBloat::Validate(SpellInfo const* spell)
{
    if (!spell->ExcludeTargetAuraSpell)
        return false;
    if (!sSpellMgr->GetSpellInfo(spell->ExcludeTargetAuraSpell))
        return false;
    return true;
}

bool SpellScriptPutricideGaseousBloat::Load()
{
    _target = NULL;
    return GetCaster()->GetTypeId() == TYPEID_UNIT;
}

void SpellScriptPutricideGaseousBloat::SelectTarget(std::list<WorldObject*>& targets)
{
    targets.remove_if(IsNotAPlayerCheck());
    targets.remove_if(BeamProtectionCheck(GetSpellInfo()->ExcludeTargetAuraSpell));
    targets.remove_if(TankCheck(ObjectAccessor::GetCreature(*GetCaster(), GetCaster()->GetInstanceScript()->GetData64(DATA_PROFESSOR_PUTRICIDE))));

    if (GetCaster()->GetMap()->IsHeroic())
        targets.remove_if(Trinity::UnitAuraCheck(true, sSpellMgr->GetSpellIdForDifficulty(SPELL_GAS_VARIABLE, GetCaster())));

    if (targets.empty())
    {
        FinishCast(SPELL_FAILED_NO_VALID_TARGETS);
        GetCaster()->ToCreature()->DespawnOrUnsummon(1);    // despawn next update
        return;
    }

    WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
    targets.clear();
    targets.push_back(target);
    _target = target;
}

void SpellScriptPutricideGaseousBloat::SetTarget(std::list<WorldObject*>& targets)
{
    targets.clear();
    if (_target)
        targets.push_back(_target);
}

void SpellScriptPutricideGaseousBloat::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideGaseousBloat::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideGaseousBloat::SetTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideGaseousBloat::SetTarget, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
}

void AuraScriptPutricideGaseousBloat::HandleExtraEffect(AuraEffect const* /*aurEff*/)
{
    Unit* target = GetTarget();
    if (Unit* caster = GetCaster())
    {
        target->RemoveAuraFromStack(GetSpellInfo()->Id, GetCasterGUID());
        if (!target->HasAura(GetId()))
            caster->CastCustomSpell(SPELL_GASEOUS_BLOAT, SPELLVALUE_AURA_STACK, 10, caster, TRIGGERED_NONE);
    }
}

void AuraScriptPutricideGaseousBloat::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(AuraScriptPutricideGaseousBloat::HandleExtraEffect, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
}
