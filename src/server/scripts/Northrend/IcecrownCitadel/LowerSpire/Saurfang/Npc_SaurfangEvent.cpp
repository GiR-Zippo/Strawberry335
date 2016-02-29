#include "Npc_SaurfangEvent.h"

///////////////////////
// Saurfang Event AI //
///////////////////////

void SaurfangEventAI::SetData(uint32 type, uint32 data)
{
    ASSERT(!type && data && data < 6);
    _index = data;
}

void SaurfangEventAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_GRIP_OF_AGONY)
    {
        me->SetDisableGravity(true);
        me->GetMotionMaster()->MovePoint(POINT_CHOKE, chokePos[_index]);
    }
}

void SaurfangEventAI::DoAction(int32 const action) 
{
    if (action == ACTION_CHARGE && _index)
        me->GetMotionMaster()->MoveCharge(chargePos[_index].GetPositionX(), chargePos[_index].GetPositionY(), chargePos[_index].GetPositionZ(), 13.0f, POINT_CHARGE);
    else if (action == ACTION_DESPAWN)
        me->DespawnOrUnsummon();
}
