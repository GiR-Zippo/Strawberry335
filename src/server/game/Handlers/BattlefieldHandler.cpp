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
 *
 * @section DESCRIPTION
 *
 * All WorldSession handles related to the class Battlefield
 */

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "WorldSession.h"
#include "Player.h"


/**
 * This send to player windows for invite player to join the war
 * @param BattleId -The BattleId of Bf
 * @param ZoneId -The zone where the battle is (4197 for wg)
 * @param time -Time in second that the player has for accept
 */
void WorldSession::SendBfInvitePlayerToWar(uint32 BattleId, uint32 ZoneId, uint32 p_time)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_ENTRY_INVITE, 12);
    data << uint32(BattleId);
    data << uint32(ZoneId);
    data << uint32((time(NULL) + p_time));

    // Sending packet to player
    SendPacket(&data);
}

/**
 * This send invitation to player to join the queue
 * @param BattleId -The BattleId of Bf
 */
void WorldSession::SendBfInvitePlayerToQueue(uint32 BattleId)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_INVITE, 5);
    data << uint32(BattleId);
    data << uint8(1); //warmup ? used ?

    // Sending packet to player
    SendPacket(&data);
}

/**
 *This send packet for inform player that he join queue
 * @param BattleId -The BattleId of Bf
 * @param ZoneId -The zone where the battle is (4197 for wg)
 * @param CanQueue -If able to queue // 0 you cannot queue wg // 1 you are queued
 * @param Full -On log in is full // 0 wg full // 1 queue for upcoming
 */
void WorldSession::SendBfQueueInviteResponse(uint32 BattleId, uint32 ZoneId, bool CanQueue, bool Full)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE, 11);
    data << uint32(BattleId);
    data << uint32(ZoneId);
    data << uint8((CanQueue ? 1 : 0)); // Accepted
    data << uint8((Full ? 0 : 1)); // Logging In
    data << uint8(1); // Warmup

    SendPacket(&data);
}

/**
 * Called when player accept to join war
 * @param BattleId -The BattleId of Bf
 */
void WorldSession::SendBfEntered(uint32 BattleId)
{
    // m_PlayerInWar[player->GetTeamId()].insert(player->GetGUID());
    WorldPacket data(SMSG_BATTLEFIELD_MGR_ENTERED, 7);
    data << uint32(BattleId);
    data << uint8(1); // unk
    data << uint8(1); // unk
    data << uint8(_player->isAFK() ? 1 : 0); // Clear AFK

    SendPacket(&data);
}

void WorldSession::SendBfLeaveMessage(uint32 BattleId, BFLeaveReason reason)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_EJECTED, 7);
    data << uint32(BattleId);
    data << uint8(reason);// byte Reason
    data << uint8(2);// byte BattleStatus
    data << uint8(0);// bool Relocated

    SendPacket(&data);
}

/**
 * This sends a packet to inform player that he has to leave the war
 * -Because the Bf is full -> queue for teleport (only implemented for wg)
 * @param BattleId -The BattleId of Bf
 */
void WorldSession::SendBfPendingLeaveMessage(uint32 BattleId)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_EJECT_PENDING, 4);
    data << uint32(BattleId);

    SendPacket(&data);
}

// DONT USE IT! ONLY FOR TESTING
void WorldSession::SendBfStateChange(uint32 BattleId, uint32 state)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_STATE_CHANGE, 8);
    data << uint32(BattleId);
    data << uint32(state);

    SendPacket(&data);
}

/**
 * This send packet for inform player that he leaves queue
 * @param BattleId -The BattleId of Bf
 * @param ZoneId  -The zone where the battle is (4197 for wg)
 */
void WorldSession::SendBfExitResponse(uint32 BattleId, uint32 ZoneId)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE, 8);
    data << uint32(BattleId);
    data << uint32(ZoneId);

    SendPacket(&data);
}

/** Send by client when he click on accept for queue */
void WorldSession::HandleBfQueueInviteResponse(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_BATTLEFIELD_MGR_QUEUE_INVITE_RESPONSE");

    uint32 BattleId;
    uint8 Accepted;

    recvData >> BattleId >> Accepted;
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "HandleQueueInviteResponse: BattleID:%u Accepted:%u", BattleId, Accepted);
    Battlefield* Bf = sBattlefieldMgr->GetBattlefieldByBattleId(BattleId);
    if (!Bf)
        return;

    if (Accepted)
        Bf->PlayerAcceptInviteToQueue(_player);
}

/** Send by client on clicking in accept or refuse of invitation windows for join game */
void WorldSession::HandleBfEntryInviteResponse(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_BATTLEFIELD_MGR_ENTRY_INVITE_RESPONSE");

    uint32 BattleId;
    uint8 Accepted;

    recvData >> BattleId >> Accepted;
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "HandleBattlefieldInviteResponse: BattleID:%u Accepted:%u", BattleId, Accepted);
    Battlefield* Bf = sBattlefieldMgr->GetBattlefieldByBattleId(BattleId);
    if (!Bf)
        return;

    // If player accept invitation
    if (Accepted)
        Bf->PlayerAcceptInviteToWar(_player);
    else
    {
        if (_player->GetZoneId() == Bf->GetZoneId())
            Bf->KickPlayerFromBattlefield(_player->GetGUID());
        else
            Bf->PlayerDeclinedInviteToWar(_player);
    }
}

/** Send by client on clicking at remove from bf queue */
void WorldSession::HandleBfExitRequest(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_BATTLEFIELD_MGR_EXIT_REQUEST");

    uint32 BattleId;

    recvData >> BattleId;
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "HandleBfExitQueueRequest: BattleID:%u ", BattleId);
    Battlefield* Bf = sBattlefieldMgr->GetBattlefieldByBattleId(BattleId);
    if (!Bf)
        return;

    Bf->AskToLeaveQueue(_player);
}