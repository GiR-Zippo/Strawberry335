#include "Boss_Rotface.h"

/////////////////////
// Boss Rotface AI //
/////////////////////

void RotfaceAI::Reset()
{
    me->SetReactState(REACT_DEFENSIVE);
    summons.DespawnAll();
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GREEN_BLIGHT_RESIDUE_DEBUFF);
    _Reset();
    InfectionStage = 0;
}

void RotfaceAI::EnterCombat(Unit* who)
{
    if (me->isDead())
    {
        EnterEvadeMode();
        return;
    }

    if (!instance->CheckRequiredBosses(DATA_ROTFACE, who->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    me->setActive(true);
    Talk(SAY_AGGRO);
    if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
        professor->AI()->DoAction(ACTION_ROTFACE_COMBAT);
    DoZoneInCombat();
    DoCast(me, SPELL_GREEN_ABOMINATION_HITTIN__YA_PROC, true);
    DoCast(me, MutatedInfectionAura[InfectionStage]);

    events.ScheduleEvent(EVENT_SLIME_SPRAY, 20000);
    events.ScheduleEvent(EVENT_MUTATED_INFECTION, MutatedInfectionTimer[InfectionStage]);
    if (IsHeroic())
        events.ScheduleEvent(EVENT_VILE_GAS, urand(22000, 27000)); 
}

void RotfaceAI::JustSummoned(Creature* summon)
{
    summons.Summon(summon);
    if (summon->GetEntry() == NPC_VILE_GAS_STALKER)
        if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
            professor->CastSpell(summon, SPELL_VILE_GAS_H, true); 
}

void RotfaceAI::JustDied(Unit* /*killer*/)
{
    _JustDied();
    Talk(SAY_DEATH);
    instance->DoRemoveAurasDueToSpellOnPlayers(MUTATED_INFECTION);
    std::list<Creature*> AddList;
    me->GetCreatureListWithEntryInGrid(AddList, NPC_LITTLE_OOZE, 150.0f);
    me->GetCreatureListWithEntryInGrid(AddList, NPC_BIG_OOZE, 150.0f);
    for (std::list<Creature*>::iterator itr = AddList.begin(); itr != AddList.end(); itr++)
        (*itr)->DespawnOrUnsummon();
    if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
        professor->AI()->DoAction(ACTION_ROTFACE_DEATH);
}

void RotfaceAI::JustReachedHome()
{
    _JustReachedHome();
    instance->SetBossState(DATA_ROTFACE, FAIL);
    instance->SetData(DATA_OOZE_DANCE_ACHIEVEMENT, uint32(true));   // reset
}

void RotfaceAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void RotfaceAI::EnterEvadeMode()
{
    ScriptedAI::EnterEvadeMode();
    if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
        professor->AI()->EnterEvadeMode();

    summons.DespawnAll();
    _DespawnAtEvade();
}

void RotfaceAI::SpellHitTarget(Unit* /*target*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_SLIME_SPRAY)
        Talk(SAY_SLIME_SPRAY);
}


void RotfaceAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim() || !CheckInRoom())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING) && events.GetEvent() != EVENT_MUTATED_INFECTION)
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_SLIME_SPRAY:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
            {
                Creature* dummy = DoSummon(NPC_OOZE_SPRAY_STALKER, *target, 8000, TEMPSUMMON_TIMED_DESPAWN);

                Talk(EMOTE_SLIME_SPRAY);
                me->SetTarget(dummy->GetGUID());
                DoCast(me, SPELL_SLIME_SPRAY, true);
                events.ScheduleEvent(EVENT_SLIME_SPRAY_RESET, 5000);
            }
            events.ScheduleEvent(EVENT_SLIME_SPRAY, 20000);
            break;
        case EVENT_SLIME_SPRAY_RESET:
            me->SetTarget(me->getVictim()->GetGUID());
            break;
        case EVENT_MUTATED_INFECTION:
            me->RemoveAura(MutatedInfectionAura[InfectionStage++]);
            DoCast(me, MutatedInfectionAura[InfectionStage], true);

            // When we changed to Stage 4, we want to stay there !
            if (InfectionStage < 4)
                events.ScheduleEvent(EVENT_MUTATED_INFECTION, MutatedInfectionTimer[InfectionStage]);
            break;
        case EVENT_VILE_GAS:
            DoCastAOE(SPELL_VILE_GAS_TRIGGER);
            events.ScheduleEvent(EVENT_VILE_GAS, urand(30000, 35000));
            break;
        default:
            break;
        }
    }

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    DoMeleeAttackIfReady();
}
