#ifndef __SPIREFROSTWYRM_H_
#define __SPIREFROSTWYRM_H_

#include "LowerSpire.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSpireFrostwyrmAI : public ScriptedAI
{
public:
    NpcSpireFrostwyrmAI(Creature* creature) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
    }

    void Reset();
    void MovementInform(uint32 type, uint32 id);
    void JustDied(Unit* killer);
    void UpdateAI(const uint32 diff);

    private:
        EventMap events;
        InstanceScript* instance;
};

class NpcSpireFrostwyrm : public CreatureScript
{
    public:
        NpcSpireFrostwyrm() : CreatureScript("npc_icc_spire_frostwyrm") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcSpireFrostwyrmAI>(pCreature);
    }
};

class AtSpireFrostwyrm : public AreaTriggerScript
{
    public:
        AtSpireFrostwyrm() : AreaTriggerScript("at_icc_land_frostwyrm") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* areaTrigger);
};

#endif