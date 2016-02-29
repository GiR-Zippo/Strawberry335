#ifndef __NPC_VENGEFUL_SHADE_H
#define __NPC_VENGEFUL_SHADE_H

#include "LadyDeathwhisper.h"

class VengefulShadeAI : public ScriptedAI
{
public:
    VengefulShadeAI(Creature* creature) : ScriptedAI(creature) { me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE); }

    void Reset();
    void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell);
    void UpdateAI(const uint32 diff);
private:
    EventMap _events;
};

class VengefulShade : public CreatureScript
{
public:
    VengefulShade() : CreatureScript("npc_vengeful_shade") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<VengefulShadeAI>(creature);
    }
};

#endif