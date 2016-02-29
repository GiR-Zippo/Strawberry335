#include "Boss_ProfessorPutricide.h"

////////////////////////////
// Professor Putricide AI //
////////////////////////////

void ProfessorPutricideAI::Reset()
{
    if (!(events.GetPhaseMask() & PHASE_MASK_NOT_SELF))
        instance->SetBossState(DATA_PROFESSOR_PUTRICIDE, NOT_STARTED);
    instance->SetData(DATA_NAUSEA_ACHIEVEMENT, uint32(true));

    events.Reset();
    summons.DespawnAll();
    SetPhase(PHASE_NONE);
    _experimentState = EXPERIMENT_STATE_OOZE;
    me->SetReactState(REACT_AGGRESSIVE);
    me->SetWalk(false);
    if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
        me->GetMotionMaster()->MovementExpired();

    me->GetMotionMaster()->MovePoint(POINT_RESET, me->GetHomePosition());

    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DECREASE_SPEED, true);
    me->ApplySpellImmune(SPELL_SLIME_PUDDLE_AURA, IMMUNITY_STATE, SPELL_AURA_NONE, true);

    _experimentGUIDs[0] = 0;
    _experimentGUIDs[1] = 0;
    _phaseTransition = false;
}

void ProfessorPutricideAI::EnterEvadeMode()
{
    _DespawnAtEvade();
    BossAI::EnterEvadeMode();
}

void ProfessorPutricideAI::InitializeAI()
{
    if (instance->GetBossState(DATA_ROTFACE) == DONE && instance->GetBossState(DATA_FESTERGUT) == DONE)
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
    else
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);

    if (!me->isDead())
        Reset();
}

