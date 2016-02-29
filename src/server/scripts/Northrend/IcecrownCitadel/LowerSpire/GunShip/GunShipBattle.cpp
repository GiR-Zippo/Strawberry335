#include "GunShipBattle.h"
#include "Npc_AxethrowerRiflemanGunShip.h"
#include "Npc_BoomboxGunShip.h"
#include "Npc_CannonGunShip.h"
#include "Npc_MageGunShip.h"
#include "Npc_MarineReaverGunShip.h"
#include "Npc_MuradinGunShip.h"
#include "Npc_OrgrimmshammerDefenderGunShip.h"
#include "Npc_OrgrimmshammerGunShip.h"
#include "Npc_OrgrimmshammerPrimalistGunShip.h"
#include "Npc_PortalGunShip.h"
#include "Npc_SaurfangGunShip.h"
#include "Npc_SergeantGunShip.h"
#include "Npc_SkybreakerGunShip.h"
#include "Npc_SkybreakerProtectorGunShip.h"
#include "Npc_SkybreakerVindicatorGunShip.h"
#include "Npc_SoldierRocketeerGunShip.h"
#include "Npc_TriggerGunShip.h"
#include "Spell_GunShip.h"
#include "Transport_GunShip.h"

TPlayerLists GetPlayersInTheMaps(Map* pMap)
{
    TPlayerLists players;
    const Map::PlayerList &PlayerList = pMap->GetPlayers();
    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                players.push_back(player);
    return players;
}

Player*  SelectRandomPlayerFromLists(TPlayerLists& players)
{
    if (players.empty())
        return NULL;
    TPlayerLists::iterator it = players.begin();
    std::advance(it, urand(0, players.size()-1));
    return *it;
}

Player*  SelectRandomPlayerInTheMaps(Map* pMap)
{
    TPlayerLists players = GetPlayersInTheMaps(pMap);
    return SelectRandomPlayerFromLists(players);
}

void StartFlyShip(Transport* t)
{
    t->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
    t->SetGoState(GO_STATE_ACTIVE);
    t->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0x10830010); // Seen in sniffs
    t->SetFloatValue(GAMEOBJECT_PARENTROTATION + 3, 1.0f);

    Map* map = t->GetMap();
    std::set<uint32> mapsUsed;
    //GameObjectTemplate const* goinfo = t->GetGOInfo();
    //t->GenerateWaypoints(goinfo->moTransport.taxiPathId, mapsUsed);
    for (Map::PlayerList::const_iterator itr = map->GetPlayers().begin(); itr != map->GetPlayers().end(); ++itr)
    {
        if (Player* pPlayer = itr->getSource())
        {
            UpdateData transData;
            t->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
            WorldPacket packet;
            transData.BuildPacket(&packet);
            pPlayer->SendDirectMessage(&packet);
        }
    }
}

void StopFlyShip(Transport* t)
{/*
    Map* map = t->GetMap();
    t->m_WayPoints.clear();
    RelocateTransport(t);
    t->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
    t->SetGoState(GO_STATE_READY);

    for (Map::PlayerList::const_iterator itr = map->GetPlayers().begin(); itr != map->GetPlayers().end(); ++itr)
    {
        if (Player* pPlayer = itr->getSource())
        {
            UpdateData transData;
            t->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
            WorldPacket packet;
            transData.BuildPacket(&packet);
            pPlayer->SendDirectMessage(&packet);
        }
    }
*/}

void UpdateTransportMotionInMap(Transport* t)
{
    Map* map = t->GetMap();

    for (Map::PlayerList::const_iterator itr = map->GetPlayers().begin(); itr != map->GetPlayers().end(); ++itr)
    {
        if (Player* pPlayer = itr->getSource())
        {
            UpdateData transData;
            t->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
            WorldPacket packet;
            transData.BuildPacket(&packet);
            pPlayer->SendDirectMessage(&packet);
        }
    }
}

