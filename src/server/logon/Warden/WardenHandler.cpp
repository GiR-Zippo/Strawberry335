/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>;
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>;
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

#include "HMACSHA1.h"
#include "WardenKeyGeneration.h"
#include "Common.h"
#include "WorldPacket.h"
#include "ClientSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "WardenHandler.h"
#include "ByteBuffer.h"
#include <openssl/md5.h>
#include "Database/DatabaseEnv.h"
#include "Logon.h"
#include "Player.h"
#include "Util.h"

#ifndef _WIN32
#include <dirent.h>;
#include <errno.h>;
#endif

CWardenModuleStorage WardenModuleStorage;

void Warden::LoadModule()
{
    Module = WardenModuleStorage.GetModuleForClient(Client);
}

void Warden::PrintHexArray(const char *Before, const uint8 *Buffer, uint32 Len, bool BreakWithNewline)
{
    printf(Before);
    for (uint32 i = 0; i < Len; ++i)
        printf("%02X ", Buffer[i]);
    if (BreakWithNewline)
        printf("\n");
}

Warden::Warden() : iCrypto(16), oCrypto(16), m_WardenCheckTimer(10000/*10 sec*/), m_WardenKickTimer(0), m_WardenDataSent(false), m_initialized(false)
{
}

Warden::~Warden()
{
    Module = NULL;
    m_initialized = false;
}

void Warden::Init(ClientSession *pClient, BigNumber *K)
{
    Client = pClient;

    QueryResult result = LoginDatabase.PQuery("SELECT os FROM account WHERE id = '%u'", Client->GetAccountId());
    if (result)
    {
        Field* fields = result->Fetch();
        uint32 os = fields[0].GetUInt32();
        if (os != 0x0057696E) // 0x0057696E = \0niW => 'Win'
        {
            if (os != 0x004F5358) // Mac
            {
                sLog->outWarden("Not supported os %u from %u", os, Client->GetAccountId());
                Client->KickPlayer();
            }
            return;
        }
    }
    else
        return;

    // Generate Warden Key
    SHA1Randx WK(K->AsByteArray(), K->GetNumBytes());
    WK.Generate(InputKey, 16);
    WK.Generate(OutputKey, 16);
    /*
    Seed: 4D808D2C77D905C41A6380EC08586AFE (0x05 packet)
    Hash: 568C054C781A972A6037A2290C22B52571A06F4E (0x04 packet)
    Module MD5: 79C0768D657977D697E10BAD956CCED1
    New Client Key: 7F 96 EE FD A5 B6 3D 20 A4 DF 8E 00 CB F4 83 04
    New Cerver Key: C2 B7 AD ED FC CC A9 C2 BF B3 F8 56 02 BA 80 9B
    */
    uint8 mod_seed[16] = { 0x4D, 0x80, 0x8D, 0x2C, 0x77, 0xD9, 0x05, 0xC4, 0x1A, 0x63, 0x80, 0xEC, 0x08, 0x58, 0x6A, 0xFE };

    memcpy(Seed, mod_seed, 16);

    iCrypto.Init(InputKey);
    oCrypto.Init(OutputKey);

    LoadModule();

    if (!Module)
    {
        sLog->outWarden("Could not find a module appropriate for %u. Aborting.", Client->GetAccountId());
        return;
    }

    sLog->outWarden("Requesting status of module %02X from the client %u ...", Module->ID, Client->GetAccountId());
    RequestModule();
}

