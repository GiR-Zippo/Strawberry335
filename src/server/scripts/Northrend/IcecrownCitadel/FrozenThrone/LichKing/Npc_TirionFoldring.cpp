#include "Npc_TirionFoldring.h"

void NpcTirionFordringAI::Reset()
{
    _events.Reset();
    if (_instance->GetBossState(DATA_THE_LICH_KING) == DONE)
        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
}

void NpcTirionFordringAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    switch (id)
    {
        case POINT_TIRION_INTRO:
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                theLichKing->AI()->DoAction(ACTION_START_ENCOUNTER);
            break;
        case POINT_TIRION_OUTRO_1:
            _events.ScheduleEvent(EVENT_OUTRO_JUMP, 1, 0, PHASE_OUTRO);
            break;
    }
}

void NpcTirionFordringAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ACTION_CONTINUE_INTRO:
            Talk(SAY_TIRION_INTRO_1);
            _events.ScheduleEvent(EVENT_INTRO_TALK_1, 34000, 0, PHASE_INTRO);
            break;
        case ACTION_OUTRO:
            _events.SetPhase(PHASE_OUTRO);
            _events.ScheduleEvent(EVENT_OUTRO_TALK_1, 7000, 0, PHASE_OUTRO);
            _events.ScheduleEvent(EVENT_OUTRO_BLESS, 18000, 0, PHASE_OUTRO);
            _events.ScheduleEvent(EVENT_OUTRO_REMOVE_ICE, 23000, 0, PHASE_OUTRO);
            _events.ScheduleEvent(EVENT_OUTRO_MOVE_1, 25000, 0, PHASE_OUTRO);
            break;
    }
}

void NpcTirionFordringAI::SpellHit(Unit* , const SpellInfo* spell)
{
    if (spell->Id == SPELL_ICE_LOCK)
        me->SetFacingTo(3.085098f);
    else if (spell->Id == SPELL_BROKEN_FROSTMOURNE_KNOCK)
        SetEquipmentSlots(true);    // remove glow on ashbringer
}

void NpcTirionFordringAI::sGossipSelect(Player* player, uint32 sender, uint32 action)
{
    if (me->GetCreatureTemplate()->GossipMenuId == sender && !action)
    {
        if (player->isGameMaster())
        {
            if (InstanceScript* instance = me->GetInstanceScript())
                instance->SetData(DATA_SKIP_CHECK_LICHKING, 1);
        }

        _events.SetPhase(PHASE_INTRO);
        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        me->SetWalk(true);
        me->GetMotionMaster()->MovePoint(POINT_TIRION_INTRO, TirionIntro);
    }
}

void NpcTirionFordringAI::JustReachedHome()
{
    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);

    if (_instance->GetBossState(DATA_THE_LICH_KING) == DONE)
        return;

    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
}

void NpcTirionFordringAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim() && !(_events.GetPhaseMask() & (PHASE_MASK_INTRO | PHASE_MASK_OUTRO)))
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_INTRO_TALK_1:
                Talk(SAY_TIRION_INTRO_2);
                _events.ScheduleEvent(EVENT_INTRO_EMOTE_1, 2000, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_CHARGE, 5000, 0, PHASE_INTRO);
                break;
            case EVENT_INTRO_EMOTE_1:
                me->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NO_SHEATHE);
                break;
            case EVENT_INTRO_CHARGE:
                me->SetWalk(false);
                me->GetMotionMaster()->MovePoint(POINT_TIRION_CHARGE, TirionCharge);
                break;
            case EVENT_OUTRO_TALK_1:
                Talk(SAY_TIRION_OUTRO_1);
                break;
            case EVENT_OUTRO_BLESS:
                DoCast(me, SPELL_LIGHTS_BLESSING);
                break;
            case EVENT_OUTRO_REMOVE_ICE:
                me->RemoveAurasDueToSpell(SPELL_ICE_LOCK);
                SetEquipmentSlots(false, EQUIP_ASHBRINGER_GLOWING);
                if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                {
                    me->SetFacingToObject(lichKing);
                    lichKing->AI()->DoAction(ACTION_PLAY_MUSIC);
                }
                break;
            case EVENT_OUTRO_MOVE_1:
                me->GetMotionMaster()->MovePoint(POINT_TIRION_OUTRO_1, OutroPosition1);
                break;
            case EVENT_OUTRO_JUMP:
                DoCastAOE(SPELL_JUMP);
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}