#ifndef __NPC_CULT_FANATIC_H
#define __NPC_CULT_FANATIC_H

#include "LadyDeathwhisper.h"

class CultFanaticAI : public ScriptedAI
{
public:
    CultFanaticAI(Creature* creature) : ScriptedAI(creature) {}

    void Reset();
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void UpdateAI(uint32 const diff);

protected:

    EventMap Events;
};

class CultFanatic : public CreatureScript
{
public:
    CultFanatic() : CreatureScript("npc_cult_fanatic") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<CultFanaticAI>(creature);
    }
};

#endif