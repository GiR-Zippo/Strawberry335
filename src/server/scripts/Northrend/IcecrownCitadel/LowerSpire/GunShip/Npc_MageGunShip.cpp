#include "Npc_MageGunShip.h"

void NpcMageGunshipAI::Reset()
{
    ScriptedAI::Reset();
    timer_BelowZero = urand(10000, 15000);
}

void NpcMageGunshipAI::DoAction(const int32 action)
{
    switch (action)
    {
        case EVENT_FREEZE_CANNON:
            if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
            {
                
                std::list<Creature*> cannonsA;
                GetCreatureListWithEntryInGrid(cannonsA, me, NPC_GB_ALLIANCE_CANON, 500.0f);
                for (std::list<Creature*>::iterator itr = cannonsA.begin(); itr != cannonsA.end(); ++itr)
                {
                    if (Vehicle* veh = (*itr)->GetVehicleKit())
                        veh->RemoveAllPassengers();
                    
                    DoCast((*itr),SPELL_BELOW_ZERO,true);
                }
            }
            else if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
            {
                std::list<Creature*> cannonsH;
                GetCreatureListWithEntryInGrid(cannonsH, me, NPC_GB_HORDE_CANON, 500.0f);
                for (std::list<Creature*>::iterator itr = cannonsH.begin(); itr != cannonsH.end(); ++itr)
                {
                    if (Vehicle* veh = (*itr)->GetVehicleKit())
                        veh->RemoveAllPassengers();
                    
                    DoCast((*itr),SPELL_BELOW_ZERO,true);
                }
            }
            break;
    }
}

void NpcMageGunshipAI::JustDied(Unit* killer)
{
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
    {
        if (me->GetGUID() == _instance->GetData64(DATA_GB_BATTLE_MAGE))
        {
            if (Creature* pSaurfangBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_HIGH_OVERLORD_SAURFANG)))
                pSaurfangBoss->AI()->DoAction(ACTION_MAGE_DIE);
        }
    }
    
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
    {
        if (me->GetGUID() == _instance->GetData64(DATA_GB_BATTLE_MAGE))
        {
            if (Creature* pMuradin = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_MURADIN_BRONZEBEARD)))
                pMuradin->AI()->DoAction(ACTION_MAGE_DIE);
        }
    }
}

void NpcMageGunshipAI::UpdateAI(const uint32 diff)
{
    if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != IN_PROGRESS)
        return;
    
    me->SetReactState(REACT_DEFENSIVE); // Dodato
    
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;
    
    if (me->GetGUID() == _instance->GetData64(DATA_GB_BATTLE_MAGE))
    {
        
        if ( timer_BelowZero <= diff)
        {
            me->AI()->DoAction(EVENT_FREEZE_CANNON);
            timer_BelowZero = urand(10000, 15000);
        } else timer_BelowZero -= diff;
    }
    else
    {
        DoCast(me,SPELL_SHADOW_CHANNELING);
    }
}