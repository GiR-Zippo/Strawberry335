#include "Spell_ProfessorPutricide.h"

/////////////////////////////////////////
// Mutated Transformation Dismiss Aura //
/////////////////////////////////////////

void AuraScriptPutricideMutatedTransformationDismiss::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (Vehicle* veh = GetTarget()->GetVehicleKit())
        veh->RemoveAllPassengers();
}

void AuraScriptPutricideMutatedTransformationDismiss::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(AuraScriptPutricideMutatedTransformationDismiss::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
}



////////////////////////////
// Mutated Transformation //
////////////////////////////

void SpellScriptPutricideMutatedTransformation::HandleSummon(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    Unit* caster = GetOriginalCaster();
    if (!caster)
        return;

    InstanceScript* instance = caster->GetInstanceScript();
    if (!instance)
        return;

    Creature* putricide = ObjectAccessor::GetCreature(*caster, instance->GetData64(DATA_PROFESSOR_PUTRICIDE));
    if (!putricide)
        return;

    if (putricide->AI()->GetData(DATA_ABOMINATION))
    {
        if (Player* player = caster->ToPlayer())
            Spell::SendCastResult(player, GetSpellInfo(), 0, SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_TOO_MANY_ABOMINATIONS);
        return;
    }

    uint32 entry = uint32(GetSpellInfo()->Effects[effIndex].MiscValue);
    SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(uint32(GetSpellInfo()->Effects[effIndex].MiscValueB));
    uint32 duration = uint32(GetSpellInfo()->GetDuration());

    Position pos;
    caster->GetPosition(&pos);
    TempSummon* summon = caster->GetMap()->SummonCreature(entry, pos, properties, duration, caster, GetSpellInfo()->Id);
    if (!summon || !summon->IsVehicle())
        return;

    summon->CastSpell(summon, SPELL_ABOMINATION_VEHICLE_POWER_DRAIN, true);
    summon->CastSpell(summon, SPELL_MUTATED_TRANSFORMATION_DAMAGE, true);
    caster->CastSpell(summon, SPELL_MUTATED_TRANSFORMATION_NAME, true);

    caster->EnterVehicle(summon, 0);    // VEHICLE_SPELL_RIDE_HARDCODED is used according to sniff, this is ok
    summon->SetCreatorGUID(caster->GetGUID());
    summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    caster->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    putricide->AI()->JustSummoned(summon);
}

void SpellScriptPutricideMutatedTransformation::Register()
{
    OnEffectHit += SpellEffectFn(SpellScriptPutricideMutatedTransformation::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
}



////////////////////////////////
// Mutated Transformation Dmg //
////////////////////////////////

void SpellScriptPutricideMutatedTransformationDmg::FilterTargetsInitial(std::list<WorldObject*>& targets)
{
    if (Unit* owner = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetCreatorGUID()))
        targets.remove(owner);
}

void SpellScriptPutricideMutatedTransformationDmg::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideMutatedTransformationDmg::FilterTargetsInitial, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
}
