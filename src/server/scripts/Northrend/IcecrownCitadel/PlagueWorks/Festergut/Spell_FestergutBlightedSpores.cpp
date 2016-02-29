#include "Spell_Festergut.h"
#include <SpellAuraEffects.h>

//////////////////////////
// Blighted Spores Aura //
//////////////////////////

bool AuraScriptSpellFestergutSpores::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_BLIGHTED_SPORE))
        return false;
    return true;
}

void AuraScriptSpellFestergutSpores::HandlePeriodic(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    std::list<Player*> targets = GetCaster()->GetNearestPlayersList(8.0f, true);
    for (std::list<Player*>::iterator it = targets.begin(); it != targets.end(); ++it)
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_BLIGHTED_SPORE))
            Aura::TryRefreshStackOrCreate(spellInfo, MAX_EFFECT_MASK, (*it), (*it));
}

void AuraScriptSpellFestergutSpores::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(AuraScriptSpellFestergutSpores::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}

bool AuraScriptFestergutBlightedSpores::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_INOCULATED))
        return false;
    return true;
}

void AuraScriptFestergutBlightedSpores::ExtraEffect(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
{
    GetTarget()->CastSpell(GetTarget(), SPELL_INOCULATED, true);
    if (InstanceScript* instance = GetTarget()->GetInstanceScript())
        if (Creature* festergut = ObjectAccessor::GetCreature(*GetTarget(), instance->GetData64(DATA_FESTERGUT)))
            festergut->AI()->SetData(DATA_INOCULATED_STACK, GetStackAmount());
}

void AuraScriptFestergutBlightedSpores::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(AuraScriptFestergutBlightedSpores::ExtraEffect, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
}
