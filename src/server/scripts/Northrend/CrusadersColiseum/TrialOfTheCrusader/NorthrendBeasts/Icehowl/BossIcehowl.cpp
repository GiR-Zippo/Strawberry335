#include "BossIcehowl.h"

enum IcehowlTimes
{
    SPELL_FEROCIOUS_BUTT                    = 66770,
    SPELL_MASSIVE_CRASH                     = 66683,
    SPELL_WHIRL                             = 67345,
    SPELL_ARCTIC_BREATH                     = 66689,
    SPELL_TRAMPLE                           = 66734,
    SPELL_FROTHING_RAGE                     = 66759,
    SPELL_STAGGERED_DAZE                    = 66758,
    SPELL_BERSERK                           = 26662,

    EMOTE_STARE                             = -1649020,
    EMOTE_CRASHED                           = -1649021,
    EMOTE_CHARGE                            = -1649022,
};

void BossIcehowlAI::MovementInform(uint32 type, uint32 id)
{
    if (type == POINT_MOTION_TYPE && id == MOVE_START)
    {
        if (m_Instance)
            m_Instance->SetData(GO_MAIN_GATE_DOOR, CLOSE);

        me->SetReactState(REACT_AGGRESSIVE);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE| UNIT_FLAG_NOT_SELECTABLE);
        me->SetInCombatWithZone();

        m_EventMap.ScheduleEvent(EVENT_WHIRL, TIME_WHIRL_INIT);
        m_EventMap.ScheduleEvent(EVENT_ARCTIC_BREATH, TIME_ARCTIC_BREATH_INIT);
        m_EventMap.ScheduleEvent(EVENT_FEROCIOUS_BUTT, TIME_FEROCIOUS_BUTT_INIT);
        m_EventMap.ScheduleEvent(EVENT_COMBO, TIME_COMBO_INIT);
        
        if (me->GetMap()->IsHeroic())
            m_EventMap.ScheduleEvent(EVENT_BERSERK, TIME_BERSERK);
    }
    else if (type == EFFECT_MOTION_TYPE && id == JUMP_MIDDLE)
        me->GetMotionMaster()->MovementExpired();
    else if (type == EFFECT_MOTION_TYPE && id == JUMP_BACK)
    {
        me->GetMotionMaster()->MovementExpired();
        m_EventMap.ScheduleEvent(EVENT_COMBO_CHARGE, TIME_COMBO_CHARGE);
    }
    else if (type == POINT_MOTION_TYPE && id == MOVE_CHARGE)
    {
        me->GetMotionMaster()->MovementExpired();
        m_EventMap.CancelEvent(EVENT_TRAMPLE);
        DoCastAOE(SPELL_TRAMPLE);
        m_EventMap.ScheduleEvent(EVENT_CRASHED_CHECK, 100);
    }
}

void BossIcehowlAI::SpellHitTarget(Unit* target, const SpellInfo* spell)
{
    if (!target || !spell)
        return;

    if (spell->Id != SPELL_TRAMPLE || target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (m_Trampled)
        return;

    m_Trampled = true;
    me->GetMotionMaster()->MovementExpired();
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    DoCast(me, SPELL_FROTHING_RAGE, true);
    m_EventMap.ScheduleEvent(EVENT_END_COMBO, TIME_END_COMBO_RAGE);
}

void BossIcehowlAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim())
        return;

    m_EventMap.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 EventId = m_EventMap.ExecuteEvent())
    {
        HandleEvent(EventId);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
    }

    if (!m_Combo)
        DoMeleeAttackIfReady();
}

