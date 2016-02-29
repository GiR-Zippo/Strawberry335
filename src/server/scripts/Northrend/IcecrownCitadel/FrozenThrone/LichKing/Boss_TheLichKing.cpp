#include "Boss_TheLichKing.h"

void BossTheLichKingAI::Reset()
{
    _Reset();
    me->SetReactState(REACT_PASSIVE);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
    events.SetPhase(PHASE_INTRO);
    _necroticPlagueStack = 0;
    _vileSpiritExplosions = 0;
    SetEquipmentSlots(true);
    secondTransition = false;

    createBoundaryMap();
}

void BossTheLichKingAI::JustDied(Unit* )
{
    _JustDied();
    DoCastAOE(SPELL_PLAY_MOVIE, false);
    me->SetDisableGravity(false);
    me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
    me->GetMotionMaster()->MoveFall();
    if (Creature* frostmourne = me->FindNearestCreature(NPC_FROSTMOURNE_TRIGGER, 50.0f))
        frostmourne->DespawnOrUnsummon();

    // Manually set players out of fight @todo
    std::list<Player*> pList = me->GetNearestPlayersList(150.0f);
    if (!pList.empty())
    {
        for (std::list<Player*>::const_iterator itr = pList.begin(); itr != pList.end(); ++itr)
        {
            if ((*itr))
                (*itr)->ClearInCombat();
        }
    }
}

void BossTheLichKingAI::EnterCombat(Unit* target)
{
    uint32 mode;
    me->GetMap()->IsHeroic() ? mode = DATA_THE_LICH_KING_HC 
        : mode = DATA_THE_LICH_KING;

    if (!instance->GetData(DATA_SKIP_CHECK_LICHKING))
    {
        if (!instance->CheckRequiredBosses(mode, target->ToPlayer()))
        {
            std::string notfify;
            me->GetMap()->IsHeroic() ? notfify = "Ihr habt nicht alle erforderlichen Bosse im heroischen Modus besiegt."
                : notfify = "Ihr habt nicht alle erforderlichen Bosse besiegt.";

            EnterEvadeMode();
            instance->DoSendNotifyToInstance(notfify.c_str());
            instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
            return;
        }
    }

    me->setActive(true);
    DoZoneInCombat();

    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);

    events.SetPhase(PHASE_ONE);
    events.ScheduleEvent(EVENT_INFEST, 5000, 0, PHASE_ONE);
    events.ScheduleEvent(EVENT_SUMMON_DRUDGE_GHOUL, 10000, 0, PHASE_ONE);
    events.ScheduleEvent(EVENT_SUMMON_SHAMBLING_HORROR, 20000, 0, PHASE_ONE);
    events.ScheduleEvent(EVENT_NECROTIC_PLAGUE, urand(30000, 33000), 0, PHASE_ONE);
    events.ScheduleEvent(EVENT_BERSERK, 900000);
    if (IsHeroic())
        events.ScheduleEvent(EVENT_SHADOW_TRAP, 15500, 0, PHASE_ONE);

    events.ScheduleEvent(EVENT_CHECK_IN_ROOM, 5 * IN_MILLISECONDS);
}

void BossTheLichKingAI::JustReachedHome()
{
    _JustReachedHome();
    instance->SetBossState(DATA_THE_LICH_KING, NOT_STARTED);

    // Reset The Frozen Throne gameobjects
    FrozenThroneResetWorker reset;
    Trinity::GameObjectWorker<FrozenThroneResetWorker> worker(me, reset);
    me->VisitNearbyGridObject(333.0f, worker);

    // Reset any light override
    SendLightOverride(0, 5000);

    // If it was a fail attempt -> reset visibility
    me->SetVisible(true);
}

bool BossTheLichKingAI::CanAIAttack(const Unit* target) const
{
    // The Lich King must not select targets in frostmourne room if he killed everyone outside
    return !target->HasAura(SPELL_IN_FROSTMOURNE_ROOM);
}

void BossTheLichKingAI::EnterEvadeMode()
{
    instance->SetBossState(DATA_THE_LICH_KING, FAIL);
    BossAI::EnterEvadeMode();
    if (Creature* tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HIGHLORD_TIRION_FORDRING)))
        tirion->AI()->EnterEvadeMode();
    DoCastAOE(SPELL_KILL_FROSTMOURNE_PLAYERS);
    EntryCheckPredicate pred(NPC_STRANGULATE_VEHICLE);
    summons.DoAction(ACTION_TELEPORT_BACK, pred);

    // Cataclysm Style
    me->SetVisible(false);
}

void BossTheLichKingAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER && !me->IsInEvadeMode() && !(events.GetPhaseMask() & PHASE_MASK_OUTRO))
        Talk(SAY_LK_KILL);
}

void BossTheLichKingAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ACTION_START_ENCOUNTER:
            instance->SetBossState(DATA_THE_LICH_KING, IN_PROGRESS);
            Talk(SAY_LK_INTRO_1);
            SendMusicToPlayers(MUSIC_FROZEN_THRONE);
            // schedule talks
            me->SetStandState(UNIT_STAND_STATE_STAND);
            events.ScheduleEvent(EVENT_INTRO_MOVE_1, 4000);
            break;
        case ACTION_START_ATTACK:
            events.ScheduleEvent(EVENT_START_ATTACK, 5000);
            break;
        case ACTION_PLAY_MUSIC:
            SendMusicToPlayers(MUSIC_FINAL);
            break;
        case ACTION_RESTORE_LIGHT:
            SendLightOverride(0, 5000);
            break;
        case ACTION_BREAK_FROSTMOURNE:
            me->CastSpell((Unit*)NULL, SPELL_SUMMON_BROKEN_FROSTMOURNE, TRIGGERED_IGNORE_CAST_IN_PROGRESS);
            me->CastSpell((Unit*)NULL, SPELL_SUMMON_BROKEN_FROSTMOURNE_2, TRIGGERED_IGNORE_CAST_IN_PROGRESS);
            SetEquipmentSlots(false, EQUIP_BROKEN_FROSTMOURNE);
            events.ScheduleEvent(EVENT_OUTRO_TALK_6, 2500, 0, PHASE_OUTRO);
            break;
        case ACTION_FINISH_OUTRO:
            events.ScheduleEvent(EVENT_OUTRO_TALK_7, 7000, 0, PHASE_OUTRO);
            events.ScheduleEvent(EVENT_OUTRO_TALK_8, 17000, 0, PHASE_OUTRO);
            break;
        case ACTION_TELEPORT_BACK:
        {
            EntryCheckPredicate pred(NPC_STRANGULATE_VEHICLE);
            summons.DoAction(ACTION_TELEPORT_BACK, pred);
            if (!IsHeroic())
                Talk(SAY_LK_FROSTMOURNE_ESCAPE);
            break;
        }
        default:
            break;
    }
}

uint32 BossTheLichKingAI::GetData(uint32 type)
{
    switch (type)
    {
        case DATA_PLAGUE_STACK:
            return _necroticPlagueStack;
        case DATA_VILE:
            return _vileSpiritExplosions;
        case DATA_IS_FROSTMOURNE_PHASE:
            return events.GetPhaseMask() & PHASE_MASK_FROSTMOURNE;
        case DATA_ENCOUNTER_DIFFICULTY:
            return GetDifficulty();
        default:
            break;
    }

    return 0;
}

void BossTheLichKingAI::SetData(uint32 type, uint32 value)
{
    switch (type)
    {
        case DATA_PLAGUE_STACK:
            _necroticPlagueStack = std::max(value, _necroticPlagueStack);
            break;
        case DATA_VILE:
            _vileSpiritExplosions += value;
            break;
        default:
            break;
    }
}

