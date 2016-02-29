#include "Spell_BloodQueenLanaThel.h"

void SpellBloodQueenFrenziedBloodthirstAuraScript::OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (InstanceScript* instance = GetTarget()->GetInstanceScript())
        if (Creature* bloodQueen = ObjectAccessor::GetCreature(*GetTarget(), instance->GetData64(DATA_BLOOD_QUEEN_LANA_THEL)))
            bloodQueen->AI()->Talk(EMOTE_BLOODTHIRST, GetTarget()->GetGUID());
}

void SpellBloodQueenFrenziedBloodthirstAuraScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    Unit* target = GetTarget();
    if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
        if (InstanceScript* instance = target->GetInstanceScript())
            if (Creature* bloodQueen = ObjectAccessor::GetCreature(*target, instance->GetData64(DATA_BLOOD_QUEEN_LANA_THEL)))
            {
                // this needs to be done BEFORE charm aura or we hit an assert in Unit::SetCharmedBy
                if (target->GetVehicleKit())
                    target->RemoveVehicleKit();

                bloodQueen->AI()->Talk(SAY_MIND_CONTROL);
                bloodQueen->CastSpell(target, SPELL_UNCONTROLLABLE_FRENZY, true);
            }
}

void SpellBloodQueenFrenziedBloodthirstAuraScript::Register()
{
    OnEffectApply += AuraEffectApplyFn(SpellBloodQueenFrenziedBloodthirstAuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
    AfterEffectRemove += AuraEffectRemoveFn(SpellBloodQueenFrenziedBloodthirstAuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
}