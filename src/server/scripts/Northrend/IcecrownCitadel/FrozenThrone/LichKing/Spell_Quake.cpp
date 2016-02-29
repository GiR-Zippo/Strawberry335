#include "Spell_TheLichKing.h"

bool SpellTheLichKingQuakeSpellScript::Load()
{
    return GetCaster()->GetInstanceScript() != NULL;
}

void SpellTheLichKingQuakeSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    if (GameObject* platform = ObjectAccessor::GetGameObject(*GetCaster(), GetCaster()->GetInstanceScript()->GetData64(DATA_ARTHAS_PLATFORM)))
        targets.remove_if(HeightDifferenceCheck(platform, 5.0f, false));
}

void SpellTheLichKingQuakeSpellScript::HandleSendEvent(SpellEffIndex /*effIndex*/)
{
    if (GetCaster()->IsAIEnabled)
        GetCaster()->GetAI()->DoAction(ACTION_START_ATTACK);
}

void SpellTheLichKingQuakeSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingQuakeSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    OnEffectHit += SpellEffectFn(SpellTheLichKingQuakeSpellScript::HandleSendEvent, EFFECT_1, SPELL_EFFECT_SEND_EVENT);
}