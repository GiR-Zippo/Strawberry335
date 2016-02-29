#include "Npc_PortalGunShip.h"

void NpcPortalGunshipAI::Reset()
{
    events.ScheduleEvent(EVENT_UNSUMMON_PORTAL, 20500); // TODO: Fix the timers
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void NpcPortalGunshipAI::JustDied(Unit* killer)
{
    me->RemoveFromWorld();
}

void NpcPortalGunshipAI::UpdateAI(const uint32 diff)
{
    events.Update(diff);
    
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_UNSUMMON_PORTAL:
                me->RemoveFromWorld();
                break;
        }
    }
}