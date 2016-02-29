#ifndef __NPC_SOLDIERROCKETEER_GUNSHIPBATTLE_H_
#define __NPC_SOLDIERROCKETEER_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSoldierRocketeerGunshipAI : public Scripted_NoMovementAI
{
public:
    NpcSoldierRocketeerGunshipAI(Creature *creature) : Scripted_NoMovementAI(creature),_instance(creature->GetInstanceScript())
    {
        Reset();
    }

    void Reset();
    void JustDied(Unit* killer);
    void UpdateAI(const uint32 diff);

    private:
        EventMap events;
        InstanceScript* _instance;
};

class NpcSoldierRocketeerGunship : public CreatureScript
{
    public:
        NpcSoldierRocketeerGunship() : CreatureScript("npc_mortar_soldier_or_rocketeer") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcSoldierRocketeerGunshipAI>(pCreature);
    }
};

#endif