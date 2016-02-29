#ifndef __BOSS_BLOODPRINCEKELESETH_H_
#define __BOSS_BLOODPRINCEKELESETH_H_

#include "BloodPrinceCouncil.h"

class BossBloodPrinceKelesethAI : public BossAI
{
public:
    BossBloodPrinceKelesethAI(Creature* creature) : BossAI(creature, DATA_BLOOD_PRINCE_COUNCIL)
    {
        _isEmpowered = false;
        _spawnHealth = creature->GetMaxHealth();
    }

    void InitializeAI();
    void Reset();
    void EnterCombat(Unit* /*who*/);
    void JustDied(Unit* /*killer*/);
    void JustReachedHome();
    void JustRespawned();
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void JustSummoned(Creature* summon);
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

class BossBloodPrinceKeleseth : public CreatureScript
{
    public:
        BossBloodPrinceKeleseth() : CreatureScript("boss_prince_keleseth_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BossBloodPrinceKelesethAI>(creature);
    }
};

#endif