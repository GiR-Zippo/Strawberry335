#ifndef __SPELL_GUNSHIPBATTLE_H_
#define __SPELL_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "SpellScript.h"

class SpellRemoveRocketPackSpellScript : public SpellScript
{
    PrepareSpellScript(SpellRemoveRocketPackSpellScript);

    void HandleEffect(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellRemoveRocketPack : public SpellScriptLoader
{
    public:
        SpellRemoveRocketPack() : SpellScriptLoader("spell_icc_remove_rocket_pack") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellRemoveRocketPackSpellScript();
    }
};

class SpellHeatDrainSpellScript : public SpellScript
{
    PrepareSpellScript(SpellHeatDrainSpellScript);

    void HandleDummy(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellHeatDrainSpell : public SpellScriptLoader
{
    public:
        SpellHeatDrainSpell() : SpellScriptLoader("spell_gb_heat_drain") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellHeatDrainSpellScript();
    }
};

class SpellOverheatDrainSpellScript : public SpellScript
{
    PrepareSpellScript(SpellOverheatDrainSpellScript);

    void HandleDummy(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellOverheatDrain : public SpellScriptLoader
{
    public:
        SpellOverheatDrain() : SpellScriptLoader("spell_gb_overheat_drain") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellOverheatDrainSpellScript();
    }
};

class SpellIncineratingBlastSpellScript : public SpellScript
{
    PrepareSpellScript(SpellIncineratingBlastSpellScript);

    void AddExtraDamage();
    void Register();
};

class SpellIncineratingBlast : public SpellScriptLoader
{
    public:
        SpellIncineratingBlast() : SpellScriptLoader("spell_gb_incinerating_blast") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellIncineratingBlastSpellScript();
    }
};

class SpellBurningPitchSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBurningPitchSpellScript);

    bool Validate(SpellInfo const* /*spellInfo*/);
    void HandleDummy(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellBurningPitch : public SpellScriptLoader
{
    public:
        SpellBurningPitch() : SpellScriptLoader("spell_gb_burning_pitch") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellBurningPitchSpellScript();
    }
};

class SpellRocketPackAuraScript : public AuraScript
{
    PrepareAuraScript(SpellRocketPackAuraScript);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellRocketPack : public SpellScriptLoader
{
    public:
        SpellRocketPack() : SpellScriptLoader("spell_rocket_pack") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellRocketPackAuraScript();
    }
};

#endif