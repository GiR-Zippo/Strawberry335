#ifndef __NPC_SKYBREAKERVINDICATOR_GUNSHIPBATTLE_H_
#define __NPC_SKYBREAKERVINDICATOR_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSkybreakerVindicatorGunshipAI : public ScriptedAI
{
public:
    NpcSkybreakerVindicatorGunshipAI(Creature* creature) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
    }

    void Reset();
    void MoveInLineOfSight(Unit* who);
    void UpdateAI(const uint32 diff);

    private:
        bool _started;
        EventMap events;
        InstanceScript* instance;
};

class NpcSkybreakerVindicatorGunship : public CreatureScript
{
    public:
        NpcSkybreakerVindicatorGunship() : CreatureScript("npc_skybreaker_vindicator") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcSkybreakerVindicatorGunshipAI>(pCreature);
    }
};

#endif