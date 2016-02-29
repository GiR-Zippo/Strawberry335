#include "Spell_Rotface.h"

/////////////////
// Ooze Summon //
/////////////////

void SpellScriptRotfaceOozeSummon::EffectSummon(SpellEffIndex /*effIndex*/)
{
    PreventHitDefaultEffect(EFFECT_0);
    uint32 entry = GetSpellInfo()->Effects[EFFECT_0].MiscValue;
    if (!entry)
        return;
    if (!GetOriginalCaster() || !GetCaster())
        return;
    Position pos;
    GetCaster()->GetNearPosition(pos, 3.0f * (float)rand_norm(), (float)rand_norm()*static_cast<float>(2*M_PI));
    TempSummon * summon = GetOriginalCaster()->SummonCreature(entry, pos, TEMPSUMMON_DEAD_DESPAWN, 0);
    if (summon && GetCaster()->GetTypeId() == TYPEID_PLAYER)
        summon->AddThreat(GetCaster(), 500000.0f);
}

void SpellScriptRotfaceOozeSummon::Register()
{
    OnEffectHit += SpellEffectFn(SpellScriptRotfaceOozeSummon::EffectSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
}
