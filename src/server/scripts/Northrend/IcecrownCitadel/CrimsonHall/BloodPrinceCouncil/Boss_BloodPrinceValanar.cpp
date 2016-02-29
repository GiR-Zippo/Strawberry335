#include "Boss_BloodPrinceValanar.h"

void BossBloodPrinceValanarAI::InitializeAI()
{
    if (CreatureData const* data = sObjectMgr->GetCreatureData(me->GetDBTableGUIDLow()))
        if (data->curhealth)
            _spawnHealth = data->curhealth;

    if (!me->isDead())
        JustRespawned();

    me->SetReactState(REACT_DEFENSIVE);
}

void BossBloodPrinceValanarAI::Reset()
{
    events.Reset();
    summons.DespawnAll();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    _isEmpowered = false;
    me->SetHealth(me->GetMaxHealth());
    instance->SetData(DATA_ORB_WHISPERER_ACHIEVEMENT, uint32(true));
    me->SetReactState(REACT_DEFENSIVE);
    instance->SetData(DATA_KINETIC_BOMB_COUNT, 0);
}

void BossBloodPrinceValanarAI::MoveInLineOfSight(Unit* )
{
}

void BossBloodPrinceValanarAI::EnterCombat(Unit* )
{
    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_BLOOD_PRINCES_CONTROL)))
        DoZoneInCombat(controller);

    events.ScheduleEvent(EVENT_BERSERK, 600000);
    if (instance->GetData(DATA_KINETIC_BOMB_COUNT) < RAID_MODE<uint32>(2, 3, 2, 3))
    {
        events.ScheduleEvent(EVENT_KINETIC_BOMB, urand(18000, 24000));
        instance->SetData(DATA_KINETIC_BOMB_COUNT, instance->GetData(DATA_KINETIC_BOMB_COUNT + 1));
    }

    events.ScheduleEvent(EVENT_SHOCK_VORTEX, urand(15000, 20000));
    if (IsHeroic())
        me->AddAura(SPELL_SHADOW_PRISON, me);
}

void BossBloodPrinceValanarAI::JustDied(Unit* )
{
    events.Reset();
    summons.DespawnAll();

    Talk(SAY_VALANAR_DEATH);
    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
}

void BossBloodPrinceValanarAI::JustReachedHome()
{
    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
    me->SetHealth(me->GetMaxHealth());
    _isEmpowered = false;
    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_BLOOD_PRINCES_CONTROL)))
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        controller->AI()->SetData(0, 1);
    }
}

void BossBloodPrinceValanarAI::JustRespawned()
{
    DoCast(me, SPELL_FEIGN_DEATH);
    me->SetHealth(_spawnHealth);
}

void BossBloodPrinceValanarAI::JustSummoned(Creature* summon)
{
    switch (summon->GetEntry())
    {
        case NPC_KINETIC_BOMB_TARGET:
            summon->SetReactState(REACT_PASSIVE);
            summon->CastSpell(summon, SPELL_KINETIC_BOMB, true, NULL, NULL, me->GetGUID());
            break;
        default:
            break;
    }
    summons.Summon(summon);
    if (me->isInCombat())
        DoZoneInCombat(summon);
}

void BossBloodPrinceValanarAI::SpellHit(Unit* , const SpellInfo* spell)
{
    if (spell->Id == SPELL_INVOCATION_OF_BLOOD_VALANAR)
        DoAction(ACTION_CAST_INVOCATION);
}

void BossBloodPrinceValanarAI::DamageDealt(Unit* , uint32& damage, DamageEffectType damageType)
{
    if (damageType != SPELL_DIRECT_DAMAGE)
        return;

    if (damage > RAID_MODE<uint32>(23000, 25000, 23000, 25000))
        instance->SetData(DATA_ORB_WHISPERER_ACHIEVEMENT, uint32(false));
}

void BossBloodPrinceValanarAI::DamageTaken(Unit* attacker, uint32& damage)
{
    if (!_isEmpowered)
    {
        me->AddThreat(attacker, float(damage));
        damage = 0;
    }
}

void BossBloodPrinceValanarAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_VALANAR_KILL);
}

void BossBloodPrinceValanarAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ACTION_STAND_UP:
            me->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
            me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
            me->ForceValuesUpdateAtIndex(UNIT_NPC_FLAGS);   // was in sniff. don't ask why
            me->m_Events.AddEvent(new StandUpEvent(*me), me->m_Events.CalculateTime(1000));
            break;
        case ACTION_CAST_INVOCATION:
            Talk(SAY_VALANAR_INVOCATION);
            DoCast(me, SPELL_INVOCATION_VISUAL_ACTIVE, true);
            _isEmpowered = true;
            break;
        case ACTION_REMOVE_INVOCATION:
            me->SetHealth(_spawnHealth);
            me->RemoveAurasDueToSpell(SPELL_INVOCATION_VISUAL_ACTIVE);
            me->RemoveAurasDueToSpell(SPELL_INVOCATION_OF_BLOOD_VALANAR);
            _isEmpowered = false;
            break;
        default:
            break;
    }
}

bool BossBloodPrinceValanarAI::CheckRoom()
{
    if (!CheckBoundary(me))
    {
        EnterEvadeMode();
        if (Creature* keleseth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_KELESETH_GUID)))
            keleseth->AI()->EnterEvadeMode();

        if (Creature* taldaram = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_TALDARAM_GUID)))
            taldaram->AI()->EnterEvadeMode();

        return false;
    }

    return true;
}

void BossBloodPrinceValanarAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim() || !CheckRoom())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_BERSERK:
                DoCast(me, SPELL_BERSERK);
                Talk(SAY_VALANAR_BERSERK);
                break;
            case EVENT_KINETIC_BOMB:
                if (instance->GetData(DATA_KINETIC_BOMB_COUNT) < RAID_MODE<uint32>(2, 3, 2, 3))
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                    {
                        DoCast(target, SPELL_KINETIC_BOMB_TARGET);
                        Talk(SAY_VALANAR_SPECIAL);
                    }
                    instance->SetData(DATA_KINETIC_BOMB_COUNT, instance->GetData(DATA_KINETIC_BOMB_COUNT) + 1);
                }
                events.ScheduleEvent(EVENT_KINETIC_BOMB, urand(18000, 24000));
                break;
            case EVENT_SHOCK_VORTEX:
                if (_isEmpowered)
                {
                    DoCast(me, SPELL_EMPOWERED_SHOCK_VORTEX);
                    Talk(EMOTE_VALANAR_SHOCK_VORTEX);
                    events.ScheduleEvent(EVENT_SHOCK_VORTEX, 30000);
                }
                else
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                        DoCast(target, SPELL_SHOCK_VORTEX);
                    events.ScheduleEvent(EVENT_SHOCK_VORTEX, urand(18000, 23000));
                }
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}