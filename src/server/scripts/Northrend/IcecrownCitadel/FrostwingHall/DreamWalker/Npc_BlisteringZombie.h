#ifndef __NPC_BLISTERINGZOMBIE_H_
#define __NPC_BLISTERINGZOMBIE_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcBlisteringZombieAI : public ScriptedAI
{
public:
    NpcBlisteringZombieAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void EnterCombat(Unit* victim);
    void DamageTaken(Unit* victim, uint32& damage);
    void UpdateAI(uint32 const /*diff*/);
    void DoAction(const int32 action);
private:
    bool acidDone;
};

class NpcBlisteringZombie : public CreatureScript
{
    public:
        NpcBlisteringZombie() : CreatureScript("npc_blistering_zombie") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcBlisteringZombieAI>(creature);
    }
};

#endif