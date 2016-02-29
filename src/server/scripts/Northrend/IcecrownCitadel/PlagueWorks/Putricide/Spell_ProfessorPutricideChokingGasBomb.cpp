#include "Spell_ProfessorPutricide.h"

//////////////////////
// Choking Gas Bomb //
//////////////////////

void SpellScriptPutricideChokingGasBomb::HandleScript(SpellEffIndex /*effIndex*/)
{
    uint32 skipIndex = urand(0, 2);
                for (uint32 i = 0; i < 3; ++i)
                {
                    if (i == skipIndex)
                        continue;

                    uint32 spellId = uint32(GetSpellInfo()->Effects[i].CalcValue());
                    GetCaster()->CastSpell(GetCaster(), spellId, true, NULL, NULL, GetCaster()->GetGUID());
                }
}

void SpellScriptPutricideChokingGasBomb::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPutricideChokingGasBomb::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}
