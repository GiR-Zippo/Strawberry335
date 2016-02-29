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

#include "BattlefieldMgr.h"
#include "Main_BattlefieldWG.h"
#include "ObjectMgr.h"
#include "Player.h"


BattlefieldMgr::BattlefieldMgr()
{
    m_UpdateTimer = 0;
    //TC_LOG_DEBUG("bg.battlefield", "Instantiating BattlefieldMgr");
}

BattlefieldMgr::~BattlefieldMgr()
{
    //TC_LOG_DEBUG("bg.battlefield", "Deleting BattlefieldMgr");
    for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
        delete *itr;
}

void BattlefieldMgr::InitBattlefield()
{
    uint32 oldMSTime = getMSTime();

    if (sWorld->getBoolConfig(CONFIG_WINTERGRASP_ENABLE))
    {
        Battlefield* pBf = new BattlefieldWG;
        // respawn, init variables
        if (!pBf->SetupBattlefield())
        {
            sLog->outString("ERROR: Wintergrasp init failed");
            delete pBf;
        }
        else
            m_BattlefieldSet.push_back(pBf);
    }

    if (m_BattlefieldSet.size() == 0)
        sLog->outString(">> No Battlefields have been loaded");
    else
        sLog->outString(">> Loaded %u %s in %u ms", m_BattlefieldSet.size(),
                        m_BattlefieldSet.size() == 1 ? "Battlefield" : "Battlefields", GetMSTimeDiffToNow(oldMSTime));

    sLog->outString();
}

void BattlefieldMgr::AddZone(uint32 zoneid, Battlefield *handle)
{
    m_BattlefieldMap[zoneid] = handle;
}

void BattlefieldMgr::HandlePlayerEnterZone(Player* player, uint32 zoneid)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneid);
    if (itr == m_BattlefieldMap.end())
        return;

    Battlefield* bf = itr->second;
    if (bf->HasPlayer(player) || !bf->IsEnabled())
        return;

    bf->HandlePlayerEnterZone(player, zoneid);
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Player %u entered battlefield id %u", player->GetGUIDLow(), bf->GetTypeId());
}

void BattlefieldMgr::HandlePlayerLeaveZone(Player* player, uint32 zoneid)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneid);
    if (itr == m_BattlefieldMap.end())
        return;

    // teleport: remove once in removefromworld, once in updatezone
    if (!itr->second->HasPlayer(player))
        return;
    itr->second->HandlePlayerLeaveZone(player, zoneid);
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Player %u left battlefield id %u", player->GetGUIDLow(), itr->second->GetTypeId());
}

void BattlefieldMgr::HandlePlayerLeaveBfGroup(Player* pPlayer, uint32 zoneid)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneid);
    if (itr == m_BattlefieldMap.end())
        return;

    if (!itr->second->HasPlayer(pPlayer))
        return;
    itr->second->HandlePlayerLeaveBfGroup(pPlayer);
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Player %u left bf group of battlefield id %u", pPlayer->GetGUIDLow(), itr->second->GetTypeId());
}

Battlefield *BattlefieldMgr::GetBattlefieldToZoneId(uint32 zoneid)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneid);
    if (itr == m_BattlefieldMap.end())
    {
        // no handle for this zone, return
        return NULL;
    }
    if (!itr->second->IsEnabled())
        return NULL;
    return itr->second;
}

Battlefield *BattlefieldMgr::GetBattlefieldByBattleId(uint32 battleid)
{
    for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
    {
        if ((*itr)->GetBattleId() == battleid)
            return (*itr);
    }
    return NULL;
}

void BattlefieldMgr::Update(uint32 diff)
{
    m_UpdateTimer += diff;
    if (m_UpdateTimer > BATTLEFIELD_OBJECTIVE_UPDATE_INTERVAL)
    {
        for (BattlefieldSet::iterator itr = m_BattlefieldSet.begin(); itr != m_BattlefieldSet.end(); ++itr)
            if ((*itr)->IsEnabled())
                (*itr)->Update(m_UpdateTimer);
        m_UpdateTimer = 0;
    }
}

ZoneScript* BattlefieldMgr::GetZoneScript(uint32 zoneId)
{
    BattlefieldMap::iterator itr = m_BattlefieldMap.find(zoneId);
    if (itr != m_BattlefieldMap.end())
        return itr->second;
    else
        return NULL;
}