void BossTheLichKingAI::DamageTaken(Unit* , uint32& )
{
    if (events.GetPhaseMask() & PHASE_MASK_ONE && !HealthAbovePct(70))
    {
        events.SetPhase(PHASE_TRANSITION);
        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
        me->SetReactState(REACT_PASSIVE);
        me->AttackStop();
        me->GetMotionMaster()->MovePoint(POINT_CENTER_1, CenterPosition);
        return;
    }

    if (events.GetPhaseMask() & PHASE_MASK_TWO && !HealthAbovePct(40))
    {
        events.SetPhase(PHASE_TRANSITION);
        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
        me->SetReactState(REACT_PASSIVE);
        me->AttackStop();
        me->GetMotionMaster()->MovePoint(POINT_CENTER_2, CenterPosition);
        return;
    }

    if (events.GetPhaseMask() & PHASE_MASK_THREE && !HealthAbovePct(10))
    {
        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
        me->SetReactState(REACT_PASSIVE);
        me->AttackStop();
        events.Reset();
        events.SetPhase(PHASE_OUTRO);
        summons.DespawnAll();
        SendMusicToPlayers(MUSIC_FURY_OF_FROSTMOURNE);
        me->InterruptNonMeleeSpells(true);
        me->CastSpell((Unit*)NULL, SPELL_FURY_OF_FROSTMOURNE, TRIGGERED_NONE);
        me->SetWalk(true);
        events.ScheduleEvent(EVENT_OUTRO_TALK_1, 2600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_OUTRO_EMOTE_TALK, 6600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_OUTRO_EMOTE_TALK, 17600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_OUTRO_EMOTE_TALK, 27600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_OUTRO_TALK_2, 34600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_OUTRO_TALK_3, 43600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_EMOTE_CAST_SHOUT, 54600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_OUTRO_EMOTE_TALK, 58600, 0, PHASE_OUTRO);
        events.ScheduleEvent(EVENT_OUTRO_MOVE_CENTER, 69600, 0, PHASE_OUTRO);
        // stop here. rest will get scheduled from MovementInform
        return;
    }
}

void BossTheLichKingAI::JustSummoned(Creature* summon)
{
    switch (summon->GetEntry())
    {
        case NPC_SHAMBLING_HORROR:
        case NPC_DRUDGE_GHOUL:
            summon->CastSpell(summon, SPELL_RISEN_WITCH_DOCTOR_SPAWN, true);
            summon->SetInCombatWithZone();
            summon->SetReactState(REACT_PASSIVE);
            summon->CombatStop();
            summon->GetMotionMaster()->MovementExpired(true);
            summon->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
            summon->m_Events.AddEvent(new StartMovementEvent(summon), summon->m_Events.CalculateTime(5000));
            break;
        case NPC_ICE_SPHERE:
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
            {
                summon->SetReactState(REACT_PASSIVE);
                summon->m_Events.AddEvent(new StartIceSphereEvent(summon, target), summon->m_Events.CalculateTime(1000));
            }
            else
                summon->DespawnOrUnsummon();
            break;
        }
        case NPC_DEFILE:
            summon->SetReactState(REACT_PASSIVE);
            summon->CastSpell(summon, SPELL_DEFILE_AURA, false);
            break;
        case NPC_FROSTMOURNE_TRIGGER:
        {
            summon->CastSpell((Unit*)NULL, SPELL_BROKEN_FROSTMOURNE, true);

            SendLightOverride(LIGHT_SOULSTORM, 10000);
            SendWeather(WEATHER_STATE_BLACKSNOW);

            events.ScheduleEvent(EVENT_OUTRO_SOUL_BARRAGE, 5000, 0, PHASE_OUTRO);
            return;
        }
        case NPC_VILE_SPIRIT:
        {
            summons.Summon(summon);
            summon->SetReactState(REACT_PASSIVE);
            summon->AttackStop();
            summon->GetMotionMaster()->MovementExpired();
            summon->GetMotionMaster()->MoveRandom(10.0f);
            if (!(events.GetPhaseMask() & PHASE_MASK_FROSTMOURNE))
                summon->m_Events.AddEvent(new VileSpiritActivateEvent(summon), summon->m_Events.CalculateTime(15000));
            return;
        }
        case NPC_STRANGULATE_VEHICLE:
            summons.Summon(summon);
            return;
        default:
            break;
    }

    BossAI::JustSummoned(summon);
}

void BossTheLichKingAI::SummonedCreatureDies(Creature* summon, Unit* )
{
    switch (summon->GetEntry())
    {
        case NPC_SHAMBLING_HORROR:
        case NPC_DRUDGE_GHOUL:
        case NPC_ICE_SPHERE:
        case NPC_VALKYR_SHADOWGUARD:
        case NPC_RAGING_SPIRIT:
        case NPC_VILE_SPIRIT:
            summon->ToTempSummon()->SetTempSummonType(TEMPSUMMON_CORPSE_DESPAWN);
            break;
        default:
            break;
    }
}

void BossTheLichKingAI::SpellHit(Unit* , const SpellInfo* spell)
{
    if (spell->Id == SPELL_HARVESTED_SOUL && me->isInCombat() && !IsHeroic())
        Talk(SAY_LK_FROSTMOURNE_KILL);
}

