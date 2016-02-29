#ifndef __NPC_BOOMBOX_GUNSHIPBATTLE_H_
#define __NPC_BOOMBOX_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcBoomboxGunship : public CreatureScript
{
public:
    NpcBoomboxGunship() : CreatureScript("npc_zafod_boombox") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature);
    bool OnGossipSelect(Player* player, Creature* pCreature, uint32 /*sender*/, uint32 action);
};

#endif