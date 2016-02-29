#ifndef __NPC_GREENDRAGONCOMBATTRIGGER_H_
#define __NPC_GREENDRAGONCOMBATTRIGGER_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcGreenDragonCombatTriggerAI : public BossAI
{
public:
    NpcGreenDragonCombatTriggerAI(Creature* creature) : BossAI(creature, DATA_VALITHRIA_DREAMWALKER)
    {
    }

    void Reset();
    void EnterCombat(Unit* target);
    void AttackStart(Unit* target);
    bool CanAIAttack(Unit const* target) const;
    void JustReachedHome();
    void DoAction(int32 const action);
    void UpdateAI(uint32 const /*diff*/);

private:
    bool _evadeCheck;
};

class NpcGreenDragonCombatTrigger : public CreatureScript
{
    public:
        NpcGreenDragonCombatTrigger() : CreatureScript("npc_green_dragon_combat_trigger") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcGreenDragonCombatTriggerAI>(creature);
    }
};

#endif