void BossTheLichKingAI::SpellHitTarget(Unit* , const SpellInfo* spell)
{
    if (spell->Id == REMORSELESS_WINTER_1 || spell->Id == REMORSELESS_WINTER_2)
    {
        SendLightOverride(LIGHT_SNOWSTORM, 5000);
        SendWeather(WEATHER_STATE_LIGHT_SNOW);
    }
}

void BossTheLichKingAI::MovementInform(uint32 type, uint32 pointId)
{
    if (type != POINT_MOTION_TYPE)
        return;

    switch (pointId)
    {
        case POINT_LK_INTRO_1:
            // schedule for next update cycle, current update must finalize movement
            events.ScheduleEvent(EVENT_INTRO_MOVE_2, 1, 0, PHASE_INTRO);
            break;
        case POINT_LK_INTRO_2:
            events.ScheduleEvent(EVENT_INTRO_MOVE_3, 1, 0, PHASE_INTRO);
            break;
        case POINT_LK_INTRO_3:
            if (Creature* tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HIGHLORD_TIRION_FORDRING)))
                tirion->AI()->DoAction(ACTION_CONTINUE_INTRO);
            events.ScheduleEvent(EVENT_INTRO_TALK_1, 9000, 0, PHASE_INTRO);
            break;
        case POINT_CENTER_1:
            me->SetFacingTo(0.0f);
            Talk(SAY_LK_REMORSELESS_WINTER);
            SendMusicToPlayers(MUSIC_SPECIAL);
            DoCast(me, SPELL_REMORSELESS_WINTER_1);
            events.RescheduleEvent(EVENT_QUAKE, 62500, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_PAIN_AND_SUFFERING, 4000, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_SUMMON_ICE_SPHERE, 6000, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, 10000, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_SUMMON_VALKYR, 78000, 0, PHASE_TWO);
            events.RescheduleEvent(EVENT_INFEST, 70000, 0, PHASE_TWO);
            events.RescheduleEvent(EVENT_DEFILE, 97000, 0, PHASE_TWO);
            events.RescheduleEvent(EVENT_SOUL_REAPER, 94000, 0, PHASE_TWO);
            break;
        case POINT_CENTER_2:
            me->SetFacingTo(0.0f);
            Talk(SAY_LK_REMORSELESS_WINTER);
            SendMusicToPlayers(MUSIC_SPECIAL);
            DoCast(me, SPELL_REMORSELESS_WINTER_2);
            summons.DespawnEntry(NPC_VALKYR_SHADOWGUARD);
            events.RescheduleEvent(EVENT_QUAKE_2, 62500, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_PAIN_AND_SUFFERING, 6000, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_SUMMON_ICE_SPHERE, 7000, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, 5000, 0, PHASE_TRANSITION);
            events.RescheduleEvent(EVENT_DEFILE, 95500, 0, PHASE_THREE);
            events.RescheduleEvent(EVENT_SOUL_REAPER, 99500, 0, PHASE_THREE);
            events.RescheduleEvent(EVENT_VILE_SPIRITS, 79500, EVENT_GROUP_VILE_SPIRITS, PHASE_THREE);
            events.RescheduleEvent(IsHeroic() ? EVENT_HARVEST_SOULS : EVENT_HARVEST_SOUL, 73500, 0, PHASE_THREE);
            secondTransition = true;
            break;
        case POINT_LK_OUTRO_1:
            events.ScheduleEvent(EVENT_OUTRO_TALK_4, 1, 0, PHASE_OUTRO);
            events.ScheduleEvent(EVENT_OUTRO_RAISE_DEAD, 1000, 0, PHASE_OUTRO);
            events.ScheduleEvent(EVENT_OUTRO_TALK_5, 29000, 0, PHASE_OUTRO);
            break;
        case POINT_LK_OUTRO_2:
            if (Creature* tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HIGHLORD_TIRION_FORDRING)))
                tirion->AI()->Talk(SAY_TIRION_OUTRO_2);
            if (Creature* frostmourne = me->FindNearestCreature(NPC_FROSTMOURNE_TRIGGER, 50.0f))
                frostmourne->AI()->DoAction(ACTION_SUMMON_TERENAS);
            break;
        default:
            break;
    }
}

