#ifndef __NPC_BONESPIKE_H
#define __NPC_BONESPIKE_H

#include "LordMarrowgar.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcBoneSpikeAI : public Scripted_NoMovementAI
{
public:
    NpcBoneSpikeAI(Creature* creature) : Scripted_NoMovementAI(creature), _hasTrappedUnit(false)
    {
        ASSERT(creature->GetVehicleKit());
    }

    void JustDied(Unit* /*killer*/);
    void KilledUnit(Unit* victim);
    void IsSummonedBy(Unit* summoner);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    bool _hasTrappedUnit;
};

class NpcBoneSpike : public CreatureScript
{
    public:
        NpcBoneSpike() : CreatureScript("npc_bone_spike") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcBoneSpikeAI>(creature);
    }
};
#endif