#include "Spell_Sindragosa.h"

bool SpellSindragosaInstabilityAuraScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_BACKLASH))
        return false;
    return true;
}

void SpellSindragosaInstabilityAuraScript::OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
{
    if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
        GetTarget()->CastCustomSpell(SPELL_BACKLASH, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), TRIGGERED_FULL_MASK, NULL, aurEff, GetCasterGUID());
}

void SpellSindragosaInstabilityAuraScript::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(SpellSindragosaInstabilityAuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
}