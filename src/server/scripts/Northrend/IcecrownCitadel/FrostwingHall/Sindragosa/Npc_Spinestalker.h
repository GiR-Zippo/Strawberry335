#ifndef __NPC_SPINESTALKER_H_
#define __NPC_SPINESTALKER_H_

#include "Sindragosa.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSpinestalkerAI : public ScriptedAI
{
public:
    NpcSpinestalkerAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _summoned(false)
    {
    }

    void InitializeAI();
    void Reset();
    void JustRespawned();
    void JustDied(Unit* /*killer*/);
    void DoAction(int32 const action);
    void MovementInform(uint32 type, uint32 point);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
    bool _summoned;
};

class NpcSpinestalker : public CreatureScript
{
    public:
        NpcSpinestalker() : CreatureScript("npc_spinestalker") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSpinestalkerAI>(creature);
    }
};

#endif