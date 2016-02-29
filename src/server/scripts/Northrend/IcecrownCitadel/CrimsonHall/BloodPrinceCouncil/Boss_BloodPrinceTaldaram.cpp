#include "Boss_BloodPrinceTaldaram.h"

void BossBloodPrinceTaldaramAI::InitializeAI()
{
    if (CreatureData const* data = sObjectMgr->GetCreatureData(me->GetDBTableGUIDLow()))
        if (data->curhealth)
            _spawnHealth = data->curhealth;

    if (!me->isDead())
        JustRespawned();

    me->SetReactState(REACT_DEFENSIVE);
}

void BossBloodPrinceTaldaramAI::Reset()
{
    events.Reset();
    summons.DespawnAll();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    _isEmpowered = false;
    me->SetHealth(_spawnHealth);
    instance->SetData(DATA_ORB_WHISPERER_ACHIEVEMENT, uint32(true));
    me->SetReactState(REACT_DEFENSIVE);
}

void BossBloodPrinceTaldaramAI::MoveInLineOfSight(Unit* )
{
}

void BossBloodPrinceTaldaramAI::EnterCombat(Unit* )
{
    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_BLOOD_PRINCES_CONTROL)))
        DoZoneInCombat(controller);

    events.ScheduleEvent(EVENT_BERSERK, 600000);
    events.ScheduleEvent(EVENT_GLITTERING_SPARKS, urand(12000, 15000));
    events.ScheduleEvent(EVENT_CONJURE_FLAME, 20000);
    if (IsHeroic())
        me->AddAura(SPELL_SHADOW_PRISON, me);
}

void BossBloodPrinceTaldaramAI::JustDied(Unit* )
{
    events.Reset();
    summons.DespawnAll();

    Talk(EMOTE_TALDARAM_DEATH);
    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
}

void BossBloodPrinceTaldaramAI::JustReachedHome()
{
    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
    me->SetHealth(_spawnHealth);
    _isEmpowered = false;
    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_BLOOD_PRINCES_CONTROL)))
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        controller->AI()->SetData(0, 1);
    }
}

void BossBloodPrinceTaldaramAI::JustRespawned()
{
    DoCast(me, SPELL_FEIGN_DEATH);
    me->SetHealth(_spawnHealth);
}

void BossBloodPrinceTaldaramAI::SpellHit(Unit* , const SpellInfo* spell)
{
    if (spell->Id == SPELL_INVOCATION_OF_BLOOD_TALDARAM)
        DoAction(ACTION_CAST_INVOCATION);
}

void BossBloodPrinceTaldaramAI::JustSummoned(Creature* summon)
{
    summons.Summon(summon);
    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, -10.0f, true); // first try at distance
    if (!target)
        target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);     // too bad for you raiders, its going to boom

    if (summon->GetEntry() == NPC_BALL_OF_INFERNO_FLAME && target)
        Talk(EMOTE_TALDARAM_FLAME, target->GetGUID());

    if (target)
        summon->AI()->SetGUID(target->GetGUID());
}

void BossBloodPrinceTaldaramAI::DamageDealt(Unit* , uint32& damage, DamageEffectType damageType)
{
    if (damageType != SPELL_DIRECT_DAMAGE)
        return;

    if (damage > RAID_MODE<uint32>(23000, 25000, 23000, 25000))
        instance->SetData(DATA_ORB_WHISPERER_ACHIEVEMENT, uint32(false));
}

void BossBloodPrinceTaldaramAI::DamageTaken(Unit* attacker, uint32& damage)
{
    if (!_isEmpowered)
    {
        me->AddThreat(attacker, float(damage));
        damage = 0;
    }
}

void BossBloodPrinceTaldaramAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_TALDARAM_KILL);
}

void BossBloodPrinceTaldaramAI::DoAction(const int32 action)
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
            Talk(SAY_TALDARAM_INVOCATION);
            DoCast(me, SPELL_INVOCATION_VISUAL_ACTIVE, true);
            _isEmpowered = true;
            break;
        case ACTION_REMOVE_INVOCATION:
            me->SetHealth(_spawnHealth);
            me->RemoveAurasDueToSpell(SPELL_INVOCATION_VISUAL_ACTIVE);
            me->RemoveAurasDueToSpell(SPELL_INVOCATION_OF_BLOOD_TALDARAM);
            _isEmpowered = false;
            break;
        default:
            break;
    }
}

bool BossBloodPrinceTaldaramAI::CheckRoom()
{
    if (!CheckBoundary(me))
    {
        EnterEvadeMode();
        if (Creature* keleseth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_KELESETH_GUID)))
            keleseth->AI()->EnterEvadeMode();

        if (Creature* valanar = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_VALANAR_GUID)))
            valanar->AI()->EnterEvadeMode();

        return false;
    }

    return true;
}

void BossBloodPrinceTaldaramAI::UpdateAI(const uint32 diff)
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
                Talk(EMOTE_TALDARAM_BERSERK);
                break;
            case EVENT_GLITTERING_SPARKS:
                if (IsHeroic())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        DoCast(target, SPELL_GLITTERING_SPARKS);
                else
                    DoCastVictim(SPELL_GLITTERING_SPARKS);
                events.ScheduleEvent(EVENT_GLITTERING_SPARKS, urand(15000, 50000));
                break;
            case EVENT_CONJURE_FLAME:
                if (_isEmpowered)
                {
                    DoCast(me, SPELL_CONJURE_EMPOWERED_FLAME);
                    events.ScheduleEvent(EVENT_CONJURE_FLAME, urand(15000, 25000));
                }
                else
                {
                    DoCast(me, SPELL_CONJURE_FLAME);
                    events.ScheduleEvent(EVENT_CONJURE_FLAME, urand(20000, 30000));
                }
                Talk(SAY_TALDARAM_SPECIAL);
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}