#ifndef __NPC_AXETHROWERRIFLEMAN_GUNSHIPBATTLE_H_
#define __NPC_AXETHROWERRIFLEMAN_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcAxethrowerRiflemanGunshipAI : public Scripted_NoMovementAI
{
public:
    NpcAxethrowerRiflemanGunshipAI(Creature *creature) : Scripted_NoMovementAI(creature),_instance(creature->GetInstanceScript())
    {
        Reset();
    }

    void Reset();
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);
    void JustDied(Unit* killer);
    void UpdateAI(const uint32 diff);

    uint32 DesperateResolve;
    uint32 attacktimer;

    private:
        EventMap events;
        bool desperated;
        InstanceScript* _instance;
};

class NpcAxethrowerRiflemanGunship : public CreatureScript
{
    public:
        NpcAxethrowerRiflemanGunship() : CreatureScript("npc_korkron_axethrower_rifleman") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcAxethrowerRiflemanGunshipAI>(pCreature);
    }
};

#endif