void Warden::Update()
{
    if (m_initialized)
    {
        uint32 ticks = getMSTime();
        uint32 diff = ticks - m_WardenTimer;
        m_WardenTimer = ticks;

        if (m_WardenDataSent)
        {
            // 1.5 minutes after send packet
            if (m_WardenKickTimer > sLogon->getIntConfig(CONFIG_WARDEN_CLIENT_RESPONSE_DELAY) * IN_MILLISECONDS)
            {
                sLog->outWarden("No response from %u within 1.5 minutes.", Client->GetAccountId());
                Client->KickPlayer();
            }
            else
                m_WardenKickTimer += diff;
        }
        else if (m_WardenCheckTimer > 0)
        {
            if (diff >= m_WardenCheckTimer)
            {
                RequestData();
                // 25-35 second
                uint32 holdOff = sLogon->getIntConfig(CONFIG_WARDEN_CLIENT_CHECK_HOLDOFF);
                m_WardenCheckTimer = (holdOff < 1 ? 1 : holdOff) * IN_MILLISECONDS;
            }
            else
                m_WardenCheckTimer -= diff;
        }
    }
}

void Warden::DecryptData(uint8 *Buffer, uint32 Len)
{
    iCrypto.UpdateData(Len, Buffer);
}

void Warden::EncryptData(uint8 *Buffer, uint32 Len)
{
    oCrypto.UpdateData(Len, Buffer);
}

void ClientSession::HandleWardenDataOpcode(WorldPacket & recv_data)
{
    m_Warden.DecryptData(const_cast<uint8*>(recv_data.contents()), recv_data.size());
    uint8 Opcode;
    recv_data >> Opcode;

    switch(Opcode)
    {
        case WARDEN_CMSG_HASMODULE_NO_ANSWER:
            m_Warden.SendModuleToClient();
            break;
        case WARDEN_CMSG_HASMODULE_YES_ANSWER:
            m_Warden.RequestHash();
            break;
        case WARDEN_CMSG_MODULE_REQUEST_REPLY:
            m_Warden.HandleData(recv_data);
            break;
        case WARDEN_CMSG_HASH_REQUEST_REPLY:
            m_Warden.RequestHashReply(recv_data);
            m_Warden.InitializeModule();
            break;
        case WARDEN_CMSG_MODULE_FAILED:
        case WARDEN_CMSG_MEM_CHECK_REPLY:
            sLog->outWarden("Got NYI opcode %02X of size %u from %u.", Opcode, recv_data.size() - 1, GetAccountId());
            break;
        default:
            sLog->outWarden("Got unknown opcode %02X of size %u from %u.", Opcode, recv_data.size() - 1, GetAccountId());
            break;
    }
}

void Warden::SendModuleToClient()
{
    WardenModuleTransfer pkt;

    uint32 size_left = Module->CompressedSize;
    uint32 pos = 0;
    uint16 burst_size;
    while (size_left > 0)
    {
        burst_size = size_left < 500 ? size_left : 500;
        pkt.Command = WARDEN_SMSG_MODULE_TRANSMIT;
        pkt.DataSize = burst_size;
        memcpy(pkt.Data, &Module->CompressedData[pos], burst_size);
        size_left -= burst_size;
        pos += burst_size;

        EncryptData((uint8*)&pkt, burst_size + 3);
        WorldPacket pkt1(SMSG_WARDEN_DATA, burst_size + 3);
        pkt1.append((uint8*)&pkt, burst_size + 3);
        Client->SendPacket(&pkt1);
    }
}

void Warden::RequestModule()
{
    // Create packet structure
    WardenHasModuleRequest Request;
    Request.Command = WARDEN_SMSG_HASMODULE_REQUEST;

    memcpy(Request.Module_Id, Module->ID, 16);
    memcpy(Request.Module_Key, Module->Key, 16);
    Request.Size = Module->CompressedSize;

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenHasModuleRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenHasModuleRequest));
    pkt.append((uint8*)&Request, sizeof(WardenHasModuleRequest));
    Client->SendPacket(&pkt);
}

