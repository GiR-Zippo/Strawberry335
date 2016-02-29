#include "Spell_TheLichKing.h"

void SpellTheLichKingShadowTrapVisualAuraScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
        GetTarget()->CastSpell(GetTarget(), SPELL_SHADOW_TRAP_AURA, TRIGGERED_NONE);
}

void SpellTheLichKingShadowTrapVisualAuraScript::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(SpellTheLichKingShadowTrapVisualAuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
}

void SpellTheLichKingShadowTrapPeriodicSpellScript::CheckTargetCount(std::list<WorldObject*>& targets)
{
    if (targets.empty())
        return;

    GetCaster()->CastSpell((Unit*)NULL, SPELL_SHADOW_TRAP_KNOCKBACK, true);
}

void SpellTheLichKingShadowTrapPeriodicSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingShadowTrapPeriodicSpellScript::CheckTargetCount, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
}