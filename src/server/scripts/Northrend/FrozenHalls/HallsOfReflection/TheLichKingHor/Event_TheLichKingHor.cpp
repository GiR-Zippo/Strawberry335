/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 25.09.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#include "Event_TheLichKingHor.h"

using namespace EVENT_THELICHKINGHOR;


typedef Npc_JainaOrSylvanasHor_SecondPart   JainaOrSylvanasSecond;
typedef Npc_JainaOrSylvanasHor_SecondPart::Npc_JainaOrSylvanasHorAI_SecondPart  JainaOrSylvanasAISecond;

bool JainaOrSylvanasSecond::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    InstanceScript* pInstance = pCreature->GetInstanceScript();

    if (!pInstance)
        return false;

    if (pInstance->GetBossState(DATA_LICHKING_EVENT) == DONE)
        return false;

    uint32 menuId = Player::GetDefaultGossipMenuForSource(pCreature);
    uint32 textId = pPlayer->GetGossipTextId(pCreature);

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM_DB(menuId, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    pPlayer->SEND_GOSSIP_MENU(textId, pCreature->GetGUID());
    return true;
}

bool JainaOrSylvanasSecond::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    pPlayer->PlayerTalkClass->ClearMenus();
    pPlayer->CLOSE_GOSSIP_MENU();

    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
        {
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->SetUInt64Value(UNIT_FIELD_TARGET, NULL);

            if (InstanceScript* pInstance = pCreature->GetInstanceScript())
                pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_START_EVENT);
            return true;
        }

        default:
            return false;
    }
}

bool JainaOrSylvanasAISecond::CanAIAttack(const Unit* /*victim*/) const
{
    if (pInstance)
        if (pInstance->GetBossState(DATA_LICHKING_EVENT) == NOT_STARTED)
            return true;
    return false;
}

