#include "Spell_BloodQueenLanaThel.h"

void SpellBloodQueenPactOfTheDarkfallenSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_PACT_OF_THE_DARKFALLEN));

    bool remove = true;
    std::list<WorldObject*>::const_iterator itrEnd = targets.end(), itr, itr2;
    // we can do this, unitList is MAX 4 in size
    for (itr = targets.begin(); itr != itrEnd && remove; ++itr)
    {
        if (!GetCaster()->IsWithinDist(*itr, 5.0f, false))
            remove = false;

        for (itr2 = targets.begin(); itr2 != itrEnd && remove; ++itr2)
            if (itr != itr2 && !(*itr2)->IsWithinDist(*itr, 5.0f, false))
                remove = false;
    }

    if (remove)
    {
        if (InstanceScript* instance = GetCaster()->GetInstanceScript())
        {
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PACT_OF_THE_DARKFALLEN);
            targets.clear();
        }
    }
}

void SpellBloodQueenPactOfTheDarkfallenSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellBloodQueenPactOfTheDarkfallenSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
}

bool SpellBloodQueenPactOfTheDarkfallenDmgAuraScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_PACT_OF_THE_DARKFALLEN_DAMAGE))
        return false;
    return true;
}

void SpellBloodQueenPactOfTheDarkfallenDmgAuraScript::PeriodicTick(AuraEffect const* aurEff)
{
    SpellInfo const* damageSpell = sSpellMgr->GetSpellInfo(SPELL_PACT_OF_THE_DARKFALLEN_DAMAGE);
    int32 damage = damageSpell->Effects[EFFECT_0].CalcValue();
    float multiplier = 0.3375f + 0.1f * uint32(aurEff->GetTickNumber()/10); // do not convert to 0.01f - we need tick number/10 as INT (damage increases every 10 ticks)
    damage = int32(damage * multiplier);
    GetTarget()->CastCustomSpell(SPELL_PACT_OF_THE_DARKFALLEN_DAMAGE, SPELLVALUE_BASE_POINT0, damage, GetTarget(), TRIGGERED_FULL_MASK);
}

void SpellBloodQueenPactOfTheDarkfallenDmgAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellBloodQueenPactOfTheDarkfallenDmgAuraScript::PeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}

void SpellBloodQueenPactOfTheFarkfallenDmgTargetSpellScript::FilterTargets(std::list<WorldObject*>& unitList)
{
    unitList.remove_if(Trinity::UnitAuraCheck(true, SPELL_PACT_OF_THE_DARKFALLEN));
    unitList.push_back(GetCaster());
}

void SpellBloodQueenPactOfTheFarkfallenDmgTargetSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellBloodQueenPactOfTheFarkfallenDmgTargetSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
}