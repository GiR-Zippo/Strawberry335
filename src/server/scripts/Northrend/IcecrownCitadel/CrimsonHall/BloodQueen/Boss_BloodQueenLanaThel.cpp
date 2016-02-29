#include "Boss_BloodQueenLanaThel.h"

void BossBloodQueenLanaThelAI::Reset()
{
    _Reset();
    events.ScheduleEvent(EVENT_BERSERK, 330000);
    events.ScheduleEvent(EVENT_VAMPIRIC_BITE, 15000);
    events.ScheduleEvent(EVENT_BLOOD_MIRROR, 2500, EVENT_GROUP_CANCELLABLE);
    events.ScheduleEvent(EVENT_DELIRIOUS_SLASH, urand(20000, 24000), EVENT_GROUP_NORMAL);
    events.ScheduleEvent(EVENT_PACT_OF_THE_DARKFALLEN, 15000, EVENT_GROUP_NORMAL);
    events.ScheduleEvent(EVENT_SWARMING_SHADOWS, 30500, EVENT_GROUP_NORMAL);
    events.ScheduleEvent(EVENT_TWILIGHT_BLOODBOLT, urand(20000, 25000), EVENT_GROUP_NORMAL);
    events.ScheduleEvent(EVENT_AIR_PHASE, 124000 + uint32(Is25ManRaid() ? 3000 : 0));
    CleanAuras();
    me->SetSpeed(MOVE_FLIGHT, 0.642857f, true);
    _offtank = NULL;
    _vampires.clear();
    _creditBloodQuickening = false;
    _killMinchar = false;
    _isAirPhase = false;
}

void BossBloodQueenLanaThelAI::EnterCombat(Unit* who)
{
    if (!instance->CheckRequiredBosses(DATA_BLOOD_QUEEN_LANA_THEL, who->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    me->setActive(true);
    DoZoneInCombat();
    Talk(SAY_AGGRO);
    instance->SetBossState(DATA_BLOOD_QUEEN_LANA_THEL, IN_PROGRESS);
    CleanAuras();

    DoCast(me, SPELL_SHROUD_OF_SORROW, true);
    DoCast(me, SPELL_FRENZIED_BLOODTHIRST_VISUAL, true);
    _creditBloodQuickening = instance->GetData(DATA_BLOOD_QUICKENING_STATE) == IN_PROGRESS;
}

void BossBloodQueenLanaThelAI::JustDied(Unit* killer)
{
    _JustDied();
    Talk(SAY_DEATH);
    CleanAuras();
    // Blah, credit the quest
    if (_creditBloodQuickening)
    {
        instance->SetData(DATA_BLOOD_QUICKENING_STATE, DONE);
        if (Player* player = killer->ToPlayer())
            player->RewardPlayerAndGroupAtEvent(MINCHAR_HELPER, player);
        if (Creature* minchar = me->FindNearestCreature(NPC_INFILTRATOR_MINCHAR_BQ_10, 200.0f))
        {
            minchar->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
            minchar->SetCanFly(false);
            minchar->SendMovementFlagUpdate();
            minchar->RemoveAllAuras();
            minchar->GetMotionMaster()->MoveCharge(4629.3711f, 2782.6089f, 401.5301f, SPEED_CHARGE/3.0f);
        }
    }
}

void BossBloodQueenLanaThelAI::CleanAuras()
{
    instance->DoRemoveAurasDueToSpellOnPlayers(ESSENCE_OF_BLOOD_QUEEN);
    instance->DoRemoveAurasDueToSpellOnPlayers(ESSENCE_OF_BLOOD_QUEEN_PLR);
    instance->DoRemoveAurasDueToSpellOnPlayers(FRENZIED_BLOODTHIRST);
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNCONTROLLABLE_FRENZY);
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_MIRROR_DAMAGE);
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_MIRROR_VISUAL);
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_MIRROR_DUMMY);
    instance->DoRemoveAurasDueToSpellOnPlayers(DELIRIOUS_SLASH);
    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PACT_OF_THE_DARKFALLEN);
    me->RemoveAurasDueToSpell(SPELL_PRESENCE_OF_THE_DARKFALLEN);
}

void BossBloodQueenLanaThelAI::DoAction(const int32 action)
{
    if (action != ACTION_KILL_MINCHAR)
        return;

    if (instance->GetBossState(DATA_BLOOD_QUEEN_LANA_THEL) == IN_PROGRESS)
        _killMinchar = true;
    else
    {
        me->SetDisableGravity(true);
        me->SetCanFly(true);
        me->SendMovementFlagUpdate();
        me->GetMotionMaster()->MovePoint(POINT_MINCHAR, mincharPos);
    }
}

