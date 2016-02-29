#ifndef __NPC_TRIGGER_GUNSHIPBATTLE_H_
#define __NPC_TRIGGER_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcTriggerGunshipAI : public Scripted_NoMovementAI
{
public:
    NpcTriggerGunshipAI(Creature *creature) : Scripted_NoMovementAI(creature),_instance(creature->GetInstanceScript())
    {

        Reset();
    }

    void Reset();
    void EnterCombat(Unit* /*who*/);
    void DamageTaken(Unit* attacker, uint32& damage);
    void UpdateAI(const uint32 diff);

    private:
        EventMap events;
        InstanceScript* _instance;
};

class NpcTriggerGunship : public CreatureScript
{
    public:
        NpcTriggerGunship() : CreatureScript("npc_gunship_trigger") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcTriggerGunshipAI>(pCreature);
    }
};

#endif