#ifndef __BOSS_SINDRAGOSA_H_
#define __BOSS_SINDRAGOSA_H_

#include "Sindragosa.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class BossSindragosaAI : public BossAI
{
public:
    BossSindragosaAI(Creature* creature) : BossAI(creature, DATA_SINDRAGOSA), _summoned(false)
    {
    }

    void Reset();
    void EnterEvadeMode();
    void JustDied(Unit* killer);
    void EnterCombat(Unit* victim);
    void JustReachedHome();
    void KilledUnit(Unit* victim);
    void DoAction(int32 const action);
    uint32 GetData(uint32 type);
    virtual void SetData(uint32 type, uint32 data);
    void MovementInform(uint32 type, uint32 point);
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);
    void JustSummoned(Creature* summon);
    void SummonedCreatureDespawn(Creature* summon);
    void SpellHitTarget(Unit* target, SpellInfo const* spell);
    void UpdateAI(uint32 const diff);

private:
    uint8 _mysticBuffetStack;
    uint8 _frostBombHitCount;
    bool _isInAirPhase;
    bool _isThirdPhase;
    bool _summoned;
};

class BossSindragosa : public CreatureScript
{
    public:
        BossSindragosa() : CreatureScript("boss_sindragosa") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BossSindragosaAI>(creature);
    }
};

#endif