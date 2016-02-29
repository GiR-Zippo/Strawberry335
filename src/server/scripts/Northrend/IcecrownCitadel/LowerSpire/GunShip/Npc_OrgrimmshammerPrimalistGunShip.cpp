#include "Npc_OrgrimmshammerPrimalistGunShip.h"

void NpcOrgrimmshammerPrimalistGunshipAI::Reset()
{
    _started = instance->GetBossState(DATA_GUNSHIP_EVENT) == DONE;
    events.Reset();
    events.ScheduleEvent(EVENT_WRATH, 10000); // TODO: Fix the timers
    events.ScheduleEvent(EVENT_HEAL, 20000); // TODO: Fix the timers
}

void NpcOrgrimmshammerPrimalistGunshipAI::MoveInLineOfSight(Unit* who)
{
    if (!instance)
        return;
    
    if (instance->GetData(DATA_TEAM_IN_INSTANCE) != HORDE)
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

void NpcOrgrimmshammerPrimalistGunshipAI::UpdateAI(const uint32 diff)
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
                Talk(SAY_FIRST_SQUAD_RESCUED_HORDE_0);
                break;
            case EVENT_FIRST_SQUAD_ASSISTED_2:
                if (Creature* tempUnit = me->FindNearestCreature(NPC_KOR_KRON_INVOKER, 120.0f, true))
                {
                    tempUnit->AI()->Talk(SAY_FIRST_SQUAD_RESCUED_HORDE_1);
                    tempUnit->AI()->Talk(SAY_SUMMON_BATTLE_STANDARD);
                    tempUnit->CastSpell(tempUnit, SPELL_SUMMON_BATTLE_STANDART_H, true);
                }
                instance->SetData(DATA_FIRST_SQUAD_STATE, DONE);
                break;
            case EVENT_WRATH:
                if (UpdateVictim())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 10.0f))
                        DoCast(target, SPELL_WRATH);
                    events.ScheduleEvent(EVENT_WRATH, 10000);
                break;
            case EVENT_HEAL:
                if (UpdateVictim())
                {
                    std::list<Unit*> TargetList;
                    Unit* finalTarget = me;
                    Trinity::AnyFriendlyUnitInObjectRangeCheck checker(me, me, 30.0f);
                    Trinity::UnitListSearcher<Trinity::AnyFriendlyUnitInObjectRangeCheck> searcher(me, TargetList, checker);
                    me->VisitNearbyObject(30.0f, searcher);
                    for (std::list<Unit*>::iterator itr = TargetList.begin(); itr != TargetList.end(); ++itr)
                        if ((*itr)->GetHealthPct() < finalTarget->GetHealthPct())
                            finalTarget = *itr;
                        
                        uint32 spellId = SPELL_HEALING_TOUCH;
                    uint32 healthPct = uint32(finalTarget->GetHealthPct());
                    if (healthPct > 15 && healthPct < 20)
                        spellId = (urand (0, 1) ? SPELL_REGROWTH : SPELL_HEALING_TOUCH);
                    else if (healthPct >= 20 && healthPct < 40)
                        spellId = SPELL_REGROWTH;
                    else if (healthPct > 40)
                        spellId = (urand(0, 1) ? SPELL_REJUVENATION : SPELL_REGROWTH);
                    
                    me->CastSpell(finalTarget, spellId, false);
                    events.ScheduleEvent(EVENT_HEAL, 20000);
                }
                break;
        }
    }
    
    DoMeleeAttackIfReady();
}