#include "Boss_LadyDeathwhisper.h"
#include "Npc_Darnavan.h"

//////////////////////////
// Lady Deathwhsiper AI //
//////////////////////////

void LadyDeathwhisperAI::Reset()
{
    _Reset();
    me->SetPower(POWER_MANA, me->GetMaxPower(POWER_MANA));
    events.SetPhase(PHASE_ONE);
    _waveCounter = 0;
    _nextVengefulShadeTargetGUID = 0;
    _darnavanGUID = 0;
    DoCast(me, SPELL_SHADOW_CHANNELING);
    me->RemoveAurasDueToSpell(SPELL_BERSERK);
    me->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
    me->SetReactState(REACT_AGGRESSIVE);
}

void LadyDeathwhisperAI::EnterEvadeMode()
{
    BossAI::EnterEvadeMode();
    _DespawnAtEvade();
}

void LadyDeathwhisperAI::MoveInLineOfSight(Unit* who)
{
    if (!_introDone && me->IsWithinDistInMap(who, 110.0f))
    {
        _introDone = true;
        Talk(SAY_INTRO_1);
        events.SetPhase(PHASE_INTRO);
        events.ScheduleEvent(EVENT_INTRO_2, 11000, 0, PHASE_INTRO);
        events.ScheduleEvent(EVENT_INTRO_3, 21000, 0, PHASE_INTRO);
        events.ScheduleEvent(EVENT_INTRO_4, 31500, 0, PHASE_INTRO);
        events.ScheduleEvent(EVENT_INTRO_5, 39500, 0, PHASE_INTRO);
        events.ScheduleEvent(EVENT_INTRO_6, 48500, 0, PHASE_INTRO);
        events.ScheduleEvent(EVENT_INTRO_7, 58000, 0, PHASE_INTRO);
    }
}

void LadyDeathwhisperAI::AttackStart(Unit* victim)
{
    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
        return;

    if (victim && me->Attack(victim, true) && !events.IsInPhase(PHASE_ONE))
        me->GetMotionMaster()->MoveChase(victim);
}

