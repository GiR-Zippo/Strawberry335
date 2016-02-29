#ifndef __SPELL_BLOODQUEENLANATHEL_H_
#define __SPELL_BLOODQUEENLANATHEL_H_

#include "BloodQueenLanaThel.h"
#include "Boss_BloodQueenLanaThel.h"

class SpellBloodQueenVampiricBiteSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBloodQueenVampiricBiteSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    SpellCastResult CheckTarget();
    void OnCast();
    void Register();
};

class SpellBloodQueenVampiricBite : public SpellScriptLoader
{
    public:
        SpellBloodQueenVampiricBite() : SpellScriptLoader("spell_blood_queen_vampiric_bite") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellBloodQueenVampiricBiteSpellScript();
    }
};

class SpellBloodQueenFrenziedBloodthirstAuraScript : public AuraScript
{
    PrepareAuraScript(SpellBloodQueenFrenziedBloodthirstAuraScript);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellBloodQueenFrenziedBloodthirst : public SpellScriptLoader
{
    public:
        SpellBloodQueenFrenziedBloodthirst() : SpellScriptLoader("spell_blood_queen_frenzied_bloodthirst") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellBloodQueenFrenziedBloodthirstAuraScript();
    }
};

class BloodboltHitCheck
{
    public:
        explicit BloodboltHitCheck(BossBloodQueenLanaThelAI* ai) : _ai(ai) {}

        bool operator()(WorldObject* object) const
        {
            return _ai->WasBloodbolted(object->GetGUID()) || object->GetTypeId()!= TYPEID_PLAYER;
        }

    private:
        BossBloodQueenLanaThelAI* _ai;
};

class SpellBloodQueenBloodboltSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBloodQueenBloodboltSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    bool Load();
    void FilterTargets(std::list<WorldObject*>& targets);
    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellBloodQueenBloodbolt : public SpellScriptLoader
{
    public:
        SpellBloodQueenBloodbolt() : SpellScriptLoader("spell_blood_queen_bloodbolt") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellBloodQueenBloodboltSpellScript();
    }
};

class SpellBloodQueenPactOfTheDarkfallenSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBloodQueenPactOfTheDarkfallenSpellScript);

    void FilterTargets(std::list<WorldObject*>& targets);
    void Register();
};

class SpellBloodQueenPactOfTheDarkfallen : public SpellScriptLoader
{
    public:
        SpellBloodQueenPactOfTheDarkfallen() : SpellScriptLoader("spell_blood_queen_pact_of_the_darkfallen") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellBloodQueenPactOfTheDarkfallenSpellScript();
    }
};

class SpellBloodQueenPactOfTheDarkfallenDmgAuraScript : public AuraScript
{
    PrepareAuraScript(SpellBloodQueenPactOfTheDarkfallenDmgAuraScript);

    bool Validate(SpellInfo const* /*spell*/);
    // this is an additional effect to be executed
    void PeriodicTick(AuraEffect const* aurEff);
    void Register();
};

class SpellBloodQueenPactOfTheDarkfallenDmg : public SpellScriptLoader
{
    public:
        SpellBloodQueenPactOfTheDarkfallenDmg() : SpellScriptLoader("spell_blood_queen_pact_of_the_darkfallen_dmg") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellBloodQueenPactOfTheDarkfallenDmgAuraScript();
    }
};

class SpellBloodQueenPactOfTheFarkfallenDmgTargetSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBloodQueenPactOfTheFarkfallenDmgTargetSpellScript);

    void FilterTargets(std::list<WorldObject*>& unitList);
    void Register();
};

class SpellBloodQueenPactOfTheFarkfallenDmgTarget : public SpellScriptLoader
{
    public:
        SpellBloodQueenPactOfTheFarkfallenDmgTarget() : SpellScriptLoader("spell_blood_queen_pact_of_the_darkfallen_dmg_target") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellBloodQueenPactOfTheFarkfallenDmgTargetSpellScript();
    }
};



#endif