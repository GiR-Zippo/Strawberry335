#include "Npc_GasCloud.h"

//////////////////
// Gas Cloud AI //
//////////////////

void GasCloudAI::Reset()
{
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
}

void GasCloudAI::SpellHitTarget(Unit* /*target*/, SpellInfo const* spell)
{
    if (!_newTargetSelectTimer && spell->Id == sSpellMgr->GetSpellIdForDifficulty(SPELL_EXPUNGED_GAS, me))
        _newTargetSelectTimer = 5000;
}

void GasCloudAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_TEAR_GAS_CREATURE)
        _newTargetSelectTimer = 5000;
}

void GasCloudAI::UpdateAI(uint32 const diff)
{

    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();

    if (!_newTargetSelectTimer)
        return;

    if (me->HasAura(SPELL_TEAR_GAS_CREATURE))
        return;

    if (_newTargetSelectTimer <= diff)
    {
        _newTargetSelectTimer = 0;
        me->CastCustomSpell(SPELL_GASEOUS_BLOAT, SPELLVALUE_AURA_STACK, 10, me, TRIGGERED_NONE);
    }
    else
        _newTargetSelectTimer -= diff;
}
