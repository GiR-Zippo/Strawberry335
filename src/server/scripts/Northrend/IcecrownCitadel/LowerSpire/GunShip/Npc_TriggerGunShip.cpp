#include "Npc_TriggerGunShip.h"

void NpcTriggerGunshipAI::Reset()
{
    ScriptedAI::Reset();
    me->SetReactState(REACT_PASSIVE);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
}

void NpcTriggerGunshipAI::EnterCombat(Unit* )
{
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    SetCombatMovement(false);
}

void NpcTriggerGunshipAI::DamageTaken(Unit* attacker, uint32& damage)
{
    damage = 0;
}

void NpcTriggerGunshipAI::UpdateAI(const uint32 diff)
{
}