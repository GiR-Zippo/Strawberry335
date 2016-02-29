#include "Boss_ValithriaDreamwalker.h"

void BossValithriaDreamwalkerAI::InitializeAI()
{
    if (CreatureData const* data = sObjectMgr->GetCreatureData(me->GetDBTableGUIDLow()))
        if (data->curhealth)
            _spawnHealth = data->curhealth;

    if (!me->isDead())
        Reset();
}

void BossValithriaDreamwalkerAI::Reset()
{
    _events.Reset();
    me->SetHealth(_spawnHealth);
    me->SetReactState(REACT_PASSIVE);
    me->LoadCreaturesAddon(true);
    // immune to percent heals
    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_OBS_MOD_HEALTH, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_HEAL_PCT, true);
    // Glyph of Dispel Magic - not a percent heal by effect, its cast with custom basepoints
    me->ApplySpellImmune(0, IMMUNITY_ID, 56131, true);
    _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
    _missedPortals = 0;
    _under25PercentTalkDone = false;
    _over75PercentTalkDone = false;
    _justDied = false;
    _done = false;
    me->SetCanModifyStats(false);
}

void BossValithriaDreamwalkerAI::AttackStart(Unit* /*target*/)
{
}

void BossValithriaDreamwalkerAI::DoAction(int32 const action)
{
    if (action != ACTION_ENTER_COMBAT)
        return;

    DoCast(me, SPELL_COPY_DAMAGE);
    _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
    _events.ScheduleEvent(EVENT_INTRO_TALK, 15000);
    _events.ScheduleEvent(EVENT_DREAM_PORTAL, urand(45000, 48000));
    if (IsHeroic())
        _events.ScheduleEvent(EVENT_BERSERK, 420000);
}

void BossValithriaDreamwalkerAI::HealReceived(Unit* healer, uint32& heal)
{
    if (!me->hasLootRecipient())
        me->SetLootRecipient(healer);

    me->LowerPlayerDamageReq(heal);

    // encounter complete
    if (me->HealthAbovePctHealed(100, heal) && !_done)
    {
        _done = true;
        Talk(SAY_VALITHRIA_SUCCESS);
        _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        me->RemoveAurasDueToSpell(SPELL_CORRUPTION_VALITHRIA);
        DoCast(me, SPELL_ACHIEVEMENT_CHECK);
        DoCastAOE(SPELL_DREAMWALKERS_RAGE);
        _events.ScheduleEvent(EVENT_DREAM_SLIP, 3500);
        if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_LICH_KING)))
            lichKing->AI()->EnterEvadeMode();
        SpawnChest();
    }
    else if (!_over75PercentTalkDone && me->HealthAbovePctHealed(75, heal))
    {
        _over75PercentTalkDone = true;
        Talk(SAY_VALITHRIA_75_PERCENT);
    }
    else if (_instance->GetBossState(DATA_VALITHRIA_DREAMWALKER) == NOT_STARTED)
        if (Creature* archmage = me->FindNearestCreature(NPC_RISEN_ARCHMAGE, 30.0f))
            archmage->AI()->DoZoneInCombat();   // call EnterCombat on one of them, that will make it all start
}

void BossValithriaDreamwalkerAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (me->HealthBelowPctDamaged(25, damage))
    {
        if (!_under25PercentTalkDone)
        {
            _under25PercentTalkDone = true;
            Talk(SAY_VALITHRIA_25_PERCENT);
        }

        if (damage > me->GetHealth())
        {
            damage = 0;
            if (!_justDied)
            {
                _justDied = true;
                Talk(SAY_VALITHRIA_DEATH);
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                if (Creature* trigger = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_TRIGGER)))
                    trigger->AI()->DoAction(ACTION_DEATH);
            }
        }
    }
}

void BossValithriaDreamwalkerAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_DREAM_SLIP)
    {
        DoCast(me, SPELL_CLEAR_ALL);
        DoCast(me, SPELL_AWARD_REPUTATION_BOSS_KILL);
        _instance->SetBossState(DATA_VALITHRIA_DREAMWALKER, DONE);
        // this display id was found in sniff instead of the one on aura
        me->SetDisplayId(11686);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->DespawnOrUnsummon(4000);
        if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_LICH_KING)))
            lichKing->CastSpell(lichKing, SPELL_SPAWN_CHEST, false);

        if (Creature* trigger = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_TRIGGER)))
            me->Kill(trigger);
    }
}

void BossValithriaDreamwalkerAI::JustSummoned(Creature* summon)
{
    if (summon->GetEntry() == NPC_DREAM_PORTAL_PRE_EFFECT)
    {
        summon->m_Events.AddEvent(new DelayedCastEvent(summon, SPELL_SUMMON_DREAM_PORTAL, me->GetGUID(), 6000), summon->m_Events.CalculateTime(15000));
        summon->m_Events.AddEvent(new AuraRemoveEvent(summon, SPELL_DREAM_PORTAL_VISUAL_PRE), summon->m_Events.CalculateTime(15000));
    }
    else if (summon->GetEntry() == NPC_NIGHTMARE_PORTAL_PRE_EFFECT)
    {
        summon->m_Events.AddEvent(new DelayedCastEvent(summon, SPELL_SUMMON_NIGHTMARE_PORTAL, me->GetGUID(), 6000), summon->m_Events.CalculateTime(15000));
        summon->m_Events.AddEvent(new AuraRemoveEvent(summon, SPELL_NIGHTMARE_PORTAL_VISUAL_PRE), summon->m_Events.CalculateTime(15000));
    }
}

void BossValithriaDreamwalkerAI::SummonedCreatureDespawn(Creature* summon)
{
    if (summon->GetEntry() == NPC_DREAM_PORTAL || summon->GetEntry() == NPC_NIGHTMARE_PORTAL)
        if (summon->AI()->GetData(MISSED_PORTALS))
            ++_missedPortals;
}

void BossValithriaDreamwalkerAI::UpdateAI(uint32 const diff)
{
    // does not enter combat
    if (_instance->GetBossState(DATA_VALITHRIA_DREAMWALKER) != IN_PROGRESS)
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_INTRO_TALK:
                Talk(SAY_VALITHRIA_ENTER_COMBAT);
                break;
            case EVENT_BERSERK:
                Talk(SAY_VALITHRIA_BERSERK);
                break;
            case EVENT_DREAM_PORTAL:
                if (!IsHeroic())
                    Talk(SAY_VALITHRIA_DREAM_PORTAL);
                for (uint32 i = 0; i < _portalCount; ++i)
                    DoCast(me, SUMMON_PORTAL);
                _events.ScheduleEvent(EVENT_DREAM_PORTAL, urand(45000, 48000));
                break;
            case EVENT_DREAM_SLIP:
                DoCast(me, SPELL_DREAM_SLIP);
                break;
            default:
                break;
        }
    }
}

uint32 BossValithriaDreamwalkerAI::GetData(uint32 type)
{
    if (type == MISSED_PORTALS)
        return _missedPortals;

    return 0;
}

void BossValithriaDreamwalkerAI::SpawnChest()
{
    uint32 chestEntry = RAID_MODE(GO_CACHE_OF_THE_DREAMWALKER_10N, GO_CACHE_OF_THE_DREAMWALKER_25N, GO_CACHE_OF_THE_DREAMWALKER_10H, GO_CACHE_OF_THE_DREAMWALKER_25H);
    me->SummonGameObject(chestEntry, PosChestSpawn.GetPositionX(), PosChestSpawn.GetPositionY(), PosChestSpawn.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
};