void ProfessorPutricideAI::EnterCombat(Unit* who)
{
    if (events.GetPhaseMask() & PHASE_MASK_NOT_SELF)
        return;

    if (!instance->CheckRequiredBosses(DATA_PROFESSOR_PUTRICIDE, who->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    me->setActive(true);
    events.Reset();
    events.ScheduleEvent(EVENT_BERSERK, 600000);
    events.ScheduleEvent(EVENT_SLIME_PUDDLE, 10000);
    events.ScheduleEvent(EVENT_UNSTABLE_EXPERIMENT, urand(25000, 30000));
    if (IsHeroic())
        events.ScheduleEvent(EVENT_UNBOUND_PLAGUE, 20000);

    SetPhase(PHASE_COMBAT_1);
    Talk(SAY_AGGRO);
    DoCast(me, SPELL_OOZE_TANK_PROTECTION, true);
    DoZoneInCombat(me);

    instance->SetBossState(DATA_PROFESSOR_PUTRICIDE, IN_PROGRESS);
}

void ProfessorPutricideAI::JustReachedHome()
{
    _JustReachedHome();
    if (instance->GetBossState(DATA_ROTFACE) == DONE && instance->GetBossState(DATA_FESTERGUT) == DONE)
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
    me->SetWalk(false);
    if (events.GetPhaseMask() & PHASE_MASK_COMBAT)
        instance->SetBossState(DATA_PROFESSOR_PUTRICIDE, FAIL);
}

void ProfessorPutricideAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void ProfessorPutricideAI::JustDied(Unit* /*killer*/)
{
    _JustDied();
    Talk(SAY_DEATH);
    DoCast(SPELL_MUTATED_PLAGUE_CLEAR);
}

void ProfessorPutricideAI::JustSummoned(Creature* summon)
{
    summons.Summon(summon);
    switch (summon->GetEntry())
    {
        case NPC_MALLEABLE_OOZE_STALKER:
            DoCast(summon, SPELL_MALLEABLE_GOO_H, true);
            return;
        case NPC_GROWING_OOZE_PUDDLE:
            summon->CastSpell(summon, SPELL_GROW_STACKER, true);
            summon->CastSpell(summon, SPELL_SLIME_PUDDLE_AURA, true);
            // blizzard casts this spell 7 times initially (confirmed in sniff)
            for (uint8 i = 0; i < 7; ++i)
                summon->CastSpell(summon, SPELL_GROW, true);
            break;
        case NPC_VOLATILE_OOZE:
            // no possible aura seen in sniff adding the aurastate
            summon->ModifyAuraState(AURA_STATE_UNKNOWN19, true);
            summon->CastSpell(summon, SPELL_OOZE_ERUPTION_SEARCH_PERIODIC, true);
            summon->CastSpell(summon, SPELL_VOLATILE_OOZE_ADHESIVE, false);
            summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            DoZoneInCombat(summon, 150.9f);
            summon->SetInCombatFor(60000);
            summon->SetReactState(REACT_PASSIVE);
            if (_phaseTransition)
                _experimentGUIDs[DATA_OOZE] = summon->GetGUID();
            return;
        case NPC_GAS_CLOUD:
            // no possible aura seen in sniff adding the aurastate
            summon->ModifyAuraState(AURA_STATE_UNKNOWN22, true);
            summon->CastSpell(summon, SPELL_GASEOUS_BLOAT_PROC, true);
            summon->CastCustomSpell(SPELL_GASEOUS_BLOAT, SPELLVALUE_AURA_STACK, 10, summon, TRIGGERED_NONE);
            summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            DoZoneInCombat(summon, 150.9f);
            summon->SetInCombatFor(60000);
            summon->SetReactState(REACT_PASSIVE);
            if (_phaseTransition)
                _experimentGUIDs[DATA_GAS] = summon->GetGUID();
            return;
        case NPC_CHOKING_GAS_BOMB:
            summon->CastSpell(summon, SPELL_CHOKING_GAS_BOMB_PERIODIC, true);
            summon->CastSpell(summon, SPELL_CHOKING_GAS_EXPLOSION_TRIGGER, true);
            return;
        case NPC_MUTATED_ABOMINATION_10:
        case NPC_MUTATED_ABOMINATION_25:
            return;
    }

    if (me->isInCombat())
    {
        if (events.GetPhaseMask() & PHASE_FESTERGUT)
            if (Creature* festergut = Unit::GetCreature(*me, instance->GetData64(DATA_FESTERGUT)))
                if (festergut->isAlive())
                    DoZoneInCombat(summon);
        if (events.GetPhaseMask() & PHASE_ROTFACE)
            if (Creature* rotface = Unit::GetCreature(*me, instance->GetData64(DATA_ROTFACE)))
                if (rotface->isAlive())
                    DoZoneInCombat(summon);
    }
}

void ProfessorPutricideAI::SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
{
    if (!_experimentGUIDs[DATA_GAS] || !_experimentGUIDs[DATA_OOZE])
        return;
    else if (summon->GetGUID() == _experimentGUIDs[DATA_GAS])
    {
        if (Creature* ooze = ObjectAccessor::GetCreature(*summon, _experimentGUIDs[DATA_OOZE]))
            if (ooze->isAlive())
                return;
    }
    else if (summon->GetGUID() == _experimentGUIDs[DATA_OOZE])
    {
        if (Creature* gas = ObjectAccessor::GetCreature(*summon, _experimentGUIDs[DATA_GAS]))
            if (gas->isAlive())
                return;
    }
    else
        return;

    if (_phase == PHASE_COMBAT_3)
        summons.remove_if(AbominationDespawner(me));
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GAS_VARIABLE_HELPER);
    _experimentGUIDs[DATA_GAS] = 0;
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OOZE_VARIABLE_HELPER);
    _experimentGUIDs[DATA_OOZE] = 0;
}