void BossIcehowlAI::HandleEvent(uint32 eventId)
{
    switch (eventId)
    {
        case EVENT_WHIRL:
        {
            DoCastAOE(SPELL_WHIRL);
            m_EventMap.ScheduleEvent(EVENT_WHIRL, TIME_WHIRL);
            break;
        }
        case EVENT_ARCTIC_BREATH:
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                me->CastSpell(target, SPELL_ARCTIC_BREATH, true);

            m_EventMap.ScheduleEvent(EVENT_ARCTIC_BREATH, TIME_ARCTIC_BREATH);
            break;
        }
        case EVENT_FEROCIOUS_BUTT:
        {
            DoCastVictim(SPELL_FEROCIOUS_BUTT);
            break;
        }
        case EVENT_COMBO:
        {
            m_Combo = true;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            me->GetMotionMaster()->MovementExpired();
            me->GetMotionMaster()->Clear();
            SetCombatMovement(false);

            me->GetMotionMaster()->MoveJump(TrialCrusaderLoc[0].GetPositionX(), TrialCrusaderLoc[0].GetPositionY(), TrialCrusaderLoc[0].GetPositionZ(), 35.0f, 2.0f, JUMP_MIDDLE);

            m_EventMap.CancelEvent(EVENT_WHIRL);
            m_EventMap.CancelEvent(EVENT_ARCTIC_BREATH);

            m_EventMap.ScheduleEvent(EVENT_MASSIVE_CRASH, TIME_MASSIVE_CRASH);
            m_EventMap.ScheduleEvent(EVENT_SELECT_TARGET, TIME_SELECT_TARGET);
            m_EventMap.ScheduleEvent(EVENT_ROAR_STOP, TIME_ROAR_STOP);
            m_EventMap.ScheduleEvent(EVENT_JUMP_BACK, TIME_JUMP_BACK);
            m_EventMap.ScheduleEvent(EVENT_COMBO, TIME_COMBO);
            break;
        }
        case EVENT_MASSIVE_CRASH:
        {
            DoCast(SPELL_MASSIVE_CRASH);
            break;
        }
        case EVENT_SELECT_TARGET:
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
            {
                m_TargetX = target->GetPositionX();
                m_TargetY = target->GetPositionY();
                me->SetTarget(target->GetGUID());
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_ROAR);
                DoScriptText(EMOTE_STARE, me, target);
            }
            break;
        }
        case EVENT_ROAR_STOP:
        {
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
            break;
        }
        case EVENT_JUMP_BACK:
        {
            float destX = me->GetPositionX() * 1.8f - m_TargetX * 0.8f;
            float destY = me->GetPositionY() * 1.8f - m_TargetY * 0.8f;
            m_TargetX = m_TargetX * 0.9 + me->GetPositionX() * 0.1;
            m_TargetY = m_TargetY * 0.9 + me->GetPositionY() * 0.1;
            me->GetMotionMaster()->MoveJump(destX, destY, TrialCrusaderLoc[0].GetPositionZ(), 35.0f, 10.0f, JUMP_BACK);
            break;
        }
        case EVENT_COMBO_CHARGE:
        {
            m_Trampled = false;
            DoScriptText(EMOTE_CHARGE, me);
            me->GetMotionMaster()->MoveCharge(m_TargetX, m_TargetY, TrialCrusaderLoc[0].GetPositionZ(), 42.0f, MOVE_CHARGE);
            m_EventMap.ScheduleEvent(EVENT_TRAMPLE, TIME_TRAMPLE);   
        }
        case EVENT_TRAMPLE:
        {
            if (m_Trampled)
                return;

            if (Unit* eventTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 12.0f, true))
                DoCastAOE(SPELL_TRAMPLE);

            if (!m_Combo)
                m_EventMap.ScheduleEvent(EVENT_TRAMPLE, TIME_TRAMPLE);
            break;
        }
        case EVENT_CRASHED_CHECK:
        {
            if (m_Trampled)
                return;

            DoCast(me, SPELL_STAGGERED_DAZE);
            DoScriptText(EMOTE_CRASHED, me);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_EventMap.ScheduleEvent(EVENT_END_COMBO, TIME_END_COMBO);
            break;
        }
        case EVENT_END_COMBO:
        {
            m_Combo = false;
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            SetCombatMovement(true);

            me->GetMotionMaster()->MovementExpired();
            me->GetMotionMaster()->Clear();

            UpdateVictim();
            me->GetMotionMaster()->MoveChase(me->getVictim());

            m_EventMap.ScheduleEvent(EVENT_WHIRL, TIME_WHIRL_COMBO);
            m_EventMap.ScheduleEvent(EVENT_ARCTIC_BREATH, TIME_ARCTIC_BREATH_COMBO);
            m_EventMap.ScheduleEvent(EVENT_FEROCIOUS_BUTT, TIME_FEROCIOUS_BUTT_COMBO);
            break;
        }
        case EVENT_BERSERK:
        {
            DoCast(me, SPELL_BERSERK);
            break;
        }
    }
}

void AddEncounterIcehowl()
{
    new BossIcehowl();
}
