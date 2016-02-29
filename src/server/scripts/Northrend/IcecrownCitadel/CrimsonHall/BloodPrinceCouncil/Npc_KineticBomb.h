#ifndef __NPC_KINETICBOMB_H_
#define __NPC_KINETICBOMB_H_

#include "BloodPrinceCouncil.h"

class NpcKineticBombAI : public ScriptedAI
{
public:
    NpcKineticBombAI(Creature* creature) : ScriptedAI(creature)
    {
        _instance = creature->GetInstanceScript();
    }

    void Reset();
    void DoAction(int32 const action);
    void MovementInform(uint32 type, uint32 id);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    float _x;
    float _y;
    float _groundZ;
    InstanceScript* _instance;
};

class NpcKineticBomb : public CreatureScript
{
    public:
        NpcKineticBomb() : CreatureScript("npc_kinetic_bomb") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcKineticBombAI>(creature);
    }
};

#endif