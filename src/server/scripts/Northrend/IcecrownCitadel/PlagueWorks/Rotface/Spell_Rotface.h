#ifndef __SPELL_ROTFACE_H
#define __SPELL_ROTFACE_H

#include "Rotface.h"

class AuraScriptRotfaceUnstableOozeExplosionSuicide : public AuraScript
{
public:
    PrepareAuraScript(AuraScriptRotfaceUnstableOozeExplosionSuicide);

    void DespawnSelf(AuraEffect const* /*aurEff*/);
    void Register();
};

class SpellScriptRotfaceOozeFlood : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceOozeFlood);

    void HandleScript(SpellEffIndex /*effIndex*/);
    void FilterTargets(std::list<WorldObject*>& targets);
    void Register();
};

class SpellScriptRotfaceMutatedInfection : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceMutatedInfection);

    bool Load() {_target = NULL; return true;}
    void FilterTargets(std::list<WorldObject*>& targets);
    void ReplaceTargets(std::list<WorldObject*>& targets);
    void NotifyTargets();
    void Register();
    WorldObject* _target;
};

class SpellScriptRotfaceLittleOozeCombine : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceLittleOozeCombine);

    void HandleScript(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellScriptRotfaceBigOozeCombine : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceBigOozeCombine);

    void HandleScript(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellScriptRotfaceBigOozeBuffCombine : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceBigOozeBuffCombine);

    void HandleScript(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellScriptRotfaceUnstableOozeExplosionInit : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceUnstableOozeExplosionInit);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleCast(SpellEffIndex effIndex);
    void Register();
};

class SpellScriptRotfaceUnstableOozeExplosion : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceUnstableOozeExplosion);

    void CheckTarget(SpellEffIndex effIndex);
    void Register();
};

class SpellScriptRotfaceOozeSummon : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceOozeSummon);

    void EffectSummon(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellScriptRotfaceSlimeSpray : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceSlimeSpray);

    void HandleScript();
    void Register();
};

class SpellScriptRotfaceSlimeSprayTick : public SpellScript
{
public:
    PrepareSpellScript(SpellScriptRotfaceSlimeSprayTick);

    void HandleScript();
    void Register();
};

struct PlayerFilter
{
    bool operator()(WorldObject* target) const
    {
        return target->GetTypeId() != TYPEID_PLAYER;
    }
};

class SpellScriptSpellRotfaceVileGasTrigger : public SpellScript
{
    PrepareSpellScript(SpellScriptSpellRotfaceVileGasTrigger);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if(PlayerFilter());
        targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
        if (targets.empty())
            return;

        std::list<WorldObject*> ranged, melee;
        std::list<WorldObject*>::iterator itr = targets.begin();
        while (itr != targets.end() && (*itr)->GetDistance(GetCaster()) < 5.0f)
        {
            melee.push_back((*itr)->ToUnit());
            ++itr;
        }

        while (itr != targets.end())
        {
            ranged.push_back((*itr)->ToUnit());
            ++itr;
        }

        uint32 minTargets = GetCaster()->GetMap()->Is25ManRaid() ? 8 : 3;
        while (ranged.size() < minTargets)
        {
            if (melee.empty())
                break;

            WorldObject* target = Trinity::Containers::SelectRandomContainerElement(melee);
            ranged.push_back(target);
            melee.remove(target);
        }

        if (!ranged.empty())
            Trinity::Containers::RandomResizeList(ranged, GetCaster()->GetMap()->Is25ManRaid() ? 3 : 1);

        targets.swap(ranged);
    }

    void HandleDummy(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        GetCaster()->CastSpell(GetHitUnit(), SPELL_VILE_GAS_TRIGGER_SUMMON);
    }

    void Register()
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptSpellRotfaceVileGasTrigger::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(SpellScriptSpellRotfaceVileGasTrigger::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};



class SpellRotfaceOozeFlood : public SpellScriptLoader
{
    public:
        SpellRotfaceOozeFlood() : SpellScriptLoader("spell_rotface_ooze_flood") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceOozeFlood();
        }
};

class SpellRotfaceMutatedInfection : public SpellScriptLoader
{
    public:
        SpellRotfaceMutatedInfection() : SpellScriptLoader("spell_rotface_mutated_infection") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceMutatedInfection();
        }
};

class SpellRotfaceLittleOozeCombine : public SpellScriptLoader
{
    public:
        SpellRotfaceLittleOozeCombine() : SpellScriptLoader("spell_rotface_little_ooze_combine") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceLittleOozeCombine();
        }
};

class SpellRotfaceBigOozeCombine : public SpellScriptLoader
{
    public:
        SpellRotfaceBigOozeCombine() : SpellScriptLoader("spell_rotface_large_ooze_combine") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceBigOozeCombine();
        }
};

class SpellRotfaceBigOozeBuffCombine : public SpellScriptLoader
{
    public:
        SpellRotfaceBigOozeBuffCombine() : SpellScriptLoader("spell_rotface_large_ooze_buff_combine") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceBigOozeBuffCombine();
        }
};

class SpellRotfaceUnstableOozeExplosionInit : public SpellScriptLoader
{
    public:
        SpellRotfaceUnstableOozeExplosionInit() : SpellScriptLoader("spell_rotface_unstable_ooze_explosion_init") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceUnstableOozeExplosionInit();
        }
};

class SpellRotfaceUnstableOozeExplosion : public SpellScriptLoader
{
    public:
        SpellRotfaceUnstableOozeExplosion() : SpellScriptLoader("spell_rotface_unstable_ooze_explosion") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceUnstableOozeExplosion();
        }
};

class SpellRotfaceUnstableOozeExplosionSuicide : public SpellScriptLoader
{
    public:
        SpellRotfaceUnstableOozeExplosionSuicide() : SpellScriptLoader("spell_rotface_unstable_ooze_explosion_suicide") { }

        AuraScript* GetAuraScript() const
        {
            return new AuraScriptRotfaceUnstableOozeExplosionSuicide();
        }
};

class SpellRotfaceOozeSummon : public SpellScriptLoader
{
    public:
        SpellRotfaceOozeSummon() : SpellScriptLoader("spell_rotface_ooze_summon") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceOozeSummon();
        }
};

class SpellRotfaceSlimeSpray : public SpellScriptLoader
{
    public:
        SpellRotfaceSlimeSpray() : SpellScriptLoader("spell_rotface_slime_spray") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceSlimeSpray();
        }
};

class SpellRotfaceSlimeSprayTick : public SpellScriptLoader
{
    public:
        SpellRotfaceSlimeSprayTick() : SpellScriptLoader("spell_rotface_slime_spray_tick") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptRotfaceSlimeSprayTick();
        }
};

class SpellRotfaceVileGasTrigger : public SpellScriptLoader
{
    public:
        SpellRotfaceVileGasTrigger() : SpellScriptLoader("spell_rotface_vile_gas_trigger") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellScriptSpellRotfaceVileGasTrigger();
        }
};

#endif