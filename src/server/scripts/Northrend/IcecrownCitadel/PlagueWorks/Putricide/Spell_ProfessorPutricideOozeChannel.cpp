#include "Spell_ProfessorPutricide.h"

//////////////////
// Ooze Channel //
//////////////////

bool SpellScriptPutricideOozeChannel::Validate(SpellInfo const* spell)
{
    if (!spell->ExcludeTargetAuraSpell)
        return false;
    if (!sSpellMgr->GetSpellInfo(spell->ExcludeTargetAuraSpell))
        return false;
    return true;
}

// set up initial variables and check if caster is creature
// this will let use safely use ToCreature() casts in entire script
bool SpellScriptPutricideOozeChannel::Load()
{
    _target = NULL;
    return GetCaster()->GetTypeId() == TYPEID_UNIT;
}

void SpellScriptPutricideOozeChannel::SelectTarget(std::list<WorldObject*>& targets)
{
    targets.remove_if(IsNotAPlayerCheck());
    targets.remove_if(BeamProtectionCheck(GetSpellInfo()->ExcludeTargetAuraSpell));
    targets.remove_if(TankCheck(ObjectAccessor::GetCreature(*GetCaster(), GetCaster()->GetInstanceScript()->GetData64(DATA_PROFESSOR_PUTRICIDE))));
    targets.remove_if(AbominationCheck());

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

void SpellScriptPutricideOozeChannel::SetTarget(std::list<WorldObject*>& targets)
{
    targets.clear();
    if (_target)
        targets.push_back(_target);
}

void SpellScriptPutricideOozeChannel::StartAttack()
{
    GetCaster()->ClearUnitState(UNIT_STATE_CASTING);
    GetCaster()->DeleteThreatList();
    GetCaster()->ToCreature()->AI()->AttackStart(GetHitUnit());
    GetCaster()->AddThreat(GetHitUnit(), 500000000.0f);    // value seen in sniff
}

// temporary, until SelectTarget are not called on empty lists
void SpellScriptPutricideOozeChannel::CheckTarget()
{
    if (_target)
        return;

    FinishCast(SPELL_FAILED_NO_VALID_TARGETS);
    GetCaster()->ToCreature()->DespawnOrUnsummon(1);    // despawn next update
}

void SpellScriptPutricideOozeChannel::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideOozeChannel::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideOozeChannel::SetTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideOozeChannel::SetTarget, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
    AfterHit += SpellHitFn(SpellScriptPutricideOozeChannel::StartAttack);
    OnCast += SpellCastFn(SpellScriptPutricideOozeChannel::CheckTarget);
}