void BossTheLichKingAI::UpdateAI(const uint32 diff)
{
    // check phase first to prevent updating victim and entering evade mode when not wanted
    if (!(events.GetPhaseMask() & PHASE_MASK_NO_VICTIM))
        if (!UpdateVictim())
            return;

    events.Update(diff);

    // during Remorseless Winter phases The Lich King is channeling a spell, but we must continue casting other spells
    if (me->HasUnitState(UNIT_STATE_CASTING) && !(events.GetPhaseMask() & PHASE_MASK_NO_CAST_CHECK))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_INTRO_MOVE_1:
                me->SetSheath(SHEATH_STATE_MELEE);
                me->RemoveAurasDueToSpell(SPELL_EMOTE_SIT_NO_SHEATH);
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(POINT_LK_INTRO_1, LichKingIntro[0]);
                break;
            case EVENT_INTRO_MOVE_2:
                me->GetMotionMaster()->MovePoint(POINT_LK_INTRO_2, LichKingIntro[1]);
                break;
            case EVENT_INTRO_MOVE_3:
                me->GetMotionMaster()->MovePoint(POINT_LK_INTRO_3, LichKingIntro[2]);
                break;
            case EVENT_INTRO_TALK_1:
                Talk(SAY_LK_INTRO_2);
                // for some reason blizz sends 2 emotes in row here so (we handle one in Talk)
                me->HandleEmoteCommand(EMOTE_ONESHOT_TALK_NO_SHEATHE);
                events.ScheduleEvent(EVENT_EMOTE_CAST_SHOUT, 7000, 0, PHASE_INTRO);
                events.ScheduleEvent(EVENT_INTRO_EMOTE_1, 13000, 0, PHASE_INTRO);
                events.ScheduleEvent(EVENT_EMOTE_CAST_SHOUT, 18000, 0, PHASE_INTRO);
                events.ScheduleEvent(EVENT_INTRO_CAST_FREEZE, 31000, 0, PHASE_INTRO);
                break;
            case EVENT_EMOTE_CAST_SHOUT:
                DoCast(me, SPELL_EMOTE_SHOUT_NO_SHEATH, false);
                break;
            case EVENT_INTRO_EMOTE_1:
                me->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NO_SHEATHE);
                break;
            case EVENT_INTRO_CAST_FREEZE:
                Talk(SAY_LK_INTRO_3);
                DoCastAOE(SPELL_ICE_LOCK, false);
                events.ScheduleEvent(EVENT_FINISH_INTRO, 1000, 0, PHASE_INTRO);
                break;
            case EVENT_FINISH_INTRO:
                me->SetWalk(false);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetInCombatWithZone();
                events.SetPhase(PHASE_ONE);
                break;
            case EVENT_SUMMON_SHAMBLING_HORROR:
                DoCast(me, SPELL_SUMMON_SHAMBLING_HORROR);
                SendMusicToPlayers(MUSIC_SPECIAL);
                events.ScheduleEvent(EVENT_SUMMON_SHAMBLING_HORROR, 60000, 0, PHASE_ONE);
                break;
            case EVENT_SUMMON_DRUDGE_GHOUL:
                DoCast(me, SPELL_SUMMON_DRUDGE_GHOULS);
                events.ScheduleEvent(EVENT_SUMMON_DRUDGE_GHOUL, 30000, 0, PHASE_ONE);
                break;
            case EVENT_INFEST:
                DoCast(me, SPELL_INFEST);
                events.ScheduleEvent(EVENT_INFEST, urand(21000, 24000), 0, (events.GetPhaseMask() & PHASE_MASK_ONE) ? PHASE_ONE : PHASE_TWO);
                break;
            case EVENT_NECROTIC_PLAGUE:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, NecroticPlagueTargetCheck(me, NECROTIC_PLAGUE_LK, NECROTIC_PLAGUE_PLR)))
                    DoCast(target, SPELL_NECROTIC_PLAGUE, false);
                events.ScheduleEvent(EVENT_NECROTIC_PLAGUE, urand(30000, 33000), 0, PHASE_ONE);
                break;
            case EVENT_SHADOW_TRAP:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                    DoCast(target, SPELL_SHADOW_TRAP);
                events.ScheduleEvent(EVENT_SHADOW_TRAP, 15500, 0, PHASE_ONE);
                break;
            case EVENT_SOUL_REAPER:
                DoCastVictim(SPELL_SOUL_REAPER);
                events.ScheduleEvent(EVENT_SOUL_REAPER, urand(33000, 35000), 0, PHASE_TWO_THREE);
                break;
            case EVENT_DEFILE:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_HARVEST_SOUL_VALKYR))
                {
                    if (target->IsFalling())
                        target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_HARVEST_SOUL_VALKYR);

                    Talk(EMOTE_DEFILE_WARNING);
                    DoCast(target, SPELL_DEFILE);
                }
                events.ScheduleEvent(EVENT_DEFILE, urand(32000, 35000), 0, PHASE_TWO_THREE);
                break;
            case EVENT_HARVEST_SOUL:
                Talk(SAY_LK_HARVEST_SOUL);
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, SpellTargetSelector(me, SPELL_HARVEST_SOUL)))
                    DoCast(target, SPELL_HARVEST_SOUL);
                events.ScheduleEvent(EVENT_HARVEST_SOUL, 75000, 0, PHASE_THREE);
                break;
            case EVENT_PAIN_AND_SUFFERING:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    me->CastSpell(target, SPELL_PAIN_AND_SUFFERING, TRIGGERED_NONE);
                events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, urand(1500, 4000), 0, PHASE_TRANSITION);
                break;
            case EVENT_SUMMON_ICE_SPHERE:
                DoCastAOE(SPELL_SUMMON_ICE_SPHERE);
                events.ScheduleEvent(EVENT_SUMMON_ICE_SPHERE, secondTransition ? 5000 : 10000, 0, PHASE_TRANSITION);
                break;
            case EVENT_SUMMON_RAGING_SPIRIT:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    me->CastSpell(target, SPELL_RAGING_SPIRIT, false);
                events.ScheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, secondTransition ? 15000 : 20000, 0, PHASE_TRANSITION);
                break;
            case EVENT_QUAKE:
                events.SetPhase(PHASE_TWO);
                me->ClearUnitState(UNIT_STATE_CASTING);  // clear state to ensure check in DoCastAOE passes
                DoCastAOE(SPELL_QUAKE);
                SendMusicToPlayers(MUSIC_SPECIAL);
                Talk(SAY_LK_QUAKE);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                break;
            case EVENT_QUAKE_2:
                events.SetPhase(PHASE_THREE);
                me->ClearUnitState(UNIT_STATE_CASTING);  // clear state to ensure check in DoCastAOE passes
                DoCastAOE(SPELL_QUAKE);
                SendMusicToPlayers(MUSIC_SPECIAL);
                Talk(SAY_LK_QUAKE);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                break;
            case EVENT_SUMMON_VALKYR:
                SendMusicToPlayers(MUSIC_SPECIAL);
                Talk(SAY_LK_SUMMON_VALKYR);
                DoCastAOE(SUMMON_VALKYR);
                events.ScheduleEvent(EVENT_SUMMON_VALKYR, urand(45000, 50000), 0, PHASE_TWO);
                break;
            case EVENT_START_ATTACK:
                me->SetReactState(REACT_AGGRESSIVE);
                if (events.GetPhaseMask() & PHASE_MASK_FROSTMOURNE)
                {
                    events.SetPhase(PHASE_THREE);

                    // Get raging spirits and set their phase
                    std::list<Creature*> ragingspiritList;
                    me->GetCreatureListWithEntryInGrid(ragingspiritList, NPC_RAGING_SPIRIT, 150.0f);
                    if (!ragingspiritList.empty())
                    {
                        for (std::list<Creature*>::const_iterator itr = ragingspiritList.begin(); itr != ragingspiritList.end(); ++itr)
                            if ((*itr))
                                (*itr)->AI()->DoAction(ACTION_RAGINGSPIRIT_FROSTMOURNE);
                    }
                }
                break;
            case EVENT_VILE_SPIRITS:
                SendMusicToPlayers(MUSIC_SPECIAL);
                DoCastAOE(SPELL_VILE_SPIRITS);
                events.ScheduleEvent(EVENT_VILE_SPIRITS, urand(35000, 40000), EVENT_GROUP_VILE_SPIRITS, PHASE_THREE);
                break;
            case EVENT_HARVEST_SOULS:
            {
                Talk(SAY_LK_HARVEST_SOUL);
                DoCastAOE(SPELL_HARVEST_SOULS);
                events.ScheduleEvent(EVENT_HARVEST_SOULS, urand(100000, 110000), 0, PHASE_THREE);
                events.SetPhase(PHASE_FROSTMOURNE); // will stop running UpdateVictim (no evading)
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                events.DelayEvents(50000, EVENT_GROUP_VILE_SPIRITS);
                events.RescheduleEvent(EVENT_DEFILE, 50000, 0, PHASE_THREE);
                events.RescheduleEvent(EVENT_SOUL_REAPER, urand(57000, 62000), 0, PHASE_THREE);
                events.ScheduleEvent(EVENT_START_ATTACK, 49000);
                events.ScheduleEvent(EVENT_FROSTMOURNE_HEROIC, 6500);

                // Get raging spirits and set their phase
                std::list<Creature*> ragingspiritList;
                me->GetCreatureListWithEntryInGrid(ragingspiritList, NPC_RAGING_SPIRIT, 150.0f);
                if (!ragingspiritList.empty())
                {
                    for (std::list<Creature*>::const_iterator itr = ragingspiritList.begin(); itr != ragingspiritList.end(); ++itr)
                        if ((*itr))
                            (*itr)->AI()->DoAction(ACTION_RAGINGSPIRIT_FROSTMOURNE);
                }

                for (SummonList::iterator i = summons.begin(); i != summons.end(); ++i)
                {
                    Creature* summon = ObjectAccessor::GetCreature(*me, *i);
                    if (summon)
                    {
                        if (summon->GetEntry() == NPC_VILE_SPIRIT)
                        {
                            summon->CombatStop(true);
                            summon->SetReactState(REACT_PASSIVE);
                            summon->m_Events.KillAllEvents(true);
                            summon->m_Events.AddEvent(new VileSpiritActivateEvent(summon), summon->m_Events.CalculateTime(59000));
                            summon->GetMotionMaster()->MoveTargetedHome();
                        }
                    }
                }
            }break;
            case EVENT_FROSTMOURNE_HEROIC:
                if (TempSummon* terenas = me->GetMap()->SummonCreature(NPC_TERENAS_MENETHIL_FROSTMOURNE_H, TerenasSpawnHeroic, NULL, 50000))
                {
                    terenas->AI()->DoAction(ACTION_FROSTMOURNE_INTRO);
                    std::list<Creature*> triggers;
                    GetCreatureListWithEntryInGrid(triggers, terenas, NPC_WORLD_TRIGGER_INFINITE_AOI, 100.0f);
                    if (!triggers.empty())
                    {
                        triggers.sort(Trinity::ObjectDistanceOrderPred(terenas, true));
                        Creature* spawner = triggers.front();

                        //spawner->CastSpell(spawner, SPELL_SUMMON_SPIRIT_BOMB_1, true);  // summons bombs randomly
                        spawner->CastSpell(spawner, SPELL_SUMMON_SPIRIT_BOMB_2, true);  // summons bombs on players
                        spawner->m_Events.AddEvent(new TriggerWickedSpirit(spawner), spawner->m_Events.CalculateTime(5000));
                        spawner->m_Events.AddEvent(new WickedSpiritResetEvent(spawner), spawner->m_Events.CalculateTime(49000));
                    }
                }
                break;
            case EVENT_OUTRO_TALK_1:
                Talk(SAY_LK_OUTRO_1);
                DoCastAOE(SPELL_FURY_OF_FROSTMOURNE_NO_REZ, true);
                break;
            case EVENT_OUTRO_TALK_2:
                Talk(SAY_LK_OUTRO_2);
                DoCastAOE(SPELL_EMOTE_QUESTION_NO_SHEATH);
                break;
            case EVENT_OUTRO_EMOTE_TALK:
                me->HandleEmoteCommand(EMOTE_ONESHOT_TALK_NO_SHEATHE);
                break;
            case EVENT_OUTRO_TALK_3:
                if (Creature* tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HIGHLORD_TIRION_FORDRING)))
                    me->SetFacingToObject(tirion);
                Talk(SAY_LK_OUTRO_3);
                break;
            case EVENT_OUTRO_MOVE_CENTER:
                me->GetMotionMaster()->MovePoint(POINT_LK_OUTRO_1, CenterPosition);
                break;
            case EVENT_OUTRO_TALK_4:
                me->SetFacingTo(0.01745329f);
                Talk(SAY_LK_OUTRO_4);
                break;
            case EVENT_OUTRO_RAISE_DEAD:
                DoCastAOE(SPELL_RAISE_DEAD);
                me->ClearUnitState(UNIT_STATE_CASTING);
                SendMusicToPlayers(MUSIC_FINAL);
                break;
            case EVENT_OUTRO_TALK_5:
                Talk(SAY_LK_OUTRO_5);
                if (Creature* tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HIGHLORD_TIRION_FORDRING)))
                    tirion->AI()->DoAction(ACTION_OUTRO);
                break;
            case EVENT_OUTRO_TALK_6:
                Talk(SAY_LK_OUTRO_6);
                if (Creature* tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HIGHLORD_TIRION_FORDRING)))
                    tirion->SetFacingToObject(me);
                me->CastSpell((Unit*)NULL, SPELL_SUMMON_BROKEN_FROSTMOURNE_3, TRIGGERED_IGNORE_CAST_IN_PROGRESS);
                SetEquipmentSlots(false, EQUIP_UNEQUIP);
                break;
            case EVENT_OUTRO_SOUL_BARRAGE:
                me->CastSpell((Unit*)NULL, SPELL_SOUL_BARRAGE, TRIGGERED_IGNORE_CAST_IN_PROGRESS);
                sCreatureTextMgr->SendSound(me, SOUND_PAIN, CHAT_MSG_MONSTER_YELL, 0, TEXT_RANGE_NORMAL, TEAM_OTHER, false);
                // set flight
                me->SetDisableGravity(true);
                me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                me->GetMotionMaster()->MovePoint(POINT_LK_OUTRO_2, OutroFlying);
                break;
            case EVENT_OUTRO_TALK_7:
                Talk(SAY_LK_OUTRO_7);
                break;
            case EVENT_OUTRO_TALK_8:
                Talk(SAY_LK_OUTRO_8);
                break;
            case EVENT_BERSERK:
                Talk(SAY_LK_BERSERK);
                DoCast(me, SPELL_BERSERK2);
                break;
            case EVENT_CHECK_IN_ROOM:
                if (CheckBoundaryMap(&boundaryMap) &&
                    me->GetPositionZ() >= (me->GetMap()->GetHeight(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ())) - 2.5f)
                    events.ScheduleEvent(EVENT_CHECK_IN_ROOM, 2 * IN_MILLISECONDS);
                else
                    EnterEvadeMode();
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}

