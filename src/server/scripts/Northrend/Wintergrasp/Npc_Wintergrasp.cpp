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

#include "Npc_Wintergrasp.h"

typedef npc_wg_spirit_guide::npc_wg_spirit_guideAI    WGSpiritGuideAI;
typedef npc_wg_queue::npc_wg_queueAI                  WGNpcQueueAI;


bool npc_wg_demolisher_engineer::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    uint32 menuId = Player::GetDefaultGossipMenuForSource(pCreature);
    uint32 textId = pPlayer->GetGossipTextId(pCreature);

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (CanBuild(pCreature))
    {
        if (pPlayer->HasAura(SPELL_CORPORAL) || pPlayer->HasAura(SPELL_LIEUTENANT))
            pPlayer->ADD_GOSSIP_ITEM_DB(menuId, WG_GOSSIP_ENGINEER_BUILD_CATAPULT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        if (pPlayer->HasAura(SPELL_LIEUTENANT))
        {
            pPlayer->ADD_GOSSIP_ITEM_DB(menuId, WG_GOSSIP_ENGINEER_BUILD_DEMOLISHER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->ADD_GOSSIP_ITEM_DB(menuId, WG_GOSSIP_ENGINEER_BUILD_SIEGE_ENGINE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        }
    }
    else
        pPlayer->ADD_GOSSIP_ITEM_DB(menuId, WG_GOSSIP_ENGINEER_CANNOT_BUILD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);

    pPlayer->SEND_GOSSIP_MENU(textId, pCreature->GetGUID());
    return true;
}

bool npc_wg_demolisher_engineer::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*sender*/, uint32 action)
{
    pPlayer->CLOSE_GOSSIP_MENU();

    if (CanBuild(pCreature))
    {
        switch (action - GOSSIP_ACTION_INFO_DEF)
        {
            case 0:
                pPlayer->CastSpell(pPlayer, SPELL_BUILD_CATAPULT_FORCE, true);
                break;
            case 1:
                pPlayer->CastSpell(pPlayer, SPELL_BUILD_DEMOLISHER_FORCE, true);
                break;
            case 2:
                pPlayer->CastSpell(pPlayer, pPlayer->GetTeamId() == TEAM_ALLIANCE ? SPELL_BUILD_SIEGE_VEHICLE_FORCE_ALLIANCE : SPELL_BUILD_SIEGE_VEHICLE_FORCE_HORDE, true);
                break;
        }

        if (Creature* pCreatureArms = pCreature->FindNearestCreature(BATTLEFIELD_WG_NPC_CONTROL_ARMS, 30.0f, true))
            pCreature->CastSpell(pCreatureArms, SPELL_ACTIVATE_CONTROL_ARMS, true);
    }
    return true;
}

bool npc_wg_demolisher_engineer::CanBuild(Creature* pCreature)
{
    Battlefield* wintergrasp = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG);
    if (!wintergrasp)
        return false;

    switch (pCreature->GetEntry())
    {
        case BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER:
            return (wintergrasp->GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H) > wintergrasp->GetData(BATTLEFIELD_WG_DATA_VEHICLE_H));
        case BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER:
            return (wintergrasp->GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A) > wintergrasp->GetData(BATTLEFIELD_WG_DATA_VEHICLE_A));
        default:
            return false;
    }
}

bool npc_wg_spirit_guide::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    Battlefield* wintergrasp = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG);
    if (!wintergrasp)
        return true;

    GraveyardVect graveyard = wintergrasp->GetGraveyardVector();
    for (uint8 i = 0; i < graveyard.size(); i++)
        if (graveyard[i]->GetControlTeamId() == pPlayer->GetTeamId())
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(((BfGraveyardWG*)graveyard[i])->GetTextId()), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + i);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool npc_wg_spirit_guide::OnGossipSelect(Player* pPlayer, Creature* /*pCreature*/, uint32 /*sender*/, uint32 action)
{
    pPlayer->CLOSE_GOSSIP_MENU();

    Battlefield* wintergrasp = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG);
    if (wintergrasp)
    {
        GraveyardVect gy = wintergrasp->GetGraveyardVector();
        for (uint8 i = 0; i < gy.size(); i++)
            if (action - GOSSIP_ACTION_INFO_DEF == i && gy[i]->GetControlTeamId() == pPlayer->GetTeamId())
                if (WorldSafeLocsEntry const* safeLoc = sWorldSafeLocsStore.LookupEntry(gy[i]->GetGraveyardId()))
                    pPlayer->TeleportTo(safeLoc->map_id, safeLoc->x, safeLoc->y, safeLoc->z, 0);
    }
    return true;
}

void WGSpiritGuideAI::UpdateAI(const uint32 /*diff*/)
{
    if (!me->HasUnitState(UNIT_STATE_CASTING))
        DoCast(me, SPELL_CHANNEL_SPIRIT_HEAL);
}

void WGNpcQueueAI::Reset()
{
    FrostArmor_Timer = 0;
}

void WGNpcQueueAI::UpdateAI(const uint32 diff)
{
    if (FrostArmor_Timer <= diff)
    {
        DoCast(me, SPELL_FROST_ARMOR);
        FrostArmor_Timer = WG_NPC_QUEUE_FROST_ARMOR_TIMER;
    }
    else
        FrostArmor_Timer -= diff;

    DoMeleeAttackIfReady();
}

bool npc_wg_queue::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    Battlefield* wintergrasp = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG);
    if (!wintergrasp)
        return true;

    if (wintergrasp->IsWarTime())
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(WG_NPCQUEUE_TEXTOPTION_JOIN), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(wintergrasp->GetDefenderTeam() ? WG_NPCQUEUE_TEXT_H_WAR : WG_NPCQUEUE_TEXT_A_WAR, pCreature->GetGUID());
    }
    else
    {
        uint32 timer = wintergrasp->GetTimer() / 1000;
        pPlayer->SendUpdateWorldState(4354, time(NULL) + timer);
        if (timer < 15 * MINUTE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(WG_NPCQUEUE_TEXTOPTION_JOIN), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(wintergrasp->GetDefenderTeam() ? WG_NPCQUEUE_TEXT_H_QUEUE : WG_NPCQUEUE_TEXT_A_QUEUE, pCreature->GetGUID());
        }
        else
            pPlayer->SEND_GOSSIP_MENU(wintergrasp->GetDefenderTeam() ? WG_NPCQUEUE_TEXT_H_NOWAR : WG_NPCQUEUE_TEXT_A_NOWAR, pCreature->GetGUID());
    }
    return true;
}

bool npc_wg_queue::OnGossipSelect(Player* pPlayer, Creature* /*pCreature*/, uint32 /*sender*/, uint32 /*action*/)
{
    pPlayer->CLOSE_GOSSIP_MENU();

    Battlefield* wintergrasp = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG);
    if (!wintergrasp)
        return true;

    if (wintergrasp->IsWarTime())
        wintergrasp->InvitePlayerToWar(pPlayer);
    else
    {
        uint32 timer = wintergrasp->GetTimer() / 1000;
        if (timer < 15 * MINUTE)
            wintergrasp->InvitePlayerToQueue(pPlayer);
    }
    return true;
}