void  RelocateTransport(Transport* t)
{/*
    Map* map = t->GetMap();
    InstanceScript* instance = t->GetInstanceScript();

    if (!t || !instance)
        return;

    // Transoprt movemend on server-side is ugly hack, so we do sincronize positions
    switch (t->GetEntry())
    {
        case GO_THE_SKYBREAKER_ALLIANCE_ICC:
            if (instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                t->Relocate(-377.184021f, 2073.548584f, 445.753387f);
            else if (instance->GetBossState(DATA_GUNSHIP_EVENT) == DONE)
                t->Relocate(-583.942627f, 2212.364990f, 534.673889f);
            break;
        case GO_ORGRIM_S_HAMMER_ALLIANCE_ICC:
            if (instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                t->Relocate(-384.878479f, 1989.831665f, 431.549438f);
            break;
        case GO_ORGRIM_S_HAMMER_HORDE_ICC:
            if (instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                t->Relocate(-438.142365f, 2395.725830f, 436.781647f);
            else if (instance->GetBossState(DATA_GUNSHIP_EVENT) == DONE)
                t->Relocate(-583.942627f, 2212.364990f, 534.673889f);
            break;
        case GO_THE_SKYBREAKER_HORDE_ICC:
            if (instance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
                t->Relocate(-435.854156f, 2475.328125f, 449.364105f);
            break;
    }

    t->Update(0);
    t->UpdatePassengerPositions(); 
*/}

Transport* CheckUnfriendlyShip(Creature* me, InstanceScript* instance, uint32 data)
{
    if (Creature* pCapitan = ObjectAccessor::GetCreature(*me, instance->GetData64(data)))
        return pCapitan->GetTransport();
    else
        return NULL;
}

void  TeleportPlayers(Map* map, uint64 TeamInInstance)
{
    if (map)
    {
        Map::PlayerList const &lPlayers = map->GetPlayers();
        if (!lPlayers.isEmpty())
        {
            for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                {
                    if (pPlayer->isDead() && !pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
                        pPlayer->ResurrectPlayer(1.0f);

                    if (TeamInInstance == ALLIANCE)
                        pPlayer->TeleportTo(631, -437.498505f, 2425.954f, 192.997f, 2.247f);
                    else
                        pPlayer->TeleportTo(631, -437.498505f, 1997.954f, 192.997f, 2.247f);
                }
            }
        }
    }
}

void  DoShipExplosion(Transport* t)
{/*
    for (Transport::CreatureSet::iterator itr = t->m_NPCPassengerSet.begin(); itr != t->m_NPCPassengerSet.end();)
    {
        if (Creature* npc = *itr)
        {
            if (npc->GetEntry() == NPC_GB_GUNSHIP_HULL)
                npc->CastSpell(npc, SPELL_SHIP_EXPLOSION, true);
        }
        ++itr;
    }
*/}

bool  DoWipeCheck(Transport* t)
{/*
    for (Transport::PlayerSet::const_iterator itr = t->GetPassengers().begin(); itr != t->GetPassengers().end();)
    {
        Player* plr = *itr;
        ++itr;

        if (plr && plr->isAlive())
            return true;
    }*/
    return false;
}

void  DoCheckFallingPlayer(Creature* me)
{
    Map* map = me->GetMap();

    if (map)
    {
        Map::PlayerList const &lPlayers = map->GetPlayers();
        if (!lPlayers.isEmpty())
        {
            for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                {
                    if (pPlayer->GetPositionZ() < 420.0f && pPlayer->IsWithinDistInMap(me, 300.0f))
                        pPlayer->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 5.0f, me->GetOrientation());
                }
            }
        }
    }
}

