#ifndef __NPC_COLDFLAME_H
#define __NPC_COLDFLAME_H

#include "LordMarrowgar.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcColdFlameAI : public ScriptedAI
{
public:
    NpcColdFlameAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void IsSummonedBy(Unit* owner);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
};

class NpcColdFlame : public CreatureScript
{
    public:
        NpcColdFlame() : CreatureScript("npc_coldflame") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcColdFlameAI>(creature);
    }
};
#endif