void Warden::InitializeModule()
{
    // Create packet structure
    WardenInitModuleRequest Request;
    Request.Command1 = WARDEN_SMSG_MODULE_INITIALIZE;
    Request.Size1 = 20;
    Request.CheckSumm1 = BuildChecksum(&Request.Unk1, 20);
    Request.Unk1 = 1;
    Request.Unk2 = 0;
    Request.Type = 1;
    Request.String_library1 = 0;
    Request.Function1[0] = 0x00024F80;      // 0x00400000 + 0x00024F80 SFileOpenFile
    Request.Function1[1] = 0x000218C0;      // 0x00400000 + 0x000218C0 SFileGetFileSize
    Request.Function1[2] = 0x00022530;      // 0x00400000 + 0x00022530 SFileReadFile
    Request.Function1[3] = 0x00022910;      // 0x00400000 + 0x00022910 SFileCloseFile

    Request.Command2 = WARDEN_SMSG_MODULE_INITIALIZE;
    Request.Size2 = 8;
    Request.CheckSumm2 = BuildChecksum(&Request.Unk2, 8);
    Request.Unk3 = 4;
    Request.Unk4 = 0;
    Request.String_library2 = 0;
    Request.Function2 = 0x00419D40;         // 0x00400000 + 0x00419D40 FrameScript::GetText
    Request.Function2_set = 1;

    Request.Command3 = WARDEN_SMSG_MODULE_INITIALIZE;
    Request.Size3 = 8;
    Request.CheckSumm3 = BuildChecksum(&Request.Unk5, 8);
    Request.Unk5 = 1;
    Request.Unk6 = 1;
    Request.String_library3 = 0;
    Request.Function3 = 0x0046AE20;         // 0x00400000 + 0x0046AE20 PerformanceCounter
    Request.Function3_set = 1;

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenInitModuleRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenInitModuleRequest));
    pkt.append((uint8*)&Request, sizeof(WardenInitModuleRequest));
    Client->SendPacket(&pkt);
}

uint32 Warden::BuildChecksum(const uint8* data, uint32 dataLen)
{
    uint8 hash[20];
    SHA1(data, dataLen, hash);
    uint32 checkSum = 0;
    for (uint8 i = 0; i < 5; ++i)
        checkSum = checkSum ^ *(uint32*)(&hash[0] + i * 4);
    return checkSum;
}

void Warden::RequestHash()
{
    // Create packet structure
    WardenHashRequest Request;
    Request.Command = WARDEN_SMSG_HASH_REQUEST;
    memcpy(Request.Seed, Seed, 16);

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenHashRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenHashRequest));
    pkt.append((uint8*)&Request, sizeof(WardenHashRequest));
    Client->SendPacket(&pkt);
}

void Warden::RequestHashReply(ByteBuffer &buff)
{
    buff.hexlike();
    buff.rpos(buff.wpos());

    const uint8 validHash[20] = { 0x56, 0x8C, 0x05, 0x4C, 0x78, 0x1A, 0x97, 0x2A, 0x60, 0x37, 0xA2, 0x29, 0x0C, 0x22, 0xB5, 0x25, 0x71, 0xA0, 0x6F, 0x4E };

    // verify key not equal kick player
    if (memcmp(buff.contents() + 1, validHash, sizeof(validHash)) != 0)
    {
        sLog->outWarden("Failed: 0x04 hash isn't equal - %u", Client->GetAccountId());
        Client->KickPlayer();
        return;
    }

    // client 7F96EEFDA5B63D20A4DF8E00CBF48304
    const uint8 client_key[16] = { 0x7F, 0x96, 0xEE, 0xFD, 0xA5, 0xB6, 0x3D, 0x20, 0xA4, 0xDF, 0x8E, 0x00, 0xCB, 0xF4, 0x83, 0x04 };

    // server C2B7ADEDFCCCA9C2BFB3F85602BA809B
    const uint8 server_key[16] = { 0xC2, 0xB7, 0xAD, 0xED, 0xFC, 0xCC, 0xA9, 0xC2, 0xBF, 0xB3, 0xF8, 0x56, 0x02, 0xBA, 0x80, 0x9B };

    // change keys here
    memcpy(InputKey, client_key, 16);
    memcpy(OutputKey, server_key, 16);

    iCrypto.Init(InputKey);
    oCrypto.Init(OutputKey);

    m_initialized = true;

    m_WardenTimer = getMSTime();
}

