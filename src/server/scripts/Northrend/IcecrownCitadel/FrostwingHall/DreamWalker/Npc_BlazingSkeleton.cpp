#include "Npc_BlazingSkeleton.h"

void NpcBlazingSkeletonAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_FIREBALL, urand(2000, 4000));
    _events.ScheduleEvent(EVENT_LEY_WASTE, urand(15000, 20000));
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
}

void NpcBlazingSkeletonAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_FIREBALL:
                if (!me->IsWithinMeleeRange(me->getVictim()))
                    DoCastVictim(SPELL_FIREBALL);
                _events.ScheduleEvent(EVENT_FIREBALL, urand(2000, 4000));
                break;
            case EVENT_LEY_WASTE:
                DoCast(me, SPELL_LEY_WASTE);
                _events.ScheduleEvent(EVENT_LEY_WASTE, urand(15000, 20000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}