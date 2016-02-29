#include "Tlaloc.h"

void TlalocAI::Reset()
{
    phase = 0;
    phase2 = 0;
    phase3 = 0;
    events.ScheduleEvent(EVENT_KRACHEN, 14000);
}

void TlalocAI::JustDied(Unit* /*killer*/)
{
}

void TlalocAI::KilledUnit(Unit* who)
{
    //DoScriptText(SAY_KILLED, me, who);
}

void TlalocAI::EnterCombat(Unit* /*who*/)
{
    //DoScriptText(SAY_AGGRO, me);
    me->SetInCombatWithZone();
}

void TlalocAI::UpdateAI(uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_KRACHEN:
                me->MonsterYell("krachen 1", 0, 0);
                phase = urand(1, 6);
                me->SetReactState(REACT_PASSIVE);
                me->SetFacingTo(dir[phase]);
                GetCreatureListWithEntryInGrid(creatures, me, creatureKrachen[phase], 100.0f);
                for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* stalker = *itr;
                    stalker->GetAI()->DoAction(ACTION_EXPLODE_START);
                }
                DoCast(SPELL_KRACHEN);
                events.ScheduleEvent(EVENT_RESET, 3500);
                events.ScheduleEvent(EVENT_KRACHEN_2, 16000);
                break;
            case EVENT_KRACHEN_2:
                me->MonsterYell("krachen 2", 0, 0);
                do 
                {
                    phase2 = urand(1, 6);
                } while (phase == phase2);
                me->SetReactState(REACT_PASSIVE);
                me->SetFacingTo(dir[phase2]);
                GetCreatureListWithEntryInGrid(creatures2, me, creatureKrachen[phase2], 100.0f);
                for (std::list<Creature*>::iterator itr = creatures2.begin(); itr != creatures2.end(); ++itr)
                {
                    Creature* stalker = *itr;
                    stalker->GetAI()->DoAction(ACTION_EXPLODE_START);
                }
                DoCast(SPELL_KRACHEN);
                events.ScheduleEvent(EVENT_RESET, 3500);
                events.ScheduleEvent(EVENT_KRACHEN_3, 16000);
                break;
            case EVENT_KRACHEN_3:
                me->MonsterYell("krachen 3", 0, 0);
                do 
                {
                    phase3 = urand(1, 6);
                } while (phase2 == phase3);
                me->SetReactState(REACT_PASSIVE);
                me->SetFacingTo(dir[phase3]);
                GetCreatureListWithEntryInGrid(creatures3, me, creatureKrachen[phase3], 100.0f);
                for (std::list<Creature*>::iterator itr = creatures3.begin(); itr != creatures3.end(); ++itr)
                {
                    Creature* stalker = *itr;
                    stalker->GetAI()->DoAction(ACTION_EXPLODE_START);
                }
                DoCast(SPELL_KRACHEN);
                events.ScheduleEvent(EVENT_RESET, 3500);
                events.ScheduleEvent(EVENT_BIG_KRACHEN, 16000);
                break;
            case EVENT_BIG_KRACHEN:
                creatures.merge(creatures2);
                creatures.merge(creatures3);
                for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* stalker = *itr;
                    stalker->GetAI()->DoAction(ACTION_EXPLODE_BIG);
                }
                phase = 0;
                phase2 = 0;
                phase3 = 0;
                events.ScheduleEvent(EVENT_KRACHEN, 16000);
                break;
            case EVENT_RESET:
                me->SetFacingToObject(me->getVictim());
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}

