#ifndef __SPELL_PLAGUEWORKS_H
#define __SPELL_PLAGUEWORKS_H

#include "PlagueWorks.h"

class SpellScriptPlagueWorksDecimate : public SpellScript
{
    PrepareSpellScript(SpellScriptPlagueWorksDecimate);

    void HandleScript(SpellEffIndex /*effIndex*/);

    void Register();
};



class SpellPlagueWorksDecimate : public SpellScriptLoader
{
public:
    SpellPlagueWorksDecimate() : SpellScriptLoader("spell_stinky_precious_decimate") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptPlagueWorksDecimate();
    }
};

#endif