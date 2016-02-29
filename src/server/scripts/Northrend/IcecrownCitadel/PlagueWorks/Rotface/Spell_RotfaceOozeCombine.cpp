#include "Spell_Rotface.h"

/////////////////////////
// Little Ooze Combine //
/////////////////////////

void SpellScriptRotfaceLittleOozeCombine::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (!(GetHitCreature() && GetHitUnit()->isAlive()))
        return;

    GetCaster()->RemoveAurasDueToSpell(SPELL_LITTLE_OOZE_COMBINE);
    GetHitCreature()->RemoveAurasDueToSpell(SPELL_LITTLE_OOZE_COMBINE);
    GetHitCreature()->CastSpell(GetCaster(), SPELL_OOZE_MERGE, true);
    GetHitCreature()->DespawnOrUnsummon();
}

void SpellScriptRotfaceLittleOozeCombine::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptRotfaceLittleOozeCombine::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}



//////////////////////
// Big Ooze Combine //
//////////////////////

void SpellScriptRotfaceBigOozeCombine::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (!(GetHitCreature() && GetHitCreature()->isAlive()))
        return;

    if (Aura* unstable = GetCaster()->GetAura(SPELL_UNSTABLE_OOZE))
    {
        if (Aura* targetAura = GetHitCreature()->GetAura(SPELL_UNSTABLE_OOZE))
            unstable->ModStackAmount(targetAura->GetStackAmount());
        else
            unstable->ModStackAmount(1);

        // no idea why, but this does not trigger explosion on retail (only small+large do)
    }

    // just for safety
    GetHitCreature()->RemoveAurasDueToSpell(SPELL_LARGE_OOZE_BUFF_COMBINE);
    GetHitCreature()->RemoveAurasDueToSpell(SPELL_LARGE_OOZE_COMBINE);
    GetHitCreature()->DespawnOrUnsummon();
}

void SpellScriptRotfaceBigOozeCombine::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptRotfaceBigOozeCombine::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}



///////////////////////////
// Big Ooze Buff Combine //
///////////////////////////

void SpellScriptRotfaceBigOozeBuffCombine::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (!(GetHitCreature() && GetHitCreature()->isAlive()))
        return;

    if (Aura* unstable = GetCaster()->GetAura(SPELL_UNSTABLE_OOZE))
    {
        uint8 newStack = uint8(unstable->GetStackAmount()+1);
        unstable->SetStackAmount(newStack);

        // explode!
        if (newStack >= 5)
        {
            GetCaster()->RemoveAurasDueToSpell(SPELL_LARGE_OOZE_BUFF_COMBINE);
            GetCaster()->RemoveAurasDueToSpell(SPELL_LARGE_OOZE_COMBINE);
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* rotface = Unit::GetCreature(*GetCaster(), instance->GetData64(DATA_ROTFACE))) 
                {
                    if (rotface->isAlive())
                    {
                        rotface->AI()->Talk(EMOTE_UNSTABLE_EXPLOSION);
                        rotface->AI()->Talk(SAY_UNSTABLE_EXPLOSION);
                    }
                }
            }
            if (Creature* cre = GetCaster()->ToCreature())
                cre->AI()->DoAction(EVENT_STICKY_OOZE);
            GetCaster()->CastSpell(GetCaster(), SPELL_UNSTABLE_OOZE_EXPLOSION, false, NULL, NULL, GetCaster()->GetGUID());
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                instance->SetData(DATA_OOZE_DANCE_ACHIEVEMENT, uint32(false));
        }
    }

    GetHitCreature()->DespawnOrUnsummon();
}

void SpellScriptRotfaceBigOozeBuffCombine::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptRotfaceBigOozeBuffCombine::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}
