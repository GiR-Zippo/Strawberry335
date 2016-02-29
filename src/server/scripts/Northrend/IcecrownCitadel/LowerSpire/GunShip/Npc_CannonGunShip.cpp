#include "Npc_CannonGunShip.h"

void NpcCannonGunshipAI::Reset()
{
    me->SetReactState(REACT_PASSIVE);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    DoCast(me, SPELL_HEAT_DRAIN, true);
}

void NpcCannonGunshipAI::EnterCombat(Unit* )
{
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    SetCombatMovement(false);
}

void NpcCannonGunshipAI::SpellHit(Unit* , const SpellInfo* spellEntry)
{
    if (spellEntry->Id == SPELL_BELOW_ZERO)
        me->GetVehicleKit()->RemoveAllPassengers();
}

void NpcCannonGunshipAI::DamageTaken(Unit* attacker, uint32& damage)
{
    if (me->GetEntry() == NPC_GB_ALLIANCE_CANON)
    {
        if (Creature* pAllianceBoss = ObjectAccessor::GetCreature(*me, me->GetInstanceScript()->GetData64(DATA_SKYBREAKER_BOSS)))
            attacker->DealDamage(pAllianceBoss, damage);
    }
    
    if (me->GetEntry() == NPC_GB_HORDE_CANON)
    {
        if (Creature* pHordeBoss = ObjectAccessor::GetCreature(*me, me->GetInstanceScript()->GetData64(DATA_ORGRIMMAR_HAMMER_BOSS)))
            attacker->DealDamage(pHordeBoss, damage);
    }
}

void NpcCannonGunshipAI::UpdateAI(const uint32 diff)
{
    if (me->HasAura(SPELL_BELOW_ZERO))
    {
        me->RemoveAurasByType(SPELL_AURA_CONTROL_VEHICLE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        
        if (Vehicle* veh = me->GetVehicleKit())
            veh->RemoveAllPassengers();
    }
    else
    {
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->RemoveExtraUnitMovementFlag(MOVEMENTFLAG_STRAFE_LEFT);
        me->RemoveExtraUnitMovementFlag(MOVEMENTFLAG_STRAFE_RIGHT);
        me->RemoveExtraUnitMovementFlag(MOVEMENTFLAG2_INTERPOLATED_TURNING);
        me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_NO_STRAFE);
        me->AddExtraUnitMovementFlag(MOVEMENTFLAG_LEFT);
        me->AddExtraUnitMovementFlag(MOVEMENTFLAG_RIGHT);
    }
}