void Warden::RequestData()
{
    if (MemCheck.empty())
        MemCheck.assign(WardenModuleStorage.memcheckid.begin(), WardenModuleStorage.memcheckid.end());

    ServerTicks = getMSTime();

    uint32 maxid = WardenModuleStorage.InternalDataID;

    uint32 id;
    uint8 type;
    WardenData *wd;

    SendDataId.clear();

    for (uint32 i = 0; i < sLogon->getIntConfig(CONFIG_WARDEN_NUM_MEM_CHECKS); ++i)
    {   
        if (MemCheck.empty())
            break;
        id = MemCheck.back();
        SendDataId.push_back(id);
        MemCheck.pop_back();
    }

    ByteBuffer buff;
    buff << uint8(WARDEN_SMSG_MODULE_REQUEST_DATA);

    for (uint32 i = 0; i < sLogon->getIntConfig(CONFIG_WARDEN_NUM_OTHER_CHECKS); ++i)
    {
        id = irand(1, maxid - 1);
        wd = WardenModuleStorage.GetWardenDataById(id);
        SendDataId.push_back(id);
        switch (wd->Type)
        {
            case MPQ_CHECK:
            case LUA_STR_CHECK:
            case DRIVER_CHECK:
                buff << uint8(wd->str.size());
                buff.append(wd->str.c_str(), wd->str.size());
                break;
            default:
                break;
        }
    }

    uint8 xorByte = InputKey[0];

    buff << uint8(0x00);
    buff << uint8(TIMING_CHECK ^ xorByte);                  // check TIMING_CHECK

    uint8 index = 1;

    for (std::vector<uint32>::iterator itr = SendDataId.begin(); itr != SendDataId.end(); ++itr)
    {

        wd = WardenModuleStorage.GetWardenDataById(*itr);

        type = wd->Type;
        buff << uint8(type ^ xorByte);
        switch (type)
        {
            case MEM_CHECK:
            {
                buff << uint8(0x00);
                buff << uint32(wd->Address);
                buff << uint8(wd->Length);
                break;
            }
            case PAGE_CHECK_A:
            case PAGE_CHECK_B:
            {
                buff.append(wd->i.AsByteArray(0, false), wd->i.GetNumBytes());
                buff << uint32(wd->Address);
                buff << uint8(wd->Length);
                break;
            }
            case MPQ_CHECK:
            case LUA_STR_CHECK:
            {
                buff << uint8(index++);
                break;
            }
            case DRIVER_CHECK:
            {
                buff.append(wd->i.AsByteArray(0, false), wd->i.GetNumBytes());
                buff << uint8(index++);
                break;
            }
            case MODULE_CHECK:
            {
                uint32 seed = static_cast<uint32>(rand32());
                buff << uint32(seed);
                HmacHash hmac(4, (uint8*)&seed);
                hmac.UpdateData(wd->str);
                hmac.Finalize();
                buff.append(hmac.GetDigest(), hmac.GetLength());
                break;
            }
            /*case PROC_CHECK:
            {
                buff.append(wd->i.AsByteArray(0, false), wd->i.GetNumBytes());
                buff << uint8(index++);
                buff << uint8(index++);
                buff << uint32(wd->Address);
                buff << uint8(wd->Length);
                break;
            }*/
            default:
                break;                                      // should never happens
        }
    }
    buff << uint8(xorByte);
    buff.hexlike();

    // Encrypt with warden RC4 key.
    EncryptData(const_cast<uint8*>(buff.contents()), buff.size());

    WorldPacket pkt(SMSG_WARDEN_DATA, buff.size());
    pkt.append(buff);
    Client->SendPacket(&pkt);

    m_WardenDataSent = true;
}

