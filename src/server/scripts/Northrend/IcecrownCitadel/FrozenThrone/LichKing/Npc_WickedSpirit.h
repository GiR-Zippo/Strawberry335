#ifndef __NPC_WICKLEDSPIRIT_H_
#define __NPC_WICKLEDSPIRIT_H_

#include "TheLichKing.h"


class NpcWickedSpiritAI : public ScriptedAI
{
    public:
        NpcWickedSpiritAI(Creature* creature) : ScriptedAI(creature),
            _instance(creature->GetInstanceScript())
        {
        }

        void Reset();

    private:
        InstanceScript* _instance;
};

class NpcWickedSpirit : public CreatureScript
{
    public:
        NpcWickedSpirit() : CreatureScript("npc_wicked_spirit") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcWickedSpiritAI>(creature);
    }
};


#endif // __NPC_WICKLEDSPIRIT_H_