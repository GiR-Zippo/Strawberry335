#ifndef __SPELL_DEATHBRINGER_SAURFANG_H
#define __SPELL_DEATHBRINGER_SAURFANG_H

#include "DeathbringerSaurfang.h"

class AuraScriptDeathbringerBloodPower : public AuraScript
{
public:

    PrepareAuraScript(AuraScriptDeathbringerBloodPower);
    
    void RecalculateHook(AuraEffect const* /*aurEffect*/, int32& amount, bool& canBeRecalculated);
    void Register();
    bool Load();
};

class AuraScriptDeathbringerBloodLinkAura : public AuraScript
{
public:

    PrepareAuraScript(AuraScriptDeathbringerBloodLinkAura);
    
    bool Validate(SpellInfo const* /*spellInfo*/);
    void HandlePeriodicTick(AuraEffect const* /*aurEff*/);
    void Register();
};



class SpellScriptDeathbringerBloodLink : public SpellScript
{
public:

    PrepareSpellScript(SpellScriptDeathbringerBloodLink);
    bool Validate(SpellInfo const* /*spellInfo*/);
    void HandleDummy(SpellEffIndex /*effIndex*/);
    void Register();

};

class SpellScriptDeathbringerBloodPower : public SpellScript
{
public:

    PrepareSpellScript(SpellScriptDeathbringerBloodPower);
    
    void ModAuraValue();
    void Register();
};

class SpellScriptDeathbringerRuneOfBlood : public SpellScript
{
public:

    PrepareSpellScript(SpellScriptDeathbringerRuneOfBlood);
    
    bool Validate(SpellInfo const* /*spellInfo*/);
    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellScriptDeathbringerBloodNova : public SpellScript
{
public:

    PrepareSpellScript(SpellScriptDeathbringerBloodNova);
    
    bool Validate(SpellInfo const* /*spellInfo*/);
    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellScriptDeathbringerBloodNovaTargeting : public SpellScript
{
public:

    PrepareSpellScript(SpellScriptDeathbringerBloodNovaTargeting);
    
    bool Load() { target = NULL; return true; }
    void FilterTargetsInitial(std::list<WorldObject*>& targets);
    void FilterTargetsSubsequent(std::list<WorldObject*>& unitList);
    void HandleForceCast(SpellEffIndex /*effIndex*/);
    void Register();

private:

    WorldObject* target;
};

class SpellScriptDeathbringerRemoveMarks : public SpellScript
{
public:

    PrepareSpellScript(SpellScriptDeathbringerRemoveMarks);
    void HandleScript(SpellEffIndex effIndex);
    void Register();

};

class SpellScriptDeathbringerCentOfBlood : public SpellScript
{
public:

    PrepareSpellScript(SpellScriptDeathbringerCentOfBlood);
    void FilterTargets(std::list<WorldObject*>& targets);
    void Register();

};



class SpellDeathbringerBloodLink : public SpellScriptLoader
{
public:
    SpellDeathbringerBloodLink() : SpellScriptLoader("spell_deathbringer_blood_link") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptDeathbringerBloodLink();
    }
};

class SpellDeathbringerBloodLinkAura : public SpellScriptLoader
{
public:
    SpellDeathbringerBloodLinkAura() : SpellScriptLoader("spell_deathbringer_blood_link_aura") { }

    AuraScript* GetAuraScript() const
    {
        return new AuraScriptDeathbringerBloodLinkAura();
    }
};

class SpellDeathbringerBloodPower : public SpellScriptLoader
{
public:
    SpellDeathbringerBloodPower() : SpellScriptLoader("spell_deathbringer_blood_power") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptDeathbringerBloodPower();
    }

    AuraScript* GetAuraScript() const
    {
        return new AuraScriptDeathbringerBloodPower();
    }
};

class SpellDeathbringerRuneOfBlood : public SpellScriptLoader
{
public:
    SpellDeathbringerRuneOfBlood() : SpellScriptLoader("spell_deathbringer_rune_of_blood") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptDeathbringerRuneOfBlood();
    }

};

class SpellDeathbringerBloodNova : public SpellScriptLoader
{
public:
    SpellDeathbringerBloodNova() : SpellScriptLoader("spell_deathbringer_blood_nova") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptDeathbringerBloodNova();
    }

};

class SpellDeathbringerBloodNovaTargeting : public SpellScriptLoader
{
public:
    SpellDeathbringerBloodNovaTargeting() : SpellScriptLoader("spell_deathbringer_blood_nova_targeting") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptDeathbringerBloodNovaTargeting();
    }

};

class SpellDeathbringerRemoveMarks : public SpellScriptLoader
{
public:
    SpellDeathbringerRemoveMarks() : SpellScriptLoader("spell_deathbringer_remove_marks") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptDeathbringerRemoveMarks();
    }

};

class SpellDeathbringerCentOfBlood : public SpellScriptLoader
{
public:
    SpellDeathbringerCentOfBlood() : SpellScriptLoader("spell_deathbringer_cent_of_blood") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptDeathbringerCentOfBlood();
    }

};

#endif