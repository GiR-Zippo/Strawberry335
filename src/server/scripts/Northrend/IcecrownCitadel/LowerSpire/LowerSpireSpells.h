#ifndef __LOWERSPIRESPELLS_H_
#define __LOWERSPIRESPELLS_H_

#include "LowerSpire.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "Npc_RottingFrostGiant.h"

class SpellFrostGiantDeathPlagueSpellScript : public SpellScript
{
    PrepareSpellScript(SpellFrostGiantDeathPlagueSpellScript);

    bool Load();
    // First effect
    void CountTargets(std::list<WorldObject*>& targets);
    // Second effect
    void FilterTargets(std::list<WorldObject*>& targets);
    void HandleScript(SpellEffIndex effIndex);
    void Register();

    bool _failed;
};

class SpellFrostGiantDeathPlague : public SpellScriptLoader
{
    public:
        SpellFrostGiantDeathPlague() : SpellScriptLoader("spell_frost_giant_death_plague") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellFrostGiantDeathPlagueSpellScript();
    }
};

#endif