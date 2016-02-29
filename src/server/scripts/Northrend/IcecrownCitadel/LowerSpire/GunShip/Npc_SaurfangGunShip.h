#ifndef __NPC_SAURFANG_GUNSHIPBATTLE_H_
#define __NPC_SAURFANG_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSaurfangGunshipAI : public ScriptedAI
{
public:
    NpcSaurfangGunshipAI(Creature *creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript()) {}

    void Reset();
    void SendMusicToPlayers(uint32 musicId) const;
    // Send packet to all players
    void SendPacketToPlayers(WorldPacket const* data) const;
    bool CanAIAttack(Unit const* target) const;
    void DoAction(int32 const action);
    void DamageTaken(Unit* /*attacker*/, uint32& damage);
    void MovementInform(uint32 type, uint32 pointId);
    void UpdateAI(const uint32 diff);

private:
    Transport* orgrimmar;
    uint32 RocketerDieCount;
    uint32 AxesDieCount;
    uint32 RocketerCount;
    uint32 AxesCount;
    uint32 SummonCount;
    uint32 count;
    bool EventScheduled;
    Map* map;
    EventMap events;
    InstanceScript* _instance;
};

class NpcSaurfangGunship : public CreatureScript
{
    public:
        NpcSaurfangGunship() : CreatureScript("npc_saurfang_gunship") { }

    bool OnGossipHello(Player* player, Creature* pCreature);
    bool OnGossipSelect(Player* player, Creature* pCreature, uint32 /*sender*/, uint32 action);

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcSaurfangGunshipAI>(pCreature);
    }
};

#endif