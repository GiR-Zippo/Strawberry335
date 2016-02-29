#ifndef __BOSS_PROFESSOR_PUTRICIDE
#define __BOSS_PROFESSOR_PUTRICIDE

#include "ProfessorPutricide.h"

class ProfessorPutricideAI : public BossAI
{
public:
    ProfessorPutricideAI(Creature* creature) : BossAI(creature, DATA_PROFESSOR_PUTRICIDE),
        _baseSpeed(creature->GetSpeedRate(MOVE_RUN)), _experimentState(EXPERIMENT_STATE_OOZE), _phase(PHASE_NONE) {}

    void Reset();

    void EnterEvadeMode();

    void InitializeAI();

    void EnterCombat(Unit* who);

    void JustReachedHome();

    void KilledUnit(Unit* victim);

    void JustDied(Unit* /*killer*/);

    void JustSummoned(Creature* summon);

    void SummonedCreatureDies(Creature* summon, Unit* killer);

    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);

    void MovementInform(uint32 type, uint32 id);

    void DoAction(int32 const action);

    uint32 GetData(uint32 type);

    void UpdateAI(uint32 const diff);

private:
    void SetPhase(Phases newPhase);

    uint64 _oozeFloodDummyGUIDs[4];
    uint64 _experimentGUIDs[2];
    Phases _phase;          // external of EventMap because event phase gets reset on evade
    float const _baseSpeed;
    uint8 _oozeFloodStage;
    bool _experimentState;
    bool _phaseTransition;
};

class ProfessorPutricide : public CreatureScript
{
public:
    ProfessorPutricide() : CreatureScript("boss_professor_putricide") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<ProfessorPutricideAI>(creature);
    }
};

#endif