#include "Npc_VileSpirit.h"


void NpcVileSpiritAI::JustReachedHome()
{
    me->GetMotionMaster()->MovementExpired(true);
    me->GetMotionMaster()->MoveRandom(10.0f);
}