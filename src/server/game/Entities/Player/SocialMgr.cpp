/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "SocialMgr.h"

#include "DatabaseEnv.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "World.h"
#include "Util.h"
#include "AccountMgr.h"

PlayerSocial::PlayerSocial()
{
    m_playerGUID = 0;
}

PlayerSocial::~PlayerSocial()
{
    m_playerSocialMap.clear();
}

uint32 PlayerSocial::GetNumberOfSocialsWithFlag(SocialFlag flag)
{
    uint32 counter = 0;
    for (PlayerSocialMap::const_iterator itr = m_playerSocialMap.begin(); itr != m_playerSocialMap.end(); ++itr)
        if (itr->second.Flags & flag)
            ++counter;

    return counter;
}

bool PlayerSocial::AddToSocialList(uint32 friendGuid, bool ignore)
{
    // check client limits
    if (ignore)
    {
        if (GetNumberOfSocialsWithFlag(SOCIAL_FLAG_IGNORED) >= SOCIALMGR_IGNORE_LIMIT)
            return false;
    }
    else
    {
        if (GetNumberOfSocialsWithFlag(SOCIAL_FLAG_FRIEND) >= SOCIALMGR_FRIEND_LIMIT)
            return false;
    }

    uint8 flag = SOCIAL_FLAG_FRIEND;
    if (ignore)
        flag = SOCIAL_FLAG_IGNORED;

    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr != m_playerSocialMap.end())
    {
        m_playerSocialMap[friendGuid].Flags |= flag;
    }
    else
    {
        FriendInfo fi;
        fi.Flags |= flag;
        m_playerSocialMap[friendGuid] = fi;
    }
    return true;
}

void PlayerSocial::RemoveFromSocialList(uint32 friendGuid, bool ignore)
{
    PlayerSocialMap::iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr == m_playerSocialMap.end())                     // not exist
        return;

    uint8 flag = SOCIAL_FLAG_FRIEND;
    if (ignore)
        flag = SOCIAL_FLAG_IGNORED;

    itr->second.Flags &= ~flag;
    if (itr->second.Flags == 0)
    {
        m_playerSocialMap.erase(itr);
    }
}

void PlayerSocial::SetFriendNote(uint32 friendGuid, std::string note)
{
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr == m_playerSocialMap.end())                     // not exist
        return;

    utf8truncate(note, 48);                                  // DB and client size limitation

    m_playerSocialMap[friendGuid].Note = note;
}

void PlayerSocial::SendSocialList(Player* player)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CONTACT_LIST");
}

bool PlayerSocial::HasFriend(uint32 friend_guid)
{
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(friend_guid);
    if (itr != m_playerSocialMap.end())
        return itr->second.Flags & SOCIAL_FLAG_FRIEND;
    return false;
}

bool PlayerSocial::HasIgnore(uint32 ignore_guid)
{
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(ignore_guid);
    if (itr != m_playerSocialMap.end())
        return itr->second.Flags & SOCIAL_FLAG_IGNORED;
    return false;
}

SocialMgr::SocialMgr()
{
}

SocialMgr::~SocialMgr()
{
}

void SocialMgr::GetFriendInfo(Player* player, uint32 friendGUID, FriendInfo &friendInfo)
{
    if (!player)
        return;

    friendInfo.Status = FRIEND_STATUS_OFFLINE;
    friendInfo.Area = 0;
    friendInfo.Level = 0;
    friendInfo.Class = 0;

    Player* pFriend = ObjectAccessor::FindPlayer(friendGUID);
    if (!pFriend)
        return;

    uint32 team = player->GetTeam(true);
    AccountTypes security = player->GetSession()->GetSecurity();
    bool allowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);
    AccountTypes gmLevelInWhoList = AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST));

    PlayerSocialMap::iterator itr = player->GetSocial()->m_playerSocialMap.find(friendGUID);
    if (itr != player->GetSocial()->m_playerSocialMap.end())
        friendInfo.Note = itr->second.Note;

    // PLAYER see his team only and PLAYER can't see MODERATOR, GAME MASTER, ADMINISTRATOR characters
    // MODERATOR, GAME MASTER, ADMINISTRATOR can see all
    if (pFriend && pFriend->GetName() &&
        (!AccountMgr::IsPlayerAccount(security) ||
        ((pFriend->GetTeam() == team || allowTwoSideWhoList) && (pFriend->GetSession()->GetSecurity() <= gmLevelInWhoList))) &&
        pFriend->IsVisibleGloballyFor(player))
    {
        friendInfo.Status = FRIEND_STATUS_ONLINE;
        if (pFriend->isAFK())
            friendInfo.Status = FRIEND_STATUS_AFK;
        if (pFriend->isDND())
            friendInfo.Status = FRIEND_STATUS_DND;

        if (pFriend->InArena()) // <TehKevin> If player is in arena, send dalaran
            friendInfo.Area = 4395;
        else
            friendInfo.Area = pFriend->GetZoneId();
        
        friendInfo.Level = pFriend->getLevel();
        friendInfo.Class = pFriend->getClass();
    }
}

void SocialMgr::MakeFriendStatusPacket(FriendsResult result, uint32 guid, WorldPacket* data)
{
    data->Initialize(SMSG_FRIEND_STATUS, 5);
    *data << uint8(result);
    *data << uint64(guid);
}

void SocialMgr::SendFriendStatus(Player* player, FriendsResult result, uint32 friend_guid, bool broadcast)
{
}

void SocialMgr::BroadcastToFriendListers(Player* player, WorldPacket* packet)
{
}

PlayerSocial* SocialMgr::LoadFromDB(PreparedQueryResult result, uint32 guid)
{
    PlayerSocial *social = &m_socialMap[guid];
    social->SetPlayerGUID(guid);

    if (!result)
        return social;

    uint32 friend_guid = 0;
    uint8 flags = 0;
    std::string note = "";

    do
    {
        Field* fields = result->Fetch();

        friend_guid = fields[0].GetUInt32();
        flags = fields[1].GetUInt8();
        note = fields[2].GetString();

        social->m_playerSocialMap[friend_guid] = FriendInfo(flags, note);

        // client's friends list and ignore list limit
        if (social->m_playerSocialMap.size() >= (SOCIALMGR_FRIEND_LIMIT + SOCIALMGR_IGNORE_LIMIT))
            break;
    }
    while (result->NextRow());

    return social;
}

