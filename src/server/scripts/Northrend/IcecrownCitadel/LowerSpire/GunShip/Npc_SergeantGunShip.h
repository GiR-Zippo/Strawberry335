#ifndef __NPC_SERGEANT_GUNSHIPBATTLE_H_
#define __NPC_SERGEANT_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSergeantGunshipAI : public ScriptedAI
{
public:
    NpcSergeantGunshipAI(Creature *creature) : ScriptedAI(creature),_instance(creature->GetInstanceScript())
    {
        Reset();
    }

    void Reset();
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);
    bool CanAIAttack(Unit const* target) const;
    void UpdateAI(const uint32 diff);

    uint32 DesperateResolve;
    bool desperated;

    private:
        EventMap events;
        InstanceScript* _instance;
};

class NpcSergeantGunship : public CreatureScript
{
    public:
        NpcSergeantGunship() : CreatureScript("npc_sergeant") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcSergeantGunshipAI>(pCreature);
    }
};

#endif