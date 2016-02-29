#ifndef __SPELL_SINDRAGOSA_H_
#define __SPELL_SINDRAGOSA_H_

#include "Sindragosa.h"
#include "SpellScript.h"
#include "ScriptedCreature.h"
#include "GridNotifiers.h"

class SpellSindragosasFurySpellScript : public SpellScript
{
    PrepareSpellScript(SpellSindragosasFurySpellScript);

    bool Load();
    void SelectDest();
    void CountTargets(std::list<WorldObject*>& targets);
    void HandleDummy(SpellEffIndex effIndex);
    void Register();

    uint32 _targetCount;
};

class SpellSindragosasFury : public SpellScriptLoader
{
    public:
        SpellSindragosasFury() : SpellScriptLoader("spell_sindragosa_s_fury") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellSindragosasFurySpellScript();
    }
};

class SpellSindragosaUnchainedMagicSpellScript : public SpellScript
{
    PrepareSpellScript(SpellSindragosaUnchainedMagicSpellScript);

    void FilterTargets(std::list<WorldObject*>& unitList);
    void Register();
};

class SpellSindragosaUnchainedMagic : public SpellScriptLoader
{
    public:
        SpellSindragosaUnchainedMagic() : SpellScriptLoader("spell_sindragosa_unchained_magic") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellSindragosaUnchainedMagicSpellScript();
    }
};

class SpellSindragosaInstabilityAuraScript : public AuraScript
{
    PrepareAuraScript(SpellSindragosaInstabilityAuraScript);

    bool Validate(SpellInfo const* /*spell*/);
    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellSindragosaInstability : public SpellScriptLoader
{
    public:
        SpellSindragosaInstability() : SpellScriptLoader("spell_sindragosa_instability") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellSindragosaInstabilityAuraScript();
    }
};

class SpellSindragosaFrostBeaconAuraScript : public AuraScript
        {
            PrepareAuraScript(SpellSindragosaFrostBeaconAuraScript);

            bool Validate(SpellInfo const* /*spell*/);
            void PeriodicTick(AuraEffect const* /*aurEff*/);
            void Register();
        };

class SpellSindragosaFrostBeacon : public SpellScriptLoader
{
    public:
        SpellSindragosaFrostBeacon() : SpellScriptLoader("spell_sindragosa_frost_beacon") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellSindragosaFrostBeaconAuraScript();
    }
};

class SpellSindragosaIceTombAuraScript : public AuraScript
{
    PrepareAuraScript(SpellSindragosaIceTombAuraScript);

    bool Validate(SpellInfo const* /*spell*/);
    bool Load();
    void PeriodicTick(AuraEffect const* /*aurEff*/);
    void Register();
    bool _apply;
};

class SpellSindragosaIceTomb : public SpellScriptLoader
{
    public:
        SpellSindragosaIceTomb() : SpellScriptLoader("spell_sindragosa_ice_tomb_trap") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellSindragosaIceTombAuraScript();
    }
};

class SpellSindragosaIcyGripSpellScript : public SpellScript
{
    PrepareSpellScript(SpellSindragosaIcyGripSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellSindragosaIcyGrip : public SpellScriptLoader
{
    public:
        SpellSindragosaIcyGrip() : SpellScriptLoader("spell_sindragosa_icy_grip") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellSindragosaIcyGripSpellScript();
    }
};

class SpellSindragosaMysticBuffetSpellScript : public SpellScript
{
    PrepareSpellScript(SpellSindragosaMysticBuffetSpellScript);

    void FilterTargets(std::list<WorldObject*>& targets);
    void Register();
};

class SpellSindragosaMysticBuffet : public SpellScriptLoader
{
    public:
        SpellSindragosaMysticBuffet() : SpellScriptLoader("spell_sindragosa_mystic_buffet") { }

    SpellScript* GetSpellScript() const
        {
            return new SpellSindragosaMysticBuffetSpellScript();
        }
};

class SpellRimefangIcyBlastSpellScript : public SpellScript
{
    PrepareSpellScript(SpellRimefangIcyBlastSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleTriggerMissile(SpellEffIndex effIndex);
    void Register();
};

class SpellRimefangIcyBlast : public SpellScriptLoader
{
    public:
        SpellRimefangIcyBlast() : SpellScriptLoader("spell_rimefang_icy_blast") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellRimefangIcyBlastSpellScript();
    }
};

class SpellFrostwardenHandlerOrderWhelpSpellScript : public SpellScript
{
    PrepareSpellScript(SpellFrostwardenHandlerOrderWhelpSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void FilterTargets(std::list<WorldObject*>& targets);
    void HandleForcedCast(SpellEffIndex effIndex);
    void Register();
};

class SpellFrostwardenHandlerOrderWhelp : public SpellScriptLoader
{
    public:
        SpellFrostwardenHandlerOrderWhelp() : SpellScriptLoader("spell_frostwarden_handler_order_whelp") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellFrostwardenHandlerOrderWhelpSpellScript();
    }
};

class SpellFrostwardenHandlerFocusFireSpellScript : public SpellScript
{
    PrepareSpellScript(SpellFrostwardenHandlerFocusFireSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellFrostwardenHandlerFocusFireAuraScript : public AuraScript
{
    PrepareAuraScript(SpellFrostwardenHandlerFocusFireAuraScript);

    void PeriodicTick(AuraEffect const* /*aurEff*/);
    void Register();
};

class SpellFrostwardenHandlerFocusFire : public SpellScriptLoader
{
    public:
        SpellFrostwardenHandlerFocusFire() : SpellScriptLoader("spell_frostwarden_handler_focus_fire") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellFrostwardenHandlerFocusFireSpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new SpellFrostwardenHandlerFocusFireAuraScript();
    }
};

class SpellSindragosaCollisionFilterSpellScript : public SpellScript
{
    PrepareSpellScript(SpellSindragosaCollisionFilterSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void FilterTargets(std::list<WorldObject*>& targets);
    void Register();
};

class SpellSindragosaCollisionFilter : public SpellScriptLoader
{
    public:
        SpellSindragosaCollisionFilter() : SpellScriptLoader("spell_sindragosa_collision_filter") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellSindragosaCollisionFilterSpellScript();
    }
};
#endif