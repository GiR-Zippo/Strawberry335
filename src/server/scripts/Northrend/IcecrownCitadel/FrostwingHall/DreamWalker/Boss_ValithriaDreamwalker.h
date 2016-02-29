#ifndef __BOSS_VALITHRIADREAMWALKER_H_
#define __BOSS_VALITHRIADREAMWALKER_H_

#include "ValithriaDreamwalker.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class BossValithriaDreamwalkerAI : public BossAI
{
public:
    BossValithriaDreamwalkerAI(Creature* creature) : BossAI(creature, DATA_VALITHRIA_DREAMWALKER),
        _instance(creature->GetInstanceScript()), _portalCount(RAID_MODE<uint32>(3, 8, 3, 8))
    {
    }

    void InitializeAI();
    void Reset();
    void AttackStart(Unit* /*target*/);
    void DoAction(int32 const action);
    void HealReceived(Unit* healer, uint32& heal);
    void DamageTaken(Unit* /*attacker*/, uint32& damage);
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void JustSummoned(Creature* summon);
    void SummonedCreatureDespawn(Creature* summon);
    void UpdateAI(uint32 const diff);
    uint32 GetData(uint32 type);
    void SpawnChest();

private:
    EventMap _events;
    InstanceScript* _instance;
    uint32 _spawnHealth;
    uint32 const _portalCount;
    uint32 _missedPortals;
    bool _under25PercentTalkDone;
    bool _over75PercentTalkDone;
    bool _justDied;
    bool _done;
};

class BossValithriaDreamwalker : public CreatureScript
{
    public:
        BossValithriaDreamwalker() : CreatureScript("boss_valithria_dreamwalker") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BossValithriaDreamwalkerAI>(creature);
    }
};

#endif