#include "Spell_BloodQueenLanaThel.h"

bool SpellBloodQueenBloodboltSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_TWILIGHT_BLOODBOLT))
        return false;
    return true;
}

bool SpellBloodQueenBloodboltSpellScript::Load()
{
    return GetCaster()->GetEntry() == NPC_BLOOD_QUEEN_LANA_THEL;
}

void SpellBloodQueenBloodboltSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    uint32 targetCount = (targets.size() + 2) / 3;
    targets.remove_if(BloodboltHitCheck(static_cast<BossBloodQueenLanaThelAI*>(GetCaster()->GetAI())));

    Trinity::Containers::RandomResizeList(targets, targetCount);
    // mark targets now, effect hook has missile travel time delay (might cast next in that time)
    for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
        GetCaster()->GetAI()->SetGUID((*itr)->GetGUID(), GUID_BLOODBOLT);
}

void SpellBloodQueenBloodboltSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetCaster()->CastSpell(GetHitUnit(), SPELL_TWILIGHT_BLOODBOLT, true);
}

void SpellBloodQueenBloodboltSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellBloodQueenBloodboltSpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
    OnEffectHitTarget += SpellEffectFn(SpellBloodQueenBloodboltSpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
}