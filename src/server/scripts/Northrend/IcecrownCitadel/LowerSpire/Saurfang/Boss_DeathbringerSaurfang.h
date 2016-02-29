#ifndef __BOSS_DEATHBRINGER_SAURFANG_H
#define __BOSS_DEATHBRINGER_SAURFANG_H

#include "DeathbringerSaurfang.h"

class DeathbringerSaurfangAI : public BossAI
{
public:
    DeathbringerSaurfangAI(Creature* creature) : BossAI(creature, DATA_DEATHBRINGER_SAURFANG), _introDone(false), _fallenChampionCastCount(0) {ASSERT(creature->GetVehicleKit());}

    static uint32 const FightWonValue = 100000;

    void Reset();
    void EnterCombat(Unit* who);
    void AttackStart(Unit* victim);
    void EnterEvadeMode();
    void JustReachedHome();
    void KilledUnit(Unit* victim);
    void DamageTaken(Unit* /*attacker*/, uint32& damage);
    void JustSummoned(Creature* summon);
    void SummonedCreatureDies(Creature* summon, Unit* /*killer*/);
    void MovementInform(uint32 type, uint32 id);
    void SpellHitTarget(Unit* target, SpellInfo const* spell);
    void UpdateAI(uint32 const diff);
    uint32 GetData(uint32 type);
    void DoAction(int32 const action);

private:

    uint32 _fallenChampionCastCount;
    bool _introDone;
    bool _frenzied;   // faster than iterating all auras to find Frenzy
    bool _dead;

};

class DeathbringerSaurfang : public CreatureScript
{
public:
    DeathbringerSaurfang() : CreatureScript("boss_deathbringer_saurfang") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<DeathbringerSaurfangAI>(creature);
    }
};

#endif