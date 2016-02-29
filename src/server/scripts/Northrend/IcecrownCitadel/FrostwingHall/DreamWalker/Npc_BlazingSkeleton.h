#ifndef __NPC_BLAZINGSKELETON_H_
#define __NPC_BLAZINGSKELETON_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"
#include "Cell.h"
#include "CellImpl.h"

class NpcBlazingSkeletonAI : public ScriptedAI
{
public:
    NpcBlazingSkeletonAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset();
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
};

class NpcBlazingSkeleton : public CreatureScript
{
    public:
        NpcBlazingSkeleton() : CreatureScript("npc_blazing_skeleton") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcBlazingSkeletonAI>(creature);
    }
};

#endif