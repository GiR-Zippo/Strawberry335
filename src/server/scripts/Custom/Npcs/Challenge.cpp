#include "Challenge.h"
#include "GroupMgr.h"

enum Options
{
    OPTION_JOIN_GRP = 1
};

bool NpcChallenge::OnGossipHello(Player* player, Creature* creature)
{
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Schließ dich meiner Gruppe an", creature->GetEntry(), OPTION_JOIN_GRP);
    player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
    return true;
}

bool NpcChallenge::OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    player->PlayerTalkClass->ClearMenus();

    switch (action)
    {
        case OPTION_JOIN_GRP:
        {
            Group* gr;

            if (gr = player->GetGroup())
            {
                if (gr->IsMember(creature->GetGUID()))
                    break;
                if (gr->IsFull())
                    gr->ConvertToRaid();
            }
            else
            {
                gr = new Group();
                // gr->RemoveInvite(player);
                if (!gr->Create(player))
                {
                    delete gr;
                    return false;
                }
                sGroupMgr->AddGroup(gr);
            }

            if (gr->AddMember((Player*)creature))
            {
                gr->BroadcastGroupUpdate();
            }

            break;
        }
        default:
            return false;
    }

    player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
    return true;
}

