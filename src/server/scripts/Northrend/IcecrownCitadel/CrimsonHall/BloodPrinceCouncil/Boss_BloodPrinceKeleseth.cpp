#include "Boss_BloodPrinceKeleseth.h"

void BossBloodPrinceKelesethAI::InitializeAI()
{
    if (CreatureData const* data = sObjectMgr->GetCreatureData(me->GetDBTableGUIDLow()))
        if (data->curhealth)
            _spawnHealth = data->curhealth;

    if (!me->isDead())
        JustRespawned();

    me->SetReactState(REACT_DEFENSIVE);
}

void BossBloodPrinceKelesethAI::Reset()
{
    events.Reset();
    summons.DespawnAll();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    _isEmpowered = false;
    me->SetHealth(_spawnHealth);
    instance->SetData(DATA_ORB_WHISPERER_ACHIEVEMENT, uint32(true));
    me->SetReactState(REACT_DEFENSIVE);
}

void BossBloodPrinceKelesethAI::EnterCombat(Unit* )
{
    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_BLOOD_PRINCES_CONTROL)))
        DoZoneInCombat(controller);

    events.ScheduleEvent(EVENT_BERSERK, 600000);
    events.ScheduleEvent(EVENT_SHADOW_RESONANCE, urand(10000, 15000));
    events.ScheduleEvent(EVENT_SHADOW_LANCE, 2000);

    if (IsHeroic())
    {
        me->AddAura(SPELL_SHADOW_PRISON, me);
        DoCast(me, SPELL_SHADOW_PRISON_DUMMY);
    }
}

void BossBloodPrinceKelesethAI::JustDied(Unit* )
{
    events.Reset();
    summons.DespawnAll();

    Talk(SAY_KELESETH_DEATH);
    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
}

void BossBloodPrinceKelesethAI::JustReachedHome()
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

void BossBloodPrinceKelesethAI::JustRespawned()
{
    DoCast(me, SPELL_FEIGN_DEATH);
    me->SetHealth(_spawnHealth);
}

void BossBloodPrinceKelesethAI::SpellHit(Unit* , const SpellInfo* spell)
{
    if (spell->Id == SPELL_INVOCATION_OF_BLOOD_KELESETH)
        DoAction(ACTION_CAST_INVOCATION);
}

void BossBloodPrinceKelesethAI::JustSummoned(Creature* summon)
{
    summons.Summon(summon);
    /*  Temporary Unused Bullshit
    Position pos;
    me->GetPosition(&pos);
    float maxRange = me->GetDistance2d(summon);
    float angle = me->GetAngle(summon);
    me->MovePositionToFirstCollision(pos, maxRange, angle);
    summon->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
    */
    summon->ToTempSummon()->SetTempSummonType(TEMPSUMMON_CORPSE_DESPAWN);
}

void BossBloodPrinceKelesethAI::DamageDealt(Unit* , uint32& damage, DamageEffectType damageType)
{
    if (damageType != SPELL_DIRECT_DAMAGE)
        return;

    if (damage > RAID_MODE<uint32>(23000, 25000, 23000, 25000))
        instance->SetData(DATA_ORB_WHISPERER_ACHIEVEMENT, uint32(false));
}

void BossBloodPrinceKelesethAI::DamageTaken(Unit* attacker, uint32& damage)
{
    if (!_isEmpowered)
    {
        me->AddThreat(attacker, float(damage));
        damage = 0;
    }
}

void BossBloodPrinceKelesethAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KELESETH_KILL);
}

void BossBloodPrinceKelesethAI::DoAction(const int32 action)
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
            Talk(SAY_KELESETH_INVOCATION);
            DoCast(me, SPELL_INVOCATION_VISUAL_ACTIVE, true);
            _isEmpowered = true;
            break;
        case ACTION_REMOVE_INVOCATION:
            me->SetHealth(_spawnHealth);
            me->RemoveAurasDueToSpell(SPELL_INVOCATION_VISUAL_ACTIVE);
            me->RemoveAurasDueToSpell(SPELL_INVOCATION_OF_BLOOD_KELESETH);
            _isEmpowered = false;
            break;
        default:
            break;
    }
}

bool BossBloodPrinceKelesethAI::CheckRoom()
{
    if (!CheckBoundary(me))
    {
        EnterEvadeMode();
        if (Creature* taldaram = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_TALDARAM_GUID)))
            taldaram->AI()->EnterEvadeMode();

        if (Creature* valanar = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PRINCE_VALANAR_GUID)))
            valanar->AI()->EnterEvadeMode();

        return false;
    }

    return true;
}

void BossBloodPrinceKelesethAI::UpdateAI(const uint32 diff)
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
                Talk(EMOTE_KELESETH_BERSERK);
                break;
            case EVENT_SHADOW_RESONANCE:
                Talk(SAY_KELESETH_SPECIAL);
                if (Creature* trigger = me->FindNearestCreature(NPC_FLOATING_TRIGGER, 100.0f, true))
                {
                    trigger->CastSpell(trigger, SPELL_SHADOW_RESONANCE, true, NULL, NULL, me->GetGUID());
                }
                events.ScheduleEvent(EVENT_SHADOW_RESONANCE, urand(10000, 15000));
                break;
            case EVENT_SHADOW_LANCE:
                if (_isEmpowered)
                    DoCastVictim(SPELL_EMPOWERED_SHADOW_LANCE);
                else
                    DoCastVictim(SPELL_SHADOW_LANCE);
                events.ScheduleEvent(EVENT_SHADOW_LANCE, 2000);
                break;
            default:
                break;
        }
    }

    // does not melee
}