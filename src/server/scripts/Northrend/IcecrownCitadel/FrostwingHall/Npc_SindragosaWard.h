#ifndef __NPC_SINDRAGOSAWARD_H_
#define __NPC_SINDRAGOSAWARD_H_

#include "FrostwingHall.h"
#include "ScriptedCreature.h"
#include "Creature.h"

enum NpcSindragosaActions
{
    ACTION_START_GAUNTLET       = 1
};

enum NpcSindragosaEvents
{
    EVENT_NEXT_WAVE             = 1,
    EVENT_UPDATE_CHECK          = 2,
    EVENT_DONE                  = 3,
    EVENT_CAST_CRYPT_SCARABS    = 4,
    EVENT_CAST_WEB              = 5,
    EVENT_CAST_RUSH             = 6,
    EVENT_CAST_DARK_MENDING     = 7,
    EVENT_CAST_FROST_BLADE      = 8,
    EVENT_CAST_ICE_TOMB         = 9,
    EVENT_CAST_FROST_NOVA       = 10,
    EVENT_CAST_FROST_BOLT       = 11,
};

enum NpcSindragosaPoints
{
    POINT_NPC_DOWN              = 1,
    POINT_NPC_GROUND            = 2,
    POINT_NPC_END               = 3,
};

enum NpcSindragosaSpells
{
    SPELL_WEBROPE               = 56223,
    SPELL_CRYPT_SCARABS         = 71326,
    SPELL_WEB                   = 71327,
    SPELL_RUSH                  = 71801,
    SPELL_DARK_MENDING          = 71020,
    SPELL_FROST_BLADE           = 71323,
    SPELL_ICE_TOMB              = 71331,
    SPELL_FROST_NOVA            = 71320,
    SPELL_FROST_BOLT            = 71203,
    SPELL_SOULFEAST             = 71203
};

class NpcSindragosaWardAI : public BossAI
{
public:
    NpcSindragosaWardAI(Creature* creature);

    void Reset();
    void DoAction(int32 const action);
    void EnterCombat(Unit* /*attacker*/);
    void DoSummonWave(uint8 number);
    bool IsAnyPlayerOutOfRange();
    void JustSummoned(Creature* summon);
    void SummonedCreatureDies(Creature* summon, Unit* /*who*/);
    void SummonedCreatureDespawn(Creature* summon);
    void UpdateAI(uint32 const diff);
private:
    bool _isEventInProgressOrDone;
    uint8 _waveNumber;
    bool _spawnThirtySeconds;
    uint8 _thirtySeconds;
    uint8 _thirtySecondsSpawns;
    uint32 _thirtySecondsSpawnsCount;
    Position _randomPos;
    std::list<Creature*> _entrys;
};

class NpcSindragosaWard : public CreatureScript
{
public:
    NpcSindragosaWard() : CreatureScript("npc_sindragosas_ward") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSindragosaWardAI>(creature);
    }
};

class NpcSindragosaWardSpiderAI : public ScriptedAI
{
public:
    NpcSindragosaWardSpiderAI(Creature* creature) : ScriptedAI(creature) {}
    
    void Reset();
    void JustDied(Unit* killer);
    void MovementInform(uint32 type, uint32 id);
    void UpdateAI(const uint32 diff);
private:
    EventMap events;
    TempSummon* stalker;
};

class NpcSindragosaWardSpider : public CreatureScript
{
public:
    NpcSindragosaWardSpider() : CreatureScript("npc_sindragosas_ward_spider") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSindragosaWardSpiderAI>(creature);
    }
};

class NpcSindragosaWardFrostwardenAI : public ScriptedAI
{
public:
    NpcSindragosaWardFrostwardenAI(Creature* creature) : ScriptedAI(creature) {}

    void EnterCombat(Unit *target);
    void Reset();
    void JustDied(Unit* killer);
    void UpdateAI(const uint32 diff);
private:
    EventMap events;
    TempSummon* stalker;
};

class NpcSindragosaWardFrostwarden : public CreatureScript
{
public:
    NpcSindragosaWardFrostwarden() : CreatureScript("npc_sindragosas_ward_frostwarden") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSindragosaWardFrostwardenAI>(creature);
    }
};

class AtStartSindragosaGauntlet : public AreaTriggerScript
{
public:
    AtStartSindragosaGauntlet() : AreaTriggerScript("at_icc_start_sindragosa_gauntlet") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/);
};

#endif