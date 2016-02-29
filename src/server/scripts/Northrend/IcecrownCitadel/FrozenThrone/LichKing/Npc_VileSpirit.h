#ifndef __NPC_VILESPIRIT_H_
#define __NPC_VILESPIRIT_H_

#include "TheLichKing.h"


class NpcVileSpiritAI : public ScriptedAI
{
    public:
    NpcVileSpiritAI(Creature* creature) : ScriptedAI(creature),
            _instance(creature->GetInstanceScript())
        {
        }

        void JustReachedHome();

    private:
        InstanceScript* _instance;
};

class NpcVileSpirit : public CreatureScript
{
    public:
        NpcVileSpirit() : CreatureScript("npc_vile_spirit") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcVileSpiritAI>(creature);
    }
};


#endif // __NPC_VILESPIRIT_H_