#ifndef __NPC_ORGRIMMSHAMMERPRIMALIST_GUNSHIPBATTLE_H_
#define __NPC_ORGRIMMSHAMMERPRIMALIST_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcOrgrimmshammerPrimalistGunshipAI : public ScriptedAI
{
public:
    NpcOrgrimmshammerPrimalistGunshipAI(Creature* creature) : ScriptedAI(creature)
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

class NpcOrgrimmshammerPrimalistGunship : public CreatureScript
{
    public:
        NpcOrgrimmshammerPrimalistGunship() : CreatureScript("npc_korkron_primalist") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcOrgrimmshammerPrimalistGunshipAI>(pCreature);
    }
};

#endif