void JainaOrSylvanasAISecond::DoAction(const int32 action)
{
    switch (action)
    {
        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_START_COMBAT:
        {
            if (pInstance)
            {
                if (Creature* pCreature = me->FindNearestCreature(BOSS_LICH_KING, 25.0f))
                {
                    if (me->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
                        me->CastSpell(me, ENCOUNTER_THELICHKINGHOR::SPELL_ICE_BARRIER, false);
                    me->setRegeneratingHealth(false);

                    me->SetHealth(CalculatePctN(me->GetMaxHealth(), MISC_HEALTH_PCT_JAINA_OR_SYLVANAS));
                    pCreature->SetHealth(CalculatePctN(pCreature->GetMaxHealth(), MISC_HEALTH_PCT_LICH_KING));

                    me->setActive(true);
                    pCreature->setActive(true);

                    me->SetInCombatWith(pCreature);
                    pCreature->SetInCombatWith(me);
                    me->AddThreat(pCreature, 100000.f);
                    AttackStart(pCreature);
                }
            }
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_START_EVENT:
        {
            Start(false, true);
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_PREPARE_WALL:
        {
            eventMap.ScheduleEvent(EVENT_SPELL_SUMMON_ICE_WALL, 100);
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_NEXT_WALL:
        {
            me->InterruptNonMeleeSpells(false);
            eventMap.RemovePhase(PHASE_WALL);
            SetEscortPaused(false);
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_LICH_KING_WIN:
        {
            me->CastStop();

            if (pInstance)
                if (Creature* pCreature = me->GetCreature(*me, pInstance->GetData(BOSS_LICH_KING)))
                    me->SetFacingToObject(pCreature);

            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
        }break;
    }
}

void JainaOrSylvanasAISecond::Reset()
{
    if (!pInstance || pInstance->GetBossState(DATA_LICHKING_EVENT) == IN_PROGRESS)
        return;

    iceWallTargetGUID = NULL;
    captainGUID = NULL;
    chestID = NULL;

    eventMap.Reset();

    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

    if (pInstance->GetBossState(DATA_LICHKING_EVENT) == DONE)
        me->SetVisible(false);
}

void JainaOrSylvanasAISecond::WaypointReached(uint32 point)
{
    ASSERT(pInstance != NULL);

    switch (point)
    {
        case 3: // Prepare wall
            pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_PREPARE_WALL);
            break;

        case 4: // Attack wall
            attackWall();
            break;

        case 6: // Prepare wall
            pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_PREPARE_WALL);
            break;

        case 8: // Attack wall
            attackWall();
            break;

        case 9:
        {
            if (me->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
                DoScriptText(SAY_JAINA_ESCAPE_01, me);
            if (me->GetEntry() == NPC_SYLVANAS_OUTRO)
                DoScriptText(SAY_SYLVANA_ESCAPE_01, me);
        }break;

        case 11: // Prepare wall
            pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_PREPARE_WALL);
            break;

        case 12: // Attack wall
            attackWall();
            break;

        case 13:
        {
            if (me->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
                DoScriptText(SAY_JAINA_ESCAPE_02, me);
            if (me->GetEntry() == NPC_SYLVANAS_OUTRO)
                DoScriptText(SAY_SYLVANA_ESCAPE_02, me);
        }break;

        case 15: // Prepare wall
            pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_PREPARE_WALL);
            break;

        case 16: // Attack wall
            attackWall();
            break;

        case 19:
        {
            if (me->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
                DoScriptText(SAY_JAINA_TRAP, me);
            else if (me->GetEntry() == NPC_SYLVANAS_OUTRO)
                DoScriptText(SAY_SYLVANA_TRAP, me);
        }break;

        case 20: // Last wall
        {
            pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_LAST_WALL);
            SetEscortPaused(true);

            if (me->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
            else if (me->GetEntry() == NPC_SYLVANAS_OUTRO)
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
        }break;
    }
}

void JainaOrSylvanasAISecond::MoveInLineOfSight(Unit* pUnit)
{
    if (!pUnit || !pInstance)
        return;

    if (!pInstance->CheckRequiredBosses(DATA_LICHKING_EVENT))
        return;

    Player* pPlayer = pUnit->ToPlayer();
    if (!pPlayer)
        return;

    if (pPlayer->isGameMaster())
        return;

    if (pPlayer->GetTeam() == ALLIANCE && me->GetEntry() == NPC_SYLVANAS_OUTRO)
        return;
    else if (pPlayer->GetTeam() == HORDE && me->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
        return;

    if (me->IsWithinDistInMap(pUnit, 35.0f) && pInstance->GetBossState(DATA_LICHKING_EVENT) == NOT_STARTED)
        pInstance->SetData(DATA_CURRENT_ENCOUNTER, ENCOUNTER_VALUE_LICH_KING_EVENT);
}

void JainaOrSylvanasAISecond::MovementInform(uint32 type, uint32 point)
{
    npc_escortAI::MovementInform(type, point);

    if (!pInstance || type != POINT_MOTION_TYPE)
        return;

    if (point == ENCOUNTER_THELICHKINGHOR::ENCOUNTER_M_P_AFTER_INTRO)
    {
        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        me->SetFacingTo(ENCOUNTER_THELICHKINGHOR::JainaOrSylvanasRunPosition.GetOrientation());
        me->SetHomePosition(me->GetPosition());
        me->RemoveAllAuras();

        const Map::PlayerList& playerList = pInstance->instance->GetPlayers();
        if (!playerList.isEmpty())
            for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                if (itr->getSource() && !itr->getSource()->isGameMaster())
                    itr->getSource()->ClearInCombat();
    }
}

void JainaOrSylvanasAISecond::JustSummoned(GameObject* pObject)
{
    if (!pInstance || !pObject)
        return;

    if (pObject->GetEntry() == GO_ICE_WALL)
        pObject->SetObjectScale(IceWallScale[pInstance->GetData(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_DATA_WAVE_COUNT)]);
}

void JainaOrSylvanasAISecond::DamageTaken(Unit* pUnit, uint32& damage)
{
    if (!pInstance)
        return;

    if (pInstance->GetBossState(DATA_LICHKING_EVENT) == NOT_STARTED
        || pInstance->GetBossState(DATA_LICHKING_EVENT) == SPECIAL)
    {
        damage = NULL;
        return;
    }
}

void JainaOrSylvanasAISecond::EnterEvadeMode()
{
    if (!pInstance)
        return;

    if (pInstance->GetBossState(DATA_LICHKING_EVENT) == NOT_STARTED)
        ScriptedAI::EnterEvadeMode();
}

void JainaOrSylvanasAISecond::JustDied(Unit* /*killer*/)
{
    if (pInstance && pInstance->GetBossState(DATA_LICHKING_EVENT) == IN_PROGRESS)
        pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_LICH_KING_WIN);
}

void JainaOrSylvanasAISecond::executeEvent(uint32 eventID)
{
    switch (eventID)
    {
        case EVENT_SPELL_SUMMON_ICE_WALL:
        {
            DoCast(SPELL_SUMMON_ICE_WALL);
        }break;

        case EVENT_SPELL_DESTROY_ICE_WALL:
        {
            if (eventMap.GetPhaseMask() & PHASE_WALL)
            {
                DoCast(SPELL_DESTROY_ICE_WALL_03);
                eventMap.ScheduleEvent(EVENT_SPELL_DESTROY_ICE_WALL, 1 * IN_MILLISECONDS);
            }
        }break;
    }
}

void JainaOrSylvanasAISecond::UpdateEscortAI(const uint32 diff)
{
    if (UpdateVictim() && !eventMap.GetPhaseMask())
    {
        DoMeleeAttackIfReady();
        return;
    }

    if (!pInstance)
        return;

    eventMap.Update(diff);

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        executeEvent(eventID);
        return;
    }
}


void JainaOrSylvanasAISecond::attackWall()
{
    SetEscortPaused(true);

    pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_REACHED_WALL);

    if (me->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
        me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
    else
        eventMap.ScheduleEvent(EVENT_SPELL_DESTROY_ICE_WALL, 1 * IN_MILLISECONDS);

    eventMap.SetPhase(PHASE_WALL);
}
