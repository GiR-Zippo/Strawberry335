#ifndef __SPELL_MARROWGAR_H
#define __SPELL_MARROWGAR_H

#include "LordMarrowgar.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"


class SpellColdflameSpellScript : public SpellScript
{
    PrepareSpellScript(SpellColdflameSpellScript);

    void SelectTarget(std::list<WorldObject*>& targets);
    void HandleScriptEffect(SpellEffIndex effIndex);
    void Register();
};

class SpellColdflame : public SpellScriptLoader
{
    public:
        SpellColdflame() : SpellScriptLoader("spell_marrowgar_coldflame") { }

    SpellScript* GetSpellScript() const
        {
            return new SpellColdflameSpellScript();
        }
};

class SpellColdflameBonestormSpellScript : public SpellScript
{
    PrepareSpellScript(SpellColdflameBonestormSpellScript);

    void HandleScriptEffect(SpellEffIndex effIndex);
    void Register();
};

class SpellColdflameBonestorm : public SpellScriptLoader
{
    public:
        SpellColdflameBonestorm() : SpellScriptLoader("spell_marrowgar_coldflame_bonestorm") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellColdflameBonestormSpellScript();
    }
};

class SpellColdflameDamageAuraScript : public AuraScript
{
    PrepareAuraScript(SpellColdflameDamageAuraScript);

    void OnPeriodic(AuraEffect const* /*aurEff*/);
    void Register();

    static bool HasCooldown(Unit* unit);

    static std::map<uint64,uint32> cooldowns;
};

class SpellColdflameDamage : public SpellScriptLoader
{
    public:
        SpellColdflameDamage() : SpellScriptLoader("spell_marrowgar_coldflame_damage") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellColdflameDamageAuraScript();
    }
};

class SpellBoneSpikeGraveyardSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBoneSpikeGraveyardSpellScript);

    SpellCastResult CheckCast();
    void HandleSpikes(SpellEffIndex effIndex);
    void Register();
};

class SpellBoneSpikeGraveyard : public SpellScriptLoader
{
    public:
        SpellBoneSpikeGraveyard() : SpellScriptLoader("spell_marrowgar_bone_spike_graveyard") { }

    SpellScript* GetSpellScript() const
        {
            return new SpellBoneSpikeGraveyardSpellScript();
        }
};

class SpellBoneStormSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBoneStormSpellScript);

    void RecalculateDamage();
    void Register();
};

class SpellBoneStorm : public SpellScriptLoader
{
    public:
        SpellBoneStorm() : SpellScriptLoader("spell_marrowgar_bone_storm") { }

    SpellScript* GetSpellScript() const
        {
            return new SpellBoneStormSpellScript();
        }
};
#endif