#ifndef __TRANSMOG_H
#define __TRANSMOG_H
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include <TransmogMgr.h>

class NpcTransmog : public CreatureScript
{
public:
    NpcTransmog() : CreatureScript("npc_transmog") { }
    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action);
};

#endif /* __TRANSMOG_H */
