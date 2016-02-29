#include "Npc_TheLichKingController.h"

void NpcTheLichKingControllerAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_GLUTTONOUS_ABOMINATION_SUMMONER, 5000);
    _events.ScheduleEvent(EVENT_SUPPRESSER_SUMMONER, 10000);
    _events.ScheduleEvent(EVENT_BLISTERING_ZOMBIE_SUMMONER, 15000);
    _events.ScheduleEvent(EVENT_RISEN_ARCHMAGE_SUMMONER, 20000);
    _events.ScheduleEvent(EVENT_BLAZING_SKELETON_SUMMONER, 30000);
    me->SetReactState(REACT_PASSIVE);
}

void NpcTheLichKingControllerAI::EnterEvadeMode()
{
    if (!_EnterEvadeMode())
        return;

    me->RemoveAurasDueToSpell(SPELL_TIMER_GLUTTONOUS_ABOMINATION);
    me->RemoveAurasDueToSpell(SPELL_TIMER_SUPPRESSER);
    me->RemoveAurasDueToSpell(SPELL_TIMER_BLISTERING_ZOMBIE);
    me->RemoveAurasDueToSpell(SPELL_TIMER_RISEN_ARCHMAGE);
    me->RemoveAurasDueToSpell(SPELL_TIMER_BLAZING_SKELETON);
    _summons.DespawnAll();

    me->GetMotionMaster()->MoveTargetedHome();
    Reset();
}

void NpcTheLichKingControllerAI::JustReachedHome()
{
    me->setActive(false);
}

void NpcTheLichKingControllerAI::EnterCombat(Unit* )
{
    Talk(SAY_LICH_KING_INTRO);
    me->setActive(true);
}

void NpcTheLichKingControllerAI::JustSummoned(Creature* summon)
{
    _summons.Summon(summon);
    // must not be in dream phase
    summon->SetPhaseMask((summon->GetPhaseMask() & ~0x10), true);
    if (summon->GetEntry() != NPC_SUPPRESSER)
        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
            summon->AI()->AttackStart(target);
}

void NpcTheLichKingControllerAI::UpdateAI(const uint32 diff)
{
    if (_instance->GetBossState(DATA_VALITHRIA_DREAMWALKER) != IN_PROGRESS)
    {
        EnterEvadeMode();
        return;
    }

    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_GLUTTONOUS_ABOMINATION_SUMMONER:
                DoCast(me, SPELL_TIMER_GLUTTONOUS_ABOMINATION);
                break;
            case EVENT_SUPPRESSER_SUMMONER:
                DoCast(me, SPELL_TIMER_SUPPRESSER);
                break;
            case EVENT_BLISTERING_ZOMBIE_SUMMONER:
                DoCast(me, SPELL_TIMER_BLISTERING_ZOMBIE);
                break;
            case EVENT_RISEN_ARCHMAGE_SUMMONER:
                DoCast(me, SPELL_TIMER_RISEN_ARCHMAGE);
                break;
            case EVENT_BLAZING_SKELETON_SUMMONER:
                DoCast(me, SPELL_TIMER_BLAZING_SKELETON);
                break;
            default:
                break;
        }
    }
}