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

#include "Common.h"
#include "RealmList.h"
#include "Database/DatabaseEnv.h"

static RealmBuildInfo ExpectedRealmdClientBuilds[] =
{
    {16135, 5, 0, 5, 'b'},                              // highest supported build, also auto accept all above for simplify future supported builds testing
    {12340, 3, 3, 5, 'a'},
    {11723, 3, 3, 3, 'a'},
    {11403, 3, 3, 2, ' '},
    {11159, 3, 3, 0, 'a'},
    {10505, 3, 2, 2, 'a'},
    {8606,  2, 4, 3, ' '},
    {6141,  1, 12, 3, ' '},
    {6005,  1, 12, 2, ' '},
    {5875,  1, 12, 1, ' '},
    {0,     0, 0, 0, ' '}                                   // terminator
};

RealmBuildInfo const* FindBuildInfo(uint16 _build)
{
    // first build is low bound of always accepted range
    if (_build >= ExpectedRealmdClientBuilds[0].build)
        return &ExpectedRealmdClientBuilds[0];

    // continue from 1 with explicit equal check
    for (int i = 1; ExpectedRealmdClientBuilds[i].build; ++i)
        if (_build == ExpectedRealmdClientBuilds[i].build)
            return &ExpectedRealmdClientBuilds[i];

    // none appropriate build
    return NULL;
}

RealmList::RealmList() : m_UpdateInterval(0), m_NextUpdateTime(time(NULL)) { }

// Load the realm list from the database
void RealmList::Initialize(uint32 updateInterval)
{
    m_UpdateInterval = updateInterval;

    // Get the content of the realmlist table in the database
    UpdateRealms(true);
}

void RealmList::UpdateRealm(uint32 ID, const std::string& name, const std::string& address, uint16 port, uint8 icon, RealmFlags flag, uint8 timezone, AccountTypes allowedSecurityLevel, float popu, uint32 build)
{
    ///- Create new if not exist or update existed
    Realm& realm = m_realms[name];

    realm.m_ID       = ID;
    realm.icon       = icon;
    realm.flag       = flag;
    realm.timezone   = timezone;
    realm.allowedSecurityLevel = allowedSecurityLevel;
    realm.populationLevel      = popu;

    realm.realmbuilds.insert(build);
    uint16 first_build = !realm.realmbuilds.empty() ? *realm.realmbuilds.begin() : 0;

    realm.realmBuildInfo.build = first_build;
    realm.realmBuildInfo.major_version = 0;
    realm.realmBuildInfo.minor_version = 0;
    realm.realmBuildInfo.bugfix_version = 0;
    realm.realmBuildInfo.hotfix_version = ' ';

    if (first_build)
        if (RealmBuildInfo const* bInfo = FindBuildInfo(first_build))
            if (bInfo->build == first_build)
                realm.realmBuildInfo = *bInfo;

    ///- Append port to IP address.
    std::ostringstream ss;
    ss << address << ":" << port;
    realm.address   = ss.str();
}

void RealmList::UpdateIfNeed()
{
    // maybe disabled or updated recently
    if (!m_UpdateInterval || m_NextUpdateTime > time(NULL))
        return;

    m_NextUpdateTime = time(NULL) + m_UpdateInterval;

    // Clears Realm list
    m_realms.clear();

    // Get the content of the realmlist table in the database
    UpdateRealms();
}

void RealmList::UpdateRealms(bool init)
{
    sLog->outDetail("Updating Realm List...");

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMLIST);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    // Circle through results and add them to the realm map
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 realmId             = fields[0].GetUInt32();
            const std::string& name    = fields[1].GetString();
            const std::string& address = fields[2].GetString();
            uint16 port                = fields[3].GetUInt16();
            uint8 icon                 = fields[4].GetUInt8();
            RealmFlags flag            = RealmFlags(fields[5].GetUInt8());
            uint8 timezone             = fields[6].GetUInt8();
            uint8 allowedSecurityLevel = fields[7].GetUInt8();
            float pop                  = fields[8].GetFloat();
            uint32 build               = fields[9].GetUInt32();

            UpdateRealm(realmId, name, address, port, icon, flag, timezone, (allowedSecurityLevel <= SEC_ADMINISTRATOR ? AccountTypes(allowedSecurityLevel) : SEC_ADMINISTRATOR), pop, build);

            if (init)
                sLog->outString("Added realm \"%s\".", fields[1].GetCString());
        }
        while (result->NextRow());
    }
}
