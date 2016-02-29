#include "Npc_SkybreakerVindicatorGunShip.h"

void NpcSkybreakerVindicatorGunshipAI::Reset()
{
    _started = instance->GetBossState(DATA_GUNSHIP_EVENT) == DONE;
    events.Reset();
    events.ScheduleEvent(EVENT_AVENGERS_SHILED, 10000); // TODO: Fix the timers
    events.ScheduleEvent(EVENT_CONCENTRATION, 20000); // TODO: Fix the timers
    events.ScheduleEvent(EVENT_HOLY_WRATH, 20000); // TODO: Fix the timers
}

void NpcSkybreakerVindicatorGunshipAI::MoveInLineOfSight(Unit* who)
{
    if (!instance)
        return;
    
    if (instance->GetData(DATA_TEAM_IN_INSTANCE) != ALLIANCE)
        return;
    
    if (who->GetTypeId() != TYPEID_PLAYER)
        return;
    
    if (instance->GetData(DATA_FIRST_SQUAD_STATE) == DONE)
        return;
    
    if (!_started && me->IsWithinDistInMap(who, 25.0f))
    {
        _started = true;
        events.ScheduleEvent(EVENT_FIRST_SQUAD_ASSISTED_1, 1000);
        events.ScheduleEvent(EVENT_FIRST_SQUAD_ASSISTED_2, 15000); // TODO : fix the timer
    }
}

void NpcSkybreakerVindicatorGunshipAI::UpdateAI(const uint32 diff)
{
    if (!instance)
        return;
    
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;
    
    events.Update(diff);
    
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_FIRST_SQUAD_ASSISTED_1:
                Talk(SAY_FIRST_SQUAD_RESCUED_ALLIANCE_0);
                break;
            case EVENT_FIRST_SQUAD_ASSISTED_2:
                if (Creature* tempUnit = me->FindNearestCreature(NPC_GB_SKYBREAKER_SORCERER, 120.0f, true))
                {
                    tempUnit->AI()->Talk(SAY_FIRST_SQUAD_RESCUED_ALLIANCE_1);
                    tempUnit->AI()->Talk(SAY_SUMMON_BATTLE_STANDARD);
                    tempUnit->CastSpell(tempUnit, SPELL_SUMMON_BATTLE_STANDART_A, true);
                }
                instance->SetData(DATA_FIRST_SQUAD_STATE, DONE);
                break;
            case EVENT_AVENGERS_SHILED:
                if (UpdateVictim())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 10.0f))
                        DoCast(target, SPELL_WRATH);
                    events.ScheduleEvent(EVENT_AVENGERS_SHILED, 10000);
                break;
            case EVENT_CONCENTRATION:
                if (UpdateVictim())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 10.0f))
                        DoCast(target, SPELL_CONCENTRATION);
                    events.ScheduleEvent(EVENT_CONCENTRATION, 20000);
                break;
            case EVENT_HOLY_WRATH:
                if (UpdateVictim())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 10.0f))
                        DoCast(target, SPELL_HOLY_WRATH);
                    events.ScheduleEvent(EVENT_HOLY_WRATH, 15000);
                break;
        }
    }
    
    DoMeleeAttackIfReady();
}