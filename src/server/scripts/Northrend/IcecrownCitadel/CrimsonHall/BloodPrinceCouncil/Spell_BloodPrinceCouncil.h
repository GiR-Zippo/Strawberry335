#ifndef _SPELL_BLOODPRINCECOUNCIL_H_
#define _SPELL_BLOODPRINCECOUNCIL_H_

#include "BloodPrinceCouncil.h"

class SpellTaldaramGlitteringSparksSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTaldaramGlitteringSparksSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellTaldaramGlitteringSparks : public SpellScriptLoader
{
    public:
        SpellTaldaramGlitteringSparks() : SpellScriptLoader("spell_taldaram_glittering_sparks") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTaldaramGlitteringSparksSpellScript();
    }
};

class SpellTaldaramSummonFlameBallSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTaldaramSummonFlameBallSpellScript);

    void HandleScript(SpellEffIndex effIndex);
    void Register();
};

class SpellTaldaramSummonFlameBall : public SpellScriptLoader
{
    public:
        SpellTaldaramSummonFlameBall() : SpellScriptLoader("spell_taldaram_summon_flame_ball") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTaldaramSummonFlameBallSpellScript();
    }
};

class SpellTaldaramFlameBallVisualAuraScript : public AuraScript
{
    PrepareAuraScript(SpellTaldaramFlameBallVisualAuraScript);

    bool Load();
    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/);
    void Register();
};

class SpellTaldaramFlameBallVisual : public SpellScriptLoader
{
    public:
        SpellTaldaramFlameBallVisual() : SpellScriptLoader("spell_taldaram_flame_ball_visual") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellTaldaramFlameBallVisualAuraScript();
    }
};

class SpellTaldaramBallOfInfernoFlameSpellScript : public SpellScript
{
    PrepareSpellScript(SpellTaldaramBallOfInfernoFlameSpellScript);

    void ModAuraStack();
    void Register();
};

class SpellTaldaramBallOfInfernoFlame : public SpellScriptLoader
{
    public:
        SpellTaldaramBallOfInfernoFlame() : SpellScriptLoader("spell_taldaram_ball_of_inferno_flame") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellTaldaramBallOfInfernoFlameSpellScript();
    }
};

class SpellValanarKineticBombSpellScript : public SpellScript
{
    PrepareSpellScript(SpellValanarKineticBombSpellScript);

    void ChangeSummonPos(SpellEffIndex /*effIndex*/);
    void Register();
};

class SpellValanarKineticBombAuraScript : public AuraScript
{
    PrepareAuraScript(SpellValanarKineticBombAuraScript);

    void HandleDummyTick(AuraEffect const* /*aurEff*/);
    void Register();
};

class SpellValanarKineticBomb : public SpellScriptLoader
{
    public:
        SpellValanarKineticBomb() : SpellScriptLoader("spell_valanar_kinetic_bomb") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellValanarKineticBombSpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new SpellValanarKineticBombAuraScript();
    }
};

class SpellValanarKineticBombKnockbackAuraScript : public AuraScript
{
    PrepareAuraScript(SpellValanarKineticBombKnockbackAuraScript);

    void onApply(AuraEffect const* aurEff, AuraEffectHandleModes mode);
    void onRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode);
    void Register();
};

class SpellValanarKineticBombKnockback : public SpellScriptLoader
{
    public:
        SpellValanarKineticBombKnockback() : SpellScriptLoader("spell_valanar_kinetic_bomb_knockback") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellValanarKineticBombKnockbackAuraScript();
    }
};

class SpellValanarKineticBombAbsorbAuraScript : public AuraScript
{
    PrepareAuraScript(SpellValanarKineticBombAbsorbAuraScript);

    void OnAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount);
    void Register();
};

class SpellValanarKineticBombAbsorb : public SpellScriptLoader
{
    public:
        SpellValanarKineticBombAbsorb() : SpellScriptLoader("spell_valanar_kinetic_bomb_absorb") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellValanarKineticBombAbsorbAuraScript();
    }
};

class SpellBloodCouncilShadowPrisonAuraScript : public AuraScript
{
    PrepareAuraScript(SpellBloodCouncilShadowPrisonAuraScript);

    void HandleDummyTick(AuraEffect const* aurEff);
    void Register();
};

class SpellBloodCouncilShadowPrison : public SpellScriptLoader
{
    public:
        SpellBloodCouncilShadowPrison() : SpellScriptLoader("spell_blood_council_shadow_prison") { }

    AuraScript* GetAuraScript() const
    {
        return new SpellBloodCouncilShadowPrisonAuraScript();
    }
};

class SpellBloodCouncilShadowPrisonDamageSpellScript : public SpellScript
{
    PrepareSpellScript(SpellBloodCouncilShadowPrisonDamageSpellScript);

    void AddExtraDamage();
    void Register();
};

class SpellBloodCouncilShadowPrisonDamage : public SpellScriptLoader
{
    public:
        SpellBloodCouncilShadowPrisonDamage() : SpellScriptLoader("spell_blood_council_shadow_prison_damage") { }

    SpellScript* GetSpellScript() const
    {
        return new SpellBloodCouncilShadowPrisonDamageSpellScript();
    }
};

#endif