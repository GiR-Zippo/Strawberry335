#ifndef __TRANSPORT_GUNSHIPBATTLE_H_
#define __TRANSPORT_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "Transport.h"

class TransportGunship : public TransportScript
{
    public:
        TransportGunship() : TransportScript("transport_gunship") { }

    void OnRelocate(Transport* transport, uint32 waypointId, uint32 mapId, float x, float y, float z, uint32 realId);
    void OnAddPassenger(Transport* transport, Player* player);
    void OnRemovePassenger(Transport* /*transport*/, Player* player);
    void OnAddCreaturePassenger(Transport* /*transport*/, Creature* /*creature*/);
};

#endif