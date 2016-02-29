#include "Npc_DarkNucleus.h"

void NpcDarkNucleusAI::Reset()
{
    me->SetReactState(REACT_DEFENSIVE);
    me->CastSpell(me, SPELL_SHADOW_RESONANCE_AURA, true);
}

void NpcDarkNucleusAI::EnterCombat(Unit* who)
{
    _targetAuraCheck = 1000;
    if (me->GetDistance(who) >= 15.0f)
    {
        DoStartMovement(who);
        return;
    }

    DoCast(who, SPELL_SHADOW_RESONANCE_RESIST);
    me->ClearUnitState(UNIT_STATE_CASTING);
}

void NpcDarkNucleusAI::MoveInLineOfSight(Unit* who)
{
}

void NpcDarkNucleusAI::DamageTaken(Unit* attacker, uint32& )
{
    if (attacker == me || !attacker)
        return;

    me->getThreatManager().addThreat(attacker, 500000.0f);
}

void NpcDarkNucleusAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (_targetAuraCheck <= diff)
    {
        _targetAuraCheck = 1000;
        if (Unit* victim = me->getVictim())
            if (me->GetDistance(victim) < 15.0f &&
                !victim->HasAura(SPELL_SHADOW_RESONANCE_RESIST, me->GetGUID()))
            {
                DoCast(victim, SPELL_SHADOW_RESONANCE_RESIST);
                me->ClearUnitState(UNIT_STATE_CASTING);
            }
            else
                MoveInLineOfSight(me->getVictim());
    }
    else
        _targetAuraCheck -= diff;
}