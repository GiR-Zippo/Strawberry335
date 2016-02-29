#ifndef __SPELL_VALITHRIADREAMWALKER_H_
#define __SPELL_VALITHRIADREAMWALKER_H_

#include "ValithriaDreamwalker.h"
#include "SpellScript.h"
#include "ScriptedCreature.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

class SpellDreamwalkerManaVoidAuraScript : public AuraScript
{
public:
    PrepareAuraScript(SpellDreamwalkerManaVoidAuraScript);

    void PeriodicTick(AuraEffect const* aurEff);
    void Register();
};

class SpellDreamwalkerManaVoid : public SpellScriptLoader
{
    public:
        SpellDreamwalkerManaVoid() : SpellScriptLoader("spell_dreamwalker_mana_void") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellDreamwalkerManaVoidAuraScript();
    }
};

class SpellDreamwalkerDecayPeriodicTimerAuraScript : public AuraScript
{
    PrepareAuraScript(SpellDreamwalkerDecayPeriodicTimerAuraScript);

    bool Load();
    void DecayPeriodicTimer(AuraEffect* aurEff);
    void Register();

    int32 _decayRate;
};

class SpellDreamwalkerDecayPeriodicTimer : public SpellScriptLoader
{
    public:
        SpellDreamwalkerDecayPeriodicTimer() : SpellScriptLoader("spell_dreamwalker_decay_periodic_timer") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellDreamwalkerDecayPeriodicTimerAuraScript();
    }
};

class SpellDreamwalkerSummonerSpellScript : public SpellScript
        {
            PrepareSpellScript(SpellDreamwalkerSummonerSpellScript);

            bool Load();
            void FilterTargets(std::list<WorldObject*>& targets);
            void HandleForceCast(SpellEffIndex effIndex);
            void Register();
        };

class SpellDreamwalkerSummoner : public SpellScriptLoader
{
    public:
        SpellDreamwalkerSummoner() : SpellScriptLoader("spell_dreamwalker_summoner") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellDreamwalkerSummonerSpellScript();
    }
};

class SpellDreamwalkerSummonSuppresserAuraScript : public AuraScript
        {
            PrepareAuraScript(SpellDreamwalkerSummonSuppresserAuraScript);

            void PeriodicTick(AuraEffect const* /*aurEff*/);
            void Register();
        };

class SpellDreamwalkerSummonSuppresser : public SpellScriptLoader
{
    public:
        SpellDreamwalkerSummonSuppresser() : SpellScriptLoader("spell_dreamwalker_summon_suppresser") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellDreamwalkerSummonSuppresserAuraScript();
    }
};

class SpellDreamwalkerSummonSuppresserEffectSpellScript : public SpellScript
        {
            PrepareSpellScript(SpellDreamwalkerSummonSuppresserEffectSpellScript);

            bool Load();
            void HandleForceCast(SpellEffIndex effIndex);
            void Register();
        };

class SpellDreamwalkerSummonSuppresserEffect : public SpellScriptLoader
{
    public:
        SpellDreamwalkerSummonSuppresserEffect() : SpellScriptLoader("spell_dreamwalker_summon_suppresser_effect") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellDreamwalkerSummonSuppresserEffectSpellScript();
    }
};

class SpellDreamwalkerSummonDreamPortalSpellScript : public SpellScript
{
    PrepareSpellScript(SpellDreamwalkerSummonDreamPortalSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellDreamwalkerSummonDreamPortal : public SpellScriptLoader
{
    public:
        SpellDreamwalkerSummonDreamPortal() : SpellScriptLoader("spell_dreamwalker_summon_dream_portal") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellDreamwalkerSummonDreamPortalSpellScript();
    }
};

class SpellDreamwalkerSummonNightmarePortalSpellScript : public SpellScript
{
    PrepareSpellScript(SpellDreamwalkerSummonNightmarePortalSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellDreamwalkerSummonNightmarePortal : public SpellScriptLoader
{
    public:
        SpellDreamwalkerSummonNightmarePortal() : SpellScriptLoader("spell_dreamwalker_summon_nightmare_portal") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellDreamwalkerSummonNightmarePortalSpellScript();
    }
};

class SpellDreamwalkerNightmareCloudAuraScript : public AuraScript
        {
            PrepareAuraScript(SpellDreamwalkerNightmareCloudAuraScript);

            bool Load();
            void PeriodicTick(AuraEffect const* /*aurEff*/);
            void Register();

            InstanceScript* _instance;
        };

class SpellDreamwalkerNightmareCloud : public SpellScriptLoader
{
    public:
        SpellDreamwalkerNightmareCloud() : SpellScriptLoader("spell_dreamwalker_nightmare_cloud") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellDreamwalkerNightmareCloudAuraScript();
    }
};

class SpellDreamwalkerTwistedNightmaresSpellScript : public SpellScript
{
    PrepareSpellScript(SpellDreamwalkerTwistedNightmaresSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellDreamwalkerTwistedNightmares : public SpellScriptLoader
{
    public:
        SpellDreamwalkerTwistedNightmares() : SpellScriptLoader("spell_dreamwalker_twisted_nightmares") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellDreamwalkerTwistedNightmaresSpellScript();
    }
};

#endif