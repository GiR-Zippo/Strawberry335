#include "Spell_TheLichKing.h"

void SpellTheLichKingInfestAuraScript::OnPeriodic(AuraEffect const* /*aurEff*/)
{
    if (GetUnitOwner()->HealthAbovePct(90))
    {
        PreventDefaultAction();
        Remove(AURA_REMOVE_BY_ENEMY_SPELL);
    }
}

void SpellTheLichKingInfestAuraScript::OnUpdate(AuraEffect* aurEff)
{
    // multiply, starting from 2nd tick
    if (aurEff->GetTickNumber() == 1)
        return;

    aurEff->SetAmount(int32(aurEff->GetAmount() * 1.15f));
}

void SpellTheLichKingInfestAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellTheLichKingInfestAuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(SpellTheLichKingInfestAuraScript::OnUpdate, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
}