void ProfessorPutricideAI::DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
{
    switch (_phase)
    {
        case PHASE_COMBAT_1:
            if (HealthAbovePct(80))
                return;
            me->SetReactState(REACT_PASSIVE);
            DoAction(ACTION_CHANGE_PHASE);
            break;
        case PHASE_COMBAT_2:
            if (HealthAbovePct(35))
                return;
            me->SetReactState(REACT_PASSIVE);
            DoAction(ACTION_CHANGE_PHASE);
            break;
    }
}

void ProfessorPutricideAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;
    switch (id)
    {
    case POINT_FESTERGUT:
        if (Creature* festergut = Unit::GetCreature(*me, instance->GetData64(DATA_FESTERGUT)))
            if (festergut->isInCombat())
            {
                instance->SetBossState(DATA_FESTERGUT, IN_PROGRESS); // needed here for delayed gate close
                me->SetSpeed(MOVE_RUN, _baseSpeed, true);
                DoAction(ACTION_FESTERGUT_GAS);

                festergut->CastSpell(festergut, SPELL_GASEOUS_BLIGHT_LARGE, false, NULL, NULL, festergut->GetGUID());
            }
            break;
        case POINT_ROTFACE:
            if (Creature* rotface = Unit::GetCreature(*me, instance->GetData64(DATA_ROTFACE)))
                if (rotface->isInCombat())
                {
                    instance->SetBossState(DATA_ROTFACE, IN_PROGRESS);   // needed here for delayed gate close
                    me->SetSpeed(MOVE_RUN, _baseSpeed, true);
                    DoAction(ACTION_ROTFACE_OOZE);
                    events.ScheduleEvent(EVENT_ROTFACE_OOZE_FLOOD, 25000, 0, PHASE_ROTFACE);
                }
            break;
        case POINT_TABLE:
            // stop attack
            me->GetMotionMaster()->MoveIdle();
            me->SetSpeed(MOVE_RUN, _baseSpeed, true);
            if (GameObject* table = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_PUTRICIDE_TABLE)))
                me->SetFacingToObject(table);
            // operating on new phase already
            switch (_phase)
            {
                case PHASE_COMBAT_2:
                {
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(SPELL_CREATE_CONCOCTION);
                    DoCast(me, SPELL_CREATE_CONCOCTION);
                    events.ScheduleEvent(EVENT_PHASE_TRANSITION, sSpellMgr->GetSpellForDifficultyFromSpell(spell, me)->CalcCastTime() + 100);
                    break;
                }
                case PHASE_COMBAT_3:
                {
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(SPELL_GUZZLE_POTIONS);
                    DoCast(me, SPELL_GUZZLE_POTIONS);
                    events.ScheduleEvent(EVENT_PHASE_TRANSITION, sSpellMgr->GetSpellForDifficultyFromSpell(spell, me)->CalcCastTime() + 100);
                    break;
                }
            }
            break;
    }
}

