#include "Spell_GunShip.h"

void SpellHeatDrainSpellScript::HandleDummy(SpellEffIndex /*effIndex*/)
{
    Unit* caster = GetCaster();

    if (caster->GetPower(POWER_ENERGY) != 0)
        caster->ModifyPower(POWER_ENERGY, -1);

    if (caster->GetPower(POWER_ENERGY) >= 99)
        caster->CastSpell(caster, SPELL_OVERHEAT, true);
}

void SpellHeatDrainSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellHeatDrainSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}

void SpellOverheatDrainSpellScript::HandleDummy(SpellEffIndex /*effIndex*/)
{
    Unit* caster = GetCaster();

    if (caster->GetPower(POWER_ENERGY) >= 10)
        caster->ModifyPower(POWER_ENERGY, -10);
}

void SpellOverheatDrainSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellOverheatDrainSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}

void SpellIncineratingBlastSpellScript::AddExtraDamage()
{
    Unit* caster = GetCaster();
    if (!caster || !caster->GetPower(POWER_ENERGY))
        return;
    
    SetHitDamage(int32(GetHitDamage() + (caster->GetPower(POWER_ENERGY) * 100.0f))); // TODO: How much should be?
    caster->SetPower(POWER_ENERGY, 0);
}

void SpellIncineratingBlastSpellScript::Register()
{
    OnHit += SpellHitFn(SpellIncineratingBlastSpellScript::AddExtraDamage);
}

bool SpellBurningPitchSpellScript::Validate(SpellInfo const* /*spellInfo*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_BURNING_PITCH_SIEGE_DMG_A))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_BURNING_PITCH_SIEGE_DMG_H))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_BURNING_PITCH_AOE_DAMAGE))
        return false;
    return true;
}

void SpellBurningPitchSpellScript::HandleDummy(SpellEffIndex /*effIndex*/)
{
    Unit* caster = GetCaster();
    Unit* target = GetHitUnit();
    
    if (!caster || !target)
        return;
    
    if (GetSpellInfo()->Id == SPELL_BURNING_PITCH_A)
        caster->CastSpell(target, SPELL_BURNING_PITCH_SIEGE_DMG_A, true);
    else
        caster->CastSpell(target, SPELL_BURNING_PITCH_SIEGE_DMG_H, true);
    
    caster->CastSpell(target, SPELL_BURNING_PITCH_AOE_DAMAGE, true);
}

void SpellBurningPitchSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellBurningPitchSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}