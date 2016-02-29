#ifndef __NPC_HIGH_OVERLORD_SAURFANG_ICC_H
#define __NPC_HIGH_OVERLORD_SAURFANG_ICC_H

#include "DeathbringerSaurfang.h"

class HighOverlordSaurfangAI : public ScriptedAI
{

public:
    HighOverlordSaurfangAI(Creature* creature) : ScriptedAI(creature) {ASSERT(creature->GetVehicleKit()); _instance = me->GetInstanceScript();}

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

class HighOverlordSaurfang : public CreatureScript
{

public:
    HighOverlordSaurfang() : CreatureScript("npc_high_overlord_saurfang_icc") { }

    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action);

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<HighOverlordSaurfangAI>(creature);
    }

};

#endif