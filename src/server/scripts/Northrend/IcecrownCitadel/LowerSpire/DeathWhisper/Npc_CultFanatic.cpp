#include "Npc_CultFanatic.h"

/////////////////////
// Cult Fanatic AI //
/////////////////////

void CultFanaticAI::Reset()
{
    Events.Reset();
    Events.ScheduleEvent(EVENT_FANATIC_NECROTIC_STRIKE, urand(10000, 12000));
    Events.ScheduleEvent(EVENT_FANATIC_SHADOW_CLEAVE, urand(14000, 16000));
    Events.ScheduleEvent(EVENT_FANATIC_VAMPIRIC_MIGHT, urand(20000, 27000));
    if (me->GetEntry() == NPC_CULT_FANATIC)
        Events.ScheduleEvent(EVENT_CULTIST_DARK_MARTYRDOM, urand(18000, 32000));
}

void CultFanaticAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_DARK_TRANSFORMATION)
        me->UpdateEntry(NPC_DEFORMED_FANATIC);
    else if (spell->Id == SPELL_DARK_TRANSFORMATION_T)
    {
        Events.CancelEvent(EVENT_CULTIST_DARK_MARTYRDOM);
        me->InterruptNonMeleeSpells(true);
        DoCast(me, SPELL_DARK_TRANSFORMATION);
    }
}

void CultFanaticAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim())
        return;

    Events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = Events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_FANATIC_NECROTIC_STRIKE:
            DoCastVictim(NECROTIC_STRIKE_HELPER);
            Events.ScheduleEvent(EVENT_FANATIC_NECROTIC_STRIKE, urand(11000, 13000));
            break;
        case EVENT_FANATIC_SHADOW_CLEAVE:
            DoCastVictim(SHADOW_CLEAVE_HELPER);
            Events.ScheduleEvent(EVENT_FANATIC_SHADOW_CLEAVE, urand(9500, 11000));
            break;
        case EVENT_FANATIC_VAMPIRIC_MIGHT:
            DoCast(me, SPELL_VAMPIRIC_MIGHT);
            Events.ScheduleEvent(EVENT_FANATIC_VAMPIRIC_MIGHT, urand(20000, 27000));
            break;
        case EVENT_CULTIST_DARK_MARTYRDOM:
            DoCast(me, DARK_MARTYRDOM_FANATIC_HELPER);
            Events.ScheduleEvent(EVENT_CULTIST_DARK_MARTYRDOM, urand(16000, 21000));
            break;
        }
    }

    DoMeleeAttackIfReady();
}

