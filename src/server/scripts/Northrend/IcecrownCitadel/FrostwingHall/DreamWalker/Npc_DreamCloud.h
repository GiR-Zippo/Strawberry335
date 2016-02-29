#ifndef __NPC_DREAMCLOUD_H_
#define __NPC_DREAMCLOUD_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcDreamCloudAI : public ScriptedAI
{
public:
    NpcDreamCloudAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    void Reset();
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
};

class NpcDreamCloud : public CreatureScript
{
    public:
        NpcDreamCloud() : CreatureScript("npc_dream_cloud") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcDreamCloudAI>(creature);
    }
};

#endif