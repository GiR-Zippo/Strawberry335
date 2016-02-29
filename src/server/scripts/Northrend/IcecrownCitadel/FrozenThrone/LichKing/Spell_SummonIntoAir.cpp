#include "Spell_TheLichKing.h"

void SpellTheLichKingSummonIntoAirSpellScript::ModDestHeight(SpellEffIndex effIndex)
{
    static Position const offset = {0.0f, 0.0f, 15.0f, 0.0f};
    WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
    dest->RelocateOffset(offset);
    GetHitDest()->RelocateOffset(offset);
    // spirit bombs get higher
    if (GetSpellInfo()->Effects[effIndex].MiscValue == NPC_SPIRIT_BOMB)
    {
        dest->RelocateOffset(offset);
        GetHitDest()->RelocateOffset(offset);
    }
}

void SpellTheLichKingSummonIntoAirSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellTheLichKingSummonIntoAirSpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_SUMMON);
}