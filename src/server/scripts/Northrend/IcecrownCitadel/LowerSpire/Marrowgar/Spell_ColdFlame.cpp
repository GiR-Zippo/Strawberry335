#include "Spell_LordMarrowgar.h"
#include "UnitAI.h"

std::map<uint64, uint32> SpellColdflameDamageAuraScript::cooldowns;

void SpellColdflameSpellScript::SelectTarget(std::list<WorldObject*>& targets)
{
    targets.clear();

    // select any unit but not the tank (by owners threatlist)
    Unit* target = GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 1, -GetCaster()->GetObjectSize(), true, -SPELL_IMPALED);
    if (!target)
        target = GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true); // or the tank if its solo

    if (!target)
        return;

    Creature* dummy = GetCaster()->SummonCreature(NPC_COLDFLAME_TARGET_STALKER, *GetCaster(), TEMPSUMMON_TIMED_DESPAWN, 10000);
    dummy->SetOrientation(GetCaster()->GetAngle(target));

    if (!GetCaster()->HasAura(SPELL_BONE_STORM))
    {
        Position pos;
        dummy->GetNearPosition(pos, GetCaster()->GetObjectSize(), 0);
        dummy->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), dummy->GetPositionZ(), dummy->GetOrientation());
    }

    GetCaster()->GetAI()->SetGUID(dummy->GetGUID(), DATA_COLDFLAME_GUID);
    targets.push_back(target);
}

void SpellColdflameSpellScript::HandleScriptEffect(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
}

void SpellColdflameSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellColdflameSpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    OnEffectHitTarget += SpellEffectFn(SpellColdflameSpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}


void SpellColdflameBonestormSpellScript::HandleScriptEffect(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    for (uint8 i = 0; i < 4; ++i)
        GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue() + i), true);
}

void SpellColdflameBonestormSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellColdflameBonestormSpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}


void SpellColdflameDamageAuraScript::OnPeriodic(AuraEffect const* /*aurEff*/)
{
    if (DynamicObject* owner = GetDynobjOwner())
        if (GetTarget()->GetExactDist2d(owner) >= owner->GetRadius() || GetTarget()->HasAura(SPELL_IMPALED) || HasCooldown(GetTarget()))
            PreventDefaultAction();
}

bool SpellColdflameDamageAuraScript::HasCooldown(Unit* unit)
{
    bool cd = false;
    std::map<uint64,uint32>::iterator ii = cooldowns.begin();
    while (ii != cooldowns.end())
    {
        if ((getMSTime()-(*ii).second) < 500)
        {
            if (unit->GetGUID() == (*ii).first)
                cd = true;
            ++ii;
        } else {
            cooldowns.erase(ii++);
        }
    }

    if (!cd)
    {
        cooldowns[unit->GetGUID()] = getMSTime();
    }

    return cd;
}

void SpellColdflameDamageAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellColdflameDamageAuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
}