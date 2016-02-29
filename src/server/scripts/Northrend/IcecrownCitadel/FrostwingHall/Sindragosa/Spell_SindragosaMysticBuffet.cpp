#include "Spell_Sindragosa.h"

void SpellSindragosaMysticBuffetSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.remove_if(MysticBuffetTargetFilter(GetCaster()));
}

void SpellSindragosaMysticBuffetSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellSindragosaMysticBuffetSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
}