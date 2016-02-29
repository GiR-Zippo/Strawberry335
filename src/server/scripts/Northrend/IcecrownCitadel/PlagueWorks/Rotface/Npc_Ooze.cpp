#include "Npc_Ooze.h"

////////////////////
// Little Ooze AI //
////////////////////

void LittleOozeAI::IsSummonedBy(Unit* summoner)
{
    DoCast(me, SPELL_LITTLE_OOZE_COMBINE, true);
    DoCast(me, SPELL_WEAK_RADIATING_OOZE, true);
    DoCast(me, SPELL_GREEN_ABOMINATION_HITTIN__YA_PROC, true); 
    events.ScheduleEvent(EVENT_STICKY_OOZE, 5000);
    me->AddThreat(summoner, 500000.0f);

    if (Creature* rotface = Unit::GetCreature(*me, instance->GetData64(DATA_ROTFACE)))
        rotface->AI()->JustSummoned(me);
}

void LittleOozeAI::JustDied(Unit* /*killer*/)
{
    me->DespawnOrUnsummon();
}

void LittleOozeAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (events.ExecuteEvent() == EVENT_STICKY_OOZE)
    {
        DoCastVictim(SPELL_STICKY_OOZE);
        events.ScheduleEvent(EVENT_STICKY_OOZE, 15000);
    }

    DoMeleeAttackIfReady();
}



/////////////////
// Big Ooze AI //
/////////////////

void BigOozeAI::IsSummonedBy(Unit* summoner)
{
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    DoCast(me, SPELL_LARGE_OOZE_COMBINE, true);
    DoCast(me, SPELL_LARGE_OOZE_BUFF_COMBINE, true);
    DoCast(me, SPELL_RADIATING_OOZE, true);
    DoCast(me, SPELL_UNSTABLE_OOZE, true);
    DoCast(me, SPELL_GREEN_ABOMINATION_HITTIN__YA_PROC, true);
    events.ScheduleEvent(EVENT_STICKY_OOZE, 5000);
    // register in Rotface's summons - not summoned with Rotface as owner
    if (Creature* rotface = Unit::GetCreature(*me, instance->GetData64(DATA_ROTFACE)))
        rotface->AI()->JustSummoned(me);
}

void BigOozeAI::JustDied(Unit* /*killer*/)
{
    if (Creature* rotface = Unit::GetCreature(*me, instance->GetData64(DATA_ROTFACE)))
        rotface->AI()->SummonedCreatureDespawn(me);
    me->DespawnOrUnsummon();
}

void BigOozeAI::DoAction(const int32 action)
{
    if (action == EVENT_STICKY_OOZE)
        events.CancelEvent(EVENT_STICKY_OOZE);
}

void BigOozeAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_STICKY_OOZE:
            DoCastVictim(SPELL_STICKY_OOZE);
            events.ScheduleEvent(EVENT_STICKY_OOZE, 15000);
        default:
            break;
        }
    }

    if (me->IsVisible())
        DoMeleeAttackIfReady();
}
