#include "Spell_Sindragosa.h"

bool SpellRimefangIcyBlastSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_ICY_BLAST_AREA))
        return false;
    return true;
}

void SpellRimefangIcyBlastSpellScript::HandleTriggerMissile(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (Position const* pos = GetExplTargetDest())
        if (TempSummon* summon = GetCaster()->SummonCreature(NPC_ICY_BLAST, *pos, TEMPSUMMON_TIMED_DESPAWN, 40000))
            summon->CastSpell(summon, SPELL_ICY_BLAST_AREA, true);
}

void SpellRimefangIcyBlastSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellRimefangIcyBlastSpellScript::HandleTriggerMissile, EFFECT_1, SPELL_EFFECT_TRIGGER_MISSILE);
}