void ProfessorPutricideAI::DoAction(int32 const action)
{
    switch (action)
    {
        case ACTION_FESTERGUT_COMBAT:
            SetPhase(PHASE_FESTERGUT);
            me->SetSpeed(MOVE_RUN, _baseSpeed*2.0f, true);
            me->GetMotionMaster()->MovePoint(POINT_FESTERGUT, festergutWatchPos);
            me->SetReactState(REACT_PASSIVE);
            DoZoneInCombat(me);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_FESTERGUT_GOO, urand(13000, 18000), 0, PHASE_FESTERGUT);
            break;
        case ACTION_FESTERGUT_GAS:
            Talk(SAY_FESTERGUT_GASEOUS_BLIGHT);
            DoCast(me, SPELL_RELEASE_GAS_VISUAL, true);
            break;
        case ACTION_FESTERGUT_DEATH:
            events.ScheduleEvent(EVENT_FESTERGUT_DIES, 4000, 0, PHASE_FESTERGUT);
            break;
        case ACTION_ROTFACE_COMBAT:
        {
            SetPhase(PHASE_ROTFACE);
            me->SetSpeed(MOVE_RUN, _baseSpeed*2.0f, true);
            me->GetMotionMaster()->MovePoint(POINT_ROTFACE, rotfaceWatchPos);
            me->SetReactState(REACT_PASSIVE);
            _oozeFloodStage = 0;
            DoZoneInCombat(me);
            // init random sequence of floods
            if (Creature* rotface = Unit::GetCreature(*me, instance->GetData64(DATA_ROTFACE)))
            {
                std::list<Creature*> list;
                GetCreatureListWithEntryInGrid(list, rotface, NPC_PUDDLE_STALKER, 36.0f);
                if (list.size() > 4)
                {
                    list.sort(Trinity::ObjectDistanceOrderPred(rotface));
                    do
                    {
                        list.pop_back();
                    }
                    while (list.size() > 4);
                }

                uint8 i = 0;
                while (!list.empty())
                {
                    std::list<Creature*>::iterator itr = list.begin();
                    std::advance(itr, urand(0, list.size() - 1));
                    _oozeFloodDummyGUIDs[i++] = (*itr)->GetGUID();
                    list.erase(itr);
                }
            }
            break;
        }
        case ACTION_ROTFACE_OOZE:
            Talk(SAY_ROTFACE_OOZE_FLOOD);
            if (Creature* dummy = Unit::GetCreature(*me, _oozeFloodDummyGUIDs[_oozeFloodStage]))
                dummy->CastSpell(dummy, oozeFloodSpells[_oozeFloodStage], true, NULL, NULL, me->GetGUID()); // cast from self for LoS (with prof's GUID for logs)
            if (++_oozeFloodStage == 4)
                _oozeFloodStage = 0;
            break;
        case ACTION_ROTFACE_DEATH:
            events.ScheduleEvent(EVENT_ROTFACE_DIES, 4500, 0, PHASE_ROTFACE);
            break;
        case ACTION_CHANGE_PHASE:
            me->SetSpeed(MOVE_RUN, _baseSpeed*2.0f, true);

            events.DelayEvents(40000);
            events.CancelEvent(EVENT_SLIME_PUDDLE);
            events.CancelEvent(EVENT_UNSTABLE_EXPERIMENT);
            if (IsHeroic())
                events.CancelEvent(EVENT_UNBOUND_PLAGUE);
            events.CancelEvent(EVENT_MALLEABLE_GOO);
            events.CancelEvent(EVENT_CHOKING_GAS_BOMB);

            me->AttackStop();
            if (!IsHeroic())
            {
                DoCast(me, SPELL_TEAR_GAS);
                events.ScheduleEvent(EVENT_TEAR_GAS, 2500);
            }
            else
            {
                _phaseTransition = true;
                Talk(SAY_PHASE_TRANSITION_HEROIC);
                DoCast(me, SPELL_UNSTABLE_EXPERIMENT, true);
                DoCast(me, SPELL_UNSTABLE_EXPERIMENT, true);
                // cast variables
                std::list<Unit*> targetList;
                {
                    const std::list<HostileReference*>& threatlist = me->getThreatManager().getThreatList();
                    for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                        if ((*itr)->getTarget()->GetTypeId() == TYPEID_PLAYER)
                            targetList.push_back((*itr)->getTarget());
                }

                size_t half = targetList.size() / 2;
                // half gets ooze variable
                while (half < targetList.size())
                {
                    std::list<Unit*>::iterator itr = targetList.begin();
                    advance(itr, urand(0, targetList.size() - 1));
                    (*itr)->CastSpell(*itr, SPELL_OOZE_VARIABLE, true);
                    targetList.erase(itr);
                }
                // and half gets gas
                for (std::list<Unit*>::iterator itr = targetList.begin(); itr != targetList.end(); ++itr)
                    (*itr)->CastSpell(*itr, SPELL_GAS_VARIABLE, true);
                me->GetMotionMaster()->MovePoint(POINT_TABLE, tablePos);
            }
            switch (_phase)
            {
                case PHASE_COMBAT_1:
                    SetPhase(PHASE_COMBAT_2);
                    break;
                case PHASE_COMBAT_2:
                    SetPhase(PHASE_COMBAT_3);

                    if (!IsHeroic())
                        summons.remove_if(AbominationDespawner(me));
                    break;
            }
            break;
    }
}

