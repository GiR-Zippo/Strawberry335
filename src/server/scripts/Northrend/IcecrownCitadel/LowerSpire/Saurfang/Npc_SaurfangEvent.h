#ifndef __NPC_SAURFANG_EVENT_H
#define __NPC_SAURFANG_EVENT_H

#include "DeathbringerSaurfang.h"

class SaurfangEventAI : public ScriptedAI
{
public:
    SaurfangEventAI(Creature* creature) : ScriptedAI(creature), _index(0) {}

    void SetData(uint32 type, uint32 data);
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void DoAction(int32 const action);

private:

    uint32 _index;
};

class SaurfangEvent : public CreatureScript
{
public:
    SaurfangEvent() : CreatureScript("npc_saurfang_event") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<SaurfangEventAI>(creature);
    }
};

#endif
