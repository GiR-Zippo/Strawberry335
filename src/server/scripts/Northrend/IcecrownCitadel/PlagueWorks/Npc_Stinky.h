#ifndef __NPC_STINKY_H
#define __NPC_STINKY_H

#include "PlagueWorks.h"
//#include "Festergut/Festergut.h"

class NpcStinkyAI : public ScriptedAI
{
public:
    NpcStinkyAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) {}

    void Reset();
    void EnterCombat(Unit* /*target*/);
    void UpdateAI(uint32 const diff);
    void JustDied(Unit* /*killer*/);

private:

    EventMap _events;
    InstanceScript* _instance;

};


class NpcStinky : public CreatureScript
{
public:
    NpcStinky() : CreatureScript("npc_stinky_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcStinkyAI>(creature);
    }
};

#endif