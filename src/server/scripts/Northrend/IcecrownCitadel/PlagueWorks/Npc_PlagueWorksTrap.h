#ifndef __NPC_PLAGUEWORKS_TRAP_H
#define __NPC_PLAGUEWORKS_TRAP_H

#include "PlagueWorks.h"
#include "Putricide/ProfessorPutricide.h"

class PlagueWorksTrapAI : public ScriptedAI
{
public:
    PlagueWorksTrapAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()), running(false) {}

    void Reset();

    void JustDied(Unit* /*killer*/);

    void DoAction(const int32 action);

    void UpdateAI(const uint32 diff);

private:
    bool running;
    EventMap events;
    InstanceScript * instance;
};

class PlagueWorksTrap : public CreatureScript
{
public:
    PlagueWorksTrap() : CreatureScript("npc_putricide_trap") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new PlagueWorksTrapAI(creature);
    }
};

#endif