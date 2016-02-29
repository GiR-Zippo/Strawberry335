#include "Boss_Sindragosa.h"
#include "SpellAuras.h"

void BossSindragosaAI::Reset()
{
    BossAI::Reset();
    me->SetReactState(REACT_AGGRESSIVE);
    DoCast(me, SPELL_TANK_MARKER, true);
    events.ScheduleEvent(EVENT_BERSERK, 600000);
    events.ScheduleEvent(EVENT_CLEAVE, 10000, EVENT_GROUP_LAND_PHASE);
    events.ScheduleEvent(EVENT_TAIL_SMASH, 20000, EVENT_GROUP_LAND_PHASE);
    events.ScheduleEvent(EVENT_FROST_BREATH, urand(8000, 12000), EVENT_GROUP_LAND_PHASE);
    events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, urand(9000, 14000), EVENT_GROUP_LAND_PHASE);
    events.ScheduleEvent(EVENT_ICY_GRIP, 33500, EVENT_GROUP_LAND_PHASE);
    events.ScheduleEvent(EVENT_AIR_PHASE, 50000);
    _mysticBuffetStack = 0;
    _frostBombHitCount = 0;
    _isInAirPhase = false;
    _isThirdPhase = false;

    if (!_summoned)
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);
    }
}

void BossSindragosaAI::EnterEvadeMode()
{
    _EnterEvadeMode();
    if (_isInAirPhase)
        me->GetMotionMaster()->MoveLand(0, me->GetHomePosition());
    else
        me->GetMotionMaster()->MoveTargetedHome();
    Reset();
    _DespawnAtEvade();
}

void BossSindragosaAI::JustDied(Unit* killer)
{
    BossAI::JustDied(killer);
    Talk(SAY_DEATH);
    DoCastAOE(SPELL_FROST_INFUSION_CREDIT, true); // Credit Quest Shadowedge
}

