#ifndef __NPC_ORGRIMMSHAMMERDEFENDER_GUNSHIPBATTLE_H_
#define __NPC_ORGRIMMSHAMMERDEFENDER_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcOrgrimmshammerDefenderGunshipAI : public ScriptedAI
{
public:
    NpcOrgrimmshammerDefenderGunshipAI(Creature* creature) : ScriptedAI(creature)
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

class NpcOrgrimmshammerDefenderGunship : public CreatureScript
{
    public:
        NpcOrgrimmshammerDefenderGunship() : CreatureScript("npc_korkron_defender") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcOrgrimmshammerDefenderGunshipAI>(pCreature);
    }
};

#endif