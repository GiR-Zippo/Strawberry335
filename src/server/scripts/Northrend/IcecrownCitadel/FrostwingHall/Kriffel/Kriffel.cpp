#include "Kriffel.h"

class npc_spawn_ersatzboss : public CreatureScript
{
public:
    npc_spawn_ersatzboss() : CreatureScript("npc_spawn_ersatzboss") {}

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*Sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->SendCloseGossip();
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (Creature* gunship = creature->SummonCreature(NPC_BOSS, midPoint))
            {
                gunship->GetAI()->DoAction(ACTION_INTRO);
                creature->DespawnOrUnsummon();
            }
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        if (InstanceScript* instance = player->GetInstanceScript())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_LEADER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};

void LoadNpcKriffelOrb();
void LoadNpcKriffelFrostbomb();
void LoadNpcKriffelEnergy();
void LoadBossKriffel();

void LoadKriffel()
{
    new npc_spawn_ersatzboss();

    LoadNpcKriffelOrb();
    LoadNpcKriffelFrostbomb();
    LoadNpcKriffelEnergy();
    LoadBossKriffel();
}
