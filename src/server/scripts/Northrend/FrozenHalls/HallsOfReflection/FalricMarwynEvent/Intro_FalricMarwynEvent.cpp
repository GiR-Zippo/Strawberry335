/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 25.09.2014
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

#include "Intro_FalricMarwynEvent.h"

using namespace INTRO_FALRIC_MARWYN_EVENT;


typedef Npc_JainaOrSylvanasHor_FirstPart    JainaOrSylvanasFirst;
typedef Npc_JainaOrSylvanasHor_FirstPart::Npc_JainaOrSylvanasHorAI_FirstPart    JainaOrSylvanasAIFirst;

bool JainaOrSylvanasFirst::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    uint32 menuId = Player::GetDefaultGossipMenuForSource(pCreature);
    uint32 textId = pPlayer->GetGossipTextId(pCreature);

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    QuestStatus status[4] =
    {	
		pPlayer->GetQuestStatus(QUEST_DELIVRANCE_FROM_THE_PIT_H2),
		pPlayer->GetQuestStatus(QUEST_WRATH_OF_THE_LICH_KING_H2),
		pPlayer->GetQuestStatus(QUEST_DELIVRANCE_FROM_THE_PIT_A2),
		pPlayer->GetQuestStatus(QUEST_WRATH_OF_THE_LICH_KING_A2)
    };

	if (status[0] == QUEST_STATUS_COMPLETE || status[0] == QUEST_STATUS_REWARDED || status[2] == QUEST_STATUS_COMPLETE || status[2] == QUEST_STATUS_REWARDED || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_DB(menuId, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

	if (status[1] == QUEST_STATUS_COMPLETE || status[1] == QUEST_STATUS_REWARDED || status[3] == QUEST_STATUS_COMPLETE || status[3] == QUEST_STATUS_REWARDED || pPlayer->isGameMaster())
        pPlayer->ADD_GOSSIP_ITEM_DB(menuId, 1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

    pPlayer->SEND_GOSSIP_MENU(textId, pCreature->GetGUID());
    return true;
}

bool JainaOrSylvanasFirst::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*sender*/, uint32 action)
{
    pPlayer->PlayerTalkClass->ClearMenus();
    pPlayer->CLOSE_GOSSIP_MENU();
    uint32 encounterAction = NULL;

    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            encounterAction = ENCOUNTER_VALUE_F_M_E;
            break;

        case GOSSIP_ACTION_INFO_DEF + 2:
            encounterAction = ENCOUNTER_VALUE_F_M_E_FAST;
            break;
    }

    if (encounterAction)
    {
        if (InstanceScript* pInstance = pCreature->GetInstanceScript())
            pInstance->SetData(DATA_CURRENT_ENCOUNTER, encounterAction);
       pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    return true;
}

void JainaOrSylvanasAIFirst::Reset()
{
    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    me->SetStandState(UNIT_STAND_STATE_STAND);
    me->SetVisible(true);
}
