#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"

class NpcChallenge : public CreatureScript
{
public:
    NpcChallenge() : CreatureScript("npc_challenge") { }
    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action);
};