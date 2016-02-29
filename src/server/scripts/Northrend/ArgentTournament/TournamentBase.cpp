/*
 * Copyright (C) 2012 Zirkon
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ArgentTournament.h"


/*######
## npc_vendor_argent_tournament
UPDATE `creature_template` SET `ScriptName`='npc_vendor_argent_tournament' WHERE `entry` IN (33553, 33554, 33556, 33555, 33557, 33307, 33310, 33653, 33650, 33657);
######*/

const uint32 ArgentTournamentVendor[10][4] =
{
    {33553,13726,2,14460}, //Orc
    {33554,13726,8,14464}, //Troll
    {33556,13728,6,14458}, //Tauren
    {33555,13729,5,14459}, //Undead
    {33557,13731,10,14465}, //BloodElf
    {33307,13699,1,14456}, //Human
    {33310,13713,3,14457}, //Dwarf
    {33653,13725,4,14463}, //NightElf
    {33650,13723,7,14462}, //Gnome
    {33657,13724,11,14461} //Draenei
};

class npc_vendor_argent_tournament : public CreatureScript
{
public:
    npc_vendor_argent_tournament() : CreatureScript("npc_vendor_argent_tournament") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (!pPlayer || !pCreature)
            return false;

        bool npcCheck = false;
        bool questCheck = false;
        bool raceCheck = false;
        uint32 textId = 0;

        for (int i = 0; (i < 10) && !npcCheck; i++)
        {
            if (pCreature->GetEntry() == ArgentTournamentVendor[i][0])
            {
                npcCheck = true;
				questCheck = pPlayer->GetQuestStatus(ArgentTournamentVendor[i][1]) == QUEST_STATUS_REWARDED;
                raceCheck = pPlayer->getRace() == ArgentTournamentVendor[i][2];
                textId = ArgentTournamentVendor[i][3];
            }
        }

        if (questCheck || raceCheck)
        {
            if (WorldSession *session = pPlayer->GetSession())
                session->SendListInventory(pCreature->GetGUID());
        }
        else
            pPlayer->SEND_GOSSIP_MENU(textId, pCreature->GetGUID());
        return true;
    }
};

/*######
## npc_dame_evniki_kapsalis
######*/

enum eDameEnvikiKapsalis
{
    TITLE_CRUSADER    = 123
};

class npc_dame_evniki_kapsalis : public CreatureScript
{
public:
    npc_dame_evniki_kapsalis() : CreatureScript("npc_dame_evniki_kapsalis") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->HasTitle(TITLE_CRUSADER))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_TRADE)
            pPlayer->GetSession()->SendListInventory(pCreature->GetGUID());
        return true;
    }
};

void AddTournamentBase()
{
    new npc_dame_evniki_kapsalis;
    new npc_vendor_argent_tournament;
}