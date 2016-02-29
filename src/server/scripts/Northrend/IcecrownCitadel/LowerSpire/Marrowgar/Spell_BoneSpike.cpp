#include "Spell_LordMarrowgar.h"
#include "UnitAI.h"

SpellCastResult SpellBoneSpikeGraveyardSpellScript::CheckCast()
{
    if (!GetCaster()->GetAI())
        return SPELL_FAILED_CASTER_DEAD;
    return GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_TOPAGGRO, 1, 0.0f, true, -SPELL_IMPALED) ? SPELL_CAST_OK : SPELL_FAILED_NO_VALID_TARGETS;
}

void SpellBoneSpikeGraveyardSpellScript::HandleSpikes(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (Creature* marrowgar = GetCaster()->ToCreature())
    {
        bool didHit = false;
        CreatureAI* marrowgarAI = marrowgar->AI();
        uint8 boneSpikeCount = uint8(GetCaster()->GetMap()->GetSpawnMode() & 1 ? 3 : 1);
        for (uint8 i = 0; i < boneSpikeCount; ++i)
        {
            // select any unit but not the tank
            Unit* target = marrowgarAI->SelectTarget(SELECT_TARGET_RANDOM, 1, 150.0f, true, -SPELL_IMPALED);
            if (!target)
                break;

            didHit = true;
            target->CastSpell(target, boneSpikeSummonId[i], true);
        }

        if (didHit)
            marrowgarAI->Talk(SAY_BONESPIKE);
    }
}

void SpellBoneSpikeGraveyardSpellScript::Register()
{
    OnCheckCast += SpellCheckCastFn(SpellBoneSpikeGraveyardSpellScript::CheckCast);
    OnEffectHitTarget += SpellEffectFn(SpellBoneSpikeGraveyardSpellScript::HandleSpikes, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
}