void LadyDeathwhisperAI::EnterCombat(Unit* who)
{
    if (!instance->CheckRequiredBosses(DATA_LADY_DEATHWHISPER, who->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    me->setActive(true);
    DoZoneInCombat();

    events.Reset();
    events.SetPhase(PHASE_ONE);
    // phase-independent events
    events.ScheduleEvent(EVENT_BERSERK, 600000);
    events.ScheduleEvent(EVENT_DEATH_AND_DECAY, 10000);
    // phase one only
    events.ScheduleEvent(EVENT_P1_SUMMON_WAVE, 5000, 0, PHASE_ONE);
    events.ScheduleEvent(EVENT_P1_SHADOW_BOLT, urand(5500, 6000), 0, PHASE_ONE);
    events.ScheduleEvent(EVENT_P1_EMPOWER_CULTIST, urand(20000, 30000), 0, PHASE_ONE);
    if (GetDifficulty() != RAID_DIFFICULTY_10MAN_NORMAL)
        events.ScheduleEvent(EVENT_DOMINATE_MIND_H, 27000);

    Talk(SAY_AGGRO);
    DoStartNoMovement(who);
    me->RemoveAurasDueToSpell(SPELL_SHADOW_CHANNELING);
    DoCast(me, SPELL_MANA_BARRIER, true);

    instance->SetBossState(DATA_LADY_DEATHWHISPER, IN_PROGRESS);
}

void LadyDeathwhisperAI::JustDied(Unit* killer)
{
    Talk(SAY_DEATH);

    std::set<uint32> livingAddEntries;
    // Full House achievement
    for (SummonList::iterator itr = summons.begin(); itr != summons.end(); ++itr)
        if (Unit* unit = ObjectAccessor::GetUnit(*me, *itr))
            if (unit->isAlive() && unit->GetEntry() != NPC_VENGEFUL_SHADE)
                livingAddEntries.insert(unit->GetEntry());

    if (livingAddEntries.size() >= 5)
        instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_FULL_HOUSE, 0, me);

    if (Creature* darnavan = ObjectAccessor::GetCreature(*me, _darnavanGUID))
    {
        if (darnavan->isAlive())
        {
            darnavan->setFaction(35);
            darnavan->CombatStop(true);
            darnavan->GetMotionMaster()->MoveIdle();
            darnavan->SetReactState(REACT_PASSIVE);
            darnavan->m_Events.AddEvent(new DarnavanMoveEvent(*darnavan), darnavan->m_Events.CalculateTime(10000));
            darnavan->AI()->Talk(SAY_DARNAVAN_RESCUED);
            if (Player* owner = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                if (Group* group = owner->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                        if (Player* member = itr->getSource())
                            member->KilledMonsterCredit(NPC_DARNAVAN_CREDIT, 0);
                }
                else
                    owner->KilledMonsterCredit(NPC_DARNAVAN_CREDIT, 0);
            }
        }
    }

    instance->SetBossState(DATA_LADY_DEATHWHISPER, DONE);
    _JustDied();
}

void LadyDeathwhisperAI::JustReachedHome()
{
    _JustReachedHome();
    instance->SetBossState(DATA_LADY_DEATHWHISPER, FAIL);

    summons.DespawnAll();
    if (Creature* darnavan = ObjectAccessor::GetCreature(*me, _darnavanGUID))
    {
        darnavan->DespawnOrUnsummon();
        _darnavanGUID = 0;
    }
}

void LadyDeathwhisperAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void LadyDeathwhisperAI::DamageTaken(Unit* /*damageDealer*/, uint32& damage)
{
    // phase transition
    if (events.IsInPhase(PHASE_ONE) && damage > me->GetPower(POWER_MANA))
    {
        Talk(SAY_PHASE_2);
        Talk(EMOTE_PHASE_2);
        DoStartMovement(me->getVictim());
        damage -= me->GetPower(POWER_MANA);
        me->SetPower(POWER_MANA, 0);
        me->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
        events.SetPhase(PHASE_TWO);
        events.ScheduleEvent(EVENT_P2_FROSTBOLT, urand(10000, 12000), 0, PHASE_TWO);
        events.ScheduleEvent(EVENT_P2_FROSTBOLT_VOLLEY, urand(19000, 21000), 0, PHASE_TWO);
        events.ScheduleEvent(EVENT_P2_TOUCH_OF_INSIGNIFICANCE, urand(6000, 9000), 0, PHASE_TWO);
        events.ScheduleEvent(EVENT_P2_SUMMON_SHADE, 13000, 0, PHASE_TWO);
        // on heroic mode Lady Deathwhisper is immune to taunt effects in phase 2 and continues summoning adds
        if (IsHeroic())
        {
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            events.ScheduleEvent(EVENT_P2_SUMMON_WAVE, 45000, 0, PHASE_TWO);
        }
    }
}

void LadyDeathwhisperAI::JustSummoned(Creature* summon)
{
    if (summon->GetEntry() == NPC_DARNAVAN)
        _darnavanGUID = summon->GetGUID();
    else
        summons.Summon(summon);

    Unit* target = NULL;
    if (summon->GetEntry() == NPC_VENGEFUL_SHADE)
    {
        target = ObjectAccessor::GetUnit(*me, _nextVengefulShadeTargetGUID);   // Vengeful Shade
        _nextVengefulShadeTargetGUID = 0;
    }
    else
        target = SelectTarget(SELECT_TARGET_RANDOM);                        // Wave adds

    summon->AI()->AttackStart(target);                                      // CAN be NULL
    if (summon->GetEntry() == NPC_REANIMATED_FANATIC)
        summon->CastSpell(summon, SPELL_FANATIC_S_DETERMINATION, true);
    else if (summon->GetEntry() == NPC_REANIMATED_ADHERENT)
        summon->CastSpell(summon, SPELL_ADHERENT_S_DETERMINATION, true);
}

void LadyDeathwhisperAI::UpdateAI(uint32 const diff)
{
    if ((!UpdateVictim() && !events.IsInPhase(PHASE_INTRO)) || !CheckInRoom())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING) && !events.IsInPhase(PHASE_INTRO))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_INTRO_2:
            Talk(SAY_INTRO_2);
            break;
        case EVENT_INTRO_3:
            Talk(SAY_INTRO_3);
            break;
        case EVENT_INTRO_4:
            Talk(SAY_INTRO_4);
            break;
        case EVENT_INTRO_5:
            Talk(SAY_INTRO_5);
            break;
        case EVENT_INTRO_6:
            Talk(SAY_INTRO_6);
            break;
        case EVENT_INTRO_7:
            Talk(SAY_INTRO_7);
            break;
        case EVENT_DEATH_AND_DECAY:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(target, DEATH_AND_DECAY_HELPER);
            events.ScheduleEvent(EVENT_DEATH_AND_DECAY, urand(22000, 30000));
            break;
        case EVENT_DOMINATE_MIND_H:
            Talk(SAY_DOMINATE_MIND);
            for (uint8 i = 0; i < _dominateMindCount; i++)
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_DOMINATE_MIND_H))
                    DoCast(target, SPELL_DOMINATE_MIND_H);
            events.ScheduleEvent(EVENT_DOMINATE_MIND_H, urand(40000, 45000));
            break;
        case EVENT_P1_SUMMON_WAVE:
            SummonWaveP1();
            events.ScheduleEvent(EVENT_P1_SUMMON_WAVE, IsHeroic() ? 45000 : 60000, 0, PHASE_ONE);
            break;
        case EVENT_P1_SHADOW_BOLT:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(target, SHADOW_BOLT_HELPER);
            events.ScheduleEvent(EVENT_P1_SHADOW_BOLT, urand(5000, 8000), 0, PHASE_ONE);
            break;
        case EVENT_P1_REANIMATE_CULTIST:
            ReanimateCultist();
            break;
        case EVENT_P1_EMPOWER_CULTIST:
            EmpowerCultist();
            events.ScheduleEvent(EVENT_P1_EMPOWER_CULTIST, urand(18000, 25000));
            break;
        case EVENT_P2_FROSTBOLT:
            DoCastVictim(FROSTBOLT_HELPER);
            events.ScheduleEvent(EVENT_P2_FROSTBOLT, urand(10000, 11000), 0, PHASE_TWO);
            break;
        case EVENT_P2_FROSTBOLT_VOLLEY:
            DoCastAOE(FROSTBOLT_VOLLEY_HELPER);
            events.ScheduleEvent(EVENT_P2_FROSTBOLT_VOLLEY, urand(13000, 15000), 0, PHASE_TWO);
            break;
        case EVENT_P2_TOUCH_OF_INSIGNIFICANCE:
            DoCastVictim(SPELL_TOUCH_OF_INSIGNIFICANCE);
            events.ScheduleEvent(EVENT_P2_TOUCH_OF_INSIGNIFICANCE, urand(9000, 13000), 0, PHASE_TWO);
            break;
        case EVENT_P2_SUMMON_SHADE:
            me->CastCustomSpell(SPELL_SUMMON_SHADE, SPELLVALUE_MAX_TARGETS, 1, NULL);
            if (Is25ManRaid())
                me->CastCustomSpell(SPELL_SUMMON_SHADE, SPELLVALUE_MAX_TARGETS, 1, NULL);
            events.ScheduleEvent(EVENT_P2_SUMMON_SHADE, 13000, 0, PHASE_TWO);
            break;
        case EVENT_P2_SUMMON_WAVE:
            SummonWaveP2();
            events.ScheduleEvent(EVENT_P2_SUMMON_WAVE, 45000, 0, PHASE_TWO);
            break;
        case EVENT_BERSERK:
            DoCast(me, SPELL_BERSERK);
            Talk(SAY_BERSERK);
            break;
        }
    }

    // We should not melee attack when barrier is up
    if (me->HasAura(SPELL_MANA_BARRIER))
        return;

    DoMeleeAttackIfReady();
}

