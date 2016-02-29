#ifndef __BOSS_BLOODPRINCECOUNCILCONTROLLER_H_
#define __BOSS_BLOODPRINCECOUNCILCONTROLLER_H_

#include "BloodPrinceCouncil.h"

class BossBloodCouncilControllerAI : public BossAI
{
public:
    BossBloodCouncilControllerAI(Creature* creature) : BossAI(creature, DATA_BLOOD_PRINCE_COUNCIL)
    {
    }

    void Reset();
    void EnterCombat(Unit* who);
    void SetData(uint32 /*type*/, uint32 data);
    void JustReachedHome();
    void JustDied(Unit* killer);
    void UpdateAI(uint32 const diff);

private:
    struct InvocationData
    {
        uint64 guid;
        uint32 spellId;
        uint32 textId;
        uint32 visualSpell;

        InvocationData(uint64 _guid, uint32 _spellId, uint32 _textId, uint32 _visualSpell)
        {
            guid = _guid;
            spellId = _spellId;
            textId = _textId;
            visualSpell = _visualSpell;
        }

        InvocationData() : guid(0), spellId(0), textId(0), visualSpell(0) { }
    } _invocationOrder[3];

    uint32 _invocationStage;
    uint32 _resetCounter;
};

class BossBloodCouncilController : public CreatureScript
{
    public:
        BossBloodCouncilController() : CreatureScript("boss_blood_council_controller") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BossBloodCouncilControllerAI>(creature);
    }
};

#endif