#ifndef __SPELL_PROFESSOR_PUTRICIDE_H
#define __SPELL_PROFESSOR_PUTRICIDE_H

#include "ProfessorPutricide.h"

class SpellScriptPutricideGaseousBloat : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideGaseousBloat);

    bool Validate(SpellInfo const* spell);

    bool Load();

    void SelectTarget(std::list<WorldObject*>& targets);

    void SetTarget(std::list<WorldObject*>& targets);

    void Register();

    WorldObject* _target;
};

class AuraScriptPutricideGaseousBloat : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptPutricideGaseousBloat);

    void HandleExtraEffect(AuraEffect const* /*aurEff*/);

    void Register();
};

class AuraScriptPutricideMutatedPlague : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptPutricideMutatedPlague);

    void HandleTriggerSpell(AuraEffect const* aurEff);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);

    void Register();
};

class AuraScriptPutricideMutationInit : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptPutricideMutationInit);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);

    void Register();
};

class AuraScriptPutricideMutatedTransformationDismiss : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptPutricideMutatedTransformationDismiss);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);

    void Register();
};



class SpellScriptPutricideOozeChannel : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideOozeChannel);

    bool Validate(SpellInfo const* spell);

    // set up initial variables and check if caster is creature
    // this will let use safely use ToCreature() casts in entire script
    bool Load();

    void SelectTarget(std::list<WorldObject*>& targets);

    void SetTarget(std::list<WorldObject*>& targets);

    void StartAttack();

    // temporary, until SelectTarget are not called on empty lists
    void CheckTarget();

    void Register();

    WorldObject* _target;
};

class SpellScriptPutricideSlimePuddle : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideSlimePuddle);

    void ScaleRange(std::list<WorldObject*>& targets);

    void Register();
};

class SpellScriptPutricideSlimePuddleAura : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideSlimePuddleAura);

    void ReplaceAura();

    void Register();

};

class SpellScriptPutricideOozeEruptionSearcher : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideOozeEruptionSearcher);

    void HandleDummy(SpellEffIndex /*effIndex*/);

    void Register();
};

class SpellScriptPutricideChokingGasBomb : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideChokingGasBomb);

    void HandleScript(SpellEffIndex /*effIndex*/);

    void Register();
};

class SpellScriptPutricideUnboundPlague : public SpellScript
{
    PrepareSpellScript(SpellScriptPutricideUnboundPlague);

    bool Validate(SpellInfo const* /*spell*/);

    void HandleScript(SpellEffIndex /*effIndex*/);

    void Register();
};

class SpellScriptPutricideEatOoze : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideEatOoze);

    void SelectTarget(std::list<WorldObject*>& targets);

    void HandleScript(SpellEffIndex /*effIndex*/);

    void Register();
};

class SpellScriptPutricideMutationInit : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideMutationInit);

    SpellCastResult CheckRequirementInternal(SpellCustomErrors& extendedError);

    SpellCastResult CheckRequirement();

    void Register();
};

class SpellScriptPutricideMutatedTransformation : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideMutatedTransformation);

    void HandleSummon(SpellEffIndex effIndex);

    void Register();
};

class SpellScriptPutricideMutatedTransformationDmg : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideMutatedTransformationDmg);

    void FilterTargetsInitial(std::list<WorldObject*>& targets);

    void Register();
};

class SpellScriptPutricideRegurgitatedOoze : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideRegurgitatedOoze);

    // the only purpose of this hook is to fail the achievement
    void ExtraEffect(SpellEffIndex /*effIndex*/);

    void Register();
};

class SpellScriptPutricideClearAuraEffectValue : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideClearAuraEffectValue);

    void HandleScript(SpellEffIndex effIndex);

    void Register();
};

class SpellScriptPutricideUnstableExperiment : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptPutricideUnstableExperiment);

    void HandleScript(SpellEffIndex effIndex);

    void Register();
};

class SpellPutricideGaseousBloat : public SpellScriptLoader
{
public:
    SpellPutricideGaseousBloat() : SpellScriptLoader("spell_putricide_gaseous_bloat") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptPutricideGaseousBloat();
    }

    AuraScript* GetAuraScript() const
    {
        return new AuraScriptPutricideGaseousBloat();
    }
};

