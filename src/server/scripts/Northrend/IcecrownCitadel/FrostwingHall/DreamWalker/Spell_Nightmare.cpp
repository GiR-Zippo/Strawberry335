#include "Spell_ValithriaDreamwalker.h"

bool SpellDreamwalkerNightmareCloudAuraScript::Load()
{
    _instance = GetOwner()->GetInstanceScript();
    return _instance != NULL;
}

void SpellDreamwalkerNightmareCloudAuraScript::PeriodicTick(AuraEffect const* /*aurEff*/)
{
    if (_instance->GetBossState(DATA_VALITHRIA_DREAMWALKER) != IN_PROGRESS)
        PreventDefaultAction();
}

void SpellDreamwalkerNightmareCloudAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellDreamwalkerNightmareCloudAuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}

void SpellDreamwalkerTwistedNightmaresSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    // impossible with TARGET_UNIT_CASTER
    //if (!GetHitUnit())
    //    return;

    if (InstanceScript* instance = GetHitUnit()->GetInstanceScript())
        GetHitUnit()->CastSpell((Unit*)NULL, GetSpellInfo()->Effects[effIndex].TriggerSpell, true, NULL, NULL, instance->GetData64(DATA_VALITHRIA_DREAMWALKER));
}

void SpellDreamwalkerTwistedNightmaresSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellDreamwalkerTwistedNightmaresSpellScript::HandleScript, EFFECT_2, SPELL_EFFECT_FORCE_CAST);
}