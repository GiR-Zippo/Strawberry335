#ifndef __NPC_RAGINGSPIRIT_H_
#define __NPC_RAGINGSPIRIT_H_

#include "TheLichKing.h"


enum RagingSpiritPhases
{
    RAGINGSPIRIT_PHASE_FROSTMOURNE = 1,
};

class NpcRagingSpiritAI : public ScriptedAI
{
public:
    NpcRagingSpiritAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    void Reset() override;
    void EnterCombat(Unit *) override;
    void IsSummonedBy(Unit* /*summoner*/) override;
    void JustDied(Unit* /*killer*/) override;
    void DoAction(const int32 action) override;
    void UpdateAI(uint32 const diff) override;

private:
    EventMap events;
    InstanceScript* _instance;
};

class NpcRagingSpirit : public CreatureScript
{
    public:
        NpcRagingSpirit() : CreatureScript("npc_raging_spirit") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcRagingSpiritAI>(creature);
    }
};

#endif