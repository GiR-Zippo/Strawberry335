#include "Spell_TheLichKing.h"


void SpellTheLichKingTriggerWickedSpiritSpellScript::SelectTarget(std::list<WorldObject*>& targets)
{
    if (targets.empty())
        return;

    WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);

    targets.clear();
    targets.push_back(target);
}

void SpellTheLichKingTriggerWickedSpiritSpellScript::HandleDummy(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);

    if (Unit* pUnit = GetHitUnit())
    {
        if (Creature* pCreature = pUnit->ToCreature())
        {
            pCreature->SetReactState(REACT_AGGRESSIVE);
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            pCreature->CastSpell(pCreature, SPELL_VILE_SPIRIT_MOVE_SEARCH, true);
            pCreature->CastSpell((Unit*)NULL, SPELL_VILE_SPIRIT_DAMAGE_SEARCH, true);
        }
    }
}

void SpellTheLichKingTriggerWickedSpiritSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingTriggerWickedSpiritSpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingTriggerWickedSpiritSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}
