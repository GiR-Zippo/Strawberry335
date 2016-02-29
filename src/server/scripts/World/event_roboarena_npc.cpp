#include "ScriptPCH.h"
#include "event_roboarena.h"


class npc_roboArena : public CreatureScript
{
public:
    npc_roboArena() : CreatureScript("npc_roboArena") { }

    bool OnGossipHello(Player *pPlayer, Creature *pCreature)
    {
    for (PlayerQueue::iterator itr = m_queuedPlayers.begin(); itr != m_queuedPlayers.end(); itr++)
        {
                    if (itr->second->GetGUID() == pPlayer->GetGUID())
                         {
                            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Willst du deine aktuelle Wertung sehen?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Willst du dich aus der Warteschlange entfernen?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                         }
                    else
                        {
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Melde dich jetzt für einen 1on1 Kampf an!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Willst du deine aktuelle Wertung sehen?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                        }
         }   
            
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();     
        pPlayer->CLOSE_GOSSIP_MENU();

        switch (uiAction)
        {
        case 1001:
            m_queuedPlayers[m_queuedPlayers.size() + 1] = pPlayer;
            break;
        case 1002:
            char msg[128];
            sprintf(msg, "Aktuell %i Player in der Warteschlange", m_queuedPlayers.size());
            break;
        case 1003:
            break;
        }
        return true;
    }
};

void AddSC_npc_roboArena()
{
    new npc_roboArena;
}
