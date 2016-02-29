#include "Spell_ProfessorPutricide.h"

//////////////////
// Slime Puddle //
//////////////////

void SpellScriptPutricideSlimePuddle::ScaleRange(std::list<WorldObject*>& targets)
{
    targets.remove_if(ExactDistanceCheck(GetCaster(), 2.5f * GetCaster()->GetFloatValue(OBJECT_FIELD_SCALE_X)));
}

void SpellScriptPutricideSlimePuddle::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideSlimePuddle::ScaleRange, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptPutricideSlimePuddle::ScaleRange, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
}



//////////////////////
// Slime Puddle Aura//
//////////////////////

// this is here only because on retail you dont actually enter HEROIC mode for ICC
void SpellScriptPutricideSlimePuddleAura::ReplaceAura()
{
    if (Unit* target = GetHitUnit())
        GetCaster()->AddAura((GetCaster()->GetMap()->GetSpawnMode() & 1) ? 72456 : 70346, target);
}

void SpellScriptPutricideSlimePuddleAura::Register()
{
    OnHit += SpellHitFn(SpellScriptPutricideSlimePuddleAura::ReplaceAura);
}
