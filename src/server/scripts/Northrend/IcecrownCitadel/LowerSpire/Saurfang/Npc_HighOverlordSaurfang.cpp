#include "Npc_HighOverlordSaurfang.h"


////////////////////////////
// High Overlord Saurfang //
////////////////////////////

bool HighOverlordSaurfang::OnGossipHello(Player* player, Creature* creature)
{
    InstanceScript* instance = creature->GetInstanceScript();
    if (instance && instance->GetBossState(DATA_DEATHBRINGER_SAURFANG) != DONE)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Wir sind bereit, Hochlord. Arthas muss fallen!", 631, -ACTION_START_EVENT);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    return true;
}

bool HighOverlordSaurfang::OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
{
    player->PlayerTalkClass->ClearMenus();
    player->CLOSE_GOSSIP_MENU();
    if (action == -ACTION_START_EVENT)
        creature->AI()->DoAction(ACTION_START_EVENT);

    return true;
}

///////////////////////////////
// High Overlord Saurfang AI //
///////////////////////////////

void HighOverlordSaurfangAI::DoAction(int32 const action)
{
    switch (action)
    {
    case ACTION_START_EVENT:
        {
            // Prevent crashes
            if (_events.GetPhaseMask() & PHASE_INTRO_MASK)
                return;

            GetCreatureListWithEntryInGrid(_guardList, me, NPC_SE_KOR_KRON_REAVER, 20.0f);
            _guardList.sort(Trinity::ObjectDistanceOrderPred(me));
            uint32 x = 1;
            for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++x, ++itr)
                (*itr)->AI()->SetData(0, x);

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            Talk(SAY_INTRO_HORDE_1);
            _events.SetPhase(PHASE_INTRO_H);
            _events.ScheduleEvent(EVENT_INTRO_HORDE_3, 18500, 0, PHASE_INTRO_H);
            _instance->HandleGameObject(_instance->GetData64(GO_SAURFANG_S_DOOR), true);
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
                deathbringer->AI()->DoAction(PHASE_INTRO_H);
            break;
        }
    case ACTION_START_OUTRO:
        {
            me->RemoveAurasDueToSpell(SPELL_GRIP_OF_AGONY);
            Talk(SAY_OUTRO_HORDE_1);
            _events.ScheduleEvent(EVENT_OUTRO_HORDE_2, 10000);   // say
            _events.ScheduleEvent(EVENT_OUTRO_HORDE_3, 18000);   // say
            _events.ScheduleEvent(EVENT_OUTRO_HORDE_4, 24000);   // cast
            _events.ScheduleEvent(EVENT_OUTRO_HORDE_5, 30000);   // move
            me->SetDisableGravity(false);
            me->GetMotionMaster()->MoveFall();
            for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++itr)
                (*itr)->AI()->DoAction(ACTION_DESPAWN);
            break;
        }
    case ACTION_INTERRUPT_INTRO:
        {
            _events.Reset();
            for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++itr)
                (*itr)->AI()->DoAction(ACTION_DESPAWN);
            break;
        }
    default:
        break;
    }
}

void HighOverlordSaurfangAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_GRIP_OF_AGONY)
    {
        me->SetDisableGravity(true);
        me->GetMotionMaster()->MovePoint(POINT_CHOKE, chokePos[0]);
    }
}

void HighOverlordSaurfangAI::MovementInform(uint32 type, uint32 id)
{
    if (type == POINT_MOTION_TYPE)
    {
        switch (id)
        {
        case POINT_FIRST_STEP:
            me->SetWalk(false);
            Talk(SAY_INTRO_HORDE_3);
            _events.ScheduleEvent(EVENT_INTRO_HORDE_5, 15500, 0, PHASE_INTRO_H);
            _events.ScheduleEvent(EVENT_INTRO_HORDE_6, 29500, 0, PHASE_INTRO_H);
            _events.ScheduleEvent(EVENT_INTRO_HORDE_7, 43800, 0, PHASE_INTRO_H);
            _events.ScheduleEvent(EVENT_INTRO_HORDE_8, 47000, 0, PHASE_INTRO_H);
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
                deathbringer->AI()->DoAction(ACTION_CONTINUE_INTRO);
            break;
        case POINT_CORPSE:
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
            {
                deathbringer->CastSpell(me, SPELL_RIDE_VEHICLE, true);  // for the packet logs.
                deathbringer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                deathbringer->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DROWNED);
            }
            _events.ScheduleEvent(EVENT_OUTRO_HORDE_5, 1000);    // move
            _events.ScheduleEvent(EVENT_OUTRO_HORDE_6, 4000);    // say
            break;
        case POINT_FINAL:
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
                deathbringer->DespawnOrUnsummon();
            me->DespawnOrUnsummon();
            break;
        default:
            break;
        }
    }
    else if (type == WAYPOINT_MOTION_TYPE && id == POINT_EXIT)
    {
        std::list<Creature*> guards;
        GetCreatureListWithEntryInGrid(guards, me, NPC_KOR_KRON_GENERAL, 50.0f);
        for (std::list<Creature*>::iterator itr = guards.begin(); itr != guards.end(); ++itr)
            (*itr)->DespawnOrUnsummon();
        me->DespawnOrUnsummon();
    }
}

void HighOverlordSaurfangAI::UpdateAI(uint32 const diff)
{
    _events.Update(diff);
    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_INTRO_HORDE_3:
            me->SetWalk(true);
            me->GetMotionMaster()->MovePoint(POINT_FIRST_STEP, firstStepPos.GetPositionX(), firstStepPos.GetPositionY(), firstStepPos.GetPositionZ());
            break;
        case EVENT_INTRO_HORDE_5:
            Talk(SAY_INTRO_HORDE_5);
            break;
        case EVENT_INTRO_HORDE_6:
            Talk(SAY_INTRO_HORDE_6);
            break;
        case EVENT_INTRO_HORDE_7:
            Talk(SAY_INTRO_HORDE_7);
            break;
        case EVENT_INTRO_HORDE_8:
            Talk(SAY_INTRO_HORDE_8);
            for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++itr)
                (*itr)->AI()->DoAction(ACTION_CHARGE);
            me->GetMotionMaster()->MoveCharge(chargePos[0].GetPositionX(), chargePos[0].GetPositionY(), chargePos[0].GetPositionZ(), 8.5f, POINT_CHARGE);
            break;
        case EVENT_OUTRO_HORDE_2:   // say
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
                me->SetFacingToObject(deathbringer);
            Talk(SAY_OUTRO_HORDE_2);
            break;
        case EVENT_OUTRO_HORDE_3:   // say
            Talk(SAY_OUTRO_HORDE_3);
            break;
        case EVENT_OUTRO_HORDE_4:   // move
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
            {
                float x, y, z;
                deathbringer->GetClosePoint(x, y, z, deathbringer->GetObjectSize());
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(POINT_CORPSE, x, y, z);
            }
            break;
        case EVENT_OUTRO_HORDE_5:   // move
            me->GetMotionMaster()->MovePoint(POINT_FINAL, finalPos);
            break;
        case EVENT_OUTRO_HORDE_6:   // say
            Talk(SAY_OUTRO_HORDE_4);
            break;
        }
    }
}
