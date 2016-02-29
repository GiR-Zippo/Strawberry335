#ifndef __NPC_SHAMBLINGHORROR_H_
#define __NPC_SHAMBLINGHORROR_H_

#include "TheLichKing.h"

class NpcShamblingHorrorAI : public ScriptedAI
{
public:
    NpcShamblingHorrorAI(Creature* creature) : ScriptedAI(creature)
    {
        _frenzied = false;
    }

    void Reset();
    void DamageTaken(Unit* /*attacker*/, uint32& damage);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    bool _frenzied;
};

class NpcShamblingHorror : public CreatureScript
{
    public:
        NpcShamblingHorror() :  CreatureScript("npc_shambling_horror_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcShamblingHorrorAI>(creature);
    }
};

#endif