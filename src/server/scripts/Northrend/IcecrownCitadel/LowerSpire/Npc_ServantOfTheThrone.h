#include "LowerSpire.h"
#include "ScriptedCreature.h"

enum NpcServantEvents
{
    EVENT_CAST_GLACIAL_BLAST    = 1,
};

enum NpcServantSpells
{
    SPELL_GLACIAL_BLAST         = 71029,
};

class NpcServantOfTheThroneAI : public ScriptedAI
{
public:
    NpcServantOfTheThroneAI(Creature* creature) : ScriptedAI(creature) {}
    
    void Reset();
    void EnterCombat(Unit* /*victim*/);
    void UpdateAI(const uint32 diff);
private:
    EventMap events;
};

class NpcServantOfTheThrone : public CreatureScript
{
public:
    NpcServantOfTheThrone() : CreatureScript("npc_servant_of_the_throne") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcServantOfTheThroneAI>(creature);
    }
};