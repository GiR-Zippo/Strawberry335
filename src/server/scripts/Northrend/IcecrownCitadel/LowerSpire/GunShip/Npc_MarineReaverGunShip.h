#ifndef __NPC_MARINEREAVER_GUNSHIPBATTLE_H_
#define __NPC_MARINEREAVER_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcMarineReaverGunshipAI : public ScriptedAI
{
public:
    NpcMarineReaverGunshipAI(Creature *creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
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

class NpcMarineReaverGunship : public CreatureScript
{
    public:
        NpcMarineReaverGunship() : CreatureScript("npc_marine_or_reaver") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcMarineReaverGunshipAI>(pCreature);
    }
};

#endif