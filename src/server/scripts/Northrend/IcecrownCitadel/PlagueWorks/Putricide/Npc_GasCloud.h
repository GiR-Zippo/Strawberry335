#ifndef __NPC_GAS_CLOUD
#define __NPC_GAS_CLOUD

#include "ProfessorPutricide.h"

class GasCloudAI : public ScriptedAI
{
public:
    GasCloudAI(Creature* creature) : ScriptedAI(creature), _newTargetSelectTimer(0) {}

    void Reset();

    void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell);

    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);

    void EnterEvadeMode() {};

    void UpdateAI(uint32 const diff);

private:
    uint32 _newTargetSelectTimer;
};

class GasCloud : public CreatureScript
{
public:
    GasCloud() : CreatureScript("npc_gas_cloud") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<GasCloudAI>(creature);
    }
};

#endif
