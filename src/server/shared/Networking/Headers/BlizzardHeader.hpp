#ifndef __BLIZZARDHEADER_HPP
#define __BLIZZARDHEADER_HPP

#pragma once

#include <ace/Basic_Types.h>

#include "Log.h"

namespace Blizzard
{
    #if defined(__GNUC__)
        #pragma pack(1)
    #else
        #pragma pack(push, 1)
    #endif

    struct ServerPktHeader
    {
        ServerPktHeader(ACE_UINT32 size, ACE_UINT16 cmd) : size(size)
        {
            ACE_UINT8 headerIndex = 0;
            if (isLargePacket())
            {
                sLog->outDebug(LOG_FILTER_NETWORKIO, "initializing large server to client packet. Size: %u, cmd: %u", size, cmd);
                header[headerIndex++] = 0x80 | (0xFF & (size>>16));
            }
            header[headerIndex++] = 0xFF & (size>>8);
            header[headerIndex++] = 0xFF & size;

            header[headerIndex++] = 0xFF & cmd;
            header[headerIndex++] = 0xFF & (cmd>>8);
        }

        ACE_UINT8 getHeaderLength()
        {
            // cmd = 2 bytes, size= 2||3bytes
            return 2 + (isLargePacket() ? 3 : 2);
        }

        bool isLargePacket() { return size > 0x7FFF; }

        const ACE_UINT32 size;
        ACE_UINT8 header[5];
    };

    struct ClientPktHeader
    {
        ACE_UINT16 size;
        ACE_UINT32 cmd;
    };

    #if defined(__GNUC__)
        #pragma pack()
    #else
        #pragma pack(pop)
    #endif
}

#endif /* __BLIZZARDHEADER_HPP */
