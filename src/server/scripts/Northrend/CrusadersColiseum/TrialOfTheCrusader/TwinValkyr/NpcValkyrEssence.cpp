#include "TwinValkyr.h"

#define ESSENCE_REMOVE 0
#define ESSENCE_APPLY 1

class NpcValkyrEssenceAI : public ScriptedAI
{
    public:
        NpcValkyrEssenceAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 GetData(uint32 data)
        {
            switch (me->GetEntry())
            {
                case NPC_LIGHT_ESSENCE:
                    return (data == ESSENCE_REMOVE) ? SPELL_DARK_ESSENCE_HELPER : SPELL_LIGHT_ESSENCE_HELPER;
                case NPC_DARK_ESSENCE:
                    return (data == ESSENCE_REMOVE) ? SPELL_LIGHT_ESSENCE_HELPER : SPELL_DARK_ESSENCE_HELPER;
            }

            return 0;
        }
};

class NpcValkyrEssence : public CreatureScript
{
    public:
        NpcValkyrEssence() : CreatureScript("mob_essence_of_twin") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new NpcValkyrEssenceAI(creature);
        };

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->RemoveAurasDueToSpell(creature->GetAI()->GetData(ESSENCE_REMOVE));
            player->CastSpell(player, creature->GetAI()->GetData(ESSENCE_APPLY), true);
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
};

void AddNpcValkyrEssence()
{
    new NpcValkyrEssence();
}