void BossTheLichKingAI::SendMusicToPlayers(uint32 musicId) const
{
    WorldPacket data(SMSG_PLAY_MUSIC, 4);
    data << uint32(musicId);
    SendPacketToPlayers(&data);
}

void BossTheLichKingAI::SendLightOverride(uint32 overrideId, uint32 fadeInTime) const
{
    WorldPacket data(SMSG_OVERRIDE_LIGHT, 12);
    data << uint32(2488);       // Light.dbc entry (map default)
    data << uint32(overrideId); // Light.dbc entry (override)
    data << uint32(fadeInTime);
    SendPacketToPlayers(&data);
}

void BossTheLichKingAI::SendWeather(WeatherState weather) const
{
    WorldPacket data(SMSG_WEATHER, 9);
    data << uint32(weather);
    data << float(0.5f);
    data << uint8(0);
    SendPacketToPlayers(&data);
}

void BossTheLichKingAI::SendPacketToPlayers(const WorldPacket* data) const
{
    Map::PlayerList const& players = me->GetMap()->GetPlayers();
    if (!players.isEmpty())
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            if (Player* player = itr->getSource())
                if (player->GetAreaId() == AREA_THE_FROZEN_THRONE)
                    player->GetSession()->SendPacket(data);
}

void BossTheLichKingAI::createBoundaryMap()
{
    boundaryMap.clear();
    boundaryMap[BOUNDARY_N] = LICH_KING_BOUNDARY_POSITION_N;
    boundaryMap[BOUNDARY_E] = LICH_KING_BOUNDARY_POSITION_E;
    boundaryMap[BOUNDARY_S] = LICH_KING_BOUNDARY_POSITION_S;
    boundaryMap[BOUNDARY_W] = LICH_KING_BOUNDARY_POSITION_W;
};