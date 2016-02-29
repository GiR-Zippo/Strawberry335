#ifndef __SPELL_FESTERGUT_H
#define __SPELL_FESTERGUT_H

#include "Festergut.h"

class AuraScriptSpellFestergutSpores : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptSpellFestergutSpores);

    bool Validate(SpellInfo const* /*spell*/);
    void HandlePeriodic(AuraEffect const* aurEff);
    void Register();
};

class AuraScriptFestergutBlightedSpores : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptFestergutBlightedSpores);

    bool Validate(SpellInfo const* /*spell*/);
    void ExtraEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellScriptFestergutPungentBlight : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptFestergutPungentBlight);

    bool Load();
    void HandleScript(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellScriptFestergutGastricBloat : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptFestergutGastricBloat);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleScript(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellFestergutSpores : public SpellScriptLoader
{
public:
    SpellFestergutSpores() : SpellScriptLoader("spell_festergut_spores") { }
    
    AuraScript* GetAuraScript() const
    {
        return new AuraScriptSpellFestergutSpores();
    }
};

class SpellFestergutPungentBlight : public SpellScriptLoader
{
public:
    SpellFestergutPungentBlight() : SpellScriptLoader("spell_festergut_pungent_blight") { }
    
    SpellScript* GetSpellScript() const
    {
        return new SpellScriptFestergutPungentBlight();
    }
};

class SpellFestergutGastricBloat : public SpellScriptLoader
{
public:
    SpellFestergutGastricBloat() : SpellScriptLoader("spell_festergut_gastric_bloat") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellScriptFestergutGastricBloat();
    }
};

class SpellFestergutBlightedSpores : public SpellScriptLoader
{
public:
    SpellFestergutBlightedSpores() : SpellScriptLoader("spell_festergut_blighted_spores") { }

    AuraScript* GetAuraScript() const
    {
        return new AuraScriptFestergutBlightedSpores();
    }
};

#endif