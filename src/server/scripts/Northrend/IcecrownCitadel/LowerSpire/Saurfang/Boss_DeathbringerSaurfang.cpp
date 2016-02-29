#include "Boss_DeathbringerSaurfang.h"

//////////////////////////////
// Deathbringer Saurfang AI //
//////////////////////////////

void DeathbringerSaurfangAI::Reset()
{
    if (instance->GetBossState(DATA_DEATHBRINGER_SAURFANG) != DONE)
    {
        _Reset();
        me->SetReactState(REACT_AGGRESSIVE);
        events.SetPhase(PHASE_COMBAT);
        _frenzied = false;
        _dead = false;
        me->SetPower(POWER_ENERGY, 0);
        DoCast(me, SPELL_ZERO_POWER, true);
        DoCast(me, SPELL_BLOOD_LINK, true);
        DoCast(me, SPELL_BLOOD_POWER, true);
        DoCast(me, SPELL_MARK_OF_THE_FALLEN_CHAMPION_S, true);
        DoCast(me, SPELL_RUNE_OF_BLOOD_S, true);
        me->RemoveAurasDueToSpell(SPELL_BERSERK);
        me->RemoveAurasDueToSpell(SPELL_FRENZY);
    }
    else
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
        DoCast(me, SPELL_PERMANENT_FEIGN_DEATH);
    }
}

void DeathbringerSaurfangAI::EnterCombat(Unit* who)
{
    if (_dead)
        return;

    if (!instance->CheckRequiredBosses(DATA_DEATHBRINGER_SAURFANG, who->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    // oh just screw intro, enter combat - no exploits please
    me->setActive(true);
    DoZoneInCombat();

    events.Reset();
    events.SetPhase(PHASE_COMBAT);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    if (!_introDone)
    {
        DoCast(me, SPELL_GRIP_OF_AGONY);
        if (Creature* creature = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SAURFANG_EVENT_NPC)))
            creature->AI()->DoAction(ACTION_INTERRUPT_INTRO);
    }

    _introDone = true;

    Talk(SAY_AGGRO);
    events.ScheduleEvent(EVENT_SUMMON_BLOOD_BEAST, 30000, 0, PHASE_COMBAT);
    events.ScheduleEvent(EVENT_BERSERK, IsHeroic() ? 360000 : 480000, 0, PHASE_COMBAT);
    events.ScheduleEvent(EVENT_BOILING_BLOOD, 15500, 0, PHASE_COMBAT);
    events.ScheduleEvent(EVENT_BLOOD_NOVA, 17000, 0, PHASE_COMBAT);
    events.ScheduleEvent(EVENT_RUNE_OF_BLOOD, 20000, 0, PHASE_COMBAT);

    _fallenChampionCastCount = 0;
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_OF_THE_FALLEN_CHAMPION);
    instance->SetBossState(DATA_DEATHBRINGER_SAURFANG, IN_PROGRESS);
}

void DeathbringerSaurfangAI::AttackStart(Unit* victim)
{
    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
        return;

    ScriptedAI::AttackStart(victim);
}

void DeathbringerSaurfangAI::EnterEvadeMode()
{
    ScriptedAI::EnterEvadeMode();
    if (_introDone)
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    _DespawnAtEvade();
}

void DeathbringerSaurfangAI::JustReachedHome()
{
    _JustReachedHome();
    if (!_dead)
        instance->SetBossState(DATA_DEATHBRINGER_SAURFANG, FAIL);
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_OF_THE_FALLEN_CHAMPION);
}

void DeathbringerSaurfangAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void DeathbringerSaurfangAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (!_frenzied && me->HealthBelowPctDamaged(31, damage)) // AT 30%, not below
    {
        _frenzied = true;
        DoCast(me, SPELL_FRENZY);
        Talk(SAY_FRENZY);
    }

    if (me->HealthBelowPctDamaged(1, damage))
    {
        if (!_dead)
        {
            _dead = true;
            damage = std::min(damage, me->GetHealth() - me->CountPctFromMaxHealth(1));

            _JustDied();
            _EnterEvadeMode();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);

            DoCastAOE(SPELL_REMOVE_MARKS_OF_THE_FALLEN_CHAMPION);
            DoCast(me, SPELL_ACHIEVEMENT, true);
            Talk(SAY_DEATH);

            //instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_OF_THE_FALLEN_CHAMPION);
            DoCast(me, SPELL_PERMANENT_FEIGN_DEATH);
            if (Creature* creature = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SAURFANG_EVENT_NPC)))
                creature->AI()->DoAction(ACTION_START_OUTRO);
        }
        else
            damage = 0;
    }
}

