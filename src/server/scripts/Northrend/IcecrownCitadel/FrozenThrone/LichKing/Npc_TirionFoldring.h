#ifndef __NPC_TIRIONFOLDRING_H_
#define __NPC_TIRIONFOLDRING_H_

#include "TheLichKing.h"

class NpcTirionFordringAI : public ScriptedAI
{
public:
    NpcTirionFordringAI(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    void Reset();
    void MovementInform(uint32 type, uint32 id);
    void DoAction(int32 const action);
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void sGossipSelect(Player* /*player*/, uint32 sender, uint32 action);
    void JustReachedHome();
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
};

class NpcTirionFordring : public CreatureScript
{
    public:
        NpcTirionFordring() : CreatureScript("npc_tirion_fordring_tft") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcTirionFordringAI>(creature);
    }
};

#endif