void BossBloodQueenLanaThelAI::EnterEvadeMode()
{
    _EnterEvadeMode();
    CleanAuras();
    if (_killMinchar)
    {
        _killMinchar = false;
        me->SetDisableGravity(true);
        me->SetCanFly(true);
        me->GetMotionMaster()->MovePoint(POINT_MINCHAR, mincharPos);
    }
    else
    {
        if (_isAirPhase)
            me->GetMotionMaster()->MoveLand(POINT_HOME, me->GetHomePosition());
        else
            me->GetMotionMaster()->MoveTargetedHome();
        Reset();
    }
    _DespawnAtEvade();
}

void BossBloodQueenLanaThelAI::JustReachedHome()
{
    me->SetDisableGravity(false);
    me->SetCanFly(false);
    me->SetReactState(REACT_AGGRESSIVE);
    _JustReachedHome();
    Talk(SAY_WIPE);
    instance->SetBossState(DATA_BLOOD_QUEEN_LANA_THEL, FAIL);
}

void BossBloodQueenLanaThelAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void BossBloodQueenLanaThelAI::SetGUID(uint64 guid, int32 type)
{
    switch (type)
    {
    case GUID_VAMPIRE:
        _vampires.insert(guid);
        break;
    case GUID_BLOODBOLT:
        _bloodboltedPlayers.insert(guid);
        break;
    default:
        break;
    }
}

void BossBloodQueenLanaThelAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
        return;

    switch (id)
    {
    case POINT_CENTER:
        DoCast(me, SPELL_INCITE_TERROR);
        events.ScheduleEvent(EVENT_AIR_PHASE, 100000 + uint32(Is25ManRaid() ? 0 : 20000));
        events.RescheduleEvent(EVENT_SWARMING_SHADOWS, 30500, EVENT_GROUP_NORMAL);
        events.RescheduleEvent(EVENT_PACT_OF_THE_DARKFALLEN, 25500, EVENT_GROUP_NORMAL);
        events.ScheduleEvent(EVENT_AIR_START_FLYING, 5000);
        break;
    case POINT_AIR:
        _bloodboltedPlayers.clear();
        DoCast(me, SPELL_BLOODBOLT_WHIRL);
        Talk(SAY_AIR_PHASE);
        events.ScheduleEvent(EVENT_AIR_FLY_DOWN, 10000);
        break;
    case POINT_GROUND:
        me->SetDisableGravity(false);
        me->SetCanFly(false);
        me->SendMovementFlagUpdate();
        _isAirPhase = false;
        me->SetReactState(REACT_AGGRESSIVE);
        if (Unit* victim = me->SelectVictim())
            AttackStart(victim);
        events.ScheduleEvent(EVENT_BLOOD_MIRROR, 2500, EVENT_GROUP_CANCELLABLE);
        break;
    case POINT_MINCHAR:
        DoCast(me, SPELL_ANNIHILATE, true);
        // already in evade mode
        if (_isAirPhase)
            me->GetMotionMaster()->MoveLand(0, me->GetHomePosition());
        else
            me->GetMotionMaster()->MoveTargetedHome();
        Reset();
        break;
    case POINT_HOME:
        JustReachedHome();
        break;
    default:
        break;
    }
}

