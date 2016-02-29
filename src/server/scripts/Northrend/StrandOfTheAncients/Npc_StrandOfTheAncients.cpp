/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 29.07.2014
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

#include "Npc_StrandOfTheAncients.h"
#include "BattlegroundMgr.h"


bool NpcSotADemolisherEngineer::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    uint32 menuId = pPlayer->GetDefaultGossipMenuForSource(pCreature);
    uint32 textId = pPlayer->GetGossipTextId(pCreature);

    if (CanBuild(pCreature))
        pPlayer->ADD_GOSSIP_ITEM_DB(menuId, BG_SA_GOSSIP_ENGINEER_BUILD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    else
    {
        if (Battleground* sota = pPlayer->GetBattleground())
        {
            if (uint32 secondsToActivate = sota->GetData(BG_SA_DATA_DEMOLISHER_TIMER))
            {
                if (secondsToActivate != 0)
                {
                    // Built string to inform player about demolisher respawn state (in seconds)
                    std::ostringstream messageOSring;
                    if (pPlayer->GetSession()->GetSessionDbcLocale() == LOCALE_deDE)
                        messageOSring << "Ich bekomme in " << secondsToActivate << (secondsToActivate == 1 ? " Sekunde " : " Sekunden ") << "Nachschub!";
                    else
                        messageOSring << "I get in " << secondsToActivate << (secondsToActivate == 1 ? " second " : " seconds ") << "replenishment!";

                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, messageOSring.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF - 1);
                }
            }
        }

        textId = BG_SA_GOSSIP_ENGINEER_CANNOT_BUILD;
    }

    pPlayer->SEND_GOSSIP_MENU(textId, pCreature->GetGUID());
    return true;
}

bool NpcSotADemolisherEngineer::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*sender*/, uint32 action)
{
    pPlayer->CLOSE_GOSSIP_MENU();

    if (CanBuild(pCreature))
    {
        switch (action - GOSSIP_ACTION_INFO_DEF)
        {
            case 0:
            {
                if (Battleground* sota = pPlayer->GetBattleground())
                {
                    if (pPlayer->GetTeamId() == TEAM_ALLIANCE)
                        sota->DoAction(BG_SA_ACTION_BUILD_DEMOLISHER_ALLIANCE, pCreature->GetGUID());
                    else
                        sota->DoAction(BG_SA_ACTION_BUILD_DEMOLISHER_HORDE, pCreature->GetGUID());
                }
                else
                {
                    pPlayer->GetSession()->SendNotification("An Error has occurred, please try it again");
                    return true;
                }
            }break;

            default:
                break;
        }
    }
    return true;
}

bool NpcSotADemolisherEngineer::CanBuild(Creature* pCreature)
{
    Battleground* sota = sBattlegroundMgr->GetBattleground(pCreature->GetInstanceId(), BATTLEGROUND_SA);
    if (!sota)
        return false;

    return sota->GetData(BG_SA_DATA_CAN_DEMOLISHER_BUILD);
}
