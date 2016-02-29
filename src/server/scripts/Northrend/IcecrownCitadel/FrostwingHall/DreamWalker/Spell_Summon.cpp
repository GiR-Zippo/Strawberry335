#include "Spell_ValithriaDreamwalker.h"

bool SpellDreamwalkerSummonerSpellScript::Load()
{
    if (!GetCaster()->GetInstanceScript())
        return false;
    return true;
}

void SpellDreamwalkerSummonerSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_RECENTLY_SPAWNED));
    if (targets.empty())
        return;

    WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
    targets.clear();
    targets.push_back(target);
}

void SpellDreamwalkerSummonerSpellScript::HandleForceCast(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (!GetHitUnit())
        return;

    GetHitUnit()->CastSpell(GetCaster(), GetSpellInfo()->Effects[effIndex].TriggerSpell, true, NULL, NULL, GetCaster()->GetInstanceScript()->GetData64(DATA_VALITHRIA_LICH_KING));
}

void SpellDreamwalkerSummonerSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellDreamwalkerSummonerSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    OnEffectHitTarget += SpellEffectFn(SpellDreamwalkerSummonerSpellScript::HandleForceCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
}

void SpellDreamwalkerSummonSuppresserAuraScript::PeriodicTick(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    Unit* caster = GetCaster();
    if (!caster)
        return;

    std::list<Creature*> summoners;
    GetCreatureListWithEntryInGrid(summoners, caster, NPC_WORLD_TRIGGER, 100.0f);
    summoners.remove_if(Trinity::UnitAuraCheck(true, SPELL_RECENTLY_SPAWNED));
    Trinity::Containers::RandomResizeList(summoners, 2);
    if (summoners.empty())
        return;

    for (uint32 i = 0; i < 3; ++i)
        caster->CastSpell(summoners.front(), SPELL_SUMMON_SUPPRESSER, true);
    for (uint32 i = 0; i < 3; ++i)
        caster->CastSpell(summoners.back(), SPELL_SUMMON_SUPPRESSER, true);
}

void SpellDreamwalkerSummonSuppresserAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellDreamwalkerSummonSuppresserAuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}

bool SpellDreamwalkerSummonSuppresserEffectSpellScript::Load()
{
    if (!GetCaster()->GetInstanceScript())
        return false;
    return true;
}

void SpellDreamwalkerSummonSuppresserEffectSpellScript::HandleForceCast(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (!GetHitUnit())
        return;

    GetHitUnit()->CastSpell(GetCaster(), GetSpellInfo()->Effects[effIndex].TriggerSpell, true, NULL, NULL, GetCaster()->GetInstanceScript()->GetData64(DATA_VALITHRIA_LICH_KING));
}

void SpellDreamwalkerSummonSuppresserEffectSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellDreamwalkerSummonSuppresserEffectSpellScript::HandleForceCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
}

void SpellDreamwalkerSummonDreamPortalSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (!GetHitUnit())
        return;

    uint32 spellId = RAND<uint32>(71301, 72220, 72223, 72225);
    GetHitUnit()->CastSpell(GetHitUnit(), spellId, true);
}

void SpellDreamwalkerSummonDreamPortalSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellDreamwalkerSummonDreamPortalSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}

void SpellDreamwalkerSummonNightmarePortalSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (!GetHitUnit())
        return;

    uint32 spellId = RAND<uint32>(71977, 72481, 72482, 72483);
    GetHitUnit()->CastSpell(GetHitUnit(), spellId, true);
}

void SpellDreamwalkerSummonNightmarePortalSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellDreamwalkerSummonNightmarePortalSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}