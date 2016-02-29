#include "Spell_TheLichKing.h"

bool SpellTheLichKingRestoreSoulSpellScript::Load()
{
    _instance = GetCaster()->GetInstanceScript();
    return _instance != NULL;
}

void SpellTheLichKingRestoreSoulSpellScript::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (Creature* spawner = GetCaster()->FindNearestCreature(NPC_WORLD_TRIGGER_INFINITE_AOI, 50.0f))
        spawner->RemoveAllAuras();

    if (Creature* lichKing = ObjectAccessor::GetCreature(*GetCaster(), _instance->GetData64(DATA_THE_LICH_KING)))
        lichKing->AI()->DoAction(ACTION_TELEPORT_BACK);
}

void SpellTheLichKingRestoreSoulSpellScript::RemoveAura()
{
    if (Unit* target = GetHitUnit())
        target->RemoveAurasDueToSpell(target->GetMap()->IsHeroic() ? SPELL_HARVEST_SOULS_TELEPORT : SPELL_HARVEST_SOUL_TELEPORT);
}

void SpellTheLichKingRestoreSoulSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellTheLichKingRestoreSoulSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    BeforeHit += SpellHitFn(SpellTheLichKingRestoreSoulSpellScript::RemoveAura);
}