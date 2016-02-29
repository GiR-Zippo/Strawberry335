/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 12.11.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#include "Npc_Customize.h"


struct CustomizeActions
{
    uint32 Amount;      // Costs in gold
    uint32 NpcTextId;   // From db

    // Used to convert int to string
    std::string GetAmountString()
    {
        if (!Amount)
            return "For Free!";
        else
        {
            std::ostringstream oss;
            oss << this->Amount << " Gold";
            return oss.str();
        }
    }
};

CustomizeActions customizeActions[GOSSIP_ACTION_MAX]
{
    // Amount   NpcTextId
    { 0,        60005   },
    { 15000,    60006   },
    { 5000,     60007   },
    { 1000,     60008   }
};


bool Npc_Customize::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    uint32 menuId = Player::GetDefaultGossipMenuForSource(pCreature);
    uint32 textId = pPlayer->GetGossipTextId(pCreature);

    if (sWorld->getIntConfig(CONFIG_FACTION_TRANSFER_FACTIONS_ENABLED) & (pPlayer->GetTeamId() + 1))
        pPlayer->ADD_GOSSIP_ITEM_DB(menuId, GOSSIP_ACTION_CHANGE_FACTION, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->ADD_GOSSIP_ITEM_DB(menuId, GOSSIP_ACTION_CHANGE_RACE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    pPlayer->ADD_GOSSIP_ITEM_DB(menuId, GOSSIP_ACTION_CHANGE_CHAR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    pPlayer->ADD_GOSSIP_ITEM_DB(menuId, GOSSIP_ACTION_CHANGE_NAME, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

    pPlayer->SEND_GOSSIP_MENU(textId, pCreature->GetGUID());
    return true;
}

bool Npc_Customize::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*sender*/, uint32 action)
{
    pPlayer->PlayerTalkClass->ClearMenus();

    if (action == GOSSIP_ACTION_INFO_DEF - 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        return true;
    }

    action -= GOSSIP_ACTION_INFO_DEF;

    uint32 menuId = Player::GetDefaultGossipMenuForSource(pCreature);
    uint32 textId = 0;

    if (action < GOSSIP_ACTION_MAX)
    {
        pPlayer->ADD_GOSSIP_ITEM_DB(menuId, GOSSIP_ACTION_MAX, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF - 1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, customizeActions[action].GetAmountString().c_str(), GOSSIP_SENDER_MAIN, (GOSSIP_ACTION_INFO_DEF + GOSSIP_ACTION_INFO_DEF + action));
    }
    else if (action >= GOSSIP_ACTION_INFO_DEF && action < (GOSSIP_ACTION_INFO_DEF + GOSSIP_ACTION_MAX))
        textId = proceedAction(pPlayer, action);
    else
        textId = NPC_TEXT_ERROR;

    pPlayer->SEND_GOSSIP_MENU(textId ? textId : customizeActions[action].NpcTextId, pCreature->GetGUID());
    return true;
};

bool Npc_Customize::checkAndTakeCurrency(Player* pPlayer, uint32 actionId)
{
    if (pPlayer->GetMoney() < (customizeActions[actionId].Amount * GOLD))
        return false;

    pPlayer->SetMoney(pPlayer->GetMoney() - (customizeActions[actionId].Amount * GOLD));
    return true;
}

uint32 Npc_Customize::proceedAction(Player* pPlayer, uint32 actionId)
{
    // Prefix
    actionId -= GOSSIP_ACTION_INFO_DEF;

    // Take money
    if (!checkAndTakeCurrency(pPlayer, actionId))
        return NPC_TEXT_NOT_ENOUGH_GOLD;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);

    switch (actionId)
    {
        case GOSSIP_ACTION_CHANGE_FACTION:
        {
            stmt->setUInt16(0, uint16(AT_LOGIN_CHANGE_FACTION));
            pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
        }break;

        case GOSSIP_ACTION_CHANGE_RACE:
        {
            stmt->setUInt16(0, uint16(AT_LOGIN_CHANGE_RACE));
            pPlayer->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
        }break;

        case GOSSIP_ACTION_CHANGE_CHAR:
        {
            stmt->setUInt16(0, uint16(AT_LOGIN_CUSTOMIZE));
            pPlayer->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
        }break;

        case GOSSIP_ACTION_CHANGE_NAME:
        {
            stmt->setUInt16(0, uint16(AT_LOGIN_RENAME));
            pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
        }break;

        default:
            return NPC_TEXT_ERROR;
    }

    stmt->setUInt32(1, pPlayer->GetGUIDLow());
    CharacterDatabase.Execute(stmt);

    pPlayer->GetSession()->LogoutRequest(time(NULL) + 1);
    return NPC_TEXT_SUCCESS;
}