#include "Spell_DeathbringerSaurfang.h"

///////////////////
// Cent Of Blood //
///////////////////

void SpellScriptDeathbringerCentOfBlood::FilterTargets(std::list<WorldObject*>& targets)
{
    for (std::list<WorldObject*>::iterator it = targets.begin(); it != targets.end();)
    {
        if (WorldObject* wob = (*it))
            if (wob->GetTypeId() == TYPEID_UNIT)
                if (wob->GetEntry() == 38508 /* Entry of Blood Beast */)
                {
                    it++;
                    continue;
                }
                targets.erase(it++);
    }
}

void SpellScriptDeathbringerCentOfBlood::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptDeathbringerCentOfBlood::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellScriptDeathbringerCentOfBlood::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
}