bool Warden::IsValidCheckSum(uint32 checksum, const uint8 *Data, const uint16 Length)
{
    return checksum == BuildChecksum(Data, Length);
}

void Warden::HandleData(ByteBuffer &buff)
{
    buff.hexlike();

    m_WardenDataSent = false;
    m_WardenKickTimer = 0;

    uint16 Length;
    buff >> Length;
    uint32 Checksum;
    buff >> Checksum;

    if (!IsValidCheckSum(Checksum, buff.contents() + buff.rpos(), Length))
    {
        buff.rpos(buff.wpos());
        sLog->outWarden("Checksum error from %u.", Client->GetAccountId());
        Client->KickPlayer();
        return;
    }

    bool found = false;
    std::ostringstream sout;

    //TIMING_CHECK
    {
        uint8 result;
        buff >> result;
        // TODO: test it.
        if (result == 0x00)
        {
            sout << "T not 0x00, ";
            found = true;
        }

        uint32 newClientTicks;
        buff >> newClientTicks;

        if (Player *plr = Client->GetPlayer())
        {
            uint32 ticksNow = getMSTime();
            uint32 ourTicks = newClientTicks + (ticksNow - ServerTicks);
        }
    }

    WardenDataResult *rs;
    WardenData *rd;
    uint8 type;

    for (std::vector<uint32>::iterator itr = SendDataId.begin(); itr != SendDataId.end(); ++itr)
    {
        rd = WardenModuleStorage.GetWardenDataById(*itr);
        rs = WardenModuleStorage.GetWardenResultById(*itr);

        type = rd->Type;
        switch (type)
        {
            case MEM_CHECK:
            {
                uint8 Mem_Result;
                buff >> Mem_Result;

                if (Mem_Result != 0)
                {
                    sout << "M " << *itr << " not 0x00, ";
                    found = true;
                    continue;
                }

                if (memcmp(buff.contents() + buff.rpos(), rs->res.AsByteArray(0, false), rd->Length) != 0)
                {
                    sout << "M " << *itr << " failed, ";
                    found = true;
                    buff.rpos(buff.rpos() + rd->Length);
                    continue;
                }

                buff.rpos(buff.rpos() + rd->Length);
                break;
            }
            case PAGE_CHECK_A:
            case PAGE_CHECK_B:
            case DRIVER_CHECK:
            case MODULE_CHECK:
            {
                const uint8 byte = 0xE9;
                if (memcmp(buff.contents() + buff.rpos(), &byte, sizeof(uint8)) != 0)
                {
                    if (type == PAGE_CHECK_A || type == PAGE_CHECK_B)
                        sout << "P " << *itr << " failed, ";
                    if (type == MODULE_CHECK)
                        sout << "O " << *itr << " failed, ";
                    if (type == DRIVER_CHECK)
                        sout << "D " << *itr << " failed, ";
                    found = true;
                    buff.rpos(buff.rpos() + 1);
                    continue;
                }

                buff.rpos(buff.rpos() + 1);
                break;
            }
            case LUA_STR_CHECK:
            {
                uint8 Lua_Result;
                uint8 luaStrLen;

                buff >> Lua_Result;
                buff >> luaStrLen;

                if (luaStrLen != 0)
                {
                    char *str = new char[luaStrLen + 1];
                    memset(str, 0, luaStrLen + 1);
                    memcpy(str, buff.contents() + buff.rpos(), luaStrLen);
                    sout << "L " << *itr << " failed, ";
                    delete[] str;
                    found = true;
                    continue;
                }

                buff.rpos(buff.rpos() + luaStrLen); // skip string
                break;
            }
            case MPQ_CHECK:
            {
                uint8 Mpq_Result;
                buff >> Mpq_Result;

                if (Mpq_Result != 0)
                {
                    sout << " Q " << *itr << " not 0x00, ";
                    found = true;
                    continue;
                }

                if (memcmp(buff.contents() + buff.rpos(), rs->res.AsByteArray(0, false), 20) != 0) // SHA1
                {
                    sout << "Q " << *itr << " failed, ";
                    found = true;
                    buff.rpos(buff.rpos() + 20);            // 20 bytes SHA1
                    continue;
                }

                buff.rpos(buff.rpos() + 20);                // 20 bytes SHA1
                break;
            }
            default:                                        // should never happens
                break;
        }
    }

    if (!sout.str().empty())
        sLog->outWarden("Results from %u: %s", Client->GetAccountId(), sout.str().c_str());

    if (found)
        sLogon->BanAccountbyId(Client->GetAccountId(), sLogon->getIntConfig(CONFIG_WARDEN_CLIENT_BAN_DURATION), "Anticheat warning", "Warden");
}

