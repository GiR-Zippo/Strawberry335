#ifndef __NPC_TERENASMENETHIL_H_
#define __NPC_TERENASMENETHIL_H_

#include "TheLichKing.h"

class NpcTerenasMenethilAI : public ScriptedAI
{
public:
    NpcTerenasMenethilAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    bool CanAIAttack(Unit const* target) const;
    void DoAction(int32 const action);
    void EnterEvadeMode();
    void DamageTaken(Unit* /*attacker*/, uint32& damage);
    void IsSummonedBy(Unit* /*summoner*/);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
};

class NpcTerenasMenethil : public CreatureScript
{
    public:
        NpcTerenasMenethil() : CreatureScript("npc_terenas_menethil") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcTerenasMenethilAI>(creature);
    }
};

#endif