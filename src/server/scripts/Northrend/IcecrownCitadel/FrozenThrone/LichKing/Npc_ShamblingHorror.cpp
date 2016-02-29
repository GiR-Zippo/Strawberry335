#include "Npc_ShamblingHorror.h"

void NpcShamblingHorrorAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_SHOCKWAVE, urand(20000, 25000));
    _events.ScheduleEvent(EVENT_ENRAGE, urand(11000, 14000));
}

void NpcShamblingHorrorAI::DamageTaken(Unit* , uint32& damage)
{
    if (!_frenzied && IsHeroic() && me->HealthBelowPctDamaged(20, damage))
    {
        _frenzied = true;
        DoCast(me, SPELL_FRENZY, true);
    }
}

void NpcShamblingHorrorAI::UpdateAI(const uint32 diff)
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
            case EVENT_SHOCKWAVE:
                DoCast(me, SPELL_SHOCKWAVE);
                _events.ScheduleEvent(EVENT_SHOCKWAVE, urand(20000, 25000));
                break;
            case EVENT_ENRAGE:
                DoCast(me, SPELL_ENRAGE);
                _events.ScheduleEvent(EVENT_ENRAGE, urand(20000, 25000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}