void BossBloodQueenLanaThelAI::UpdateAI(const uint32 diff)
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
        case EVENT_VAMPIRIC_BITE:
            {
                std::list<Player*> targets;
                SelectRandomTarget(false, &targets);
                if (!targets.empty())
                {
                    Unit* target = targets.front();
                    if (IsHeroic())
                        target->CastSpell(target, SPELL_PRESENCE_OF_THE_DARKFALLEN, true);
                    DoCast(target, SPELL_VAMPIRIC_BITE);
                    Talk(SAY_VAMPIRIC_BITE);
                    _vampires.insert(target->GetGUID());
                }
                break;
            }
        case EVENT_BLOOD_MIRROR:
            {
                // victim can be NULL when this is processed in the same update tick as EVENT_AIR_PHASE
                if (me->getVictim())
                {
                    Player* newOfftank = SelectRandomTarget(true);
                    if (_offtank != newOfftank)
                    {
                        _offtank = newOfftank;
                        if (_offtank)
                        {
                            // both spells have SPELL_ATTR5_SINGLE_TARGET_SPELL, no manual removal needed
                            _offtank->CastSpell(me->getVictim(), SPELL_BLOOD_MIRROR_DAMAGE, true);
                            me->getVictim()->CastSpell(_offtank, SPELL_BLOOD_MIRROR_DUMMY, true);
                            DoCastVictim(SPELL_BLOOD_MIRROR_VISUAL);
                            if (Is25ManRaid())
                                if (Item* shadowsEdge = _offtank->GetWeaponForAttack(BASE_ATTACK, true))
                                    if (!_offtank->HasAura(SPELL_THIRST_QUENCHED) && shadowsEdge->GetEntry() == ITEM_SHADOW_S_EDGE && !_offtank->HasAura(SPELL_GUSHING_WOUND))
                                        _offtank->CastSpell(_offtank, SPELL_GUSHING_WOUND, true);

                        }
                    }
                }
                events.ScheduleEvent(EVENT_BLOOD_MIRROR, 2500, EVENT_GROUP_CANCELLABLE);
                break;
            }
        case EVENT_DELIRIOUS_SLASH:
            if (_offtank && !me->HasByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER))
                DoCast(_offtank, SPELL_DELIRIOUS_SLASH);
            events.ScheduleEvent(EVENT_DELIRIOUS_SLASH, urand(20000, 24000), EVENT_GROUP_NORMAL);
            break;
        case EVENT_PACT_OF_THE_DARKFALLEN:
            {
                std::list<Player*> targets;
                SelectRandomTarget(false, &targets);
                uint32 targetCount = 2;
                // do not combine these checks! we want it incremented TWICE when both conditions are met
                if (IsHeroic())
                    ++targetCount;
                if (Is25ManRaid())
                    ++targetCount;
                Trinity::Containers::RandomResizeList<Player*>(targets, targetCount);
                if (targets.size() > 1)
                {
                    Talk(SAY_PACT_OF_THE_DARKFALLEN);
                    for (std::list<Player*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        DoCast(*itr, SPELL_PACT_OF_THE_DARKFALLEN);
                }
                events.ScheduleEvent(EVENT_PACT_OF_THE_DARKFALLEN, 30500, EVENT_GROUP_NORMAL);
                break;
            }
        case EVENT_SWARMING_SHADOWS:
            if (Player* target = SelectRandomTarget(false))
            {
                Talk(EMOTE_SWARMING_SHADOWS, target->GetGUID());
                Talk(SAY_SWARMING_SHADOWS);
                DoCast(target, SPELL_SWARMING_SHADOWS);
            }
            events.ScheduleEvent(EVENT_SWARMING_SHADOWS, 30500, EVENT_GROUP_NORMAL);
            break;
        case EVENT_TWILIGHT_BLOODBOLT:
            {
                std::list<Player*> targets;
                SelectRandomTarget(false, &targets, true);
                if (targets.size() < uint32(Is25ManRaid() ? 4 : 2))
                    SelectRandomTarget(false, &targets);
                Trinity::Containers::RandomResizeList<Player*>(targets, uint32(Is25ManRaid() ? 4 : 2));
                for (std::list<Player*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    DoCast(*itr, SPELL_TWILIGHT_BLOODBOLT);
                DoCast(me, SPELL_TWILIGHT_BLOODBOLT_TARGET);
                events.ScheduleEvent(EVENT_TWILIGHT_BLOODBOLT, urand(10000, 15000), EVENT_GROUP_NORMAL);
                break;
            }
        case EVENT_AIR_PHASE:
            DoStopAttack();
            me->SetReactState(REACT_PASSIVE);
            events.DelayEvents(10000, EVENT_GROUP_NORMAL);
            events.CancelEventGroup(EVENT_GROUP_CANCELLABLE);
            me->GetMotionMaster()->MovePoint(POINT_CENTER, centerPos);
            break;
        case EVENT_AIR_START_FLYING:
            _isAirPhase = true;
            me->SetDisableGravity(true);
            me->SetCanFly(true);
            me->SendMovementFlagUpdate();
            me->GetMotionMaster()->MoveTakeoff(POINT_AIR, airPos);
            break;
        case EVENT_AIR_FLY_DOWN:
            me->GetMotionMaster()->MoveLand(POINT_GROUND, centerPos);
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}

bool BossBloodQueenLanaThelAI::WasVampire(uint64 guid)
{
    return _vampires.count(guid) != 0;
}

bool BossBloodQueenLanaThelAI::WasBloodbolted(uint64 guid)
{
    return _bloodboltedPlayers.count(guid) != 0;
}

Player* BossBloodQueenLanaThelAI::SelectRandomTarget(bool includeOfftank, std::list< Player* >* targetList, bool bloodbolt)
{
    std::list<HostileReference*> const& threatlist = me->getThreatManager().getThreatList();
    std::list<Player*> tempTargets;

    if (threatlist.empty())
        return NULL;

    for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
        if (Unit* refTarget = (*itr)->getTarget())
            if (bloodbolt)
            {
                if (refTarget != me->getVictim() && refTarget->GetTypeId() == TYPEID_PLAYER && (includeOfftank ? true : (refTarget != _offtank)) && !refTarget->IsWithinDist(me, 8.0f))
                    tempTargets.push_back(refTarget->ToPlayer());
            } else {
                if (refTarget != me->getVictim() && refTarget->GetTypeId() == TYPEID_PLAYER && (includeOfftank ? true : (refTarget != _offtank)))
                    tempTargets.push_back(refTarget->ToPlayer());			
            }
    if (tempTargets.empty())
        return NULL;

    if (targetList)
    {
        *targetList = tempTargets;
        return NULL;
    }

    if (includeOfftank)
    {
        tempTargets.sort(Trinity::ObjectDistanceOrderPred(me->getVictim()));
        return tempTargets.front();
    }

    return Trinity::Containers::SelectRandomContainerElement(tempTargets);
}