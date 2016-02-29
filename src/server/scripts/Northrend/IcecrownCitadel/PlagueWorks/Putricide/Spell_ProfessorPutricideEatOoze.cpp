#include "Spell_ProfessorPutricide.h"

//////////////
// Eat Ooze //
//////////////

void SpellScriptPutricideEatOoze::SelectTarget(std::list<WorldObject*>& targets)
{
    if (targets.empty())
    {
        if (Unit* pUnit = GetCaster())
        {
            if (Creature* pCreature = pUnit->FindNearestCreature(NPC_GROWING_OOZE_PUDDLE, 15.0f))
                if (pCreature->HasAura(SPELL_GROW))
                    targets.push_back(pCreature);
        }
        return;
    }

    targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
    WorldObject* pWorldObject = targets.front();
    targets.clear();
    targets.push_back(pWorldObject);
}

void SpellScriptPutricideEatOoze::HandleScript(SpellEffIndex /*effIndex*/)
{
    Creature* target = GetHitCreature();
    if (!target)
        return;

    if (Aura* grow = target->GetAura(uint32(GetEffectValue())))
    {
        if (grow->GetStackAmount() < 3)
        {
            target->RemoveAurasDueToSpell(SPELL_GROW_STACKER);
            target->RemoveAura(grow);
            target->DespawnOrUnsummon(1);
        }
        else
            grow->ModStackAmount(-3);
    }
}

void SpellScriptPutricideEatOoze::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPutricideEatOoze::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideEatOoze::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
}
