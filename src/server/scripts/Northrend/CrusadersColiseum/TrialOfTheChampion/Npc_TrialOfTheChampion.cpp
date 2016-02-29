/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 01.09.2014
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

#include "Npc_TrialOfTheChampion.h"


typedef Npc_TrialOfTheChampion_Announcer                    AnnouncerScript;
typedef AnnouncerScript::Npc_TrialOfTheChampion_AnnouncerAI AnnouncerAI;

bool AnnouncerScript::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    InstanceScript* pInstance = pCreature->GetInstanceScript();
    if (!pInstance)
        return false;

    uint32 gossipTextId = pPlayer->GetGossipTextId(pCreature);
    uint32 gossipMenuId = Player::GetDefaultGossipMenuForSource(pCreature);

    if (pPlayer->isGameMaster())
    {
        pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT1);
        pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT1_FAST, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT1_FAST);
        pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT2);
        pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT3);
    }
    else if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) != DONE)
    {
        if (AreAllPlayersMounted(pCreature->GetMap()))
        {
            pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT1);
            pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT1_FAST, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT1_FAST);
        }
        else
            gossipTextId = pCreature->GetEntry() == NPC_JAEREN ? GOSSIP_TEXT_NOT_MOUNTED_HORDE : GOSSIP_TEXT_NOT_MOUNTED_ALLIANCE;
    }
    else if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != DONE)
    {
        pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT2);
        gossipTextId = GOSSIP_TEXT_FACTION_CHAMPIONS_DEFEATED;
    }
    else
    {
        pPlayer->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_EVENT3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT3);
        gossipTextId = GOSSIP_TEXT_ARGENT_CHAMPIONS_DEFEATED;
    }

    pPlayer->SEND_GOSSIP_MENU(gossipTextId, pCreature->GetGUID());

    return true;
}

bool AnnouncerScript::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 action)
{
    pPlayer->PlayerTalkClass->ClearMenus();

    // Remove chest (if any) before starting anything else
    InstanceScript* pInstance = pCreature->GetInstanceScript();
    if (pInstance)
        if (GameObject* pObject = sObjectAccessor->GetGameObject(*pCreature, pInstance->GetData64(DATA64_CHEST)))
            pObject->RemoveFromWorld();

    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT1:
        {
            if (pInstance)
                pInstance->SetData(DATA_CURRENT_ENCOUNTER, ENCOUNTER_VALUE_GRAND_CHAMPIONS);

            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }break;

        case GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT1_FAST:
        {
            if (pInstance)
                pInstance->SetData(DATA_CURRENT_ENCOUNTER, ENCOUNTER_VALUE_GRAND_CHAMPIONS_FAST);

            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }break;

        case GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT2:
        {
            if (pInstance)
                pInstance->SetData(DATA_CURRENT_ENCOUNTER, ENCOUNTER_VALUE_ARGENTUM_CHALLENGE);

            if (uint32(pCreature->GetPosition().GetPositionX()) != uint32(AnnouncerPosition[0].GetPositionX()))
                pCreature->SetFacingTo(ORIENTATION_TO_DOOR);

            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }break;

        case GOSSIP_ACTION_INFO_DEF + GOSSIP_OPTIONID_EVENT3:
        {
            if (pInstance)
                pInstance->SetData(DATA_CURRENT_ENCOUNTER, ENCOUNTER_VALUE_BLACK_KNIGHT);

            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }break;
    }

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

void AnnouncerAI::DoAction(int32 action)
{
    switch (action)
    {
        case ANNOUNCER_ACTION_MOVE_HOME:
        {
            if (uint32(me->GetPosition().GetPositionX()) == uint32(AnnouncerPosition[0].GetPositionX()))
                MovementInform(POINT_MOTION_TYPE, ANNOUNCER_HOME_POSITION);
            else
                me->GetMotionMaster()->MovePoint(ANNOUNCER_HOME_POSITION, AnnouncerPosition[0]);
        }break;

        case ANNOUNCER_ACTION_MOVE_CHEST:
        {
            me->GetMotionMaster()->MovePoint(ANNOUNCER_CHEST_POSITION, AnnouncerPosition[1]);
        }break;
    }
}

void AnnouncerAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    switch (id)
    {
        case ANNOUNCER_HOME_POSITION:
        {
            if (pInstance)
                pInstance->DoAction(ENCOUNTER_ACTION_AC_START);

            me->SetFacingTo(AnnouncerPosition[0].GetOrientation());
        }break;

        case ANNOUNCER_CHEST_POSITION:
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }break;
    }
}