void BossSindragosaAI::EnterCombat(Unit* victim)
{
    if (!instance->CheckRequiredBosses(DATA_SINDRAGOSA, victim->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    BossAI::EnterCombat(victim);
    DoCast(me, SPELL_FROST_AURA);
    DoCast(me, SPELL_PERMAEATING_CHILL);
    Talk(SAY_AGGRO);
}

void BossSindragosaAI::JustReachedHome()
{
    BossAI::JustReachedHome();
    instance->SetBossState(DATA_SINDRAGOSA, FAIL);
    me->setActive(false);
    me->SetCanFly(false);
    me->SetDisableGravity(false);
    me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
}

void BossSindragosaAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void BossSindragosaAI::DoAction(const int32 action)
{
    if (action == ACTION_START_FROSTWYRM)
    {
        if (_summoned)
            return;

        _summoned = true;
        if (TempSummon* summon = me->ToTempSummon())
            summon->SetTempSummonType(TEMPSUMMON_DEAD_DESPAWN);

        if (me->isDead())
            return;

        instance->DoUseDoorOrButton(instance->GetData64(GO_SINDRAGOSA_ENTRANCE_DOOR), true);
        me->setActive(true);
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
        me->SetSpeed(MOVE_FLIGHT, 4.0f);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        float moveTime = me->GetExactDist(&SindragosaFlyPos) / (me->GetSpeed(MOVE_FLIGHT) * 0.001f);
        me->m_Events.AddEvent(new FrostwyrmLandEvent(*me, SindragosaLandPos), me->m_Events.CalculateTime(uint64(moveTime) + 250));
        me->GetMotionMaster()->MovePoint(POINT_FROSTWYRM_FLY_IN, SindragosaFlyPos);
        DoCast(me, SPELL_SINDRAGOSA_S_FURY);
    }
}

uint32 BossSindragosaAI::GetData(uint32 type)
{
    if (type == DATA_MYSTIC_BUFFET_STACK)
        return _mysticBuffetStack;
    return 0xFFFFFFFF;
}

void BossSindragosaAI::SetData(uint32 type, uint32 data)
{
    if (type == DATA_FROST_BOMB_HIT)
    {
        _frostBombHitCount += data;
        if (_frostBombHitCount == 4)
        {
            _frostBombHitCount = 0;
            events.ScheduleEvent(EVENT_ASPHYXIATION, 1000);
        }
    }
}

void BossSindragosaAI::MovementInform(uint32 type, uint32 point)
{
    if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
        return;

    switch (point)
    {
    case POINT_FROSTWYRM_LAND:
        instance->DoUseDoorOrButton(instance->GetData64(GO_SINDRAGOSA_ENTRANCE_DOOR), false);
        me->setActive(false);
        me->SetCanFly(false);
        me->SetDisableGravity(false);
        me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
        me->SetHomePosition(SindragosaLandPos);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->SetReactState(REACT_AGGRESSIVE);
        me->SetSpeed(MOVE_FLIGHT, 2.5f);

        // Sindragosa enters combat as soon as she lands
        DoZoneInCombat();
        break;
    case POINT_TAKEOFF:
        events.ScheduleEvent(EVENT_AIR_MOVEMENT, 1);
        break;
    case POINT_AIR_PHASE:
        me->CastCustomSpell(SPELL_ICE_TOMB_TARGET, SPELLVALUE_MAX_TARGETS, RAID_MODE<int32>(2, 5, 2, 6), NULL, TRIGGERED_FULL_MASK);
        me->SetFacingTo(float(M_PI));
        events.ScheduleEvent(EVENT_AIR_MOVEMENT_FAR, 1);
        events.ScheduleEvent(EVENT_FROST_BOMB, 9000);
        break;
    case POINT_AIR_PHASE_FAR:
        me->SetFacingTo(float(M_PI));
        events.ScheduleEvent(EVENT_LAND, 30000);
        break;
    case POINT_LAND:
        events.ScheduleEvent(EVENT_LAND_GROUND, 1);
        break;
    case POINT_LAND_GROUND:
        {
            me->SetCanFly(false);
            me->SetDisableGravity(false);
            me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
            me->SetReactState(REACT_AGGRESSIVE);
            if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                me->GetMotionMaster()->MovementExpired();
            _isInAirPhase = false;
            break;
        }
    default:
        break;
    }
}

void BossSindragosaAI::DamageTaken(Unit* , uint32& )
{
    if (!_isThirdPhase && !HealthAbovePct(35))
    {
        events.CancelEvent(EVENT_AIR_PHASE);
        events.ScheduleEvent(EVENT_THIRD_PHASE_CHECK, 1000);
        _isThirdPhase = true;
    }
}

void BossSindragosaAI::JustSummoned(Creature* summon)
{
    summons.Summon(summon);
    if (summon->GetEntry() == NPC_FROST_BOMB)
    {
        summon->CastSpell(summon, SPELL_FROST_BOMB_VISUAL, true);
        summon->CastSpell(summon, SPELL_BIRTH_NO_VISUAL, true);
        summon->m_Events.AddEvent(new FrostBombExplosion(summon, me->GetGUID()), summon->m_Events.CalculateTime(5500));
    }
}

void BossSindragosaAI::SummonedCreatureDespawn(Creature* summon)
{
    BossAI::SummonedCreatureDespawn(summon);
    if (summon->GetEntry() == NPC_ICE_TOMB)
        summon->AI()->JustDied(summon);
}

void BossSindragosaAI::SpellHitTarget(Unit* target, const SpellInfo* spell)
{
    if (uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(70127, me))
    {
        if (spellId == spell->Id)
        {
            if (Aura const* mysticBuffet = target->GetAura(spell->Id))
                _mysticBuffetStack = std::max<uint8>(_mysticBuffetStack, mysticBuffet->GetStackAmount());

            return;
        }
    }

    if (!Is25ManRaid())
    {
        return; // Frost Infusion should only work in 25 man raid
    }

    // Frost Infusion
    if (Player* player = target->ToPlayer())
    {
        if (uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(_isThirdPhase ? SPELL_FROST_BREATH_P2 : SPELL_FROST_BREATH_P1, me))
        {
            if (spellId == spell->Id)
            {
                Item* shadowsEdge = player->GetWeaponForAttack(BASE_ATTACK, true);
                if (player->GetQuestStatus(QUEST_FROST_INFUSION) == QUEST_STATUS_INCOMPLETE && shadowsEdge)
                {
                    if (!player->HasAura(SPELL_FROST_IMBUED_BLADE) && shadowsEdge->GetEntry() == ITEM_SHADOW_S_EDGE)
                    {
                        if (Aura* infusion = player->GetAura(SPELL_FROST_INFUSION))
                        {
                            if (infusion->GetStackAmount() == 3)
                            {
                                target->CastSpell(target, SPELL_FROST_IMBUED_BLADE, TRIGGERED_FULL_MASK, NULL, NULL, me->GetGUID());
                                player->RemoveAura(infusion);
                            }
                            else
                                target->CastSpell(target, SPELL_FROST_INFUSION, TRIGGERED_FULL_MASK, NULL, NULL, me->GetGUID());
                        }
                        else
                            target->CastSpell(target, SPELL_FROST_INFUSION, TRIGGERED_FULL_MASK, NULL, NULL, me->GetGUID());
                    }
                }

                return;
            }
        }
    }
}

void BossSindragosaAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim() || !CheckInRoom())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_BERSERK:
            DoScriptText(EMOTE_GENERIC_BERSERK_RAID, me);
            Talk(SAY_BERSERK);
            DoCast(me, SPELL_BERSERK);
            break;
        case EVENT_CLEAVE:
            DoCastVictim(SPELL_CLEAVE);
            events.ScheduleEvent(EVENT_CLEAVE, urand(15000, 20000), EVENT_GROUP_LAND_PHASE);
            break;
        case EVENT_TAIL_SMASH:
            DoCast(me, SPELL_TAIL_SMASH);
            events.ScheduleEvent(EVENT_TAIL_SMASH, urand(27000, 32000), EVENT_GROUP_LAND_PHASE);
            break;
        case EVENT_FROST_BREATH:
            DoCastVictim(_isThirdPhase ? SPELL_FROST_BREATH_P2 : SPELL_FROST_BREATH_P1);
            events.ScheduleEvent(EVENT_FROST_BREATH, urand(20000, 25000), EVENT_GROUP_LAND_PHASE);
            break;
        case EVENT_UNCHAINED_MAGIC:
            Talk(SAY_UNCHAINED_MAGIC);
            DoCast(me, SPELL_UNCHAINED_MAGIC);
            events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, urand(30000, 35000), EVENT_GROUP_LAND_PHASE);
            break;
        case EVENT_ICY_GRIP:
            if ((events.GetNextEventTime(EVENT_FROST_BREATH) - events.GetTimer()) < 2000)
                events.RescheduleEvent(EVENT_FROST_BREATH, (events.GetNextEventTime(EVENT_FROST_BREATH) - events.GetTimer()) + 3000, EVENT_GROUP_LAND_PHASE);
            DoCast(me, SPELL_ICY_GRIP);
            events.ScheduleEvent(EVENT_BLISTERING_COLD, 1500, EVENT_GROUP_LAND_PHASE);
            break;
        case EVENT_BLISTERING_COLD:
            Talk(EMOTE_WARN_BLISTERING_COLD);
            DoCast(me, SPELL_BLISTERING_COLD);
            events.ScheduleEvent(EVENT_BLISTERING_COLD_YELL, 5000, EVENT_GROUP_LAND_PHASE);
            break;
        case EVENT_BLISTERING_COLD_YELL:
            Talk(SAY_BLISTERING_COLD);
            break;
        case EVENT_AIR_PHASE:
            {
                _isInAirPhase = true;
                Talk(SAY_AIR_PHASE);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                Position pos;
                pos.Relocate(me);
                pos.m_positionZ += 17.0f;
                me->GetMotionMaster()->MoveTakeoff(POINT_TAKEOFF, pos);
                events.CancelEventGroup(EVENT_GROUP_LAND_PHASE);
                events.ScheduleEvent(EVENT_AIR_PHASE, 110000);
                break;
            }
        case EVENT_AIR_MOVEMENT:
            me->GetMotionMaster()->MovePoint(POINT_AIR_PHASE, SindragosaAirPos);
            break;
        case EVENT_AIR_MOVEMENT_FAR:
            me->GetMotionMaster()->MovePoint(POINT_AIR_PHASE_FAR, SindragosaAirPosFar);
            break;
        case EVENT_ICE_TOMB:
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, FrostBeaconSelector(me)))
            {
                Talk(EMOTE_WARN_FROZEN_ORB, target->GetGUID());
                DoCast(target, SPELL_FROST_BEACON, true);
                me->CastSpell(target, SPELL_ICE_TOMB_DUMMY, TRIGGERED_IGNORE_SET_FACING);
                // me->CastCustomSpell(SPELL_ICE_TOMB_TARGET, SPELLVALUE_MAX_TARGETS, 1, target);
            }
            if ((events.GetNextEventTime(EVENT_ICY_GRIP) - events.GetTimer()) < 6000)
                events.RescheduleEvent(EVENT_ICY_GRIP, (events.GetNextEventTime(EVENT_ICY_GRIP) - events.GetTimer())+8000, EVENT_GROUP_LAND_PHASE);
            // uint32 rand = urand(15000);
            events.ScheduleEvent(EVENT_ICE_TOMB, 15000);
            events.ScheduleEvent(EVENT_ASPHYXIATION, 14999);
            break;
        }
        case EVENT_FROST_BOMB:
            {
                float destX, destY, destZ;
                destX = float(rand_norm()) * 75.0f + 4350.0f;
                destY = float(rand_norm()) * 75.0f + 2450.0f;
                destZ = 205.0f; // random number close to ground, get exact in next call
                me->UpdateGroundPositionZ(destX, destY, destZ);
                me->CastSpell(destX, destY, destZ, SPELL_FROST_BOMB_TRIGGER, false);
                events.ScheduleEvent(EVENT_FROST_BOMB, urand(6000, 8000));
                break;
            }
        case EVENT_LAND:
            {
                events.CancelEvent(EVENT_FROST_BOMB);
                me->GetMotionMaster()->MovePoint(POINT_LAND, SindragosaFlyInPos);
                break;
            }
        case EVENT_LAND_GROUND:
            events.ScheduleEvent(EVENT_CLEAVE, urand(13000, 15000), EVENT_GROUP_LAND_PHASE);
            events.ScheduleEvent(EVENT_TAIL_SMASH, urand(19000, 23000), EVENT_GROUP_LAND_PHASE);
            events.ScheduleEvent(EVENT_FROST_BREATH, urand(10000, 15000), EVENT_GROUP_LAND_PHASE);
            events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, urand(12000, 17000), EVENT_GROUP_LAND_PHASE);
            events.ScheduleEvent(EVENT_ICY_GRIP, urand(35000, 40000), EVENT_GROUP_LAND_PHASE);
            me->GetMotionMaster()->MoveLand(POINT_LAND_GROUND, SindragosaLandPos);
            break;
        case EVENT_THIRD_PHASE_CHECK:
            {
                if (!_isInAirPhase)
                {
                    Talk(SAY_PHASE_2);
                    events.ScheduleEvent(EVENT_ICE_TOMB, urand(7000, 10000));
                    events.RescheduleEvent(EVENT_ICY_GRIP, urand(35000, 40000));
                    DoCast(me, SPELL_MYSTIC_BUFFET, true);
                }
                else
                    events.ScheduleEvent(EVENT_THIRD_PHASE_CHECK, 6000);
                break;
            }
        case EVENT_ASPHYXIATION:
        {
            // trigger Asphyxiation
            EntryCheckPredicate pred(NPC_ICE_TOMB);
            summons.DoAction(ACTION_TRIGGER_ASPHYXIATION, pred);
            break;
        }
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}