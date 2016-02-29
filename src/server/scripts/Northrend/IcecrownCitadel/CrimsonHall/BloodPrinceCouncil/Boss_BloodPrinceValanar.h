#ifndef __BOSS_BLOODPRINCEVALANAR_H_
#define __BOSS_BLOODPRINCEVALANAR_H_

#include "BloodPrinceCouncil.h"

class BossBloodPrinceValanarAI : public BossAI
{
public:
    BossBloodPrinceValanarAI(Creature* creature) : BossAI(creature, DATA_BLOOD_PRINCE_COUNCIL)
    {
        _isEmpowered = false;
        _spawnHealth = creature->GetMaxHealth();
    }

    void InitializeAI();
    void Reset();
    void MoveInLineOfSight(Unit* /*who*/);
    void EnterCombat(Unit* /*who*/);
    void JustDied(Unit* /*killer*/);
    void JustReachedHome();
    void JustRespawned();
    void JustSummoned(Creature* summon);
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void DamageDealt(Unit* /*target*/, uint32& damage, DamageEffectType damageType);
    void DamageTaken(Unit* attacker, uint32& damage);
    void KilledUnit(Unit* victim);
    void DoAction(int32 const action);
    bool CheckRoom();
    void UpdateAI(uint32 const diff);

private:
    uint32 _spawnHealth;
    bool _isEmpowered;
};

class BossBloodPrinceValanar : public CreatureScript
{
    public:
        BossBloodPrinceValanar() : CreatureScript("boss_prince_valanar_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BossBloodPrinceValanarAI>(creature);
    }
};

#endif