void LadyDeathwhisperAI::SummonWaveP1()
{
    uint8 addIndex = _waveCounter & 1;
    uint8 addIndexOther = uint8(addIndex ^ 1);

    // Summon first add, replace it with Darnavan if weekly quest is active
    if (_waveCounter || !sPoolMgr->IsSpawnedObject<Quest>(QUEST_DEPROGRAMMING))
        Summon(SummonEntries[addIndex], SummonPositions[addIndex * 3]);
    else
        Summon(NPC_DARNAVAN, SummonPositions[addIndex * 3]);

    Summon(SummonEntries[addIndexOther], SummonPositions[addIndex * 3 + 1]);
    Summon(SummonEntries[addIndex], SummonPositions[addIndex * 3 + 2]);
    if (Is25ManRaid())
    {
        Summon(SummonEntries[addIndexOther], SummonPositions[addIndexOther * 3]);
        Summon(SummonEntries[addIndex], SummonPositions[addIndexOther * 3 + 1]);
        Summon(SummonEntries[addIndexOther], SummonPositions[addIndexOther * 3 + 2]);
        Summon(SummonEntries[urand(0, 1)], SummonPositions[6]);
    }

    ++_waveCounter;
}

void LadyDeathwhisperAI::SummonWaveP2()
{
    if (Is25ManRaid())
    {
        uint8 addIndex = _waveCounter & 1;
        Summon(SummonEntries[addIndex], SummonPositions[addIndex * 3]);
        Summon(SummonEntries[addIndex ^ 1], SummonPositions[addIndex * 3 + 1]);
        Summon(SummonEntries[addIndex], SummonPositions[addIndex * 3+ 2]);
    }
    else
        Summon(SummonEntries[urand(0, 1)], SummonPositions[6]);

    ++_waveCounter;
}

