#ifndef __NPC_ORGRIMMSHAMMER_GUNSHIPBATTLE_H_
#define __NPC_ORGRIMMSHAMMER_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcOrgrimmshammerGunshipAI : public Scripted_NoMovementAI
{
public:
    NpcOrgrimmshammerGunshipAI(Creature *creature) : Scripted_NoMovementAI(creature), _instance(creature->GetInstanceScript())
    {
        Reset();
    }

    void Reset();
    void EnterCombat(Unit* /*who*/);
    void JustDied(Unit* killer);

    private:
        EventMap events;
        InstanceScript* _instance;
};

class NpcOrgrimmshammerGunship : public CreatureScript
{
    public:
        NpcOrgrimmshammerGunship() : CreatureScript("npc_gunship_orgrimmar") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcOrgrimmshammerGunshipAI>(pCreature);
    }
};

#endif