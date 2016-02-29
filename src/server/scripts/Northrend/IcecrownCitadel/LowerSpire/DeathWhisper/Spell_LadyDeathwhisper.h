#ifndef __SPELL_LADY_DEATHWHISPER_H
#define __SPELL_LADY_DEATHWHISPER_H

#include "LadyDeathwhisper.h"


class AuraScriptDeathwhisperManaBarrier : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptDeathwhisperManaBarrier);

    void HandlePeriodicTick(AuraEffect const* /*aurEff*/);
    void Register();

};



class SpellScriptCultistDarkMartyrdom : public SpellScript
{
public: 
    PrepareSpellScript(SpellScriptCultistDarkMartyrdom);

    void HandleEffect(SpellEffIndex /*effIndex*/);
    void Register();

};



class SpellDeathwhisperManaBarrier : public SpellScriptLoader
{
public:
    SpellDeathwhisperManaBarrier() : SpellScriptLoader("spell_deathwhisper_mana_barrier") { }

    AuraScript* GetAuraScript() const
    {
        return new AuraScriptDeathwhisperManaBarrier();
    }
};

class SpellCultistDarkMartyrdom : public SpellScriptLoader
{
public:
    SpellCultistDarkMartyrdom() : SpellScriptLoader("spell_cultist_dark_martyrdom") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptCultistDarkMartyrdom();
    }
};

#endif