void RestartEvent(Transport* t1, Transport* t2, Map* instance, uint64 TeamInInstance)
{/*
    sMapMgr->UnLoadTransportFromMap(t1);
    sMapMgr->UnLoadTransportFromMap(t2);
    sMapMgr->LoadTransports(); // Try

    Map::PlayerList const& players = instance->GetPlayers();
    if (players.isEmpty())
        return;

    if (TeamInInstance == ALLIANCE)
    {
        if (Transport* th = sMapMgr->LoadTransportInMap(instance, GO_ORGRIM_S_HAMMER_ALLIANCE_ICC, 108000))
        {
            th->AddNPCPassengerInInstance(NPC_GB_ORGRIMS_HAMMER, 1.845810f, 1.268872f, 34.526218f, 1.5890f);
            th->AddNPCPassengerInInstance(NPC_GB_HIGH_OVERLORD_SAURFANG, 37.18615f, 0.00016f, 36.78849f, 3.13683f);
            th->AddNPCPassengerInInstance(NPC_GB_INVISIBLE_STALKER, 37.18615f, 0.00016f, 36.78849f, 3.13683f);
            th->AddNPCPassengerInInstance(NPC_GB_KORKRON_BATTLE_MAGE, 47.2929f, -4.308941f, 37.5555f, 3.05033f);
            th->AddNPCPassengerInInstance(NPC_GB_KORKRON_BATTLE_MAGE, 47.34621f, 4.032004f, 37.70952f, 3.05033f);
            th->AddNPCPassengerInInstance(NPC_GB_KORKRON_BATTLE_MAGE, 15.03016f, 0.00016f, 37.70952f, 1.55138f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -13.19547f, -27.160213f, 35.47252f, 3.10672f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -18.33902f, -25.230491f, 33.04052f, 3.00672f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -60.1251f, -1.27014f, 42.8335f, 5.16073f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -48.2651f, 16.78034f, 34.2515f, 0.04292f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -14.8356f, 27.931688f, 33.363f, 1.73231f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 10.2702f, 20.62966f, 35.37483f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 39.32459f, 14.50176f, 36.88428f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 46.17223f, -6.638763f, 37.35444f, 1.32f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 27.4456f, -13.397498f, 36.34746f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 18.16184f, 1.37897f, 35.31705f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -18.11516f, -0.196236f, 45.15709f, 2.9f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -18.11844f, -0.19624f, 49.18192f, 1.6f);

            if (instance->ToInstanceMap()->GetMaxPlayers() == 10)
            {
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -3.170555f, 28.30652f, 34.21082f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -12.0928f, 27.65942f, 33.58557f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 14.92804f, 26.18018f, 35.47803f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 24.70331f, 25.36584f, 35.97845f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, -11.44849f, -25.71838f, 33.64343f, 1.49248f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, 12.30336f, -25.69653f, 35.32373f, 1.49248f);
            }
            else
            {
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -3.170555f, 28.30652f, 34.21082f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -12.0928f, 27.65942f, 33.58557f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 14.92804f, 26.18018f, 35.47803f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 24.70331f, 25.36584f, 35.97845f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, 19.92804f, 27.18018f, 35.47803f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_AXETHROWER, -7.70331f, 28.36584f, 33.88557f, 1.66527f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, -11.44849f, -25.71838f, 33.64343f, 1.49248f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, 12.30336f, -25.69653f, 35.32373f, 1.49248f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, -3.44849f, -25.71838f, 34.21082f, 1.49248f);
                th->AddNPCPassengerInInstance(NPC_GB_KORKRON_ROCKETEER, 3.30336f, -25.69653f, 35.32373f, 1.49248f);
            }
        }

        if (Transport* t = sMapMgr->LoadTransportInMap(instance, GO_THE_SKYBREAKER_ALLIANCE_ICC, 108000))
        {
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER, -17.156807f, -1.633260f, 20.81273f, 4.52672f);
            t->AddNPCPassengerInInstance(NPC_GB_MURADIN_BRONZEBEARD, 13.51547f, -0.160213f, 20.87252f, 3.10672f);
            t->AddNPCPassengerInInstance(NPC_GB_HIHG_CAPTAIN_JUSTIN_BARTLETT, 42.78902f, -0.010491f, 25.24052f, 3.00672f);
            t->AddNPCPassengerInInstance(NPC_GB_HIGH_OVERLORD_SAURFANG_NOT_VISUAL, -12.9806f, -22.9462f, 21.659f, 4.72416f);
            t->AddNPCPassengerInInstance(NPC_GB_ZAFOD_BOOMBOX, 18.8042f, 9.907914f, 20.33559f, 3.10672f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_DECKHAND, -64.8423f, 4.4658f, 23.4352f, 2.698897f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_DECKHAND, 35.54972f, 19.93269f, 25.0333f, 4.71242f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_DECKHAND, -36.39837f, 3.13127f, 20.4496f, 1.5708f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_DECKHAND, -36.23974f, -2.75767f, 20.4506f, 4.69496f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_DECKHAND, 41.94677f, 44.08411f, 24.66587f, 1.62032f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 13.51547f, -0.160213f, 20.87252f, 3.10672f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 42.78902f, -0.010491f, 25.24052f, 3.00672f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 14.0551f, 3.65014f, 20.7935f, 3.16073f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 14.0551f, -4.65034f, 20.7915f, 3.04292f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -17.8356f, 0.031688f, 20.823f, 4.73231f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -34.2702f, -26.18966f, 21.37483f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -11.64459f, -19.85176f, 20.88428f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -19.88223f, -6.578763f, 20.57444f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -41.4456f, -7.647498f, 20.49746f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 0.554884f, -1.232897f, 20.53705f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -50.16516f, 9.716236f, 23.58709f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 11.45844f, 16.36624f, 20.54192f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 19.72286f, -2.193787f, 33.06982f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 19.72286f, -2.193787f, 33.06982f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 8.599396f, -28.55855f, 24.79919f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 38.94339f, -33.808f, 25.39618f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 58.15474f, 0.748094f, 41.87663f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 5.607554f, -6.350654f, 34.00357f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 4.780305f, -29.05227f, 35.09634f, 1.6f);

            if (instance->ToInstanceMap()->GetMaxPlayers() == 10)
            {
                t->AddNPCPassengerInInstance(NPC_GB_ALLIANCE_CANON, -5.15231f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_ALLIANCE_CANON, -28.0876f, -22.9462f, 21.659f, 4.72416f);
            }
            else
            {
                t->AddNPCPassengerInInstance(NPC_GB_ALLIANCE_CANON, -5.15231f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_ALLIANCE_CANON, -14.9806f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_ALLIANCE_CANON, -21.7406f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_ALLIANCE_CANON, -28.0876f, -22.9462f, 21.659f, 4.72416f);
            }
        }

    }

    if (TeamInInstance == HORDE)
    {
        if (Transport* t = sMapMgr->LoadTransportInMap(instance, GO_THE_SKYBREAKER_HORDE_ICC, 77800))
        {
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER, -17.156807f, -1.633260f, 20.81273f, 4.52672f);
            t->AddNPCPassengerInInstance(NPC_GB_MURADIN_BRONZEBEARD, 13.51547f, -0.160213f, 20.87252f, 3.10672f);
            t->AddNPCPassengerInInstance(NPC_GB_HIHG_CAPTAIN_JUSTIN_BARTLETT, 42.78902f, -0.010491f, 25.24052f, 3.00672f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_SORCERERS, 14.0551f, 3.65014f, 20.7935f, 3.16073f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_SORCERERS, 14.0551f, -4.65034f, 20.7915f, 3.04292f);
            t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_SORCERERS, -17.8356f, 0.031688f, 20.823f, 4.73231f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 13.51547f, -0.160213f, 20.87252f, 3.10672f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 42.78902f, -0.010491f, 25.24052f, 3.00672f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 14.0551f, 3.65014f, 20.7935f, 3.16073f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 14.0551f, -4.65034f, 20.7915f, 3.04292f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -17.8356f, 0.031688f, 20.823f, 4.73231f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -34.2702f, -26.18966f, 21.37483f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -11.64459f, -19.85176f, 20.88428f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -19.88223f, -6.578763f, 20.57444f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -41.4456f, -7.647498f, 20.49746f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 0.554884f, -1.232897f, 20.53705f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -50.16516f, 9.716236f, 23.58709f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 11.45844f, 16.36624f, 20.54192f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 19.72286f, -2.193787f, 33.06982f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 19.72286f, -2.193787f, 33.06982f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 8.599396f, -28.55855f, 24.79919f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 38.94339f, -33.808f, 25.39618f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 58.15474f, 0.748094f, 41.87663f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 5.607554f, -6.350654f, 34.00357f, 1.6f);
            t->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 4.780305f, -29.05227f, 35.09634f, 1.6f);

            if (instance->ToInstanceMap()->GetMaxPlayers() == 10)
            {
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -5.15231f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -14.9806f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -21.7406f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -28.0876f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -8.61003f, 15.483f, 20.4158f, 4.69854f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -27.9583f, 14.8875f, 20.4428f, 4.77865f);
            }
            else
            {
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, 0.15231f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -5.15231f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -14.9806f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -21.7406f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -28.0876f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_RIFLEMAN, -33.0876f, -22.9462f, 21.659f, 4.72416f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -8.61003f, 15.483f, 20.4158f, 4.69854f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -27.9583f, 14.8875f, 20.4428f, 4.77865f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -15.61003f, 15.483f, 20.4158f, 4.69854f);
                t->AddNPCPassengerInInstance(NPC_GB_SKYBREAKER_MORTAR_SOLDIER, -20.9583f, 14.8875f, 20.4428f, 4.77865f);
            }
        }

        if (Transport* th = sMapMgr->LoadTransportInMap(instance,GO_ORGRIM_S_HAMMER_HORDE_ICC, 77800))
        {
            th->AddNPCPassengerInInstance(NPC_GB_ORGRIMS_HAMMER, 1.845810f, 1.268872f, 34.526218f, 1.5890f);
            th->AddNPCPassengerInInstance(NPC_GB_HIGH_OVERLORD_SAURFANG, 37.18615f, 0.00016f, 36.78849f, 3.13683f);
            th->AddNPCPassengerInInstance(NPC_GB_MURADIN_BRONZEBEARD_NOT_VISUAL, -7.09684f, 30.582f, 34.5013f, 1.53591f);
            th->AddNPCPassengerInInstance(NPC_GB_INVISIBLE_STALKER, 37.30764f, -0.143823f, 36.7936f, 3.13683f);
            th->AddNPCPassengerInInstance(NPC_GB_ZAFOD_BOOMBOX, 35.18615f, 15.30652f, 37.64343f, 3.05033f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -13.19547f, -27.160213f, 35.47252f, 3.10672f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -18.33902f, -25.230491f, 33.04052f, 3.00672f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -60.1251f, -1.27014f, 42.8335f, 5.16073f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -48.2651f, 16.78034f, 34.2515f, 0.04292f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -14.8356f, 27.931688f, 33.363f, 1.73231f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 10.2702f, 20.62966f, 35.37483f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 39.32459f, 14.50176f, 36.88428f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 46.17223f, -6.638763f, 37.35444f, 1.32f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 27.4456f, -13.397498f, 36.34746f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, 18.16184f, 1.37897f, 35.31705f, 1.6f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -18.11516f, -0.196236f, 45.15709f, 2.9f);
            th->AddNPCPassengerInInstance(NPC_GB_GUNSHIP_HULL, -18.11844f, -0.19624f, 49.18192f, 1.6f);
            
            if (instance->ToInstanceMap()->GetMaxPlayers() == 10)
            {
                th->AddNPCPassengerInInstance(NPC_GB_HORDE_CANON, 22.6225f, 28.9309f, 36.3929f, 1.53591f);
                th->AddNPCPassengerInInstance(NPC_GB_HORDE_CANON, -21.7509f, 29.4207f, 34.2588f, 1.53591f);
            }
            else
            {
                th->AddNPCPassengerInInstance(NPC_GB_HORDE_CANON, 22.6225f, 28.9309f, 36.3929f, 1.53591f);
                th->AddNPCPassengerInInstance(NPC_GB_HORDE_CANON, 9.87745f, 30.5047f, 35.7147f, 1.53591f);
                th->AddNPCPassengerInInstance(NPC_GB_HORDE_CANON, -7.09684f, 30.582f, 34.5013f, 1.53591f);
                th->AddNPCPassengerInInstance(NPC_GB_HORDE_CANON, -21.7509f, 29.4207f, 34.2588f, 1.53591f);
            }
        }
    }
*/}

