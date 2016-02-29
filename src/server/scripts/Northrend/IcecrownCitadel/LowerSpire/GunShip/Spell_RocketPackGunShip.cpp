#include "Spell_GunShip.h"

void SpellRocketPackAuraScript::OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    GetTarget()->CastSpell(GetTarget(), 68645, true);
}

void SpellRocketPackAuraScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    GetTarget()->RemoveAurasDueToSpell(68645);
}

void SpellRocketPackAuraScript::Register()
{
    OnEffectApply += AuraEffectApplyFn(SpellRocketPackAuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    OnEffectRemove += AuraEffectRemoveFn(SpellRocketPackAuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
}

void SpellRemoveRocketPackSpellScript::HandleEffect(SpellEffIndex /*effIndex*/)
{
    Player* hitPlr = GetHitPlayer();
    if (!hitPlr) // If player is offline
                    return;

    int32 itemId = GetEffectValue();
    int32 itemCount = hitPlr->GetItemCount(itemId, false); // Should be 1, but just in case.
    hitPlr->DestroyItemCount(itemId, -itemCount, true, false);
}

void SpellRemoveRocketPackSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellRemoveRocketPackSpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}