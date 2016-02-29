#include "Spell_Sindragosa.h"
#include "ScriptedCreature.h"

void SpellSindragosaUnchainedMagicSpellScript::FilterTargets(std::list<WorldObject*>& unitList)
{
    unitList.remove_if(UnchainedMagicTargetSelector());
    uint32 maxSize = uint32(GetCaster()->GetMap()->GetSpawnMode() & 1 ? 6 : 2);
    if (unitList.size() > maxSize)
        Trinity::Containers::RandomResizeList(unitList, maxSize);
}

void SpellSindragosaUnchainedMagicSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellSindragosaUnchainedMagicSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
}