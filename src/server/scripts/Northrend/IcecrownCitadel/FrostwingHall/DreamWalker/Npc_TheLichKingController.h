#ifndef __NPC_THELICHKINGCONTROLLER_H_
#define __NPC_THELICHKINGCONTROLLER_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcTheLichKingControllerAI : public ScriptedAI
{
public:
    NpcTheLichKingControllerAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript()), _summons(creature)
    {
    }

    void Reset();
    void EnterEvadeMode();
    void JustReachedHome();
    void EnterCombat(Unit* /*target*/);
    void JustSummoned(Creature* summon);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    SummonList _summons;
    InstanceScript* _instance;
};

class NpcTheLichKingController : public CreatureScript
{
    public:
        NpcTheLichKingController() : CreatureScript("npc_the_lich_king_controller") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcTheLichKingControllerAI>(creature);
    }
};

#endif