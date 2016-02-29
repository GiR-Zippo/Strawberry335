#include "Spell_Rotface.h"
#include "SpellAuraEffects.h"

///////////////////////
// Mutated Infection //
///////////////////////

void SpellScriptRotfaceMutatedInfection::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.clear();
    std::list<Player*> targetList = GetCaster()->GetNearestPlayersList(500.0f);
    if (targetList.empty())
        return;

    targetList.remove_if(MutatedInfectionCheck(GetCaster()->getVictim(), GetSpellInfo()->Id));

    if (targetList.empty())
        return;

    if (Player* target = Trinity::Containers::SelectRandomContainerElement(targetList))
    {
        if (target == GetCaster()->getThreatManager().getHostilTarget())
        {
            targetList.remove(target);
            if (targetList.empty())
                return;

            target = Trinity::Containers::SelectRandomContainerElement(targetList);
            if (!target)
                return;
        }

        targets.push_back(target);
        _target = target;
    }
}

void SpellScriptRotfaceMutatedInfection::ReplaceTargets(std::list<WorldObject*>& targets)
{
    targets.clear();
    if (_target)
        targets.push_back(_target);
}

void SpellScriptRotfaceMutatedInfection::NotifyTargets()
{
    if (Creature* caster = GetCaster()->ToCreature())
        if (Unit* target = GetHitUnit())
            caster->AI()->BossWhisper(EMOTE_MUTATED_INFECTION, target->GetGUID());
}

void SpellScriptRotfaceMutatedInfection::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptRotfaceMutatedInfection::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptRotfaceMutatedInfection::ReplaceTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptRotfaceMutatedInfection::ReplaceTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
    AfterHit += SpellHitFn(SpellScriptRotfaceMutatedInfection::NotifyTargets);
}