void  StopFight(Transport* t1, Transport* t2)
{/*
    Map* map = t1->GetMap();

    for (Transport::CreatureSet::iterator itr = t1->m_NPCPassengerSet.begin(); itr != t1->m_NPCPassengerSet.end();)
    {
        if (Creature *npc = *itr)
        {
            if (npc->GetEntry() != NPC_GB_SKYBREAKER && npc->GetEntry() != NPC_GB_ORGRIMS_HAMMER && npc->GetEntry() != NPC_GB_HIGH_OVERLORD_SAURFANG
                && npc->GetEntry() != NPC_GB_MURADIN_BRONZEBEARD && npc->GetEntry() != NPC_GB_GUNSHIP_HULL)
            {
                if (Vehicle* veh = npc->GetVehicleKit())
                    veh->RemoveAllPassengers();
                npc->Kill(npc);
            }
        }
        ++itr;
    }

    for (Transport::CreatureSet::iterator itr = t2->m_NPCPassengerSet.begin(); itr != t2->m_NPCPassengerSet.end();)
    {
        if (Creature *npc = *itr)
        {
            if (npc->GetEntry() != NPC_GB_GUNSHIP_HULL && npc->GetEntry() != NPC_GB_SKYBREAKER && npc->GetEntry() != NPC_GB_ORGRIMS_HAMMER)
                npc->Kill(npc);
        }
        ++itr;
    }

    for (Map::PlayerList::const_iterator itr = map->GetPlayers().begin(); itr != map->GetPlayers().end(); ++itr)
    {
        if (Player* pPlayer = itr->getSource())
            pPlayer->CombatStop();
    }
*/}

void LoadGunShip()
{
    new NpcAxethrowerRiflemanGunship();
    new NpcBoomboxGunship();
    new NpcCannonGunship();
    new NpcMageGunship();
    new NpcMuradinGunship();
    new NpcOrgrimmshammerDefenderGunship();
    new NpcOrgrimmshammerGunship();
    new NpcOrgrimmshammerPrimalistGunship();
    new NpcPortalGunship();
    new NpcSaurfangGunship();
    new NpcSergeantGunship();
    new NpcSkybreakerGunship();
    new NpcSkybreakerProtectorGunship();
    new NpcSkybreakerVindicatorGunship();
    new NpcSoldierRocketeerGunship();
    new NpcTriggerGunship();

    new SpellBurningPitch();
    new SpellHeatDrainSpell();
    new SpellIncineratingBlast();
    new SpellOverheatDrain();
    new SpellRemoveRocketPack();
    new SpellRocketPack();

    new TransportGunship();
}