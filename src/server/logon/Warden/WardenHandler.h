/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
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

#ifndef _WARDEN_HANDLER_H
#define _WARDEN_HANDLER_H

#include "ARC4.h"
#include <map>
#include "BigNumber.h"
#include "ByteBuffer.h"

enum WardenOpcodes
{
    // Client->Server
    WARDEN_CMSG_HASMODULE_NO_ANSWER             = 0,
    WARDEN_CMSG_HASMODULE_YES_ANSWER            = 1,
    WARDEN_CMSG_MODULE_REQUEST_REPLY            = 2,
    WARDEN_CMSG_MEM_CHECK_REPLY                 = 3,        // only sent if MEM_CHECK bytes doesn't match
    WARDEN_CMSG_HASH_REQUEST_REPLY              = 4,
    WARDEN_CMSG_MODULE_FAILED                   = 5,        // this is sent when client failed to load uploaded module due to cache fail

    // Server->Client
    WARDEN_SMSG_HASMODULE_REQUEST               = 0,
    WARDEN_SMSG_MODULE_TRANSMIT                 = 1,
    WARDEN_SMSG_MODULE_REQUEST_DATA             = 2,
    WARDEN_SMSG_MODULE_INITIALIZE               = 3,
    WARDEN_SMSG_MEM_CHECK_REQUEST               = 4,        // byte len; whole(!EOF) { byte unk(1); byte index(++); string module(can be 0); int offset; byte len; byte[] bytes_to_compare[len]; }
    WARDEN_SMSG_HASH_REQUEST                    = 5
};

enum WardenCheckType
{
    MEM_CHECK               = 0xF3,                         // byte moduleNameIndex + uint Offset + byte Len (check to ensure memory isn't modified)
    PAGE_CHECK_A            = 0xB2,                         // uint Seed + byte[20] SHA1 + uint Addr + byte Len (scans all pages for specified hash)
    PAGE_CHECK_B            = 0xBF,                         // uint Seed + byte[20] SHA1 + uint Addr + byte Len (scans only pages starts with MZ+PE headers for specified hash)
    MPQ_CHECK               = 0x98,                         // byte fileNameIndex (check to ensure MPQ file isn't modified)
    LUA_STR_CHECK           = 0x8B,                         // byte luaNameIndex (check to ensure LUA string isn't used)
    DRIVER_CHECK            = 0x71,                         // uint Seed + byte[20] SHA1 + byte driverNameIndex (check to ensure driver isn't loaded)
    TIMING_CHECK            = 0x57,                         // empty (check to ensure GetTickCount() isn't detoured)
    PROC_CHECK              = 0x7E,                         // uint Seed + byte[20] SHA1 + byte moluleNameIndex + byte procNameIndex + uint Offset + byte Len (check to ensure proc isn't detoured)
    MODULE_CHECK            = 0xD9,                         // uint Seed + byte[20] SHA1 (check to ensure module isn't injected)
};

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

struct WardenHasModuleRequest
{
    uint8 Command;
    uint8 Module_Id[16];
    uint8 Module_Key[16];
    uint32 Size;
};

struct WardenModuleTransfer
{
    uint8 Command;
    uint16 DataSize;
    uint8 Data[500];
};

struct WardenInitModuleRequest
{
    uint8 Command1;
    uint16 Size1;
    uint32 CheckSumm1;
    uint8 Unk1;
    uint8 Unk2;
    uint8 Type;
    uint8 String_library1;
    uint32 Function1[4];

    uint8 Command2;
    uint16 Size2;
    uint32 CheckSumm2;
    uint8 Unk3;
    uint8 Unk4;
    uint8 String_library2;
    uint32 Function2;
    uint8 Function2_set;

    uint8 Command3;
    uint16 Size3;
    uint32 CheckSumm3;
    uint8 Unk5;
    uint8 Unk6;
    uint8 String_library3;
    uint32 Function3;
    uint8 Function3_set;
};

struct WardenHashRequest
{
    uint8 Command;
    uint8 Seed[16];
};

#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

struct ClientWardenModule
{
    uint8 ID[16];
    uint8 Key[16];
    uint32 CompressedSize;
    uint8 *CompressedData;
};

struct WardenData
{
    uint8 Type;
    BigNumber i;
    uint32 Address;                                         // PROC_CHECK, MEM_CHECK, PAGE_CHECK
    uint8 Length;                                           // PROC_CHECK, MEM_CHECK, PAGE_CHECK
    std::string str;                                        // LUA, MPQ, DRIVER
};

struct WardenDataResult
{
    BigNumber res;                                          // MEM_CHECK
};

class ClientSession;

class Warden
{
    public:
        Warden();
        ~Warden();

        void Init(ClientSession *pClient, BigNumber *K);
        void Update();
        bool Initialized() { return m_initialized; }
        void DecryptData(uint8 *Buffer, uint32 Len);
        void EncryptData(uint8 *Buffer, uint32 Len);
        void SendModuleToClient();
        void PrintHexArray(const char *Before, const uint8 *Buffer, uint32 Len, bool BreakWithNewline);
        void LoadModule();
        void RequestModule();
        void InitializeModule();
        void RequestHash();
        void RequestHashReply(ByteBuffer &buff);
        void RequestData();
        void HandleData(ByteBuffer &buff);
        bool IsValidCheckSum(uint32 checksum, const uint8 * Data, const uint16 Lenght);
        uint32 BuildChecksum(const uint8* data, uint32 dataLen);

    private:
        ClientSession * Client;
        uint8 InputKey[16];
        uint8 OutputKey[16];
        uint8 Seed[16];
        ARC4 iCrypto;
        ARC4 oCrypto;
        uint32 ServerTicks;
        uint32 m_WardenCheckTimer;                          // timer between data packets
        bool m_WardenDataSent;
        uint32 m_WardenKickTimer;                           // time after send packet
        uint32 m_WardenTimer;
        ClientWardenModule *Module;
        std::vector<uint32> SendDataId;
        std::vector<uint32> MemCheck;
        bool m_initialized;
};

class CWardenModuleStorage
{
    public:
        CWardenModuleStorage();
        ~CWardenModuleStorage();
        uint32 InternalDataID;
        std::vector<uint32> memcheckid;

    private:
        uint32 InternalModuleIDHigh;
        std::map<uint32, ClientWardenModule*> _modules;

        //data
        std::map<uint32, WardenData*> _data_map;
        std::map<uint32, WardenDataResult*> _result_map;

    public:
        ClientWardenModule *GetModuleForClient(ClientSession *Session);
        inline uint32 GenerateInternalModuleID() { return InternalModuleIDHigh++; }
        inline uint32 GenerateInternalDataID() { return InternalDataID++; }
        WardenData *GetWardenDataById(uint32 Id);
        WardenDataResult *GetWardenResultById(uint32 Id);
        void Init();

    protected:
        ClientWardenModule *LoadModule(const char *FileNameMod);
        void LoadWardenDataResult();
};

extern CWardenModuleStorage WardenModuleStorage;
#endif

