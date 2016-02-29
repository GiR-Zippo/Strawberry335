#include "Spell_TheLichKing.h"

bool SpellTheLichKingInFrostmourneRoomAuraScript::Load()
{
    return GetOwner()->GetInstanceScript() != NULL;
}

void SpellTheLichKingInFrostmourneRoomAuraScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    // m_originalCaster to allow stacking from different casters, meh
    if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
        GetTarget()->CastSpell((Unit*)NULL, SPELL_HARVESTED_SOUL, true, NULL, NULL, GetTarget()->GetInstanceScript()->GetData64(DATA_THE_LICH_KING));
}

void SpellTheLichKingInFrostmourneRoomAuraScript::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(SpellTheLichKingInFrostmourneRoomAuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
}