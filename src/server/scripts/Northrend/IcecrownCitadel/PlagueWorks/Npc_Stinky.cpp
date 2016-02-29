#include "Npc_Stinky.h"

///////////////////
// Npc Stinky AI //
///////////////////

void NpcStinkyAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_DECIMATE, urand(20000, 25000));
    _events.ScheduleEvent(EVENT_MORTAL_WOUND, urand(3000, 7000));
}

void NpcStinkyAI::EnterCombat(Unit* /*target*/)
{
    DoCast(me, SPELL_PLAGUE_STENCH);
}

void NpcStinkyAI::UpdateAI(uint32 const diff)
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
        case EVENT_DECIMATE:
            DoCastVictim(SPELL_DECIMATE);
            _events.ScheduleEvent(EVENT_DECIMATE, urand(20000, 25000));
            break;
        case EVENT_MORTAL_WOUND:
            DoCastVictim(SPELL_MORTAL_WOUND);
            _events.ScheduleEvent(EVENT_MORTAL_WOUND, urand(10000, 12500));
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}

void NpcStinkyAI::JustDied(Unit* /*killer*/)
{
    if (Creature* festergut = me->GetCreature(*me, _instance->GetData64(DATA_FESTERGUT)))
        if (festergut->isAlive())
            festergut->AI()->Talk(SAY_STINKY_DEAD);
}
