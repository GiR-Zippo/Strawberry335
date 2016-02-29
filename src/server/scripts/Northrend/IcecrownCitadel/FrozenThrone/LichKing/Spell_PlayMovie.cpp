#include "Spell_TheLichKing.h"

bool SpellTheLichKingPlayMovieSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sMovieStore.LookupEntry(MOVIE_FALL_OF_THE_LICH_KING))
        return false;
    return true;
}

void SpellTheLichKingPlayMovieSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    if (Player* player = GetHitPlayer())
        player->SendMovieStart(MOVIE_FALL_OF_THE_LICH_KING);
}

void SpellTheLichKingPlayMovieSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingPlayMovieSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}