#include "Npc_WickedSpirit.h"


void NpcWickedSpiritAI::Reset()
{
    me->SetCorpseDelay(0);
    me->SetReactState(REACT_PASSIVE);

    me->SetSpeed(MOVE_WALK, 0.5f, true);
    me->SetSpeed(MOVE_RUN, 0.5f, true);
    me->SetSpeed(MOVE_FLIGHT, 0.5f, true);
    me->SetCanFly(true);

    me->GetMotionMaster()->MoveRandom(10.0f);
}