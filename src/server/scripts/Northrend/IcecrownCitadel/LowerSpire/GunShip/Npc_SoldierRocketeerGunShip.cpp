#include "Npc_SoldierRocketeerGunShip.h"

void NpcSoldierRocketeerGunshipAI::Reset()
{
    ScriptedAI::Reset();
    me->RemoveAurasDueToSpell(SPELL_EXPERIENCED);
    me->RemoveAurasDueToSpell(SPELL_ELITE);
    me->RemoveAurasDueToSpell(SPELL_VETERAN);
    events.ScheduleEvent(EVENT_EXPERIENCED, urand(19000, 21000)); // ~20 sec
    events.ScheduleEvent(EVENT_VETERAN, urand(39000, 41000)); // ~40 sec
    events.ScheduleEvent(EVENT_ELITE, urand(59000, 61000)); // ~60 sec
    events.ScheduleEvent(EVENT_ROCKET_ART, urand(10000, 15000)); // ~12 sec
}

void NpcSoldierRocketeerGunshipAI::JustDied(Unit* killer)
{
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
    {
        if (Creature* pSaurfangBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_HIGH_OVERLORD_SAURFANG)))
            pSaurfangBoss->AI()->DoAction(ACTION_ROCK_DIE);
    }
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
    {
        if (Creature* pMuradin = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_MURADIN_BRONZEBEARD)))
            pMuradin->AI()->DoAction(ACTION_ROCK_DIE);
    }
}

void NpcSoldierRocketeerGunshipAI::UpdateAI(const uint32 diff)
{
    if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != IN_PROGRESS)
        return;

    me->SetReactState(REACT_DEFENSIVE); // Dodato

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_EXPERIENCED:
                DoCast(me, SPELL_EXPERIENCED);
                break;
            case EVENT_VETERAN:
                me->RemoveAurasDueToSpell(SPELL_EXPERIENCED);
                DoCast(me, SPELL_VETERAN);
                break;
            case EVENT_ELITE:
                me->RemoveAurasDueToSpell(SPELL_VETERAN);
                DoCast(me, SPELL_ELITE);
                break;
            case EVENT_ROCKET_ART:
                if (Player* target = SelectRandomPlayerInTheMaps(me->GetMap()))
                {
                    if (me->GetEntry() == NPC_GB_KORKRON_ROCKETEER && target->HasAura(SPELL_ON_SKYBREAKERS_DECK))
                    {
                        me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_ROCKET_ARTILLERY_TRIGGERED, true);
                        me->CastSpell(target, 69400, true);
                    }
                    else if (me->GetEntry() == NPC_GB_SKYBREAKER_MORTAR_SOLDIER && target->HasAura(SPELL_ON_ORGRIMS_HAMMERS_DECK))
                    {
                        me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_ROCKET_ARTILLERY_TRIGGERED, true);
                        me->CastSpell(target, 70173, true);
                    }
                }
                events.ScheduleEvent(EVENT_ROCKET_ART, urand(10000, 15000)); // ~12 sec
                break;
            default:
                break;
        }
    }
}