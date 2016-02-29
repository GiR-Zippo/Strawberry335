#ifndef __NPC_MAGE_GUNSHIPBATTLE_H_
#define __NPC_MAGE_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcMageGunshipAI : public Scripted_NoMovementAI
{
public:
    NpcMageGunshipAI(Creature *creature) : Scripted_NoMovementAI(creature),_instance(creature->GetInstanceScript())
    {
        Reset();
    }

    void Reset();
    void DoAction(int32 const action);
    void JustDied(Unit* killer);
    void UpdateAI(const uint32 diff);

    uint32 timer_BelowZero;

    private:
        EventMap events;
        InstanceScript* _instance;
};

class NpcMageGunship : public CreatureScript
{
    public:
        NpcMageGunship() : CreatureScript("npc_gunship_mage") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcMageGunshipAI>(pCreature);
    }
};

#endif