class SpellPutricideOozeChannel : public SpellScriptLoader
{
public:
    SpellPutricideOozeChannel() : SpellScriptLoader("spell_putricide_ooze_channel") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptPutricideOozeChannel();
    }
};

class SpellPutricideSlimePuddle : public SpellScriptLoader
{
    public:
        SpellPutricideSlimePuddle() : SpellScriptLoader("spell_putricide_slime_puddle") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideSlimePuddle();
        }
};

// this is here only because on retail you dont actually enter HEROIC mode for ICC
class SpellPutricideSlimePuddleAura : public SpellScriptLoader
{
    public:
        SpellPutricideSlimePuddleAura() : SpellScriptLoader("spell_putricide_slime_puddle_aura") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideSlimePuddleAura();
        }
};

class SpellPutricideOozeEruptionSearcher : public SpellScriptLoader
{
    public:
        SpellPutricideOozeEruptionSearcher() : SpellScriptLoader("spell_putricide_ooze_eruption_searcher") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideOozeEruptionSearcher();
        }
};

class SpellPutricideChokingGasBomb : public SpellScriptLoader
{
    public:
        SpellPutricideChokingGasBomb() : SpellScriptLoader("spell_putricide_choking_gas_bomb") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideChokingGasBomb();
        }
};

class SpellPutricideUnboundPlague : public SpellScriptLoader
{
    public:
        SpellPutricideUnboundPlague() : SpellScriptLoader("spell_putricide_unbound_plague") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideUnboundPlague();
        }
};

class SpellPutricideEatOoze : public SpellScriptLoader
{
    public:
        SpellPutricideEatOoze() : SpellScriptLoader("spell_putricide_eat_ooze") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideEatOoze();
        }
};

class SpellPutricideMutatedPlague : public SpellScriptLoader
{
    public:
        SpellPutricideMutatedPlague() : SpellScriptLoader("spell_putricide_mutated_plague") { }

        AuraScript* GetAuraScript() const
        {
            return new AuraScriptPutricideMutatedPlague();
        }
};

class SpellPutricideMutationInit : public SpellScriptLoader
{
    public:
        SpellPutricideMutationInit() : SpellScriptLoader("spell_putricide_mutation_init") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideMutationInit();
        }

        AuraScript* GetAuraScript() const
        {
            return new AuraScriptPutricideMutationInit();
        }
};

class SpellPutricideMutatedTransformationDismiss : public SpellScriptLoader
{
    public:
        SpellPutricideMutatedTransformationDismiss() : SpellScriptLoader("spell_putricide_mutated_transformation_dismiss") { }

        AuraScript* GetAuraScript() const
        {
            return new AuraScriptPutricideMutatedTransformationDismiss();
        }
};

class SpellPutricideMutatedTransformation : public SpellScriptLoader
{
    public:
        SpellPutricideMutatedTransformation() : SpellScriptLoader("spell_putricide_mutated_transformation") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideMutatedTransformation();
        }
};

class SpellPutricideMutatedTransformationDmg : public SpellScriptLoader
{
    public:
        SpellPutricideMutatedTransformationDmg() : SpellScriptLoader("spell_putricide_mutated_transformation_dmg") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideMutatedTransformationDmg();
        }
};

class SpellPutricideRegurgitatedOoze : public SpellScriptLoader
{
    public:
        SpellPutricideRegurgitatedOoze() : SpellScriptLoader("spell_putricide_regurgitated_ooze") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideRegurgitatedOoze();
        }
};

// Removes aura with id stored in effect value
class SpellPutricideClearAuraEffectValue : public SpellScriptLoader
{
    public:
        SpellPutricideClearAuraEffectValue() : SpellScriptLoader("spell_putricide_clear_aura_effect_value") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptPutricideClearAuraEffectValue();
        }
};

class SpellPutricideUnstableExperiment : public SpellScriptLoader
{
public:
    SpellPutricideUnstableExperiment() : SpellScriptLoader("spell_putricide_unstable_experiment") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptPutricideUnstableExperiment();
    }
};

#endif