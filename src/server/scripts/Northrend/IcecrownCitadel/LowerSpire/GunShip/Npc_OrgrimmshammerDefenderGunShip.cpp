#include "Npc_OrgrimmshammerDefenderGunShip.h"

void NpcOrgrimmshammerDefenderGunshipAI::Reset()
{
    events.Reset();
    events.ScheduleEvent(EVENT_DEVASTATE, 10000);
    events.ScheduleEvent(EVENT_THUNDERCLAP, 20000);
    events.ScheduleEvent(EVENT_SPELL_REFLECT, 25000);
    events.ScheduleEvent(EVENT_SUNDER_ARMOR, 15000);
}

void NpcOrgrimmshammerDefenderGunshipAI::MoveInLineOfSight(Unit* who)
{
    if (!instance)
        return;
    
    if (instance->GetData(DATA_TEAM_IN_INSTANCE) != HORDE)
        return;
    
    if (who->GetTypeId() != TYPEID_PLAYER)
        return;
    
    if (me->IsWithinDistInMap(who, 25.0f))
    {
        if (instance->GetData(DATA_FIRST_SQUAD_STATE) == DONE && instance->GetData(DATA_SECOND_SQUAD_STATE) != DONE)
        {
            instance->SetData(DATA_SECOND_SQUAD_STATE, DONE);
            events.ScheduleEvent(EVENT_SECOND_SQUAD_ASSISTED_1, 1000);
            events.ScheduleEvent(EVENT_SECOND_SQUAD_ASSISTED_2, 15000); // TODO : fix the timer
        }
    }
}

void NpcOrgrimmshammerDefenderGunshipAI::UpdateAI(const uint32 diff)
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
            case EVENT_SECOND_SQUAD_ASSISTED_1:
                Talk(SAY_SECOND_SQUAD_RESCUED_HORDE_0);
                break;
            case EVENT_SECOND_SQUAD_ASSISTED_2:
                Talk(SAY_SECOND_SQUAD_RESCUED_HORDE_1);
                Talk(SAY_FROSTWYRM_SUMMON_0);
                break;
            case EVENT_DEVASTATE:
                if (UpdateVictim())
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_DEVASTATE);
                }
                events.ScheduleEvent(EVENT_DEVASTATE, 10000);
                break;
            case EVENT_SPELL_REFLECT:
                if (UpdateVictim())
                {
                    DoCast(me, SPELL_SPELL_REFLECT);
                }
                events.ScheduleEvent(EVENT_SPELL_REFLECT, 25000);
                break;
            case EVENT_THUNDERCLAP:
                if (UpdateVictim())
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        DoCast(target, SPELL_THUNDERCLAP);
                }
                events.ScheduleEvent(EVENT_THUNDERCLAP, 20000);
                break;
            case EVENT_SUNDER_ARMOR:
                if (UpdateVictim())
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        DoCast(target, SPELL_SUNDER_ARMOR);
                }
                events.ScheduleEvent(EVENT_SUNDER_ARMOR, 15000);
                break;
        }
    }
    
    DoMeleeAttackIfReady();
}