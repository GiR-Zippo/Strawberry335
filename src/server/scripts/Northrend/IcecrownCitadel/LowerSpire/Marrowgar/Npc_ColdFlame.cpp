#include "Npc_ColdFlame.h"

void NpcColdFlameAI::IsSummonedBy(Unit* owner)
{
    if (owner->GetTypeId() != TYPEID_UNIT)
        return;

    if (!owner->HasAura(SPELL_BONE_STORM))
    {
        Creature* target = ObjectAccessor::GetCreature(*owner, owner->GetAI()->GetGUID(DATA_COLDFLAME_GUID));
        if (!target)
        {
            me->DespawnOrUnsummon();
            return;
        }

        me->NearTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation());
    }

    _events.ScheduleEvent(EVENT_COLDFLAME_TRIGGER, 250);
}

void NpcColdFlameAI::UpdateAI(const uint32 diff)
{
    _events.Update(diff);

    if (_events.ExecuteEvent() == EVENT_COLDFLAME_TRIGGER)
    {
        Position newPos;
        me->GetNearPosition(newPos, 4.f, 0.0f);

        me->NearTeleportTo(newPos.GetPositionX(), newPos.GetPositionY(), me->GetPositionZ(), me->GetOrientation());
        DoCast(SPELL_COLDFLAME_SUMMON);
        _events.ScheduleEvent(EVENT_COLDFLAME_TRIGGER, 750);
    }
}