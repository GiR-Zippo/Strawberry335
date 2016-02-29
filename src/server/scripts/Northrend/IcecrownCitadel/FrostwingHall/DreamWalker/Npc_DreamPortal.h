#ifndef __NPC_DREAMPORTAL_H_
#define __NPC_DREAMPORTAL_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcDreamPortalAI : public CreatureAI
{
public:
    NpcDreamPortalAI(Creature* creature) : CreatureAI(creature),
        _used(false)
    {
    }

    void OnSpellClick(Unit* /*clicker*/);
    uint32 GetData(uint32 type);
    void UpdateAI(uint32 const /*diff*/);

private:
    bool _used;
};

class NpcDreamPortal : public CreatureScript
{
    public:
        NpcDreamPortal() : CreatureScript("npc_dream_portal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcDreamPortalAI>(creature);
    }
};

#endif