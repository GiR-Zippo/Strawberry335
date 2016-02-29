#include "Spell_ProfessorPutricide.h"

////////////////////////
// Mutation Init Aura //
////////////////////////

void AuraScriptPutricideMutationInit::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    uint32 spellId = 70311;
    if (GetTarget()->GetMap()->GetSpawnMode() & 1)
        spellId = 71503;

    GetTarget()->CastSpell(GetTarget(), spellId, true);
}

void AuraScriptPutricideMutationInit::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(AuraScriptPutricideMutationInit::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
}



///////////////////
// Mutation Init //
///////////////////

SpellCastResult SpellScriptPutricideMutationInit::CheckRequirementInternal(SpellCustomErrors& extendedError)
{
    InstanceScript* instance = GetExplTargetUnit()->GetInstanceScript();
    if (!instance)
        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

    Creature* professor = ObjectAccessor::GetCreature(*GetExplTargetUnit(), instance->GetData64(DATA_PROFESSOR_PUTRICIDE));
    if (!professor)
        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

    if (professor->AI()->GetData(DATA_PHASE) == PHASE_COMBAT_3 || !professor->isAlive())
    {
        extendedError = SPELL_CUSTOM_ERROR_ALL_POTIONS_USED;
        return SPELL_FAILED_CUSTOM_ERROR;
    }

    if (professor->AI()->GetData(DATA_ABOMINATION))
    {
        extendedError = SPELL_CUSTOM_ERROR_TOO_MANY_ABOMINATIONS;
        return SPELL_FAILED_CUSTOM_ERROR;
    }

    return SPELL_CAST_OK;
}

SpellCastResult SpellScriptPutricideMutationInit::CheckRequirement()
{
    if (!GetExplTargetUnit())
        return SPELL_FAILED_BAD_TARGETS;

    if (GetExplTargetUnit()->GetTypeId() != TYPEID_PLAYER)
        return SPELL_FAILED_TARGET_NOT_PLAYER;

    SpellCustomErrors extension = SPELL_CUSTOM_ERROR_NONE;
    SpellCastResult result = CheckRequirementInternal(extension);
    if (result != SPELL_CAST_OK)
    {
        Spell::SendCastResult(GetExplTargetUnit()->ToPlayer(), GetSpellInfo(), 0, result, extension);
        return result;
    }

    return SPELL_CAST_OK;
}

void SpellScriptPutricideMutationInit::Register()
{
    OnCheckCast += SpellCheckCastFn(SpellScriptPutricideMutationInit::CheckRequirement);
}
