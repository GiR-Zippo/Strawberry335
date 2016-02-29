#include "Npc_SpiritBomb.h"

void NpcSpiritBombAI::IsSummonedBy ( Unit* )
{
    float destX, destY, destZ;
    me->GetPosition(destX, destY);
    destZ = 1055.0f;    // approximation, gets more precise later
    me->UpdateGroundPositionZ(destX, destY, destZ);
    // me->GetMotionMaster()->MovePoint(POINT_GROUND, destX, destY, destZ);
    me->GetMotionMaster()->MoveCharge(destX, destY, destZ, 2.0f, POINT_GROUND);
}

void NpcSpiritBombAI::MovementInform ( uint32 type, uint32 point )
{
    if (type != POINT_MOTION_TYPE || point != POINT_GROUND)
        return;

    me->RemoveAllAuras();
    DoCastAOE(SPELL_EXPLOSION);
    me->DespawnOrUnsummon(1000);
}

void NpcSpiritBombAI::AttackStart ( Unit* )
{
}

void NpcSpiritBombAI::UpdateAI ( const uint32 )
{
    UpdateVictim();
    // no melee attacks
}