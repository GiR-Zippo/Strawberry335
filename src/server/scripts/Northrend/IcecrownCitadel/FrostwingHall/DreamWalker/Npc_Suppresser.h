#ifndef __NPC_SUPPRESSER_H_
#define __NPC_SUPPRESSER_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSuppresserAI : public ScriptedAI
{
public:
    NpcSuppresserAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    void Reset();
    void IsSummonedBy(Unit* /*summoner*/);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* const _instance;
};

class NpcSuppresser : public CreatureScript
{
    public:
        NpcSuppresser() : CreatureScript("npc_suppresser") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSuppresserAI>(creature);
    }
};

#endif