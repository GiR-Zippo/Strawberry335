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

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Map.h"
#include "MapManager.h"
#include "Group.h"
#include "WorldPacket.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "CreatureTextMgr.h"
#include "GroupMgr.h"


Battlefield::Battlefield()
{
    m_Timer = 0;
    m_IsEnabled = true;
    m_isActive = false;
    m_DefenderTeam = TEAM_NEUTRAL;

    m_TypeId = 0;
    m_BattleId = 0;
    m_ZoneId = 0;
    m_Map = NULL;
    m_MapId = 0;
    m_MaxPlayer = 0;
    m_MinPlayer = 0;
    m_MinLevel = 0;
    m_BattleTime = 0;
    m_NoWarBattleTime = 0;
    m_RestartAfterCrash = 0;
    m_TimeForAcceptInvite = 20;
    m_uiKickDontAcceptTimer = 1000;

    m_uiKickAfkPlayersTimer = 1000;

    m_LastResurectTimer = 30 * IN_MILLISECONDS;
    m_StartGroupingTimer = 0;
    m_StartGrouping = false;
}

Battlefield::~Battlefield()
{
    for (BfCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        delete itr->second;

    for (GraveyardVect::const_iterator itr = m_GraveyardList.begin(); itr != m_GraveyardList.end(); ++itr)
        delete *itr;
}

// Called when a player enters the zone
void Battlefield::HandlePlayerEnterZone(Player* player, uint32 /*zone*/)
{
    // If battle is started,
    // If not full of players > invite player to join the war
    // If full of players > announce to player that BF is full and kick him after a few second if he desn't leave
    // If is on taxi -> skip invite till player "landed"
    if (IsWarTime() && !player->isInFlight())
    {
        if (CanPlayerJoinWar(player))
        {
            // Check if player has aura after teleport (no invite - join war) - special handling for wintergrasp
            if (player->HasAura(49759) && m_BattleId == BATTLEFIELD_BATTLEID_WG)
                PlayerAcceptInviteToWar(player);
            else
                InvitePlayerToWar(player);
        }
        else
        {
            m_PlayersWillBeKick[player->GetTeamId()][player->GetGUID()] = time(NULL) + 10;
            player->GetSession()->SendBfPendingLeaveMessage(m_BattleId);
        }
    }
    else
    {
        // If time left is < 15 minutes invite player to join queue
        if (m_Timer <= m_StartGroupingTimer)
        {
            if (!player->isInFlight())
                InvitePlayerToQueue(player);
            else
                playersOnTaxi.insert(player->GetGUID());
        }
    }

    // Add player in the list of player in zone
    m_players[player->GetTeamId()].insert(player->GetGUID());
    OnPlayerEnterZone(player);
}

// Called when a player leave the zone
void Battlefield::HandlePlayerLeaveZone(Player* player, uint32 /*zone*/)
{
    if (IsWarTime())
    {
        // If the player is participating to the battle
        if (m_PlayersInWar[player->GetTeamId()].find(player->GetGUID()) != m_PlayersInWar[player->GetTeamId()].end())
        {
            m_PlayersInWar[player->GetTeamId()].erase(player->GetGUID());
            if (Group* group = player->GetGroup()) // Remove the player from the raid group
                group->RemoveMember(player->GetGUID());
            OnPlayerLeaveWar(player);

            // This should be sent if player is in war group or not (Battle popup disappear)
            player->GetSession()->SendBfLeaveMessage(m_BattleId);
        }
        // Special handling for wintergrasp (Popup dissappear)
        else if (!player->isInFlight() && m_BattleId == BATTLEFIELD_BATTLEID_WG)
            player->GetSession()->SendBfLeaveMessage(m_BattleId);
    }
    else if (playersOnTaxi.find(player->GetGUID()) != playersOnTaxi.end())
        playersOnTaxi.erase(player->GetGUID());

    for (BfCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        itr->second->HandlePlayerLeave(player);

    m_InvitedPlayers[player->GetTeamId()].erase(player->GetGUID());
    m_PlayersWillBeKick[player->GetTeamId()].erase(player->GetGUID());
    m_players[player->GetTeamId()].erase(player->GetGUID());
    SendRemoveWorldStates(player);
    RemovePlayerFromResurrectQueue(player->GetGUID());
    OnPlayerLeaveZone(player);
}

// Called when a player killed unit
void Battlefield::HandleKill(Player* pPlayer, Unit* pVictim)
{
    if (pVictim->GetTypeId() == TYPEID_PLAYER)
    {
        // To be able to remove insignia
        pVictim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);

        // Reward XP for kill
        if (sWorld->getBoolConfig(CONFIG_BG_XP_FOR_KILL) && pPlayer && pVictim)
            pPlayer->RewardPlayerAndGroupAtKill(pVictim, true);
    }
}

// Called when a player leaves the bf group
void Battlefield::HandlePlayerLeaveBfGroup(Player* player)
{
    if (!IsWarTime())
        return;

    // Add player to kick qeue
    m_PlayersWillBeKick[player->GetTeamId()][player->GetGUID()] = time(NULL) + 10;

    // Other necessary things will be proceeded by HandlePlayerLeaveZone
}

bool Battlefield::Update(uint32 diff)
{
    if (m_Timer <= diff)
    {
        // Battlefield ends on time
        if (IsWarTime())
            EndBattle(true);
        else // Time to start a new battle!
            StartBattle();
    }
    else
        m_Timer -= diff;

    // Invite players a few minutes before the battle's beginning
    if (!IsWarTime() && !m_StartGrouping && m_Timer <= m_StartGroupingTimer)
    {
        m_StartGrouping = true;
        InvitePlayersInZoneToQueue();
        OnStartGrouping();
    }

    if (m_StartGrouping && !playersOnTaxi.empty() && !IsWarTime())
        updateOnTaxiPlayers();

    bool objective_changed = false;
    if (IsWarTime())
    {
        if (m_uiKickAfkPlayersTimer <= diff)
        {
            m_uiKickAfkPlayersTimer = 1000;
            KickAfkPlayers();
        }
        else
            m_uiKickAfkPlayersTimer -= diff;

        // Kick players who chose not to accept invitation to the battle
        if (m_uiKickDontAcceptTimer <= diff)
        {
            time_t now = time(NULL);
            for (int team = 0; team < 2; team++)
                for (PlayerTimerMap::iterator itr = m_InvitedPlayers[team].begin(); itr != m_InvitedPlayers[team].end(); ++itr)
                    if (itr->second <= now)
                        KickPlayerFromBattlefield(itr->first);

            InvitePlayersInZoneToWar();

            for (int team = 0; team < 2; team++)
                for (PlayerTimerMap::iterator itr = m_PlayersWillBeKick[team].begin(); itr != m_PlayersWillBeKick[team].end(); ++itr)
                    if (itr->second <= now)
                        KickPlayerFromBattlefield(itr->first);

            m_uiKickDontAcceptTimer = 1000;
        }
        else
            m_uiKickDontAcceptTimer -= diff;

        for (BfCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            if (itr->second->Update(diff))
                objective_changed = true;
    }

    if (m_LastResurectTimer <= diff)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::Update: Call resurrect of graveyards");
        uint8 i = 0;

        for (i; i < m_GraveyardList.size(); i++)
        {
            if (GetGraveyardById(i) && IsWarTime())
                m_GraveyardList[i]->Resurrect();
            else
                sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::Update: Couldn't find graveyard %u for call resurrect");

            m_LastResurectTimer = RESURRECTION_INTERVAL;
        }

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::Update: Resurrect called at %u of %u graveyards", i, m_GraveyardList.size());
    }
    else
        m_LastResurectTimer -= diff;

    return objective_changed;
}

void Battlefield::InvitePlayersInZoneToQueue()
{
    for (uint8 team = 0; team < 2; ++team)
    {
        for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
        {
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
            {
                // Dont invite player who are on taxi
                if (player->isInFlight())
                    continue;

                InvitePlayerToQueue(player);
            }
        }
    }
}

void Battlefield::InvitePlayerToQueue(Player* player)
{
    if (player->isGameMaster())
        return;

    if (m_PlayersInQueue[player->GetTeamId()].count(player->GetGUID()))
        return;

    if (m_PlayersInQueue[player->GetTeamId()].size() <= m_MinPlayer || m_PlayersInQueue[GetOtherTeam(player->GetTeamId())].size() >= m_MinPlayer)
        player->GetSession()->SendBfInvitePlayerToQueue(m_BattleId);
}

void Battlefield::InvitePlayersInQueueToWar()
{
    for (uint8 team = 0; team < BG_TEAMS_COUNT; ++team)
    {
        for (GuidSet::const_iterator itr = m_PlayersInQueue[team].begin(); itr != m_PlayersInQueue[team].end(); ++itr)
        {
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
            {
                if (m_PlayersInWar[player->GetTeamId()].size() + m_InvitedPlayers[player->GetTeamId()].size() < m_MaxPlayer)
                    InvitePlayerToWar(player);
                else
                {
                    //Full
                }
            }
        }
        m_PlayersInQueue[team].clear();
    }
}

void Battlefield::InvitePlayersInZoneToWar()
{
    for (uint8 team = 0; team < BG_TEAMS_COUNT; ++team)
    {
        for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
        {
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
            {
                // Dont invite player who are on inWar / already invited / taxi
                if (m_PlayersInWar[player->GetTeamId()].count(player->GetGUID()) || m_InvitedPlayers[player->GetTeamId()].count(player->GetGUID()) || player->isInFlight())
                    continue;

                if (m_PlayersInWar[player->GetTeamId()].size() + m_InvitedPlayers[player->GetTeamId()].size() < m_MaxPlayer)
                    InvitePlayerToWar(player);
                else // Battlefield is full of players
                    m_PlayersWillBeKick[player->GetTeamId()][player->GetGUID()] = time(NULL) + 10;
            }
        }
    }
}

void Battlefield::InvitePlayerToWar(Player* player)
{
    if (!player)
        return;

    if (player->isGameMaster())
        return;

    // If the player is in an battleground or arena or in an dungeon or raid, kick him
    if (player->GetMap()->IsBattlegroundOrArena() || player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
    {
        m_PlayersInQueue[player->GetTeamId()].erase(player->GetGUID());
        player->GetSession()->SendBfExitResponse(m_BattleId, GetZoneId());
        return;
    }

    // If the player does not match minimal level requirements for the battlefield, kick him
    if (player->getLevel() < m_MinLevel)
    {
        if (player->isDead())
            player->ResurrectPlayer(1.f);

        if (m_PlayersWillBeKick[player->GetTeamId()].count(player->GetGUID()) == 0)
            m_PlayersWillBeKick[player->GetTeamId()][player->GetGUID()] = time(NULL) + 10;
        return;
    }

    // Check if player is not already in war
    if (m_PlayersInWar[player->GetTeamId()].count(player->GetGUID()) || m_InvitedPlayers[player->GetTeamId()].count(player->GetGUID()))
        return;

    m_PlayersWillBeKick[player->GetTeamId()].erase(player->GetGUID());
    m_InvitedPlayers[player->GetTeamId()][player->GetGUID()] = time(NULL) + m_TimeForAcceptInvite;
    player->GetSession()->SendBfInvitePlayerToWar(m_BattleId, m_ZoneId, m_TimeForAcceptInvite);
}

void Battlefield::KickAfkPlayers()
{
    for (uint8 team = 0; team < BG_TEAMS_COUNT; ++team)
    {
        for (GuidSet::const_iterator itr = m_PlayersInWar[team].begin(); itr != m_PlayersInWar[team].end(); ++itr)
        {
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
            {
                if (player->isAFK() && m_PlayersWillBeKick[player->GetTeamId()].count(player->GetGUID()) == 0)
                {
                    if (player->isDead())
                        player->ResurrectPlayer(1.f);

                    m_PlayersWillBeKick[player->GetTeamId()][player->GetGUID()] = time(NULL) + 10;
                }
            }
        }
    }
}

void Battlefield::KickPlayerFromBattlefield(uint64 guid)
{
    if (Player* player = sObjectAccessor->FindPlayer(guid))
    {
        if (player->GetZoneId() == GetZoneId())
            player->TeleportTo(KickPosition);
    }
}

void Battlefield::KickPlayerFromBattlefield(Player* pPlayer)
{
    if (!pPlayer)
        return;

    if (pPlayer->GetZoneId() == GetZoneId())
        pPlayer->TeleportTo(KickPosition);
}

void Battlefield::StartBattle()
{
    if (m_isActive)
        return;

    for (int team = 0; team < BG_TEAMS_COUNT; team++)
    {
        m_PlayersInWar[team].clear();
        m_Groups[team].clear();
    }

    if (!playersOnTaxi.empty())
        playersOnTaxi.clear();

    m_Timer = m_BattleTime;
    m_isActive = true;

    InvitePlayersInZoneToWar();
    InvitePlayersInQueueToWar();

    DoPlaySoundToAll(BF_START);

    OnBattleStart();

    // Send worldstates to all on server
    SendInitWorldStatesToAll(true);
}

void Battlefield::EndBattle(bool endByTimer)
{
    if (!m_isActive)
        return;

    m_isActive = false;

    m_StartGrouping = false;

    if (!endByTimer)
        SetDefenderTeam(GetAttackerTeam());

    if (GetDefenderTeam() == TEAM_ALLIANCE)
        DoPlaySoundToAll(BF_ALLIANCE_WINS);
    else
        DoPlaySoundToAll(BF_HORDE_WINS);

    OnBattleEnd(endByTimer);

    // Reset battlefield timer
    m_Timer = m_NoWarBattleTime;

    // Send worldstates to all on server
    SendInitWorldStatesToAll(true);
}

void Battlefield::DoPlaySoundToAll(uint32 SoundID)
{
    WorldPacket data;
    data.Initialize(SMSG_PLAY_SOUND, 4);
    data << uint32(SoundID);

    for (int team = 0; team < BG_TEAMS_COUNT; team++)
        for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
                player->GetSession()->SendPacket(&data);
}

bool Battlefield::HasPlayer(Player* player) const
{
    return m_players[player->GetTeamId()].find(player->GetGUID()) != m_players[player->GetTeamId()].end();
}

// Called in WorldSession::HandleBfQueueInviteResponse
void Battlefield::PlayerAcceptInviteToQueue(Player* player)
{
    // Add player in queue
    m_PlayersInQueue[player->GetTeamId()].insert(player->GetGUID());
    // Send notification
    player->GetSession()->SendBfQueueInviteResponse(m_BattleId, m_ZoneId);
}

// Called in WorldSession::HandleBfExitRequest
void Battlefield::AskToLeaveQueue(Player* player)
{
    // Remove player from queue
    m_PlayersInQueue[player->GetTeamId()].erase(player->GetGUID());
    // Send notification
    player->GetSession()->SendBfExitResponse(m_BattleId, m_ZoneId);
}

// Called in WorldSession::HandleBfEntryInviteResponse
void Battlefield::PlayerAcceptInviteToWar(Player* player)
{
    if (!IsWarTime())
        return;

    if (AddOrSetPlayerToCorrectBfGroup(player))
    {
        if (player->isDead())
            player->ResurrectPlayer(1.0f);

        player->GetSession()->SendBfEntered(m_BattleId);
        m_PlayersInWar[player->GetTeamId()].insert(player->GetGUID());
        m_InvitedPlayers[player->GetTeamId()].erase(player->GetGUID());

        if (player->isAFK())
            player->ToggleAFK();

        OnPlayerJoinWar(player); //for scripting
    }
}

// Called in WorldSession::HandleBfEntryInviteResponse
void Battlefield::PlayerDeclinedInviteToWar(Player* player)
{
    if (!IsWarTime())
        return;

    // Erase player
    m_InvitedPlayers[player->GetTeamId()].erase(player->GetGUID());

    // Send notification
    player->GetSession()->SendBfExitResponse(m_BattleId, m_ZoneId);
}

void Battlefield::TeamCastSpell(TeamId team, int32 spellId)
{
    if (spellId > 0)
    {
        for (GuidSet::const_iterator itr = m_PlayersInWar[team].begin(); itr != m_PlayersInWar[team].end(); ++itr)
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
                player->CastSpell(player, uint32(spellId), true);
    }
    else
    {
        for (GuidSet::const_iterator itr = m_PlayersInWar[team].begin(); itr != m_PlayersInWar[team].end(); ++itr)
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
                player->RemoveAuraFromStack(uint32(-spellId));
    }
}

void Battlefield::BroadcastPacketToZone(WorldPacket& data) const
{
    for (uint8 team = 0; team < BG_TEAMS_COUNT; ++team)
        for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
                player->GetSession()->SendPacket(&data);
}

void Battlefield::BroadcastPacketToQueue(WorldPacket& data) const
{
    for (uint8 team = 0; team < BG_TEAMS_COUNT; ++team)
        for (GuidSet::const_iterator itr = m_PlayersInQueue[team].begin(); itr != m_PlayersInQueue[team].end(); ++itr)
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
                player->GetSession()->SendPacket(&data);
}

void Battlefield::BroadcastPacketToWar(WorldPacket& data) const
{
    for (uint8 team = 0; team < BG_TEAMS_COUNT; ++team)
        for (GuidSet::const_iterator itr = m_PlayersInWar[team].begin(); itr != m_PlayersInWar[team].end(); ++itr)
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
                player->GetSession()->SendPacket(&data);
}

// @param entry = trinity_string.entry for the main message
// @param additionalEntry = trinity_string.entry for the item message (Alliance / Horde or south-tower/north-tower as an example)
// If the param additionalEntry is given, the trinity_string(entry) must contain a format string '%s' to parse the trinity_string(additionalEntry)
void Battlefield::SendWarningToAllInZone(uint32 entry, uint32 additionalEntry)
{
    for (uint8 team = 0; team < BG_TEAMS_COUNT; ++team)
    {
        if (m_players[team].empty())
            continue;

        for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
        {
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
            {
                std::string core = player->GetSession()->GetTrinityString(entry);

                if (additionalEntry != 0)
                {
                    std::string additional = player->GetSession()->GetTrinityString(additionalEntry);

                    ChatHandler(player->GetSession()).PSendSysMessage(core.c_str(), additional.c_str());
                    player->GetSession()->SendAreaTriggerMessage(core.c_str(), additional.c_str());
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(core.c_str());
                    player->GetSession()->SendAreaTriggerMessage(core.c_str());
                }
            }
        }
    }
}

// @param entry = trinity_string.entry
// @param additionalEntry = trinity_string.entry for the item message (Alliance / Horde or south-tower/north-tower as an example)
// If the param additionalEntry is given, the trinity_string(entry) must contain a format string '%s' to parse the trinity_string(additionalEntry)
void Battlefield::SendWarningToPlayer(Player* player, uint32 entry, uint32 additionalEntry)
{
    std::string core = player->GetSession()->GetTrinityString(entry);

    if (additionalEntry != 0)
    {
        std::string additional = player->GetSession()->GetTrinityString(additionalEntry);

        ChatHandler(player->GetSession()).PSendSysMessage(core.c_str(), additional.c_str());
        player->GetSession()->SendAreaTriggerMessage(core.c_str(), additional.c_str());
    }
    else
    {
        ChatHandler(player->GetSession()).PSendSysMessage(core.c_str());
        player->GetSession()->SendAreaTriggerMessage(core.c_str());
    }
}

void Battlefield::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (uint8 i = 0; i < BG_TEAMS_COUNT; ++i)
        for (GuidSet::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
            if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                pPlayer->SendUpdateWorldState(field, value);
}

void Battlefield::RegisterZone(uint32 zoneId)
{
    sBattlefieldMgr->AddZone(zoneId, this);
}

void Battlefield::HideNpc(Creature* creature)
{
    creature->CombatStop();
    creature->SetReactState(REACT_PASSIVE);
    creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
    creature->SetPhaseMask(2, true);
    creature->DisappearAndDie();
    creature->SetVisible(false);
}

void Battlefield::ShowNpc(Creature* creature, bool aggressive)
{
    creature->SetPhaseMask(1, true);
    creature->SetVisible(true);
    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
    if (!creature->isAlive())
        creature->Respawn(true);
    if (aggressive)
        creature->SetReactState(REACT_AGGRESSIVE);
    else
    {
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        creature->SetReactState(REACT_PASSIVE);
    }
}

// ****************************************************
// ******************* Group System *******************
// ****************************************************
Group* Battlefield::GetFreeBfRaid(TeamId TeamId)
{
    for (GuidSet::const_iterator itr = m_Groups[TeamId].begin(); itr != m_Groups[TeamId].end(); ++itr)
        if (Group* group = sGroupMgr->GetGroupByGUID(*itr))
            if (!group->IsFull())
                return group;
    return NULL;
}

Group* Battlefield::GetGroupPlayer(uint64 guid, TeamId TeamId)
{
    for (GuidSet::const_iterator itr = m_Groups[TeamId].begin(); itr != m_Groups[TeamId].end(); ++itr)
        if (Group* group = sGroupMgr->GetGroupByGUID(*itr))
            if (group->IsMember(guid))
                return group;

            return NULL;
}

bool Battlefield::AddOrSetPlayerToCorrectBfGroup(Player* player)
{
    if (!player->IsInWorld())
        return false;

    if (Group* group = player->GetGroup())
        group->RemoveMember(player->GetGUID());

    Group* group = GetFreeBfRaid(player->GetTeamId());
    if (!group)
    {
        group = new Group;
        group->SetBattlefieldGroup(this);
        group->Create(player);
        sGroupMgr->AddGroup(group);
        m_Groups[player->GetTeamId()].insert(group->GetGUID());
    }
    else if (group->IsMember(player->GetGUID()))
    {
        uint8 subgroup = group->GetMemberGroup(player->GetGUID());
        player->SetBattlegroundOrBattlefieldRaid(group, subgroup);
    }
    else
        group->AddMember(player);

    return true;
}

//***************End of Group System*******************

//*****************************************************
//***************Spirit Guide System*******************
//*****************************************************

//--------------------
//-Battlefield Method-
//--------------------
BfGraveyard* Battlefield::GetGraveyardById(uint32 id) const
{
    if (id < m_GraveyardList.size())
    {
        if (BfGraveyard* graveyard = m_GraveyardList.at(id))
            return graveyard;
        else
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::GetGraveyardById Id: %u not exists", id);
    }
    else
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::GetGraveyardById Id: %u cant be found", id);

    return NULL;
}

WorldSafeLocsEntry const* Battlefield::GetClosestGraveYard(Player* player)
{
    BfGraveyard* closestGY = NULL;
    float maxdist = -1;
    for (uint8 i = 0; i < m_GraveyardList.size(); i++)
    {
        if (m_GraveyardList[i])
        {
            if (m_GraveyardList[i]->GetControlTeamId() != player->GetTeamId())
                continue;

            float dist = m_GraveyardList[i]->GetDistance(player);
            if (dist < maxdist || maxdist < 0)
            {
                closestGY = m_GraveyardList[i];
                maxdist = dist;
            }
        }
    }

    if (closestGY)
        return sWorldSafeLocsStore.LookupEntry(closestGY->GetGraveyardId());

    return NULL;
}

void Battlefield::AddPlayerToResurrectQueue(uint64 npcGuid, uint64 playerGuid)
{
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::AddPlayerToResurrectQueue: Called for npc %u player %u", npcGuid, playerGuid);

    for (uint8 i = 0; i < m_GraveyardList.size(); i++)
    {
        if (!m_GraveyardList[i])
            continue;

        if (m_GraveyardList[i]->HasNpc(npcGuid))
        {
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::AddPlayerToResurrectQueue: Added player to resurrect queue of graveyard %u with id %u", m_GraveyardList[i]->GetGraveyardId(), i);
            m_GraveyardList[i]->AddPlayer(playerGuid);
            break;
        }
    }
}

void Battlefield::RemovePlayerFromResurrectQueue(uint64 playerGuid)
{
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::RemovePlayerFromResurrectQueue: Called player %u", playerGuid);

    for (uint8 i = 0; i < m_GraveyardList.size(); i++)
    {
        if (!m_GraveyardList[i])
            continue;

        if (m_GraveyardList[i]->HasPlayer(playerGuid))
        {
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::RemovePlayerFromResurrectQueue: Removed player from resurrect queue of graveyard %u with id %u", m_GraveyardList[i]->GetGraveyardId(), i);
            m_GraveyardList[i]->RemovePlayer(playerGuid);
            break;
        }
    }
}

void Battlefield::SendAreaSpiritHealerQueryOpcode(Player* player, uint64 guid)
{
    WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
    uint32 time = m_LastResurectTimer; // resurrect every 30 seconds

    data << guid << time;
    ASSERT(player && player->GetSession());
    player->GetSession()->SendPacket(&data);
}

// ----------------------
// - BfGraveyard Method -
// ----------------------
BfGraveyard::BfGraveyard(Battlefield* battlefield)
{
    m_Bf = battlefield;
    m_GraveyardId = 0;
    m_ControlTeam = TEAM_NEUTRAL;
    m_SpiritGuide[0] = 0;
    m_SpiritGuide[1] = 0;
}

void BfGraveyard::Initialize(TeamId startControl, uint32 graveyardId)
{
    m_ControlTeam = startControl;
    m_GraveyardId = graveyardId;
}

void BfGraveyard::SetSpirit(Creature* spirit, TeamId team)
{
    if (!spirit)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BfGraveyard::SetSpirit: Invalid Spirit.");
        return;
    }

    m_SpiritGuide[team] = spirit->GetGUID();
    spirit->SetReactState(REACT_PASSIVE);
}

float BfGraveyard::GetDistance(Player* player)
{
    const WorldSafeLocsEntry* safeLoc = sWorldSafeLocsStore.LookupEntry(m_GraveyardId);
    return player->GetDistance2d(safeLoc->x, safeLoc->y);
}

void BfGraveyard::AddPlayer(uint64 playerGuid)
{
    if (!m_ResurrectQueue.count(playerGuid))
    {
        m_ResurrectQueue.insert(playerGuid);

        if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
            player->CastSpell(player, SPELL_WAITING_FOR_RESURRECT, true);
    }
}

void BfGraveyard::RemovePlayer(uint64 playerGuid)
{
    m_ResurrectQueue.erase(m_ResurrectQueue.find(playerGuid));

    if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
        player->RemoveAurasDueToSpell(SPELL_WAITING_FOR_RESURRECT);
}

void BfGraveyard::Resurrect()
{
    if (m_ResurrectQueue.empty())
        return;

    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BfGraveyard::Resurrect: Start resurrect for graveyard %u", m_GraveyardId);

    for (GuidSet::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
    {
        // Get player object from his guid
        Player* player = sObjectAccessor->FindPlayer(*itr);
        if (!player)
            continue;

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BfGraveyard::Resurrect: Start resurrect for player %u", *itr);

        // Check if the player is in world and on the good graveyard
        if (player->IsInWorld())
        {
            if (Creature* spirit = m_Bf->GetCreature(m_SpiritGuide[m_ControlTeam]))
                spirit->CastSpell(spirit, SPELL_SPIRIT_HEAL, true);
            else
                sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BfGraveyard::Resurrect: Graveyard %u attempted to resurrect player with no spirit keeper", m_GraveyardId);
        }

        // Resurect player
        player->CastSpell(player, SPELL_RESURRECTION_VISUAL, true);
        player->ResurrectPlayer(1.0f);
        player->CastSpell(player, 6962, true);
        player->CastSpell(player, SPELL_SPIRIT_HEAL_MANA, true);

        sObjectAccessor->ConvertCorpseForPlayer(player->GetGUID());

        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BfGraveyard::Resurrect: Player %u resurrected", *itr);
    }

    m_ResurrectQueue.clear();

    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BfGraveyard::Resurrect: End resurrect for graveyard %u", m_GraveyardId);
}

// For changing graveyard control
void BfGraveyard::GiveControlTo(TeamId team)
{
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BfGraveyard::GiveControlTo: Graveyard %u is now under control of team %u", m_GraveyardId, team);

    // Guide switching
    if (Creature* pSpirit = m_Bf->GetCreature(m_SpiritGuide[1 - team]))
        m_Bf->HideNpc(pSpirit);
    if (Creature* pSpirit = m_Bf->GetCreature(m_SpiritGuide[team]))
    {
        // Only ShowNpc if it's wartime and gy is not neutral
        if (m_Bf->IsWarTime() && team != TEAM_NEUTRAL)
        {
            m_Bf->ShowNpc(pSpirit, false);
            pSpirit->AddAura(BATTLEFIELD_SPELL_SPIRITUAL_IMMUNITY, pSpirit);
        }
        else
            m_Bf->HideNpc(pSpirit);
    }

    m_ControlTeam = team;
    // Teleport to other graveyard, player which were on this graveyard
    RelocateDeadPlayers();
}

void BfGraveyard::RelocateDeadPlayers()
{
    WorldSafeLocsEntry const* closestGrave = NULL;
    for (GuidSet::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
    {
        Player* player = sObjectAccessor->FindPlayer(*itr);
        if (!player)
            continue;

        if (closestGrave)
            player->TeleportTo(player->GetMapId(), closestGrave->x, closestGrave->y, closestGrave->z, player->GetOrientation());
        else
        {
            closestGrave = m_Bf->GetClosestGraveYard(player);
            if (closestGrave)
                player->TeleportTo(player->GetMapId(), closestGrave->x, closestGrave->y, closestGrave->z, player->GetOrientation());
        }
    }
}

bool BfGraveyard::HasNpc(uint64 guid)
{
    if (!m_SpiritGuide[0] && !m_SpiritGuide[1])
        return false;

    if (!m_Bf->GetCreature(m_SpiritGuide[0]) &&
        !m_Bf->GetCreature(m_SpiritGuide[1]))
        return false;

    return (m_SpiritGuide[0] == guid || m_SpiritGuide[1] == guid);
}

// *******************************************************
// *************** End Spirit Guide system ***************
// *******************************************************
// ********************** Misc ***************************
// *******************************************************

void Battlefield::updateOnTaxiPlayers()
{
    // Delayed invite to queue for player who were on taxi
    for (GuidSet::iterator itr = playersOnTaxi.begin(); itr != playersOnTaxi.end();)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
        {
            if (pPlayer->isInFlight())
            {
                ++itr;
                continue;
            }

            InvitePlayerToQueue(pPlayer);
        }

        playersOnTaxi.erase(itr++);
    }
}

Creature* Battlefield::SpawnCreature(uint32 entry, const Position& pos, TeamId team)
{
    return SpawnCreature(entry, pos.m_positionX, pos.m_positionY, pos.m_positionZ, pos.m_orientation, team);
}

Creature* Battlefield::SpawnCreature(uint32 entry, float x, float y, float z, float o, TeamId team)
{
    //Get map object
    Map* map = sMapMgr->CreateBaseMap(m_MapId);
    if (!map)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::SpawnCreature: Can't create creature entry: %u map not found", entry);
        return 0;
    }

    CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(entry);
    if (!cinfo)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::SpawnCreature: entry %u does not exist.", entry);
        return NULL;
    }

    Creature* creature = new Creature();
    if (!creature->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), map, PHASEMASK_NORMAL, entry, 0, team, x, y, z, o))
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::SpawnCreature: Can't create creature entry: %u", entry);
        delete creature;
        return NULL;
    }

    creature->SetHomePosition(x, y, z, o);

    // force using DB speeds -- do we really need this?
    creature->SetSpeed(MOVE_WALK, cinfo->speed_walk);
    creature->SetSpeed(MOVE_RUN, cinfo->speed_run);

    // Set creature in world
    map->AddToMap(creature);
    creature->setActive(true);

    return creature;
}