uint32 ProfessorPutricideAI::GetData(uint32 type)
{
    switch (type)
    {
        case DATA_EXPERIMENT_STAGE:
        {
            // ALSO MODIFIES!
            uint32 ret = uint32(_experimentState);
            _experimentState ^= true;
            return ret;
        }
        case DATA_PHASE:
            return _phase;
        case DATA_ABOMINATION:
            summons.RemoveNotExisting();
            return uint32(summons.HasEntry(NPC_MUTATED_ABOMINATION_10) || summons.HasEntry(NPC_MUTATED_ABOMINATION_25));
    }

    return 0;
}

void ProfessorPutricideAI::UpdateAI(uint32 const diff)
{
    if ((!(events.GetPhaseMask() & PHASE_MASK_NOT_SELF) && !UpdateVictim()) || !CheckInRoom())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_FESTERGUT_DIES:
                Talk(SAY_FESTERGUT_DEATH);
                EnterEvadeMode();
                break;
            case EVENT_FESTERGUT_GOO:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me, true)))
                {
                    target->CastSpell(target, SPELL_MALLEABLE_GOO_SUMMON);
                    DoCast(target, SPELL_MALLEABLE_GOO_H, true);
                }
                events.ScheduleEvent(EVENT_FESTERGUT_GOO, (Is25ManRaid() ? 10000 : 30000), 0, PHASE_FESTERGUT);
                break;
            case EVENT_ROTFACE_DIES:
                Talk(SAY_ROTFACE_DEATH);
                EnterEvadeMode();
                break;
            case EVENT_ROTFACE_OOZE_FLOOD:
                DoAction(ACTION_ROTFACE_OOZE);
                events.ScheduleEvent(EVENT_ROTFACE_OOZE_FLOOD, 25000, 0, PHASE_ROTFACE);
                break;
            case EVENT_BERSERK:
                Talk(SAY_BERSERK);
                DoCast(me, SPELL_BERSERK2);
                break;
            case EVENT_SLIME_PUDDLE:
            {
                std::list<Unit*> targets;
                SelectTargetList(targets, 2, SELECT_TARGET_RANDOM, 0.0f, true);
                if (!targets.empty())
                    for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        DoCast(*itr, SPELL_SLIME_PUDDLE_TRIGGER);
                events.ScheduleEvent(EVENT_SLIME_PUDDLE, 35000);
                break;
            }
            case EVENT_UNSTABLE_EXPERIMENT:
                Talk(EMOTE_UNSTABLE_EXPERIMENT);
                DoCast(me, SPELL_UNSTABLE_EXPERIMENT);
                events.ScheduleEvent(EVENT_UNSTABLE_EXPERIMENT, urand(35000, 40000));
                break;
            case EVENT_TEAR_GAS:
                me->GetMotionMaster()->MovePoint(POINT_TABLE, tablePos);
                DoCast(me, SPELL_TEAR_GAS_PERIODIC_TRIGGER, true);
                break;
            case EVENT_RESUME_ATTACK:
                _phaseTransition = false;
                me->SetReactState(REACT_DEFENSIVE);
                AttackStart(me->getVictim());
                // remove Tear Gas
                me->RemoveAurasDueToSpell(SPELL_TEAR_GAS_PERIODIC_TRIGGER);
                instance->DoRemoveAurasDueToSpellOnPlayers(71615);
                DoCastAOE(SPELL_TEAR_GAS_CANCEL);

                switch (_phase)
                {
                    case PHASE_COMBAT_2:
                        events.RescheduleEvent(EVENT_SLIME_PUDDLE, 10000);
                        events.RescheduleEvent(EVENT_UNSTABLE_EXPERIMENT, urand(25000, 30000));

                        if (IsHeroic())
                            events.RescheduleEvent(EVENT_UNBOUND_PLAGUE, 20000);

                        events.RescheduleEvent(EVENT_MALLEABLE_GOO, urand(21000, 26000));
                        events.RescheduleEvent(EVENT_CHOKING_GAS_BOMB, urand(35000, 40000));
                        break;
                    case PHASE_COMBAT_3:
                        events.ScheduleEvent(EVENT_MUTATED_PLAGUE, 25000);

                        events.RescheduleEvent(EVENT_SLIME_PUDDLE, 10000);

                        if (IsHeroic())
                            events.RescheduleEvent(EVENT_UNBOUND_PLAGUE, 20000);

                        events.RescheduleEvent(EVENT_MALLEABLE_GOO, urand(21000, 26000));
                        events.RescheduleEvent(EVENT_CHOKING_GAS_BOMB, urand(35000, 40000));
                        break;
                }
                break;
            case EVENT_MALLEABLE_GOO:
                if (Is25ManRaid())
                {
                    std::list<Unit*> targets;
                    SelectTargetList(targets, 2, SELECT_TARGET_RANDOM, -7.0f, true);
                    if (!targets.empty())
                    {
                        Talk(EMOTE_MALLEABLE_GOO);
                        for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            DoCast(*itr, SPELL_MALLEABLE_GOO);
                    }
                }
                else
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, -7.0f, true))
                    {
                        Talk(EMOTE_MALLEABLE_GOO);
                        DoCast(target, SPELL_MALLEABLE_GOO);
                    }
                }
                events.ScheduleEvent(EVENT_MALLEABLE_GOO, urand(25000, 30000));
                break;
            case EVENT_CHOKING_GAS_BOMB:
                Talk(EMOTE_CHOKING_GAS_BOMB);
                DoCast(me, SPELL_CHOKING_GAS_BOMB);
                events.ScheduleEvent(EVENT_CHOKING_GAS_BOMB, urand(35000, 40000));
                break;
            case EVENT_UNBOUND_PLAGUE:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                {
                    me->CastCustomSpell(SPELL_UNBOUND_PLAGUE, SPELLVALUE_BASE_POINT0, 775, target);
                    DoCast(target, SPELL_UNBOUND_PLAGUE_SEARCHER);
                }
                events.ScheduleEvent(EVENT_UNBOUND_PLAGUE, 60000);
                break;
            case EVENT_MUTATED_PLAGUE:
                DoCastVictim(SPELL_MUTATED_PLAGUE);
                events.ScheduleEvent(EVENT_MUTATED_PLAGUE, 10000);
                break;
            case EVENT_PHASE_TRANSITION:
            {
                switch (_phase)
                {
                    case PHASE_COMBAT_2:
                        if (Creature* face = me->FindNearestCreature(NPC_TEAR_GAS_TARGET_STALKER, 50.0f))
                            me->SetFacingToObject(face);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                        Talk(SAY_TRANSFORM_1);
                        events.ScheduleEvent(EVENT_RESUME_ATTACK, 5500, 0, PHASE_COMBAT_2);
                        break;
                    case PHASE_COMBAT_3:
                        if (Creature* face = me->FindNearestCreature(NPC_TEAR_GAS_TARGET_STALKER, 50.0f))
                            me->SetFacingToObject(face);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                        Talk(SAY_TRANSFORM_2);
                        events.ScheduleEvent(EVENT_RESUME_ATTACK, 8500, 0, PHASE_COMBAT_3);
                        break;
                }
            }
        }
    }

    DoMeleeAttackIfReady();
}

void ProfessorPutricideAI::SetPhase(Phases newPhase)
{
    _phase = newPhase;
    events.SetPhase(newPhase);
}
