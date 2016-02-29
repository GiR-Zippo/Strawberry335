#include "BloodPrinceCouncil.h"

class NpcShockvortexAI : public ScriptedAI
{
    public:
        NpcShockvortexAI(Creature* creature) : ScriptedAI(creature)
        { }

        void Reset();
        void EnterCombat(Unit* who);
        void UpdateAI(uint32 const diff);

    private:
        EventMap _events;
};

class NpcShockvortex : public CreatureScript
{
    public:
        NpcShockvortex() : CreatureScript("npc_shock_vortex") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcShockvortexAI>(creature);
    }
};