#include "Spell_BloodPrinceCouncil.h"

void SpellValanarKineticBombSpellScript::ChangeSummonPos(SpellEffIndex /*effIndex*/)
{
    WorldLocation summonPos = *GetExplTargetDest();
    Position offset = {0.0f, 0.0f, 20.0f, 0.0f};
    summonPos.RelocateOffset(offset);
    SetExplTargetDest(summonPos);
    GetHitDest()->RelocateOffset(offset);
}

void SpellValanarKineticBombSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellValanarKineticBombSpellScript::ChangeSummonPos, EFFECT_0, SPELL_EFFECT_SUMMON);
}

void SpellValanarKineticBombAuraScript::HandleDummyTick(AuraEffect const* /*aurEff*/)
{
    Unit* target = GetTarget();
    if (target->GetTypeId() != TYPEID_UNIT)
        return;

    if (Creature* bomb = target->FindNearestCreature(NPC_KINETIC_BOMB, 0.1f, true))
    {
        bomb->CastSpell(bomb, SPELL_KINETIC_BOMB_EXPLOSION, true);
        bomb->RemoveAurasDueToSpell(SPELL_KINETIC_BOMB_VISUAL);
        target->RemoveAura(GetAura());
        bomb->AI()->DoAction(SPELL_KINETIC_BOMB_EXPLOSION);
    }
}

void SpellValanarKineticBombAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellValanarKineticBombAuraScript::HandleDummyTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
}

void SpellValanarKineticBombKnockbackAuraScript::onApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (Creature* target = GetUnitOwner()->ToCreature())
        target->AI()->DoAction(ACTION_KINETIC_BOMB_UP);
}

void SpellValanarKineticBombKnockbackAuraScript::onRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (Creature* target = GetUnitOwner()->ToCreature())
        target->AI()->DoAction(ACTION_KINETIC_BOMB_DOWN);
}

void SpellValanarKineticBombKnockbackAuraScript::Register()
{
    AfterEffectApply += AuraEffectApplyFn(SpellValanarKineticBombKnockbackAuraScript::onApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    AfterEffectRemove += AuraEffectRemoveFn(SpellValanarKineticBombKnockbackAuraScript::onRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
}

void SpellValanarKineticBombAbsorbAuraScript::OnAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
{
    absorbAmount = CalculatePctN(dmgInfo.GetDamage(), aurEff->GetAmount());
    RoundToInterval<uint32>(absorbAmount, 0, dmgInfo.GetDamage());
    dmgInfo.AbsorbDamage(absorbAmount);
}

void SpellValanarKineticBombAbsorbAuraScript::Register()
{
    OnEffectAbsorb += AuraEffectAbsorbFn(SpellValanarKineticBombAbsorbAuraScript::OnAbsorb, EFFECT_0);
}