#include "Spell_Rotface.h"

//////////////////////////////////
// Unstable Ooze Explosion Init //
//////////////////////////////////

bool SpellScriptRotfaceUnstableOozeExplosionInit::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_UNSTABLE_OOZE_EXPLOSION_TRIGGER))
        return false;
    return true;
}

void SpellScriptRotfaceUnstableOozeExplosionInit::HandleCast(SpellEffIndex effIndex)
{
    PreventHitEffect(effIndex);
    if (!GetHitUnit())
        return;

    float x, y, z;
    GetHitUnit()->GetPosition(x, y, z);
    Creature* dummy = GetCaster()->SummonCreature(NPC_UNSTABLE_EXPLOSION_STALKER, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 60000);
    GetCaster()->CastSpell(dummy, SPELL_UNSTABLE_OOZE_EXPLOSION_TRIGGER, true);
}

void SpellScriptRotfaceUnstableOozeExplosionInit::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptRotfaceUnstableOozeExplosionInit::HandleCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
}



/////////////////////////////
// Unstable Ooze Explosion //
/////////////////////////////

void SpellScriptRotfaceUnstableOozeExplosion::CheckTarget(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(EFFECT_0);
    if (!GetExplTargetDest())
        return;

    uint32 triggered_spell_id = GetSpellInfo()->Effects[effIndex].TriggerSpell;

    float x, y, z;
    GetExplTargetDest()->GetPosition(x, y, z);
    // let Rotface handle the cast - caster dies before this executes
    if (InstanceScript* script = GetCaster()->GetInstanceScript())
        if (Creature* rotface = script->instance->GetCreature(script->GetData64(DATA_ROTFACE)))
            rotface->CastSpell(x, y, z, triggered_spell_id, true, NULL, NULL, GetCaster()->GetGUID());
}

void SpellScriptRotfaceUnstableOozeExplosion::Register()
{
    OnEffectHit += SpellEffectFn(SpellScriptRotfaceUnstableOozeExplosion::CheckTarget, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
}



//////////////////////////////////////////
// Unstable Ooze Explosion Suicide Aura //
//////////////////////////////////////////

void AuraScriptRotfaceUnstableOozeExplosionSuicide::DespawnSelf(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    Unit* target = GetTarget();
    if (target->GetTypeId() != TYPEID_UNIT)
        return;

    target->RemoveAllAuras();
    target->SetVisible(false);
    target->ToCreature()->DespawnOrUnsummon(60000);
}

void AuraScriptRotfaceUnstableOozeExplosionSuicide::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(AuraScriptRotfaceUnstableOozeExplosionSuicide::DespawnSelf, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}
