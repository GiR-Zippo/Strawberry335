#ifndef __BOSS_FESTERGUT_H
#define __BOSS_FESTERGUT_H

#include "Festergut.h"

class FestergutAI : public BossAI
{
public:
    FestergutAI(Creature* creature) : BossAI(creature, DATA_FESTERGUT), _maxInoculatedStack(0), _inhaleCounter(0), _gasDummyGUID(0) {}

    void Reset();
    void EnterCombat(Unit* who);
    void JustDied(Unit* /*killer*/);
    void JustReachedHome();
    void EnterEvadeMode();
    void KilledUnit(Unit* victim);
    void SpellHitTarget(Unit* target, SpellInfo const* spell);
    void UpdateAI(uint32 const diff);
    void SetData(uint32 type, uint32 data);
    uint32 GetData(uint32 type);
    void RemoveBlight();

private:

    uint64 _gasDummyGUID;
    uint32 _maxInoculatedStack;
    uint32 _inhaleCounter;

};


class Festergut : public CreatureScript
{
public:
    Festergut() : CreatureScript("boss_festergut") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<FestergutAI>(creature);
    }
};

#endif