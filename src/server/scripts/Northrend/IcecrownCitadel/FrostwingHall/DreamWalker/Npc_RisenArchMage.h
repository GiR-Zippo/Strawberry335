#ifndef __NPC_RISENARCHMAGE_H_
#define __NPC_RISENARCHMAGE_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

class NpcRisenArchmageAI : public ScriptedAI
{
public:
    NpcRisenArchmageAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    bool CanAIAttack(Unit const* target) const;
    void Reset();
    void EnterCombat(Unit* /*target*/);
    void DoAction(int32 const action);
    void JustSummoned(Creature* summon);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
    bool _canCallEnterCombat;
};

class NpcRisenArchmage : public CreatureScript
{
    public:
        NpcRisenArchmage() : CreatureScript("npc_risen_archmage") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcRisenArchmageAI>(creature);
    }
};

#endif