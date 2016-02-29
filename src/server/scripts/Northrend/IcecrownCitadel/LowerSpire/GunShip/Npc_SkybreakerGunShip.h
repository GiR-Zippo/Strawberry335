#ifndef __NPC_SKYBREAKER_GUNSHIPBATTLE_H_
#define __NPC_SKYBREAKER_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSkybreakerGunshipAI : public Scripted_NoMovementAI
{
public:
    NpcSkybreakerGunshipAI(Creature *creature) : Scripted_NoMovementAI(creature), _instance(creature->GetInstanceScript())
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

class NpcSkybreakerGunship : public CreatureScript
{
    public:
        NpcSkybreakerGunship() : CreatureScript("npc_gunship_skybreaker") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcSkybreakerGunshipAI>(pCreature);
    }
};

#endif