// Method for spawning gameobject on map
GameObject* Battlefield::SpawnGameObject(uint32 entry, float x, float y, float z, float o)
{
    // Get map object
    Map* map = sMapMgr->CreateBaseMap(571); // *vomits*
    if (!map)
        return 0;

    // Create gameobject
    GameObject* go = new GameObject;
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, map, PHASEMASK_NORMAL, x, y, z, o, 0, 0, 0, 0, 100, GO_STATE_READY))
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::SpawnGameObject: Gameobject template %u not found in database! Battlefield not created!", entry);
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battlefield::SpawnGameObject: Cannot create gameobject template %u! Battlefield not created!", entry);
        delete go;
        return NULL;
    }

    // Add to world
    map->AddToMap(go);
    go->setActive(true);

    return go;
}

Creature* Battlefield::GetCreature(uint64 GUID)
{
    if (!m_Map)
        return NULL;
    return m_Map->GetCreature(GUID);
}

GameObject* Battlefield::GetGameObject(uint64 GUID)
{
    if (!m_Map)
        return NULL;
    return m_Map->GetGameObject(GUID);
}

// *******************************************************
// ******************* CapturePoint **********************
// *******************************************************

BfCapturePoint::BfCapturePoint(Battlefield* battlefield) : m_Bf(battlefield), m_capturePointGUID(0)
{
    m_team = TEAM_NEUTRAL;
    m_value = 0;
    m_minValue = 0.0f;
    m_maxValue = 0.0f;
    m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL;
    m_OldState = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL;
    m_capturePointEntry = 0;
    m_neutralValuePct = 0;
    m_maxSpeed = 0;
}

