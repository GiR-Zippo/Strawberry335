#include "Kriffel.h"

class Kriffel_EnergyAI : public ScriptedAI
{
public:
    Kriffel_EnergyAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset()
    {
        events.Reset();
    }

    void UpdateAI(const uint32 diff)
    {
        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_EXPLODE_ENERGY_PRE:
                    DoCast(SPELL_LIGHT_ESSENCE);
                    DoCast(SPELL_DARK_ESSENCE);
                    GetCreatureListWithEntryInGrid(fires, me, NPC_FIRE, 2.5f);
                    for (std::list<Creature*>::iterator itr = fires.begin(); itr != fires.end(); ++itr)
                    {
                        Creature* stalker = *itr;
                        stalker->DespawnOrUnsummon(1);
                    }
                    events.ScheduleEvent(EVENT_EXPLODE_ENERGY, 5000);
                    break;
                case EVENT_EXPLODE_ENERGY:
                {
                    Player* player = me->SelectNearestPlayer(2.5f);
                    std::list<Player*> PlayerList;
                    if (player)
                    {
                        DoCast(SPELL_SHADOW_EXPLO3);
                    }
                    else
                    {
                        DoCastAOE(SPELL_SHADOW_EXPLO2, true);
                    }
                    DoCast(SPELL_SHADOW_GLITTER);
                    me->DespawnOrUnsummon(1200);
                    break;
                }
                default:
                    break;
            }
        }
    }

    void DoAction(const int32 action)
    {
        switch (action)
        {
            case ACTION_APPEAR:
                events.ScheduleEvent(EVENT_EXPLODE_ENERGY_PRE, 1800);
                break;
            default:
                break;
        }
    }

private:
    EventMap events;
    std::list<Creature*> energy;
    std::list<Creature*> fires;
};

class Kriffel_Energy : public CreatureScript
{
public:
    Kriffel_Energy() : CreatureScript("custom_kriffel_energy") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Kriffel_EnergyAI(creature);
    };
};

void LoadNpcKriffelEnergy()
{
    new Kriffel_Energy();
}
