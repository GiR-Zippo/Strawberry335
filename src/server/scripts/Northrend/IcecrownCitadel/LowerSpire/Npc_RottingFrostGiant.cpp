#include "Npc_RottingFrostGiant.h"

void NpcRottingFrostGiantAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
    _events.ScheduleEvent(EVENT_STOMP, urand(5000, 8000));
    _events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(10000, 15000));
}

void NpcRottingFrostGiantAI::JustDied(Unit* )
{
    _events.Reset();
}

void NpcRottingFrostGiantAI::UpdateAI(const uint32 diff)
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
            case EVENT_DEATH_PLAGUE:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                {
                    Talk(EMOTE_DEATH_PLAGUE_WARNING, target->GetGUID());
                    DoCast(target, SPELL_DEATH_PLAGUE);
                }
                _events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
                break;
            case EVENT_STOMP:
                DoCastVictim(SPELL_STOMP);
                _events.ScheduleEvent(EVENT_STOMP, urand(15000, 18000));
                break;
            case EVENT_ARCTIC_BREATH:
                DoCastVictim(SPELL_ARCTIC_BREATH);
                _events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(26000, 33000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}

bool DeathPlagueTargetSelector::operator()(WorldObject* object) const
{
    if (object == _caster)
        return true;

    if (object->GetTypeId() != TYPEID_PLAYER)
        return true;

    if (object->ToUnit()->HasAura(SPELL_RECENTLY_INFECTED) || object->ToUnit()->HasAura(SPELL_DEATH_PLAGUE_AURA))
        return true;

    return false;
}