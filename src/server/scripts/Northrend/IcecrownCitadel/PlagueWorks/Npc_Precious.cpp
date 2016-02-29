#include "Npc_Precious.h"

/////////////////
// Precious AI //
/////////////////

void PreciousAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_DECIMATE, urand(20000, 25000));
    _events.ScheduleEvent(EVENT_MORTAL_WOUND, urand(3000, 7000));
    _events.ScheduleEvent(EVENT_SUMMON_ZOMBIES, urand(20000, 22000));
    _summons.DespawnAll();
}

void PreciousAI::JustSummoned(Creature* summon)
{
    _summons.Summon(summon);
    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
        summon->AI()->AttackStart(target);
}

void PreciousAI::SummonedCreatureDespawn(Creature* summon)
{
    _summons.Despawn(summon);
}

void PreciousAI::JustDied(Unit* /*killer*/)
{
    _summons.DespawnAll();
    if (Creature* rotface = Unit::GetCreature(*me, _instance->GetData64(DATA_ROTFACE)))
        if (rotface->isAlive())
            rotface->AI()->Talk(SAY_PRECIOUS_DIES);
}

void PreciousAI::UpdateAI(const uint32 diff)
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
        case EVENT_SUMMON_ZOMBIES:
            Talk(EMOTE_PRECIOUS_ZOMBIES);
            for (uint32 i = 0; i < 11; ++i)
                DoCast(me, SPELL_AWAKEN_PLAGUED_ZOMBIES, false);
            _events.ScheduleEvent(EVENT_SUMMON_ZOMBIES, urand(20000, 22000));
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}
