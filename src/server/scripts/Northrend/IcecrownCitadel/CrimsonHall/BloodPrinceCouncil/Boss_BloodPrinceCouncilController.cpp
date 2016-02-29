#include "Boss_BloodPrinceCouncilController.h"

void BossBloodCouncilControllerAI::Reset()
{
    events.Reset();
    me->SetReactState(REACT_PASSIVE);
    _invocationStage = 0;
    _resetCounter = 0;

    instance->SetBossState(DATA_BLOOD_PRINCE_COUNCIL, NOT_STARTED);
}

void BossBloodCouncilControllerAI::EnterCombat(Unit* who)
{
    if (instance->GetBossState(DATA_BLOOD_PRINCE_COUNCIL) == IN_PROGRESS)
        return;

    if (!instance->CheckRequiredBosses(DATA_BLOOD_PRINCE_COUNCIL, who->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    instance->SetBossState(DATA_BLOOD_PRINCE_COUNCIL, IN_PROGRESS);

    DoCast(me, SPELL_INVOCATION_OF_BLOOD_VALANAR);

    if (Creature* keleseth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_KELESETH_GUID)))
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, keleseth);
        DoZoneInCombat(keleseth);
    }

    if (Creature* taldaram = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_TALDARAM_GUID)))
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, taldaram);
        DoZoneInCombat(taldaram);
    }

    if (Creature* valanar = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_VALANAR_GUID)))
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, valanar);
        DoZoneInCombat(valanar);
    }

    events.ScheduleEvent(EVENT_INVOCATION_OF_BLOOD, 46500);

    _invocationOrder[0] = InvocationData(instance->GetData64(DATA_PRINCE_VALANAR_GUID), SPELL_INVOCATION_OF_BLOOD_VALANAR, EMOTE_VALANAR_INVOCATION, 71070);
    if (urand(0, 1))
    {
        _invocationOrder[1] = InvocationData(instance->GetData64(DATA_PRINCE_TALDARAM_GUID), SPELL_INVOCATION_OF_BLOOD_TALDARAM, EMOTE_TALDARAM_INVOCATION, 71081);
        _invocationOrder[2] = InvocationData(instance->GetData64(DATA_PRINCE_KELESETH_GUID), SPELL_INVOCATION_OF_BLOOD_KELESETH, EMOTE_KELESETH_INVOCATION, 71080);
    }
    else
    {
        _invocationOrder[1] = InvocationData(instance->GetData64(DATA_PRINCE_KELESETH_GUID), SPELL_INVOCATION_OF_BLOOD_KELESETH, EMOTE_KELESETH_INVOCATION, 71080);
        _invocationOrder[2] = InvocationData(instance->GetData64(DATA_PRINCE_TALDARAM_GUID), SPELL_INVOCATION_OF_BLOOD_TALDARAM, EMOTE_TALDARAM_INVOCATION, 71081);
    }
}

void BossBloodCouncilControllerAI::SetData(uint32 , uint32 data)
{
    _resetCounter += uint8(data);
    if (_resetCounter == 3)
        EnterEvadeMode();
}

void BossBloodCouncilControllerAI::JustReachedHome()
{
    _resetCounter = 0;
    if (Creature* keleseth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_KELESETH_GUID)))
        keleseth->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

    if (Creature* taldaram = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_TALDARAM_GUID)))
        taldaram->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

    if (Creature* valanar = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_VALANAR_GUID)))
        valanar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
}

void BossBloodCouncilControllerAI::JustDied(Unit* killer)
{
    _JustDied();
    // kill all prices
    for (uint8 i = 0; i < 3; ++i)
    {
        if (++_invocationStage == 3)
            _invocationStage = 0;

        if (Creature* prince = ObjectAccessor::GetCreature(*me, _invocationOrder[_invocationStage].guid))
        {
            // make sure looting is allowed
            if (me->IsDamageEnoughForLootingAndReward())
                prince->LowerPlayerDamageReq(prince->GetMaxHealth());
            killer->Kill(prince);
        }
    }
}

void BossBloodCouncilControllerAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_INVOCATION_OF_BLOOD:
            {
                Creature* oldPrince = ObjectAccessor::GetCreature(*me, _invocationOrder[_invocationStage].guid);
                if (++_invocationStage == 3)
                    _invocationStage = 0;

                Creature* newPrince = ObjectAccessor::GetCreature(*me, _invocationOrder[_invocationStage].guid);
                if (oldPrince)
                {
                    oldPrince->AI()->DoAction(ACTION_REMOVE_INVOCATION);
                    if (newPrince)
                        oldPrince->CastSpell(newPrince, _invocationOrder[_invocationStage].visualSpell, true);
                }

                if (newPrince)
                {
                    newPrince->SetHealth(me->GetHealth());
                    newPrince->AI()->Talk(uint8(_invocationOrder[_invocationStage].textId));
                }

                DoCast(me, _invocationOrder[_invocationStage].spellId);
                events.ScheduleEvent(EVENT_INVOCATION_OF_BLOOD, 46500);
                break;
            }
            default:
                break;
        }
    }
}