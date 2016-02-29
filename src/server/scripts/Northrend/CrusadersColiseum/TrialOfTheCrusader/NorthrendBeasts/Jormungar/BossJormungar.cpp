#include "BossJormungar.h"

#define TIME_BURROW 52000
#define TIME_UNBURROW 8000

void BossJormungarAI::StartUp()
{
    me->RemoveFlag(UNIT_FIELD_FLAGS,  UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);

    if (me->GetEntry() == NPC_DREADSCALE)
        SetMoving();
    else if (me->GetEntry() == NPC_ACIDMAW)
    {
        DoScriptText(EMOTE_UNBURROW, me);
        me->RemoveAurasDueToSpell(SPELL_BURROW);
        me->CastSpell(me, SPELL_UNBURROW);

        SetRooted();
    }

    m_Transit = false;
    me->SetReactState(REACT_AGGRESSIVE);
    DoZoneInCombat();
}

void BossJormungarAI::HandleEvent(uint32 eventID)
{
    switch (eventID)
    {
        case EVENT_SPEW:
        {
            DoCastAOE(spellId[SPELL_SPEW]);
            m_EventMap.ScheduleEvent(EVENT_SPEW, urand(20000, 25000));
            break;
        }
        case EVENT_BITE:
            DoCastVictim(spellId[SPELL_BITE]);
            m_EventMap.ScheduleEvent(EVENT_BITE, urand(8000, 10000));
            break;
        case EVENT_SLIME_POOL:
            DoCast(SPELL_SLIME_POOL);
            m_EventMap.ScheduleEvent(EVENT_SLIME_POOL, 15000);
            break;
        case EVENT_SPRAY:
        {
            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true);
        
            if (target)
                DoCast(target, spellId[SPELL_SPRAY]);
        
            m_EventMap.ScheduleEvent(EVENT_SPRAY, urand(10000, 20000));
            break;
        }
        case EVENT_SWEEP:
            DoCast(SPELL_SWEEP);
            m_EventMap.ScheduleEvent(EVENT_SWEEP, urand(8000, 10000));
            break;
        case EVENT_BURROW:
        {
            DoScriptText(EMOTE_BURROW, me);
            me->CastSpell(me, SPELL_BURROW);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE| UNIT_FLAG_NOT_SELECTABLE);

            m_Transit = true;

            float x = TrialCrusaderLoc[0].m_positionX - me->GetPositionX();
            float y = TrialCrusaderLoc[0].m_positionY - me->GetPositionY();

            float delta = sqrt(x * x + y * y);

            delta = (frand(20.0f, 45.0f) + delta) / delta;

            x = x * delta + me->GetPositionX();
            y = y * delta + me->GetPositionY();

            me->GetMotionMaster()->MovePoint(999, x, y, TrialCrusaderLoc[0].m_positionZ);

            m_EventMap.ScheduleEvent(EVENT_UNBURROW, TIME_UNBURROW); 
            break;
        }
        case EVENT_UNBURROW:
        {
            DoScriptText(EMOTE_UNBURROW, me);
            me->RemoveAurasDueToSpell(SPELL_BURROW);
            me->CastSpell(me, SPELL_UNBURROW);

            if (m_Rooted)
                SetMoving();
            else
                SetRooted();

            m_Transit = false;
            m_EventMap.ScheduleEvent(EVENT_BURROW, TIME_BURROW);
            break;
        }
    }
}

void BossJormungarAI::Enrage()
{
    m_EventMap.CancelEvent(EVENT_UNBURROW);
    if (m_Transit)
    {
        DoScriptText(EMOTE_UNBURROW, me);
        me->RemoveAurasDueToSpell(SPELL_BURROW);
        me->CastSpell(me, SPELL_UNBURROW);

        SetMoving();
        m_Transit = false;
    }
    else if (m_Rooted)
        SetMoving();

    m_EventMap.CancelEvent(EVENT_BURROW);

    me->AddAura(SPELL_ENRAGE, me);
    DoScriptText(EMOTE_ENRAGE, me);
}

void BossJormungarAI::SetMoving()
{
    m_EventMap.Reset();
    m_EventMap.ScheduleEvent(EVENT_SPEW, urand(20000, 25000));
    m_EventMap.ScheduleEvent(EVENT_BITE, urand(8000, 10000));
    m_EventMap.ScheduleEvent(EVENT_SLIME_POOL, 15000);
    m_EventMap.ScheduleEvent(EVENT_BURROW, TIME_BURROW);

    m_Moving = true;
    m_Rooted = false;
    me->SetDisplayId(modelId[MODEL_MOVING]);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    SetCombatMovement(true);

    if (UpdateVictim())
        me->GetMotionMaster()->MoveChase(me->getVictim());
}

void BossJormungarAI::SetRooted()
{
    m_EventMap.Reset();
    m_EventMap.ScheduleEvent(EVENT_SPRAY, urand(10000, 20000));
    m_EventMap.ScheduleEvent(EVENT_SWEEP, urand(8000, 10000));
    m_EventMap.ScheduleEvent(EVENT_BURROW, TIME_BURROW);

    m_Moving = false;
    m_Rooted = true;
    me->SetDisplayId(modelId[MODEL_ROOTED]);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    SetCombatMovement(false);

    me->GetMotionMaster()->MoveIdle();
}

void AddBossAcidmaw();
void AddBossDreadscale();
void AddNpcSlimepool();

void AddEncounterJormungar()
{
    AddBossAcidmaw();
    AddBossDreadscale();
    AddNpcSlimepool();
}