void LadyDeathwhisperAI::Summon(uint32 entry, const Position& pos)
{
    if (TempSummon* summon = me->SummonCreature(entry, pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
        summon->AI()->DoCast(summon, SPELL_TELEPORT_VISUAL);
}

void LadyDeathwhisperAI::SetGUID(uint64 guid, int32 id/* = 0*/)
{
    if (id != GUID_CULTIST)
        return;

    _reanimationQueue.push_back(guid);
    events.ScheduleEvent(EVENT_P1_REANIMATE_CULTIST, 3000, 0, PHASE_ONE);
}

void LadyDeathwhisperAI::ReanimateCultist()
{
    if (_reanimationQueue.empty())
        return;

    uint64 cultistGUID = _reanimationQueue.front();
    Creature* cultist = ObjectAccessor::GetCreature(*me, cultistGUID);
    _reanimationQueue.pop_front();
    if (!cultist)
        return;

    Talk(SAY_ANIMATE_DEAD);
    DoCast(cultist, SPELL_DARK_MARTYRDOM_T);
}

void LadyDeathwhisperAI::SpellHitTarget(Unit* target, SpellInfo const* spell)
{
    if (spell->Id == SPELL_DARK_MARTYRDOM_T)
    {
        Position pos;
        target->GetPosition(&pos);
        if (target->GetEntry() == NPC_CULT_FANATIC)
            me->SummonCreature(NPC_REANIMATED_FANATIC, pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
        else
            me->SummonCreature(NPC_REANIMATED_ADHERENT, pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);

        if (TempSummon* summon = target->ToTempSummon())
            summon->UnSummon();
    }
}

void LadyDeathwhisperAI::EmpowerCultist()
{
    if (summons.empty())
        return;

    std::list<Creature*> temp;
    for (SummonList::iterator itr = summons.begin(); itr != summons.end(); ++itr)
        if (Creature* cre = ObjectAccessor::GetCreature(*me, *itr))
            if (cre->isAlive() && (cre->GetEntry() == NPC_CULT_FANATIC || cre->GetEntry() == NPC_CULT_ADHERENT))
                temp.push_back(cre);

    // noone to empower
    if (temp.empty())
        return;

    // select random cultist
    Creature* cultist = Trinity::Containers::SelectRandomContainerElement(temp);
    DoCast(cultist, cultist->GetEntry() == NPC_CULT_FANATIC ? SPELL_DARK_TRANSFORMATION_T : SPELL_DARK_EMPOWERMENT_T, true);
    Talk(uint8(cultist->GetEntry() == NPC_CULT_FANATIC ? SAY_DARK_TRANSFORMATION : SAY_DARK_EMPOWERMENT));
}