bool BfCapturePoint::HandlePlayerEnter(Player* player)
{
    if (m_capturePointGUID)
    {
        if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
        {
            player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldState1, 1);
            player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate2, uint32(ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f)));
            player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate3, m_neutralValuePct);
        }
    }

    return m_activePlayers[player->GetTeamId()].insert(player->GetGUID()).second;
}

bool BfCapturePoint::HandlePlayerLeave(Player* player)
{
    if (m_capturePointGUID)
        if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
            player->SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldState1, 0);

    GuidSet::iterator current = m_activePlayers[player->GetTeamId()].find(player->GetGUID());
    if (current != m_activePlayers[player->GetTeamId()].end())
    {
        m_activePlayers[player->GetTeamId()].erase(current);
        return true;
    }
    return false;
}

void BfCapturePoint::SendChangePhase()
{
    if (!m_capturePointGUID)
        return;

    if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
    {
        // send this too, sometimes the slider disappears, dunno why :(
        SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldState1, 1);
        // send these updates to only the ones in this objective
        SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate2, (uint32) ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f));
        // send this too, sometimes it resets :S
        SendUpdateWorldState(capturePoint->GetGOInfo()->capturePoint.worldstate3, m_neutralValuePct);
    }
}

bool BfCapturePoint::SetCapturePointData(GameObject* capturePoint)
{
    ASSERT(capturePoint);
    
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Creating capture point %u", capturePoint->GetEntry());
    
    m_capturePointGUID = MAKE_NEW_GUID(capturePoint->GetGUIDLow(), capturePoint->GetEntry(), HIGHGUID_GAMEOBJECT);
    
    // check info existence
    GameObjectTemplate const* goinfo = capturePoint->GetGOInfo();
    if (goinfo->type != GAMEOBJECT_TYPE_CAPTURE_POINT)
    {
        sLog->outDebug(LOG_FILTER_NONE, "OutdoorPvP: GO %u is not capture point!", capturePoint->GetEntry());
        return false;
    }
    
    // get the needed values from goinfo
    m_maxValue = goinfo->capturePoint.maxTime;
    m_maxSpeed = m_maxValue / (goinfo->capturePoint.minTime ? goinfo->capturePoint.minTime : 60);
    m_neutralValuePct = goinfo->capturePoint.neutralPercent;
    m_minValue = m_maxValue * goinfo->capturePoint.neutralPercent / 100;
    m_capturePointEntry = capturePoint->GetEntry();
    if (m_team == TEAM_ALLIANCE)
    {
        m_value = m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE;
    }
    else
    {
        m_value = -m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE;
    }
    
    return true;
}

