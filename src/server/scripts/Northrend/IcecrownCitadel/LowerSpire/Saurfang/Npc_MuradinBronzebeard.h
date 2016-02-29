#ifndef __NPC_MURADIN_BRONZEBEARD_ICC_H
#define __NPC_MURADIN_BRONZEBEARD_ICC_H

#include "DeathbringerSaurfang.h"

class MuradinBronzebeardAI : public ScriptedAI
{
public:
    MuradinBronzebeardAI(Creature* creature) : ScriptedAI(creature), _instance(me->GetInstanceScript()) {}

    void Reset() { _events.Reset(); }
    void DoAction(int32 const action);
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void MovementInform(uint32 type, uint32 id);
    void UpdateAI(uint32 const diff);

private:

    EventMap _events;
    InstanceScript* _instance;
    std::list<Creature*> _guardList;
};

class MuradinBronzebeard : public CreatureScript
{
public:
    MuradinBronzebeard() : CreatureScript("npc_muradin_bronzebeard_icc") { }

    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action);

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<MuradinBronzebeardAI>(creature);
    }
};

#endif