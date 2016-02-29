#include "Spell_TheLichKing.h"

void SpellTheLichKingDefileSpellScript::CorrectRange(std::list<WorldObject*>& targets)
{
    targets.remove_if(ExactDistanceCheck(GetCaster(), 10.0f * GetCaster()->GetFloatValue(OBJECT_FIELD_SCALE_X)));
}

void SpellTheLichKingDefileSpellScript::ChangeDamageAndGrow()
{
    if (Unit* target = GetHitUnit())
    {
        if (HARVEST_SOUL_CHECK(target))
        {
            PreventHitDamage();
            return;
        }
    }

    // HACK: target player should cast this spell on defile
    // however with current aura handling auras cast by different units
    // cannot stack on the same aura object increasing the stack count
    GetCaster()->CastSpell(GetCaster(), SPELL_DEFILE_GROW, true);
    SetHitDamage(int32(GetHitDamage() * GetCaster()->GetFloatValue(OBJECT_FIELD_SCALE_X)));
}

void SpellTheLichKingDefileSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingDefileSpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingDefileSpellScript::CorrectRange, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
    OnHit += SpellHitFn(SpellTheLichKingDefileSpellScript::ChangeDamageAndGrow);
}