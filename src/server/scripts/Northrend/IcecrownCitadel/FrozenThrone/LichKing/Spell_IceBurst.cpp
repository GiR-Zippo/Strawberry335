#include "Spell_TheLichKing.h"

bool SpellTheLichKingIceBurstTargetSearchSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_ICE_BURST))
        return false;
    return true;
}

void SpellTheLichKingIceBurstTargetSearchSpellScript::CheckTargetCount(std::list<WorldObject*>& unitList)
{
    if (unitList.empty())
        return;

    // if there is at least one affected target cast the explosion
    GetCaster()->CastSpell(GetCaster(), SPELL_ICE_BURST, true);
    if (GetCaster()->GetTypeId() == TYPEID_UNIT)
    {
        GetCaster()->ToCreature()->SetReactState(REACT_PASSIVE);
        GetCaster()->AttackStop();
        GetCaster()->ToCreature()->DespawnOrUnsummon(500);
    }
}

void SpellTheLichKingIceBurstTargetSearchSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingIceBurstTargetSearchSpellScript::CheckTargetCount, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
}