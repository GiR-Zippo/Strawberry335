#include "Boss_LordMarrowgar.h"

void BossLordMarrowgarAI::Reset()
{
    BossAI::Reset();

    me->SetSpeed(MOVE_RUN, _baseSpeed, true);
    me->RemoveAurasDueToSpell(SPELL_BONE_STORM);
    me->RemoveAurasDueToSpell(SPELL_BERSERK);
    _boneSlice = false;
    me->SetReactState(REACT_AGGRESSIVE);
}

void BossLordMarrowgarAI::EnterEvadeMode()
{
    instance->SetData(DATA_BONED_ACHIEVEMENT, uint32(true));    // reset

    _DespawnAtEvade();

    BossAI::EnterEvadeMode();
}

void BossLordMarrowgarAI::EnterCombat(Unit* who)
{
    BossAI::EnterCombat(who);

    Talk(SAY_AGGRO);

    events.ScheduleEvent(EVENT_ENABLE_BONE_SLICE, 10000);
    events.ScheduleEvent(EVENT_BONE_SPIKE_GRAVEYARD, urand(10000, 15000), EVENT_GROUP_SPECIAL);
    events.ScheduleEvent(EVENT_COLDFLAME, urand(5000, 8000));
    events.ScheduleEvent(EVENT_WARN_BONE_STORM, urand(45000, 50000));
    events.ScheduleEvent(EVENT_ENRAGE, 600000);

    DoZoneInCombat(me, 150.0f);
}

void BossLordMarrowgarAI::JustDied(Unit* killer)
{
    BossAI::JustDied(killer);

    Talk(SAY_DEATH);
}

void BossLordMarrowgarAI::KilledUnit(Unit* victim)
{
    BossAI::KilledUnit(victim);

    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void BossLordMarrowgarAI::MoveInLineOfSight(Unit* who)
{
    if (!_introDone && me->IsWithinDistInMap(who, 70.0f))
    {
        Talk(SAY_ENTER_ZONE);
        _introDone = true;
    }
}

void BossLordMarrowgarAI::JustSummoned(Creature* summon)
{
    if (summon->GetEntry() == 36672) // Npc_Coldflame EntryID
    {
        if (me->HasAura(SPELL_BONE_STORM))
        {
            Position const* ownerPos = GetLastColdflamePosition();
            float ang = summon->GetAngle(ownerPos) - static_cast<float>(M_PI);
            MapManager::NormalizeOrientation(ang);
            summon->SetOrientation(ang);

            Position pos;
            me->GetNearPosition(pos, 0.0f, 0.0f);
            summon->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), summon->GetPositionZ(), summon->GetOrientation());
        }
    }
}

void BossLordMarrowgarAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim() || !CheckInRoom())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_BONE_SPIKE_GRAVEYARD:
                if (IsHeroic() || !me->HasAura(SPELL_BONE_STORM))
                    DoCast(me, BONE_SPIKE_GRAVEYARD_HELPER);
                events.ScheduleEvent(EVENT_BONE_SPIKE_GRAVEYARD, urand(15000, 20000), EVENT_GROUP_SPECIAL);
                break;
            case EVENT_COLDFLAME:
                _coldflameLastPos.Relocate(me);
                _coldflameTarget = 0LL;
                if (!me->HasAura(SPELL_BONE_STORM))
                {
                    DoCastAOE(SPELL_COLDFLAME_NORMAL);
                    events.ScheduleEvent(EVENT_COLDFLAME, urand(5000, 8000));
                }
                else
                {
                    DoCast(me, SPELL_COLDFLAME_BONE_STORM);
                }
                break;
            case EVENT_WARN_BONE_STORM:
                me->SetReactState(REACT_PASSIVE);

                _boneSlice = false;
                Talk(EMOTE_BONE_STORM);
                me->FinishSpell(CURRENT_MELEE_SPELL, false);
                DoCast(me, SPELL_BONE_STORM);
                events.CancelEvent(EVENT_COLDFLAME);
                events.DelayEvents(EVENT_GROUP_SPECIAL, 3050);
                events.ScheduleEvent(EVENT_BONE_STORM_BEGIN, 3000);
                events.ScheduleEvent(EVENT_WARN_BONE_STORM, urand(90000, 95000));
                break;
            case EVENT_BONE_STORM_BEGIN:
                if (Aura* pStorm = me->GetAura(SPELL_BONE_STORM))
                    pStorm->SetDuration(int32(_boneStormDuration));

                me->SetSpeed(MOVE_RUN, _baseSpeed * 3.0f, true);
                Talk(SAY_BONE_STORM);
                events.ScheduleEvent(EVENT_BONE_STORM_END, _boneStormDuration+1);
                // no break here
            case EVENT_BONE_STORM_MOVE:
            {
                // If timer has been reached before boss moved
                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    MovementInform(POINT_MOTION_TYPE, POINT_TARGET_BONESTORM_PLAYER);

                events.ScheduleEvent(EVENT_BONE_STORM_MOVE, _boneStormDuration/3);
                Unit* unit = NULL;
                if (!unit)
                {
                    if (!IsHeroic())
                        unit = SelectTarget(SELECT_TARGET_FARTHEST, 0, 0.0f, true, -SPELL_IMPALED);
                    else
                    {
                        unit = SelectTarget(SELECT_TARGET_RANDOM, 0, -10.0f, true, -SPELL_IMPALED);
                        if (!unit)
                            unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, -SPELL_IMPALED);
                    }
                }
                if (unit)
                    me->GetMotionMaster()->MovePoint(POINT_TARGET_BONESTORM_PLAYER, unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ());
                break;
            }
            case EVENT_BONE_STORM_END:
                me->SetReactState(REACT_AGGRESSIVE);

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                {
                    me->GetMotionMaster()->MovementExpired();

                    _coldflameLastPos.Relocate(me);
                    _coldflameTarget = 0LL;
                    DoCast(me, SPELL_COLDFLAME_BONE_STORM);
                }

                DoStartMovement(me->getVictim());
                me->SetSpeed(MOVE_RUN, _baseSpeed, true);
                events.CancelEvent(EVENT_BONE_STORM_MOVE);
                events.ScheduleEvent(EVENT_ENABLE_BONE_SLICE, 10000);
                events.CancelEvent(EVENT_COLDFLAME);
                events.ScheduleEvent(EVENT_COLDFLAME, urand(5000, 8000));
                if (!IsHeroic())
                    events.RescheduleEvent(EVENT_BONE_SPIKE_GRAVEYARD, urand(15000, 20000), EVENT_GROUP_SPECIAL);
                break;
            case EVENT_ENABLE_BONE_SLICE:
                _boneSlice = true;
                break;
            case EVENT_ENRAGE:
                DoCast(me, SPELL_BERSERK, true);
                Talk(SAY_BERSERK);
                break;
        }
    }

    // We should not melee attack when storming
    if (me->HasAura(SPELL_BONE_STORM))
        return;

    // 10 seconds since encounter start Bone Slice replaces melee attacks
    if (_boneSlice && !me->GetCurrentSpell(CURRENT_MELEE_SPELL))
        DoCastVictim(BONE_SLICE_HELPER);

    DoMeleeAttackIfReady();
}

void BossLordMarrowgarAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE || id != POINT_TARGET_BONESTORM_PLAYER)
        return;

    // Cast flame next tick
    events.ScheduleEvent(EVENT_COLDFLAME, 1);

    // lock movement
    DoStartNoMovement(me->getVictim());
}

Position const* BossLordMarrowgarAI::GetLastColdflamePosition() const
{
    return &_coldflameLastPos;
}

uint64 BossLordMarrowgarAI::GetGUID(int32 type/* = 0 */)
{
    if (type == DATA_COLDFLAME_GUID)
        return _coldflameTarget;
    return 0LL;
}

void BossLordMarrowgarAI::SetGUID(uint64 guid, int32 type/* = 0 */)
{
    if (type != DATA_COLDFLAME_GUID)
        return;

    _coldflameTarget = guid;
}
