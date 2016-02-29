#include "Npc_MuradinBronzebeard.h"

/////////////////////////
// Muradin Bronzebeard //
/////////////////////////

bool MuradinBronzebeard::OnGossipHello(Player* player, Creature* creature)
{
    InstanceScript* instance = creature->GetInstanceScript();
    if (instance && instance->GetBossState(DATA_DEATHBRINGER_SAURFANG) != DONE)
    {
        player->ADD_GOSSIP_ITEM(0, "Lasst es beginnen...", 631, -ACTION_START_EVENT + 1);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    return true;
}

bool MuradinBronzebeard::OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
{
    player->PlayerTalkClass->ClearMenus();
    player->CLOSE_GOSSIP_MENU();
    if (action == -ACTION_START_EVENT + 1)
        creature->AI()->DoAction(ACTION_START_EVENT);

    return true;
}

////////////////////////////
// Muradin Bronzebeard AI //
////////////////////////////

void MuradinBronzebeardAI::DoAction(int32 const action)
{
    switch (action)
    {
    case ACTION_START_EVENT:
        {
            // Prevent crashes
            if (_events.GetPhaseMask() & PHASE_INTRO_MASK)
                return;

            _events.SetPhase(PHASE_INTRO_A);
            GetCreatureListWithEntryInGrid(_guardList, me, NPC_SE_SKYBREAKER_MARINE, 20.0f);
            _guardList.sort(Trinity::ObjectDistanceOrderPred(me));
            uint32 x = 1;
            for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++x, ++itr)
                (*itr)->AI()->SetData(0, x);

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            Talk(SAY_INTRO_ALLIANCE_1);
            _events.ScheduleEvent(EVENT_INTRO_ALLIANCE_4, 2500+17500+9500, 0, PHASE_INTRO_A);
            _instance->HandleGameObject(_instance->GetData64(GO_SAURFANG_S_DOOR), true);
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
                deathbringer->AI()->DoAction(PHASE_INTRO_A);
            break;
        }
    case ACTION_START_OUTRO:
        {
            me->RemoveAurasDueToSpell(SPELL_GRIP_OF_AGONY);
            Talk(SAY_OUTRO_ALLIANCE_1);
            me->SetDisableGravity(false);
            me->GetMotionMaster()->MoveFall();
            for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++itr)
                (*itr)->AI()->DoAction(ACTION_DESPAWN);

            // temp until outro fully done - to put deathbringer on respawn timer (until next reset)
            if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
                deathbringer->DespawnOrUnsummon(5000);
            break;
        }
    case ACTION_INTERRUPT_INTRO:
        _events.Reset();
        for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++itr)
            (*itr)->AI()->DoAction(ACTION_DESPAWN);
        break;
    }
}

void MuradinBronzebeardAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_GRIP_OF_AGONY)
    {
        me->SetDisableGravity(true);
        me->GetMotionMaster()->MovePoint(POINT_CHOKE, chokePos[0]);
    }
}

void MuradinBronzebeardAI::MovementInform(uint32 type, uint32 id)
{
    if (type == POINT_MOTION_TYPE && id == POINT_FIRST_STEP)
    {
        me->SetWalk(false);
        Talk(SAY_INTRO_ALLIANCE_4);
        _events.ScheduleEvent(EVENT_INTRO_ALLIANCE_5, 5000, 0, PHASE_INTRO_A);
        if (Creature* deathbringer = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_DEATHBRINGER_SAURFANG)))
            deathbringer->AI()->DoAction(ACTION_CONTINUE_INTRO);
    }
    else if (type == WAYPOINT_MOTION_TYPE && id == POINT_EXIT)
    {
        std::list<Creature*> guards;
        GetCreatureListWithEntryInGrid(guards, me, NPC_ALLIANCE_COMMANDER, 50.0f);
        for (std::list<Creature*>::iterator itr = guards.begin(); itr != guards.end(); ++itr)
            (*itr)->DespawnOrUnsummon();
        me->DespawnOrUnsummon();
    }
}

void MuradinBronzebeardAI::UpdateAI(uint32 const diff)
{
    _events.Update(diff);
    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_INTRO_ALLIANCE_4:
            me->SetWalk(true);
            me->GetMotionMaster()->MovePoint(POINT_FIRST_STEP, firstStepPos.GetPositionX(), firstStepPos.GetPositionY(), firstStepPos.GetPositionZ());
            break;
        case EVENT_INTRO_ALLIANCE_5:
            Talk(SAY_INTRO_ALLIANCE_5);
            for (std::list<Creature*>::iterator itr = _guardList.begin(); itr != _guardList.end(); ++itr)
                (*itr)->AI()->DoAction(ACTION_CHARGE);
            me->GetMotionMaster()->MoveCharge(chargePos[0].GetPositionX(), chargePos[0].GetPositionY(), chargePos[0].GetPositionZ(), 8.5f, POINT_CHARGE);
            break;
        }
    }
}
