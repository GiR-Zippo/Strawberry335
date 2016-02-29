#include "LowerSpire.h"
#include "ScriptedCreature.h"

enum NpcAncientEvents
{
    EVENT_CAST_SHIELD_BASH      = 1,
};

enum NpcAncientSpells
{
    SPELL_SHIELD_BASH           = 70964,
};

class NpcAncientSkeletonAI : public ScriptedAI
{
public:
    NpcAncientSkeletonAI(Creature* creature) : ScriptedAI(creature) {}
    
    void Reset();
    void EnterCombat(Unit* /*victim*/);
    void UpdateAI(const uint32 diff);
private:
    EventMap events;
};

class NpcAncientSkeleton : public CreatureScript
{
public:
    NpcAncientSkeleton() : CreatureScript("npc_ancint_skeleton") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcAncientSkeletonAI>(creature);
    }
};