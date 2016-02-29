#include "Spell_Sindragosa.h"

bool SpellSindragosaCollisionFilterSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_ICE_TOMB_DAMAGE))
        return false;
    return true;
}

void SpellSindragosaCollisionFilterSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    std::list<Creature*> tombs;
    GetCreatureListWithEntryInGrid(tombs, GetCaster(), NPC_ICE_TOMB, 200.0f);
    targets.remove_if(FrostBombTargetSelector(GetCaster(), tombs));
}

void SpellSindragosaCollisionFilterSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellSindragosaCollisionFilterSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
}