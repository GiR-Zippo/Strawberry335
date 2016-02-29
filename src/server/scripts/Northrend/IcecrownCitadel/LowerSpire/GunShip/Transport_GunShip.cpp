#include "Transport_GunShip.h"

void TransportGunship::OnRelocate(Transport* transport, uint32 waypointId, uint32 mapId, float x, float y, float z, uint32 readId)
{
    if (!transport)
        return;

    switch (transport->GetEntry())
    {
        case GO_THE_SKYBREAKER_ALLIANCE_ICC:
            sLog->outString("Skybreaker relocated: Waypoint=%u Mapid=%u x=%f y=%f z=%f real=%u", waypointId, mapId, x, y, z, readId);
            break;
        case GO_ORGRIM_S_HAMMER_HORDE_ICC:
            sLog->outString("Orgrims Hammer relocated: Waypoint=%u Mapid=%u x=%f y=%f z=%f real=%u", waypointId, mapId, x, y, z, readId);
            break;
        default:
            break;
    }
}

void TransportGunship::OnAddPassenger(Transport* transport, Player* player)
{
    if (!transport || !player)
        return;

    switch (transport->GetEntry())
    {
        case GO_THE_SKYBREAKER_ALLIANCE_ICC:
            player->AddAura(SPELL_ON_SKYBREAKERS_DECK, player);
            sLog->outString("Player added to skybreakers deck.");
            break;
        case GO_ORGRIM_S_HAMMER_HORDE_ICC:
            player->AddAura(SPELL_ON_ORGRIMS_HAMMERS_DECK, player);
            sLog->outString("Player added to orgrims hammers deck.");
            break;
        default:
            break;
    }
}

void TransportGunship::OnRemovePassenger(Transport* , Player* player)
{
    if (!player)
        return;

    sLog->outString("Player removed from deck.");
    player->RemoveAurasDueToSpell(SPELL_ON_ORGRIMS_HAMMERS_DECK);
    player->RemoveAurasDueToSpell(SPELL_ON_SKYBREAKERS_DECK);
}

void TransportGunship::OnAddCreaturePassenger(Transport* , Creature*)
{
    sLog->outString("Creature added to gunship.");
}
