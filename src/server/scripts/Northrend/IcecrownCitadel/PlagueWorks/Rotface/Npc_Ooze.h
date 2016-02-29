#ifndef __NPC_OOZE_H
#define __NPC_OOZE_H

#include "Rotface.h"

class LittleOozeAI : public ScriptedAI
{
public:
    LittleOozeAI(Creature* creature) : ScriptedAI(creature), instance(me->GetInstanceScript()) {}

    void IsSummonedBy(Unit* summoner);
    void JustDied(Unit* /*killer*/);
    void UpdateAI(const uint32 diff);

private:

    InstanceScript* instance;
    EventMap events;

};

class BigOozeAI : public ScriptedAI
{
public:
    BigOozeAI(Creature* creature) : ScriptedAI(creature), instance(me->GetInstanceScript()) {}

    void IsSummonedBy(Unit* summoner);
    void JustDied(Unit* /*killer*/);
    void DoAction(const int32 action);
    void UpdateAI(const uint32 diff);

private:

    InstanceScript* instance;
    EventMap events;

};



class LittleOoze : public CreatureScript
{
public:
    LittleOoze() : CreatureScript("npc_little_ooze") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<LittleOozeAI>(creature);
    }
};

class BigOoze : public CreatureScript
{
public:
    BigOoze() : CreatureScript("npc_big_ooze") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BigOozeAI>(creature);
    }
};

#endif