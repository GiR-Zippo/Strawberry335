#include "AutoBroadcast.h"
#include "Logon.h"
#include "ClientSessionMgr.h"

void AutoBroadcast::load()
{
    uint32 oldMSTime = getMSTime();

    m_Autobroadcasts.clear();

    QueryResult result = WorldDatabase.Query("SELECT text FROM autobroadcast");

    if (!result)
    {
        sLog->outString(">> Loaded 0 autobroadcasts definitions. DB table `autobroadcast` is empty!");
        sLog->outString();
        return;
    }

    uint32 count = 0;

    do
    {

        Field* fields = result->Fetch();
        std::string message = fields[0].GetString();

        m_Autobroadcasts.push_back(message);

        ++count;
    } while (result->NextRow());

    sLog->outString(">> Loaded %u autobroadcasts definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void AutoBroadcast::send()
{
    if (m_Autobroadcasts.empty())
        return;

    std::string msg;

    msg = Trinity::Containers::SelectRandomContainerElement(m_Autobroadcasts);

    uint32 abcenter = sLogon->getIntConfig(CONFIG_AUTOBROADCAST_CENTER);

    if (abcenter == 0)
    {
        //sClientSessionMgr->SendWorldText(LANG_AUTO_BROADCAST, msg.c_str());
        sClientSessionMgr->SendServerMessage(SERVER_MSG_STRING, msg.c_str());
    }
    else if (abcenter == 1)
    {
        WorldPacket data(SMSG_NOTIFICATION, (msg.size() + 1));
        data << msg;
        sClientSessionMgr->SendGlobalMessage(&data);
    }

    else if (abcenter == 2)
    {
        //sLogon->SendWorldText(LANG_AUTO_BROADCAST, msg.c_str());
        sClientSessionMgr->SendServerMessage(SERVER_MSG_STRING, msg.c_str());

        WorldPacket data(SMSG_NOTIFICATION, (msg.size() + 1));
        data << msg;
        sClientSessionMgr->SendGlobalMessage(&data);
    }

    sLog->outDetail("AutoBroadcast: '%s'", msg.c_str());
}