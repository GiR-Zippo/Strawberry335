#ifndef __NPC_GLUTTONOUSABOMINATION_H_
#define __NPC_GLUTTONOUSABOMINATION_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcGluttonousAbominationAI : public ScriptedAI
{
public:
    NpcGluttonousAbominationAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset();
    void JustDied(Unit* /*killer*/);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
};

class NpcGluttonousAbomination : public CreatureScript
{
    public:
        NpcGluttonousAbomination() : CreatureScript("npc_gluttonous_abomination") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcGluttonousAbominationAI>(creature);
    }
};

#endif