#ifndef __NPC_DARNAVAN_H
#define __NPC_DARNAVAN_H

#include "LadyDeathwhisper.h"

class DarnavanMoveEvent : public BasicEvent
{
public:
    DarnavanMoveEvent(Creature& darnavan) : _darnavan(darnavan) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/);

private:

    Creature& _darnavan;
};

class DarnavanAI : public ScriptedAI
{
public:
    DarnavanAI(Creature* creature) : ScriptedAI(creature) {}

    void Reset();
    void JustDied(Unit* killer);
    void MovementInform(uint32 type, uint32 id);
    void EnterCombat(Unit* /*victim*/);
    void UpdateAI(const uint32 diff);

private:

    EventMap _events;
    bool _canCharge;
    bool _canShatter;

};

class Darnavan : public CreatureScript
{
public:
    Darnavan() : CreatureScript("npc_darnavan") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<DarnavanAI>(creature);
    }
};

#endif