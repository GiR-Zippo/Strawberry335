#ifndef __NPC_MURADIN_GUNSHIPBATTLE_H_
#define __NPC_MURADIN_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcMuradinGunshipAI : public ScriptedAI
{
public:
    NpcMuradinGunshipAI(Creature *creature) : ScriptedAI(creature),
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
        Transport* skybreaker;
        Map* map;
        uint32 SummonCount;
        EventMap events;
        InstanceScript* _instance;
        uint32 count;
        uint32 RocketerDieCount;
        uint32 RiflDieCount;
        uint32 RocketerCount;
        uint32 RiflCount;
        bool EventScheduled;
};

class NpcMuradinGunship : public CreatureScript
{
    public:
        NpcMuradinGunship() : CreatureScript("npc_muradin_gunship") { }

    bool OnGossipHello(Player* player, Creature* pCreature);
    bool OnGossipSelect(Player* player, Creature* pCreature, uint32 /*sender*/, uint32 action);

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcMuradinGunshipAI>(pCreature);
    }
};

#endif