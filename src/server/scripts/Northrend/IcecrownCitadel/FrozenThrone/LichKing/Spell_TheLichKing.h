#ifndef __SPELL_THELICHKING_H_
#define __SPELL_THELICHKING_H_

#include "TheLichKing.h"

class SpellTheLichKingInfestAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingInfestAuraScript);

    void OnPeriodic(AuraEffect const* /*aurEff*/);
    void OnUpdate(AuraEffect* aurEff);
    void Register();
};

class SpellTheLichKingInfest : public SpellScriptLoader
{
    public:
        SpellTheLichKingInfest() :  SpellScriptLoader("spell_the_lich_king_infest") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingInfestAuraScript();
    }
};

class SpellTheLichKingNecroticPlagueAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingNecroticPlagueAuraScript);

    bool Validate(SpellInfo const* /*spell*/);
    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellTheLichKingNecroticPlague : public SpellScriptLoader
{
    public:
        SpellTheLichKingNecroticPlague() :  SpellScriptLoader("spell_the_lich_king_necrotic_plague") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingNecroticPlagueAuraScript();
    }
};

class NecrotigPlagueJumpTargetFilter
{
    public:
        bool operator()(WorldObject* pWObj)
        {
            if (Unit* pUnit = pWObj->ToUnit())
            {
                if (pUnit->GetEntry() == NPC_THE_LICH_KING)
                    return true;

                Unit *owner = pUnit->GetOwner();
                if (owner && owner->GetTypeId() == TYPEID_PLAYER)
                    return true;
            }

            return false;
        }
};

class SpellTheLichKingNecroticPlagueJumpSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingNecroticPlagueJumpSpellScript);

    bool Load();
    void FilterTargets(std::list<WorldObject*>& targets);
    void CheckAura();
    void AddMissingStack();
    void Register();

    bool _hadAura;
};

class SpellTheLichKingNecroticPlagueJumpAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingNecroticPlagueJumpAuraScript);

    bool Load();
    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/);
    void OnDispel(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/);
    void AfterDispel(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/);
    void Register();

    int32 _lastAmount;
};

class SpellTheLichKingNecroticPlagueJump : public SpellScriptLoader
{
    public:
        SpellTheLichKingNecroticPlagueJump() :  SpellScriptLoader("spell_the_lich_king_necrotic_plague_jump") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingNecroticPlagueJumpSpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingNecroticPlagueJumpAuraScript();
    }
};

class SpellTheLichKingShadowTrapVisualAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingShadowTrapVisualAuraScript);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellTheLichKingShadowTrapVisual : public SpellScriptLoader
{
    public:
        SpellTheLichKingShadowTrapVisual() : SpellScriptLoader("spell_the_lich_king_shadow_trap_visual") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingShadowTrapVisualAuraScript();
    }
};

class SpellTheLichKingShadowTrapPeriodicSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingShadowTrapPeriodicSpellScript);

    void CheckTargetCount(std::list<WorldObject*>& targets);
    void Register();
};

class SpellTheLichKingShadowTrapPeriodic : public SpellScriptLoader
{
    public:
        SpellTheLichKingShadowTrapPeriodic() : SpellScriptLoader("spell_the_lich_king_shadow_trap_periodic") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingShadowTrapPeriodicSpellScript();
    }
};

class SpellTheLichKingQuakeSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingQuakeSpellScript);

    bool Load();
    void FilterTargets(std::list<WorldObject*>& targets);
    void HandleSendEvent(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellTheLichKingQuake : public SpellScriptLoader
{
    public:
        SpellTheLichKingQuake() : SpellScriptLoader("spell_the_lich_king_quake") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingQuakeSpellScript();
    }
};

class SpellTheLichKingIceBurstTargetSearchSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingIceBurstTargetSearchSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void CheckTargetCount(std::list<WorldObject*>& unitList);
    void Register();
};

class SpellTheLichKingIceBurstTargetSearch : public SpellScriptLoader
{
    public:
        SpellTheLichKingIceBurstTargetSearch() : SpellScriptLoader("spell_the_lich_king_ice_burst_target_search") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingIceBurstTargetSearchSpellScript();
    }
};

class SpellTheLichKingRagingSpiritSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingRagingSpiritSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingRagingSpirit : public SpellScriptLoader
{
    public:
        SpellTheLichKingRagingSpirit() : SpellScriptLoader("spell_the_lich_king_raging_spirit") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingRagingSpiritSpellScript();
    }
};

class ExactDistanceCheck
{
    public:
        ExactDistanceCheck(Unit* source, float dist) : _source(source), _dist(dist) {}

        bool operator()(WorldObject* unit)
        {
            return _source->GetExactDist2d(unit) > _dist;
        }

    private:
        Unit* _source;
        float _dist;
};

class SpellTheLichKingDefileSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingDefileSpellScript);

    void CorrectRange(std::list<WorldObject*>& targets);
    void ChangeDamageAndGrow();
    void Register();
};

class SpellTheLichKingDefile : public SpellScriptLoader
{
    public:
        SpellTheLichKingDefile() : SpellScriptLoader("spell_the_lich_king_defile") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingDefileSpellScript();
    }
};

class SpellTheLichKingSummonIntoAirSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingSummonIntoAirSpellScript);

    void ModDestHeight(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingSummonIntoAir : public SpellScriptLoader
{
    public:
        SpellTheLichKingSummonIntoAir() : SpellScriptLoader("spell_the_lich_king_summon_into_air") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingSummonIntoAirSpellScript();
    }
};

class SpellTheLichKingSoulReaperAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingSoulReaperAuraScript);

    bool Validate(SpellInfo const* /*spell*/);
    void OnPeriodic(AuraEffect const* /*aurEff*/);
    void Register();
};

class SpellTheLichKingSoulReaper : public SpellScriptLoader
{
    public:
        SpellTheLichKingSoulReaper() :  SpellScriptLoader("spell_the_lich_king_soul_reaper") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingSoulReaperAuraScript();
    }
};

class SpellTheLichKingValkyrTargetSearchSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingValkyrTargetSearchSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    bool Load();
    void SelectTarget(std::list<WorldObject*>& targets);
    void ReplaceTarget(std::list<WorldObject*>& targets);
    void HandleScript(SpellEffIndex effIndex);
    void Register();

    WorldObject* _target;
};

class SpellTheLichKingValkyrTargetSearch : public SpellScriptLoader
{
    public:
        SpellTheLichKingValkyrTargetSearch() : SpellScriptLoader("spell_the_lich_king_valkyr_target_search") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingValkyrTargetSearchSpellScript();
    }
};

class SpellTheLichKingEjectAllPassengersSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingEjectAllPassengersSpellScript);

    bool Load();
    void HandleDummy(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingEjectAllPassengers : public SpellScriptLoader
{
    public:
        SpellTheLichKingEjectAllPassengers() : SpellScriptLoader("spell_the_lich_king_eject_all_passengers") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingEjectAllPassengersSpellScript();
    }
};

class SpellTheLichKingCastBackToCasterSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingCastBackToCasterSpellScript);

    void HandleScript(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellTheLichKingCastBackToCaster : public SpellScriptLoader
{
    public:
        SpellTheLichKingCastBackToCaster() :  SpellScriptLoader("spell_the_lich_king_cast_back_to_caster") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingCastBackToCasterSpellScript();
    }
};

class SpellTheLichKingLifeSiphonSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingLifeSiphonSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void TriggerHeal();
    void Register();
};

class SpellTheLichKingLifeSiphon : public SpellScriptLoader
{
    public:
        SpellTheLichKingLifeSiphon() : SpellScriptLoader("spell_the_lich_king_life_siphon") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingLifeSiphonSpellScript();
    }
};

class SpellTheLichKingVileSpiritsAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingVileSpiritsAuraScript);

    bool Load();
    void OnPeriodic(AuraEffect const* aurEff);
    void Register();

    bool _is25Man;
};

class SpellTheLichKingVileSpirits : public SpellScriptLoader
{
    public:
        SpellTheLichKingVileSpirits() : SpellScriptLoader("spell_the_lich_king_vile_spirits") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingVileSpiritsAuraScript();
    }
};

class SpellTheLichKingVileSpiritsVisualSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingVileSpiritsVisualSpellScript);

    void ModDestHeight(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellTheLichKingVileSpiritsVisual : public SpellScriptLoader
{
    public:
        SpellTheLichKingVileSpiritsVisual() : SpellScriptLoader("spell_the_lich_king_vile_spirits_visual") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingVileSpiritsVisualSpellScript();
    }
};

class SpellTheLichKingVileSpiritMoveTargetSearchSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingVileSpiritMoveTargetSearchSpellScript);

    bool Load();
    void SelectTarget(std::list<WorldObject*>& targets);
    void HandleScript(SpellEffIndex effIndex);
    void Register();

    WorldObject* _target;
};

class SpellTheLichKingVileSpiritMoveTargetSearch : public SpellScriptLoader
{
    public:
        SpellTheLichKingVileSpiritMoveTargetSearch() : SpellScriptLoader("spell_the_lich_king_vile_spirit_move_target_search") { }

    SpellScript* GetSpellScript() const
        {
            return new SpellTheLichKingVileSpiritMoveTargetSearchSpellScript();
        }
};

class SpellTheLichKingVileSpiritDamageTargetSearchSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingVileSpiritDamageTargetSearchSpellScript);

    bool Load();
    void CheckTargetCount(std::list<WorldObject*>& targets);
    void Register();

    Unit* _target;
};

class SpellTheLichKingVileSpiritDamageTargetSearch : public SpellScriptLoader
{
    public:
        SpellTheLichKingVileSpiritDamageTargetSearch() : SpellScriptLoader("spell_the_lich_king_vile_spirit_damage_target_search") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingVileSpiritDamageTargetSearchSpellScript();
    }
};

class SpellTheLichKingHarvestSoulAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingHarvestSoulAuraScript);

    bool Load();
    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellTheLichKingHarvestSoul : public SpellScriptLoader
{
    public:
        SpellTheLichKingHarvestSoul() : SpellScriptLoader("spell_the_lich_king_harvest_soul") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingHarvestSoulAuraScript();
    }
};

class SpellTheLichKingLightsFavorAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingLightsFavorAuraScript);

    void OnPeriodic(AuraEffect const* /*aurEff*/);
    void CalculateBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated);
    void Register();
};

class SpellTheLichKingLightsFavor : public SpellScriptLoader
{
    public:
        SpellTheLichKingLightsFavor() : SpellScriptLoader("spell_the_lich_king_lights_favor") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingLightsFavorAuraScript();
    }
};

class SpellTheLichKingSoulRipAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingSoulRipAuraScript);

    void OnPeriodic(AuraEffect const* aurEff);
    void Register();
};

class SpellTheLichKingSoulRip : public SpellScriptLoader
{
    public:
        SpellTheLichKingSoulRip() : SpellScriptLoader("spell_the_lich_king_soul_rip") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingSoulRipAuraScript();
    }
};

class SpellTheLichKingRestoreSoulSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingRestoreSoulSpellScript);

    bool Load();
    void HandleScript(SpellEffIndex /*effIndex*/);
    void RemoveAura();
    void Register();
    InstanceScript* _instance;
};

class SpellTheLichKingRestoreSoul : public SpellScriptLoader
{
    public:
        SpellTheLichKingRestoreSoul() : SpellScriptLoader("spell_the_lich_king_restore_soul") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingRestoreSoulSpellScript();
    }
};

class SpellTheLichKingInFrostmourneRoomAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTheLichKingInFrostmourneRoomAuraScript);

    bool Load();
    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellTheLichKingInFrostmourneRoom : public SpellScriptLoader
{
    public:
        SpellTheLichKingInFrostmourneRoom() : SpellScriptLoader("spell_the_lich_king_in_frostmourne_room") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTheLichKingInFrostmourneRoomAuraScript();
    }
};

class SpellTheLichKingSummonSpiritBombSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingSummonSpiritBombSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingSummonSpiritBomb : public SpellScriptLoader
{
    public:
        SpellTheLichKingSummonSpiritBomb() : SpellScriptLoader("spell_the_lich_king_summon_spirit_bomb") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingSummonSpiritBombSpellScript();
    }
};

class SpellTheLichKingTriggerVileSpiritSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingTriggerVileSpiritSpellScript);

    void ActivateSpirit();
    void Register();
};

class SpellTheLichKingTriggerVileSpirit : public SpellScriptLoader
{
    public:
        SpellTheLichKingTriggerVileSpirit() : SpellScriptLoader("spell_the_lich_king_trigger_vile_spirit") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingTriggerVileSpiritSpellScript();
    }
};

class SpellTheLichKingJumpSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingJumpSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingJump : public SpellScriptLoader
{
    public:
        SpellTheLichKingJump() : SpellScriptLoader("spell_the_lich_king_jump") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingJumpSpellScript();
    }
};

class SpellTheLichKingJumpRemoveAuraSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingJumpRemoveAuraSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingJumpRemoveAura : public SpellScriptLoader
{
    public:
        SpellTheLichKingJumpRemoveAura() : SpellScriptLoader("spell_the_lich_king_jump_remove_aura") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingJumpRemoveAuraSpellScript();
    }
};

class SpellTheLichKingPlayMovieSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingPlayMovieSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingPlayMovie : public SpellScriptLoader
{
    public:
        SpellTheLichKingPlayMovie() : SpellScriptLoader("spell_the_lich_king_play_movie") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingPlayMovieSpellScript();
    }
};

class SpellTheLichKingTriggerWickedSpiritSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingTriggerWickedSpiritSpellScript);

    void SelectTarget(std::list<WorldObject*>& targets);
    void HandleDummy(SpellEffIndex effIndex);
    void Register();
};

class SpellTheLichKingTriggerWickedSpirit : public SpellScriptLoader
{
    public:
        SpellTheLichKingTriggerWickedSpirit() : SpellScriptLoader("spell_the_lich_king_trigger_wicked_spirit") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTheLichKingTriggerWickedSpiritSpellScript();
    }
};

class SpellTheLichKingHarvestSoulTeleportHeroicSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTheLichKingHarvestSoulTeleportHeroicSpellScript);

    bool Validate(SpellInfo const* /*spell*/);
    void HandleTeleport(SpellEffIndex /*effIndex*/);
    void Register();

    const float portPositionX = 495.1f;
    const float portPositionY = -2522.6f;
    const float portPositionZ = 1052.0f;

    const float teleportCoef10 = 0.62831f;
    const float teleportCoef25 = 0.25132f;
    const float teleportRadius = 16.5f;
};

class SpellTheLichKingHarvestSoulTeleportHeroic : public SpellScriptLoader
{
    public:
    SpellTheLichKingHarvestSoulTeleportHeroic() : SpellScriptLoader("spell_the_lich_king_harvest_soul_teleport_hc") { }

        SpellScript* GetSpellScript() const
        {
            return new SpellTheLichKingHarvestSoulTeleportHeroicSpellScript();
        }
};


#endif