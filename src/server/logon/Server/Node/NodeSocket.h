#ifndef __NODESOCKET_H
#define __NODESOCKET_H

#include <ace/Svc_Handler.h>
#include <ace/Unbounded_Queue.h>
#include <ace/SOCK_Connector.h>
#include <ace/Connector.h>

#include "Socket.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Common.h"
#include "AuthCrypt.h"
#include "BigNumber.h"
#include "RoutingHelper.h"
#include "SocketConnector.h"

class ACE_Message_Block;
class WorldPacket;

class NodeSocket : public Socket
{
public:
    NodeSocket();

private:
    virtual int _sendPacket(const WorldPacket &pct);
    virtual int _handleInputHeader();
    virtual int _processIncoming(WorldPacket *new_pct);

    AuthCrypt _crypt;
    uint32 _seed;
};

class NodeSocketConnector : public SocketConnector<NodeSocket>
{
public:
    NodeSocket * OpenConnection(uint32 nodeID)
    {
        if (!sRoutingHelper->CheckNodeID(nodeID))
            return false;

        NodeList nodeInfo = sRoutingHelper->GetNodeConnectionData(nodeID);

        return SocketConnector<NodeSocket>::OpenConnection(nodeInfo.port, nodeInfo.IP);
    }
};

#define sNodeSocketConnector ACE_Singleton<NodeSocketConnector, ACE_Thread_Mutex>::instance()

#endif
