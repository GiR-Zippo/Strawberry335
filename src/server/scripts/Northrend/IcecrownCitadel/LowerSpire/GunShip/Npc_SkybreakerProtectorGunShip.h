#ifndef __NPC_SKYBREAKERPROTECTOR_GUNSHIPBATTLE_H_
#define __NPC_SKYBREAKERPROTECTOR_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSkybreakerProtectorGunshipAI : public ScriptedAI
{
public:
    NpcSkybreakerProtectorGunshipAI(Creature* creature) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
    }

    void Reset();
    void MoveInLineOfSight(Unit* who);
    void UpdateAI(const uint32 diff);

    private:
        EventMap events;
        InstanceScript* instance;
};

class NpcSkybreakerProtectorGunship : public CreatureScript
{
    public:
        NpcSkybreakerProtectorGunship() : CreatureScript("npc_skybreaker_protector") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcSkybreakerProtectorGunshipAI>(pCreature);
    }
};

#endif