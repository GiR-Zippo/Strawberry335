#ifndef __NPC_FROSTFREEZETRAP_H_
#define __NPC_FROSTFREEZETRAP_H_

#include "UpperSpire.h"
#include "ScriptedCreature.h"

class Npc_FrostFreezeTrapAI: public Scripted_NoMovementAI
{
public:
    Npc_FrostFreezeTrapAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

    void DoAction(int32 const action);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
};

class Npc_FrostFreezeTrap : public CreatureScript
{
public:
    Npc_FrostFreezeTrap() : CreatureScript("npc_frost_freeze_trap") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<Npc_FrostFreezeTrapAI>(creature);
    }
};

#endif