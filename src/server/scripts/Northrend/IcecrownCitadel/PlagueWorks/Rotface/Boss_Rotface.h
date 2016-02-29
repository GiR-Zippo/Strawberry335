#ifndef __BOSS_ROTFACE_H
#define __BOSS_ROTFACE_H

#include "Rotface.h"

class RotfaceAI : public BossAI
{
public:
    RotfaceAI(Creature* creature) : BossAI(creature, DATA_ROTFACE), InfectionStage(0) {}

    void Reset();
    void EnterCombat(Unit* who);
    void JustSummoned(Creature* summon);
    void JustDied(Unit* /*killer*/);
    void JustReachedHome();
    void KilledUnit(Unit* victim);
    void EnterEvadeMode();
    void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell);
    void UpdateAI(const uint32 diff);

private:
    uint32 InfectionStage;
};

class Rotface : public CreatureScript
{
public:
    Rotface() : CreatureScript("boss_rotface") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<RotfaceAI>(creature);
    }
};

#endif