#ifndef __NPC_SPIRITWARDEN_H_
#define __NPC_SPIRITWARDEN_H_

#include "TheLichKing.h"

class NpcSpiritWardenAI : public ScriptedAI
{
public:
    NpcSpiritWardenAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    void Reset();
    void JustDied(Unit* /*killer*/);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
};

class NpcSpiritWarden : public CreatureScript
{
    public:
        NpcSpiritWarden() : CreatureScript("npc_spirit_warden") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSpiritWardenAI>(creature);
    }
};

#endif