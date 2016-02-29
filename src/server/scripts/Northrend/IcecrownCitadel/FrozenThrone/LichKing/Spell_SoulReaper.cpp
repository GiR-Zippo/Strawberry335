#include "Spell_TheLichKing.h"

bool SpellTheLichKingSoulReaperAuraScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_SOUL_REAPER_BUFF))
        return false;
    return true;
}

void SpellTheLichKingSoulReaperAuraScript::OnPeriodic(AuraEffect const* /*aurEff*/)
{
    if (Unit* caster = GetCaster())
        GetTarget()->CastSpell(caster, SPELL_SOUL_REAPER_BUFF, true);
}

void SpellTheLichKingSoulReaperAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellTheLichKingSoulReaperAuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
}