GameObject* BfCapturePoint::GetCapturePointGo()
{
    return m_Bf->GetGameObject(m_capturePointGUID);
}

bool BfCapturePoint::DelCapturePoint()
{
    if (m_capturePointGUID)
    {
        if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
        {
            capturePoint->SetRespawnTime(0); // not save respawn time
            capturePoint->Delete();
            capturePoint = NULL;
        }
        m_capturePointGUID = 0;
    }

    return true;
}

bool BfCapturePoint::Update(uint32 diff)
{
    if (!m_capturePointGUID)
        return false;

    if (GameObject* capturePoint = m_Bf->GetGameObject(m_capturePointGUID))
    {
        float radius = capturePoint->GetGOInfo()->capturePoint.radius;

        for (uint8 team = 0; team < 2; ++team)
        {
            for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end();)
            {
                if (Player* player = sObjectAccessor->FindPlayer(*itr++))
                {
                    if (!capturePoint->IsWithinDistInMap(player, radius) || !player->IsOutdoorPvPActive())
                        HandlePlayerLeave(player);
                }
                else
                    ++itr;
            }
        }

        std::list<Player*> players;
        Trinity::AllWorldObjectsInRange checker(capturePoint, radius);
        Trinity::PlayerListSearcher<Trinity::AllWorldObjectsInRange> searcher(capturePoint, players, checker);
        capturePoint->VisitNearbyWorldObject(radius, searcher);

        for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
            if ((*itr)->IsOutdoorPvPActive())
                if (m_activePlayers[(*itr)->GetTeamId()].insert((*itr)->GetGUID()).second)
                    HandlePlayerEnter(*itr);
    }

    // get the difference of numbers
    float fact_diff = ((float) m_activePlayers[0].size() - (float) m_activePlayers[1].size()) * diff / BATTLEFIELD_OBJECTIVE_UPDATE_INTERVAL;
    if (G3D::fuzzyEq(fact_diff, 0.0f))
        return false;

    uint32 Challenger = 0;
    float maxDiff = m_maxSpeed * diff;

    if (fact_diff < 0)
    {
        // horde is in majority, but it's already horde-controlled -> no change
        if (m_State == BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE && m_value <= -m_maxValue)
            return false;

        if (fact_diff < -maxDiff)
            fact_diff = -maxDiff;

        Challenger = HORDE;
    }
    else
    {
        // ally is in majority, but it's already ally-controlled -> no change
        if (m_State == BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE && m_value >= m_maxValue)
            return false;

        if (fact_diff > maxDiff)
            fact_diff = maxDiff;

        Challenger = ALLIANCE;
    }

    float oldValue = m_value;
    TeamId oldTeam = m_team;

    m_OldState = m_State;

    m_value += fact_diff;

    if (m_value < -m_minValue) // red
    {
        if (m_value < -m_maxValue)
            m_value = -m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE;
        m_team = TEAM_HORDE;
    }
    else if (m_value > m_minValue) // blue
    {
        if (m_value > m_maxValue)
            m_value = m_maxValue;
        m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE;
        m_team = TEAM_ALLIANCE;
    }
    else if (oldValue * m_value <= 0) // grey, go through mid point
    {
        // if challenger is ally, then n->a challenge
        if (Challenger == ALLIANCE)
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE;
        // if challenger is horde, then n->h challenge
        else if (Challenger == HORDE)
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }
    else // grey, did not go through mid point
    {
        // old phase and current are on the same side, so one team challenges the other
        if (Challenger == ALLIANCE && (m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE || m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE))
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE;
        else if (Challenger == HORDE && (m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE || m_OldState == BF_CAPTUREPOINT_OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE))
            m_State = BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE;
        m_team = TEAM_NEUTRAL;
    }

    if (G3D::fuzzyNe(m_value, oldValue))
        SendChangePhase();

    if (m_OldState != m_State)
    {
        //TC_LOG_ERROR("bg.battlefield", "%u->%u", m_OldState, m_State);
        if (oldTeam != m_team)
            ChangeTeam(m_team);
        return true;
    }

    return false;
}

void BfCapturePoint::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (uint8 team = 0; team < 2; ++team)
        for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr) // send to all players present in the area
            if (Player* player = sObjectAccessor->FindPlayer(*itr))
                player->SendUpdateWorldState(field, value);
}

void BfCapturePoint::SendObjectiveComplete(uint32 id, uint64 guid)
{
    uint8 team;
    switch (m_State)
    {
        case BF_CAPTUREPOINT_OBJECTIVESTATE_ALLIANCE:
            team = 0;
            break;
        case BF_CAPTUREPOINT_OBJECTIVESTATE_HORDE:
            team = 1;
            break;
        default:
            return;
    }

    // send to all players present in the area
    for (GuidSet::iterator itr = m_activePlayers[team].begin(); itr != m_activePlayers[team].end(); ++itr)
        if (Player* player = sObjectAccessor->FindPlayer(*itr))
            player->KilledMonsterCredit(id, guid);
}

bool BfCapturePoint::IsInsideObjective(Player* player) const
{
    return m_activePlayers[player->GetTeamId()].find(player->GetGUID()) != m_activePlayers[player->GetTeamId()].end();
}