CWardenModuleStorage::CWardenModuleStorage()
{
    InternalDataID = 1;
}

CWardenModuleStorage::~CWardenModuleStorage()
{
    std::map<uint32, ClientWardenModule*>::iterator itr = _modules.begin();
    ClientWardenModule *mod;
    for (; itr != _modules.end(); ++itr)
    {
        mod = itr->second;
        delete [] mod->CompressedData;
        delete mod;
    }
    std::map<uint32, WardenData*>::iterator itr1 = _data_map.begin();
    for (; itr1 != _data_map.end(); ++itr1)
        delete itr1->second;

    std::map<uint32, WardenDataResult*>::iterator itr2 = _result_map.begin();
    for (; itr2 != _result_map.end(); ++itr2)
        delete itr2->second;
}

void CWardenModuleStorage::Init()
{
    sLog->outWarden("===================================================================");
    sLog->outWarden("WardenModuleStorage - Server Side Storage of Client Warden Modules");
    sLog->outWarden("  Searching for modules...");

#ifndef _WIN32

    DIR *dirp;
    struct dirent *dp;
    dirp = opendir("./warden/");
    if (!dirp)
        return;
    while (dirp)
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            int l = strlen(dp->d_name);
            if (l < 36)
                continue;
            if (!memcmp(&dp->d_name[l-4],".wrd",4))
                LoadModule(dp->d_name);
        }
        else
        {
            if (errno != 0)
            {
                closedir(dirp);
                return;
            }
            break;
        }
    }

    if (dirp)
        closedir(dirp);

#else

    WIN32_FIND_DATA fil;
    //HANDLE hFil = FindFirstFile("D:/Development/C++Projekte/SingleCore/Sarana/win/bin/RelWithDebInfo/warden/*.wrd", &fil);
    HANDLE hFil = FindFirstFile("./warden/*.wrd", &fil);
    if (hFil == INVALID_HANDLE_VALUE)
        return;
    do
    {
        LoadModule(fil.cFileName);
    }
    while (FindNextFile(hFil, &fil));
    FindClose(hFil);
#endif
    sLog->outWarden("  Loaded %u modules.", _modules.size());
    sLog->outWarden("===================================================================");
    LoadWardenDataResult();
}

ClientWardenModule *CWardenModuleStorage::LoadModule(const char *FileName)
{
    char fn_module[256];
    snprintf(fn_module, 256, "./warden/%s", FileName);
    //snprintf(fn_module, 256, "D:/Development/C++Projekte/SingleCore/Sarana/win/bin/RelWithDebInfo/warden/%s", FileName);
    ClientWardenModule *mod = new ClientWardenModule;
    FILE *f_mod = fopen(fn_module, "rb");

    if (f_mod == 0)
        return NULL;

    // now read module
    fseek(f_mod, 0, SEEK_END);
    uint32 len = ftell(f_mod);
    fseek(f_mod, 0, SEEK_SET);
    // data assign
    mod->CompressedSize = len;
    mod->CompressedData = new uint8[len];
    fread(mod->CompressedData, len, 1, f_mod);

    // md5 hash
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, mod->CompressedData, len);
    MD5_Final((uint8*)&mod->ID, &ctx);

    fclose(f_mod);

    std::string fName = FileName;
    fName.erase(fName.end() - 4, fName.end());

    std::string path = "./warden/";
    //std::string path = "D:/Development/C++Projekte/SingleCore/Sarana/win/bin/RelWithDebInfo/warden/";
    path += fName;
    path += ".key";

    FILE *f_key = fopen(path.c_str(), "rb");

    if (f_key == 0)
        return NULL;

    // read key
    fread(mod->Key, 16, 1, f_key);

    fclose(f_key);
    _modules[GenerateInternalModuleID()] = mod;

    return mod;
}

