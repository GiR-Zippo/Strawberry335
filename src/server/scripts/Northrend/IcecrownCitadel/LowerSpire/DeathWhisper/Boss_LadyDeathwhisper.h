#ifndef __BOSS_LADY_DEATHWHISPER_H
#define __BOSS_LADY_DEATHWHISPER_H

#include "LadyDeathwhisper.h"

class LadyDeathwhisperAI : public BossAI
{
public:
    LadyDeathwhisperAI(Creature* creature) : BossAI(creature, DATA_LADY_DEATHWHISPER), _dominateMindCount(RAID_MODE<uint8>(0, 1, 1, 3)), _introDone(false) {}

    void Reset();
    void EnterEvadeMode();
    void MoveInLineOfSight(Unit* who);
    void AttackStart(Unit* victim);
    void EnterCombat(Unit* who);
    void JustDied(Unit* killer);
    void JustReachedHome();
    void KilledUnit(Unit* victim);
    void DamageTaken(Unit* /*damageDealer*/, uint32& damage);
    void JustSummoned(Creature* summon);
    void UpdateAI(uint32 const diff);
    void SummonWaveP1();
    void SummonWaveP2();
    void Summon(uint32 entry, const Position& pos);
    void SetGUID(uint64 guid, int32 id/* = 0*/);
    void ReanimateCultist();
    void SpellHitTarget(Unit* target, SpellInfo const* spell);
    void EmpowerCultist();

private:

    uint64 _nextVengefulShadeTargetGUID;
    uint64 _darnavanGUID;
    std::deque<uint64> _reanimationQueue;
    uint32 _waveCounter;
    uint8 const _dominateMindCount;
    bool _introDone;

};

class LadyDeathwhisper : public CreatureScript
{
public:
    LadyDeathwhisper() : CreatureScript("boss_lady_deathwhisper") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<LadyDeathwhisperAI>(creature);
    }
};

#endif