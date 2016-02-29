#include "Spell_ProfessorPutricide.h"

///////////////////////
// Regurgitated Ooze //
///////////////////////

// the only purpose of this hook is to fail the achievement
void SpellScriptPutricideRegurgitatedOoze::ExtraEffect(SpellEffIndex /*effIndex*/)
{
    if (InstanceScript* instance = GetCaster()->GetInstanceScript())
        instance->SetData(DATA_NAUSEA_ACHIEVEMENT, uint32(false));
}

void SpellScriptPutricideRegurgitatedOoze::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPutricideRegurgitatedOoze::ExtraEffect, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
}
