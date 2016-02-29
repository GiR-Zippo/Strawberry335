#include "Kriffel.h"

class Kriffel_FrostbombAI : public ScriptedAI
{
public:
    Kriffel_FrostbombAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset()
    {
        events.Reset();
        active = false;
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE);
        me->SetReactState(REACT_PASSIVE);
        DoZoneInCombat();

        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);

        events.ScheduleEvent(EVENT_ACTIVATE_VERDERBNIS, 1500);
    }

    void UpdateAI(const uint32 diff)
    {
        events.Update(diff);

        if (Player* target = me->FindNearestPlayer(0.5f))
            if (active && !target->isGameMaster())
            {
                active = false;
                //DoCast(SPELL_VERDERBNIS_EXPLOSION_VIS);
                DoCast(SPELL_VERDERBNIS_EXPLOSION);
                me->DespawnOrUnsummon(1000);
            }

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_ACTIVATE_VERDERBNIS:
                    active = true;
                    break;
                default:
                    break;
            }
        }
    }

private:
    bool active;
    EventMap events;
};

class Kriffel_Frostbomb : public CreatureScript
{
public:
    Kriffel_Frostbomb() : CreatureScript("custom_kriffel_frostbomb") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Kriffel_FrostbombAI(creature);
    };
};

void LoadNpcKriffelFrostbomb()
{
    new Kriffel_Frostbomb();
}
