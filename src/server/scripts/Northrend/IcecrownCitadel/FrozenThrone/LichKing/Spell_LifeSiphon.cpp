#include "Spell_TheLichKing.h"

bool SpellTheLichKingLifeSiphonSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_LIFE_SIPHON_HEAL))
        return false;
    return true;
}

void SpellTheLichKingLifeSiphonSpellScript::TriggerHeal()
{
    GetHitUnit()->CastCustomSpell(SPELL_LIFE_SIPHON_HEAL, SPELLVALUE_BASE_POINT0, GetHitDamage() * 10, GetCaster(), TRIGGERED_FULL_MASK);
}

void SpellTheLichKingLifeSiphonSpellScript::Register()
{
    AfterHit += SpellHitFn(SpellTheLichKingLifeSiphonSpellScript::TriggerHeal);
}