void CWardenModuleStorage::LoadWardenDataResult()
{
    QueryResult result = WorldDatabase.Query("SELECT `check`, `data`, `result`, `address`, `length`, `str` FROM warden_data_result");

    uint32 count = 0;

    if (!result)
    {
        sLog->outString(">> Loaded 0 warden data and results");
        return;
    }

    do
    {
        ++count;

        Field *fields = result->Fetch();

        uint8 type = fields[0].GetUInt8();

        uint32 id = GenerateInternalDataID();
        WardenData *wd = new WardenData();
        wd->Type = type;

        if (type == PAGE_CHECK_A || type == PAGE_CHECK_B || type == DRIVER_CHECK)
        {
            std::string data = fields[1].GetString();
            wd->i.SetHexStr(data.c_str());
            int len = data.size() / 2;
            if (wd->i.GetNumBytes() < len)
            {
                uint8 temp[24];
                memset(temp, 0, len);
                memcpy(temp, wd->i.AsByteArray(), wd->i.GetNumBytes());
                std::reverse(temp, temp + len);
                wd->i.SetBinary((uint8*)temp, len);
            }
        }

        if (type == MEM_CHECK || type == MODULE_CHECK)
            memcheckid.push_back(id);

        if (type == MEM_CHECK || type == PAGE_CHECK_A || type == PAGE_CHECK_B || type == PROC_CHECK)
        {
            wd->Address = fields[3].GetUInt32();
            wd->Length = fields[4].GetUInt8();
        }

        // PROC_CHECK support missing
        if (type == MEM_CHECK || type == MPQ_CHECK || type == LUA_STR_CHECK || type == DRIVER_CHECK || type == MODULE_CHECK)
            wd->str = fields[5].GetString();

        _data_map[id] = wd;

        if (type == MPQ_CHECK || type == MEM_CHECK)
        {
            std::string result = fields[2].GetString();
            WardenDataResult *wr = new WardenDataResult();
            wr->res.SetHexStr(result.c_str());
            int len = result.size() / 2;
            if (wr->res.GetNumBytes() < len)
            {
                uint8 *temp = new uint8[len];
                memset(temp, 0, len);
                memcpy(temp, wr->res.AsByteArray(), wr->res.GetNumBytes());
                std::reverse(temp, temp + len);
                wr->res.SetBinary((uint8*)temp, len);
                delete [] temp;
            }
            _result_map[id] = wr;
        }
    } while (result->NextRow());

    sLog->outString(">> Loaded %u warden data and results", count);
}

ClientWardenModule *CWardenModuleStorage::GetModuleForClient(ClientSession *Session)
{
    // At the moment, return first
    if (_modules.empty())
        return NULL;

    ClientWardenModule *mod = _modules.begin()->second;
    return mod;
}

WardenData *CWardenModuleStorage::GetWardenDataById(uint32 Id)
{
    std::map<uint32, WardenData*>::const_iterator itr = _data_map.find(Id);
    if (itr != _data_map.end())
        return itr->second;
    return NULL;
}

WardenDataResult *CWardenModuleStorage::GetWardenResultById(uint32 Id)
{
    std::map<uint32, WardenDataResult*>::const_iterator itr = _result_map.find(Id);
    if (itr != _result_map.end())
        return itr->second;
    return NULL;
}
