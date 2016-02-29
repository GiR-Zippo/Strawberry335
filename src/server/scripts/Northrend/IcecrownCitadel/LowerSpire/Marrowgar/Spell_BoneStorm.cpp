#include "Spell_LordMarrowgar.h"

void SpellBoneStormSpellScript::RecalculateDamage()
{
    float dist = GetHitUnit()->GetExactDist2d(GetCaster());
    if (dist < 0.0f)
        return;

    float damage = GetHitDamage();
    damage *= (1 - 0.01f * dist) / ( 1 + 0.08f * dist);

    SetHitDamage(int32(damage));
}

void SpellBoneStormSpellScript::Register()
{
    OnHit += SpellHitFn(SpellBoneStormSpellScript::RecalculateDamage);
}