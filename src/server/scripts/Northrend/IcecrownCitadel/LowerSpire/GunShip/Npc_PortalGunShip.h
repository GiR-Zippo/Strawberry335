#ifndef __NPC_PORTAL_GUNSHIPBATTLE_H_
#define __NPC_PORTAL_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcPortalGunshipAI : public Scripted_NoMovementAI
{
public:
    NpcPortalGunshipAI(Creature *creature) : Scripted_NoMovementAI(creature),_instance(creature->GetInstanceScript())
    {
        Reset();
    }

    void Reset();
    void JustDied(Unit* killer);
    void UpdateAI( const uint32 diff);

private:
    EventMap events;
    InstanceScript* _instance;

};

class NpcPortalGunship : public CreatureScript
{
    public:
        NpcPortalGunship() : CreatureScript("npc_gunship_portal") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcPortalGunshipAI>(pCreature);
    }
};

#endif