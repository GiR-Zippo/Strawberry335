#ifndef __NPC_VOLATILE_OOZE
#define __NPC_GAS_CLOUD

#include "ProfessorPutricide.h"

class VolatileOozeAI : public ScriptedAI
{
    public:

        VolatileOozeAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override;
        void EnterCombat(Unit* /*attacker*/) override;
        void EnterEvadeMode() override {};

        void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) override;
        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override;

        void UpdateAI(uint32 const diff) override;

    private:

        void executeEvent(uint32 eventID);

        EventMap eventMap;
};


class VolatileOoze : public CreatureScript
{
public:
    VolatileOoze() : CreatureScript("npc_volatile_ooze") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<VolatileOozeAI>(creature);
    }
};

#endif