#include "Npc_DreamPortal.h"

void NpcDreamPortalAI::OnSpellClick(Unit* )
{
    _used = true;
    me->DespawnOrUnsummon();
}

uint32 NpcDreamPortalAI::GetData(uint32 type)
{
    return (type == MISSED_PORTALS && _used) ? 0 : 1;
}

void NpcDreamPortalAI::UpdateAI(const uint32 )
{
    UpdateVictim();
}