void DeathbringerSaurfangAI::JustSummoned(Creature* summon)
{
    summon->SetReactState(REACT_PASSIVE);
    summon->AttackStop();
    summon->GetMotionMaster()->MovementExpired();

    summon->CastSpell(summon, SPELL_BLOOD_LINK_BEAST, true);
    summon->CastSpell(summon, SPELL_RESISTANT_SKIN, true);
    summons.Summon(summon);
}

void DeathbringerSaurfangAI::SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
{
    summons.Despawn(summon);
}

void DeathbringerSaurfangAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE && id != POINT_SAURFANG)
        return;

    instance->HandleGameObject(instance->GetData64(GO_SAURFANG_S_DOOR), false);
}

void DeathbringerSaurfangAI::SpellHitTarget(Unit* target, SpellInfo const* spell)
{
    switch (spell->Id)
    {
    case SPELL_MARK_OF_THE_FALLEN_CHAMPION:
        Talk(SAY_MARK_OF_THE_FALLEN_CHAMPION);
        break;
    case 72255: // Mark of the Fallen Champion, triggered id
    case 72444:
    case 72445:
    case 72446:
        if (me->GetPower(POWER_ENERGY) != me->GetMaxPower(POWER_ENERGY))
            target->CastCustomSpell(SPELL_BLOOD_LINK_DUMMY, SPELLVALUE_BASE_POINT0, 1, me, TRIGGERED_FULL_MASK);
        break;
    default:
        break;
    }
}

void DeathbringerSaurfangAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim() && !(events.GetPhaseMask() & PHASE_INTRO_MASK))
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_INTRO_ALLIANCE_2:
            Talk(SAY_INTRO_ALLIANCE_2);
            break;
        case EVENT_INTRO_ALLIANCE_3:
            Talk(SAY_INTRO_ALLIANCE_3);
            break;
        case EVENT_INTRO_ALLIANCE_6:
            Talk(SAY_INTRO_ALLIANCE_6);
            Talk(SAY_INTRO_ALLIANCE_7);
            DoCast(me, SPELL_GRIP_OF_AGONY);
            break;
        case EVENT_INTRO_HORDE_2:
            Talk(SAY_INTRO_HORDE_2);
            break;
        case EVENT_INTRO_HORDE_4:
            Talk(SAY_INTRO_HORDE_4);
            break;
        case EVENT_INTRO_HORDE_9:
            DoCast(me, SPELL_GRIP_OF_AGONY);
            Talk(SAY_INTRO_HORDE_9);
            break;
        case EVENT_INTRO_FINISH:
            events.SetPhase(PHASE_COMBAT);
            _introDone = true;
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            break;
        case EVENT_SUMMON_BLOOD_BEAST:
            for (uint32 i10 = 0; i10 < 2; ++i10) // 2 Beasts in 10 Man
                DoCast(me, SPELL_SUMMON_BLOOD_BEAST+i10);
            if (Is25ManRaid())
                for (uint32 i25 = 0; i25 < 2; ++i25) // 2 + 2 Additional in 25
                    DoCast(me, SPELL_SUMMON_BLOOD_BEAST_25_MAN+i25);
            if (Is25ManRaid() && IsHeroic()) // 2 + 2 + 1 Additional in heroic 25 man
                DoCast(me, SPELL_SUMMON_BLOOD_BEAST_25_MAN + 2);
            Talk(SAY_BLOOD_BEASTS);
            events.ScheduleEvent(EVENT_SUMMON_BLOOD_BEAST, 40000, 0, PHASE_COMBAT);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_SCENT_OF_BLOOD, 10000, 0, PHASE_COMBAT);
            events.ScheduleEvent(EVENT_START_BLOOD_BEAST, 1500, 0, PHASE_COMBAT); // delay blood beasts
            break;
        case EVENT_BLOOD_NOVA:
            DoCastAOE(SPELL_BLOOD_NOVA_TRIGGER);
            events.ScheduleEvent(EVENT_BLOOD_NOVA, urand(20000, 25000), 0, PHASE_COMBAT);
            break;
        case EVENT_RUNE_OF_BLOOD:
            DoCastVictim(SPELL_RUNE_OF_BLOOD);
            events.ScheduleEvent(EVENT_RUNE_OF_BLOOD, urand(20000, 25000), 0, PHASE_COMBAT);
            break;
        case EVENT_BOILING_BLOOD:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
            {
                DoCast(target, BOILING_BLOOD_HELPER);
                if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(BOILING_BLOOD_HELPER))
                    Aura::TryRefreshStackOrCreate(spellInfo, MAX_EFFECT_MASK, target, me);
            }
            events.ScheduleEvent(EVENT_BOILING_BLOOD, urand(15000, 20000), 0, PHASE_COMBAT);
            break;
        case EVENT_BERSERK:
            DoCast(me, SPELL_BERSERK);
            Talk(SAY_BERSERK);
            break;
        case EVENT_SCENT_OF_BLOOD:
            if (!summons.empty())
            {
                Talk(EMOTE_SCENT_OF_BLOOD);
                DoCastAOE(SPELL_SCENT_OF_BLOOD);
            }
            break;
        case EVENT_START_BLOOD_BEAST:
            if (!summons.empty())
            {
                for (SummonList::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                {
                    if (Creature* pCreature = sObjectAccessor->GetCreature(*me, *itr))
                    {
                        if (pCreature->isAlive())
                        {
                            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                            pCreature->SetReactState(REACT_AGGRESSIVE);
                            DoZoneInCombat(pCreature, 50.0f, false);
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}

uint32 DeathbringerSaurfangAI::GetData(uint32 type)
{
    if (type == DATA_MADE_A_MESS)
        if (_fallenChampionCastCount < RAID_MODE<uint32>(3, 5, 3, 5))
            return 1;

    return 0;
}

void DeathbringerSaurfangAI::DoAction(int32 const action)
{
    switch (action)
    {
    case PHASE_INTRO_A:
    case PHASE_INTRO_H:
        {
            if (GameObject* teleporter = GameObject::GetGameObject(*me, instance->GetData64(GO_SCOURGE_TRANSPORTER_SAURFANG)))
            {
                instance->HandleGameObject(0, false, teleporter);
                teleporter->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
            }

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            // controls what events will execute
            events.SetPhase(uint32(action));

            me->SetHomePosition(deathbringerPos.GetPositionX(), deathbringerPos.GetPositionY(), deathbringerPos.GetPositionZ(), me->GetOrientation());
            me->GetMotionMaster()->MovePoint(POINT_SAURFANG, deathbringerPos.GetPositionX(), deathbringerPos.GetPositionY(), deathbringerPos.GetPositionZ());

            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_2, 2500, 0, PHASE_INTRO_A);
            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_3, 20000, 0, PHASE_INTRO_A);

            events.ScheduleEvent(EVENT_INTRO_HORDE_2, 5000, 0, PHASE_INTRO_H);
            break;
        }
    case ACTION_CONTINUE_INTRO:
        {
            if (_introDone)
                return;

            events.ScheduleEvent(EVENT_INTRO_ALLIANCE_6, 6500+500, 0, PHASE_INTRO_A);
            events.ScheduleEvent(EVENT_INTRO_FINISH, 8000, 0, PHASE_INTRO_A);

            events.ScheduleEvent(EVENT_INTRO_HORDE_4, 6500, 0, PHASE_INTRO_H);
            events.ScheduleEvent(EVENT_INTRO_HORDE_9, 46700+1000+500, 0, PHASE_INTRO_H);
            events.ScheduleEvent(EVENT_INTRO_FINISH,  46700+1000+8000, 0, PHASE_INTRO_H);
            break;
        }
    case ACTION_MARK_OF_THE_FALLEN_CHAMPION:
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_MARK_OF_THE_FALLEN_CHAMPION))
            {
                ++_fallenChampionCastCount;
                DoCast(target, SPELL_MARK_OF_THE_FALLEN_CHAMPION);
                me->SetPower(POWER_ENERGY, 0);
                if (Aura* bloodPower = me->GetAura(SPELL_BLOOD_POWER))
                    bloodPower->RecalculateAmountOfEffects();
            }
            break;
        }
    default:
        break;
    }
}
