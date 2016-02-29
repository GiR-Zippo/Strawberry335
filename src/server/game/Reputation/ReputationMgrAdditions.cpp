#include "ReputationMgr.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "ClusterDefines.h"

void ReputationMgr::SetReputationAddition(FactionEntry const* factionEntry, int32 standing, bool incremental)
{
    WorldPacket data(NODE_MISC_DATA);  // last check 2.4.0
    data << uint32 (CL_DEF_REP_SET_REPUTATION);
    data << uint32(factionEntry->ID);
    data << int32(standing);
    data << bool (incremental);
    _player->SendDirectMessage(&data);
}