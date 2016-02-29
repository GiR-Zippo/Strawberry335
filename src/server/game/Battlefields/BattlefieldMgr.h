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
 * The class BattlefieldMgr handles all not instanceable battlegrounds (like BattlegroundMgr)
 * The only bf which is currently handled is wintergrasp
 */

#ifndef BATTLEFIELD_MGR_H_
#define BATTLEFIELD_MGR_H_

#include "ace/Singleton.h"

class Player;
class GameObject;
class Creature;
class ZoneScript;
class Battlefield;
struct GossipMenuItems;


// class to handle player enter / leave / areatrigger / GO use events
class BattlefieldMgr
{
public:
    // ctor
    BattlefieldMgr();
    // dtor
    ~BattlefieldMgr();

    // create battlefield events
    void InitBattlefield();
    // called when a player enters an battlefield area
    void HandlePlayerEnterZone(Player* player, uint32 areaflag);
    // called when player leaves an battlefield area
    void HandlePlayerLeaveZone(Player* player, uint32 areaflag);
    // called when player resurrects
    void HandlePlayerResurrects(Player* player, uint32 areaflag);
    // called when player leaves bf group
    void HandlePlayerLeaveBfGroup(Player* pPlayer, uint32 zoneId);
    // return assigned battlefield
    Battlefield* GetBattlefieldToZoneId(uint32 zoneid);
    Battlefield* GetBattlefieldByBattleId(uint32 battleid);

    ZoneScript* GetZoneScript(uint32 zoneId);

    void AddZone(uint32 zoneid, Battlefield * handle);

    void Update(uint32 diff);

    void HandleGossipOption(Player* player, uint64 guid, uint32 gossipid);

    bool CanTalkTo(Player* player, Creature* creature, GossipMenuItems gso);

    void HandleDropFlag(Player* player, uint32 spellId);

    typedef std::vector < Battlefield * >BattlefieldSet;
    typedef std::map < uint32 /* zoneid */, Battlefield * >BattlefieldMap;
private:
    // contains all initiated battlefield events
    // used when initing / cleaning up
    BattlefieldSet m_BattlefieldSet;
    // maps the zone ids to an battlefield event
    // used in player event handling
    BattlefieldMap m_BattlefieldMap;
    // update interval
    uint32 m_UpdateTimer;
};

#define sBattlefieldMgr ACE_Singleton<BattlefieldMgr, ACE_Null_Mutex>::instance()

#endif
