#include "Spell_Sindragosa.h"

bool SpellSindragosasFurySpellScript::Load()
{
    _targetCount = 0;

    // This script should execute only in Icecrown Citadel
    if (InstanceMap* instance = GetCaster()->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(ICCScriptName))
                return true;

    return false;
}

void SpellSindragosasFurySpellScript::SelectDest()
{
    if (Position* dest = const_cast<WorldLocation*>(GetExplTargetDest()))
    {
        float destX = float(rand_norm()) * 75.0f + 4350.0f;
        float destY = float(rand_norm()) * 75.0f + 2450.0f;
        float destZ = 205.0f; // random number close to ground, get exact in next call
        GetCaster()->UpdateGroundPositionZ(destX, destY, destZ);
        dest->Relocate(destX, destY, destZ);
    }
}

void SpellSindragosasFurySpellScript::CountTargets(std::list<WorldObject*>& targets)
{
    _targetCount = targets.size();
}

void SpellSindragosasFurySpellScript::HandleDummy(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);

    if (!GetHitUnit()->isAlive() || !_targetCount)
        return;

    float resistance = float(GetHitUnit()->GetResistance(SpellSchoolMask(GetSpellInfo()->SchoolMask)));
    uint32 minResistFactor = uint32((resistance / (resistance + 510.0f)) * 10.0f) * 2;
    uint32 randomResist = urand(0, (9 - minResistFactor) * 100) / 100 + minResistFactor;

    uint32 damage = (uint32(GetEffectValue() / _targetCount) * randomResist) / 10;

    SpellNonMeleeDamage damageInfo(GetCaster(), GetHitUnit(), GetSpellInfo()->Id, GetSpellInfo()->SchoolMask);
    damageInfo.damage = damage;
    GetCaster()->SendSpellNonMeleeDamageLog(&damageInfo);
    GetCaster()->DealSpellDamage(&damageInfo, false);
}

void SpellSindragosasFurySpellScript::Register()
{
    BeforeCast += SpellCastFn(SpellSindragosasFurySpellScript::SelectDest);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellSindragosasFurySpellScript::CountTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
    OnEffectHitTarget += SpellEffectFn(SpellSindragosasFurySpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
}