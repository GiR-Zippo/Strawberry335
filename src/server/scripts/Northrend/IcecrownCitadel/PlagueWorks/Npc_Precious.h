#ifndef __NPC_PRECIOUS_H
#define __NPC_PRECIOUS_H

#include "PlagueWorks.h"
//#include "Rotface/Rotface.h"

struct PreciousAI : public ScriptedAI
{
public:
    PreciousAI(Creature* creature) : ScriptedAI(creature), _summons(me), _instance(creature->GetInstanceScript()) {}

    void Reset();
    void JustSummoned(Creature* summon);
    void SummonedCreatureDespawn(Creature* summon);
    void JustDied(Unit* /*killer*/);
    void UpdateAI(const uint32 diff);

private:

    EventMap _events;
    SummonList _summons;
    InstanceScript* _instance;

};

class Precious : public CreatureScript
{
public:
    Precious() : CreatureScript("npc_precious_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<PreciousAI>(creature);
    }
};

#endif