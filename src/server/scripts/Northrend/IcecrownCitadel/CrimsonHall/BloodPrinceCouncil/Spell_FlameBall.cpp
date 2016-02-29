#include "Spell_BloodPrinceCouncil.h"

void SpellTaldaramSummonFlameBallSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetCaster()->CastSpell(GetCaster(), uint32(GetEffectValue()), true);
}

void SpellTaldaramSummonFlameBallSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTaldaramSummonFlameBallSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
}

bool SpellTaldaramFlameBallVisualAuraScript::Load()
{
    if (GetCaster()->GetEntry() == NPC_BALL_OF_FLAME || GetCaster()->GetEntry() == NPC_BALL_OF_INFERNO_FLAME)
        return true;
    return false;
}

void SpellTaldaramFlameBallVisualAuraScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    Creature* target = GetTarget()->ToCreature();
    if (!target)
        return;

    // SPELL_FLAME_SPHERE_SPAWN_EFFECT
    if (GetSpellInfo()->Id == SPELL_FLAME_SPHERE_SPAWN_EFFECT)
    {
        target->CastSpell(target, SPELL_BALL_OF_FLAMES, true);
        target->AI()->DoAction(ACTION_FLAME_BALL_CHASE);
    }
    else // SPELL_FLAME_SPHERE_DEATH_EFFECT
        target->DespawnOrUnsummon();
}

void SpellTaldaramFlameBallVisualAuraScript::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(SpellTaldaramFlameBallVisualAuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
}

void SpellTaldaramBallOfInfernoFlameSpellScript::ModAuraStack()
{
    if (Aura* aur = GetHitAura())
        aur->SetStackAmount(uint8(GetSpellInfo()->StackAmount));
}

void SpellTaldaramBallOfInfernoFlameSpellScript::Register()
{
    AfterHit += SpellHitFn(SpellTaldaramBallOfInfernoFlameSpellScript::ModAuraStack);
}