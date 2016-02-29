/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 11.10.2014
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

#include "Encounter_TheLichKingHor.h"

using namespace ENCOUNTER_THELICHKINGHOR;


void Encounter_TheLichKingHor::OnFail(bool handleReal)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TheLichKingHor::OnFail()");

    if (!handleReal)
        pInstance->DoSendNotifyToInstance(BOSS_REQUIREMENTS, "'General der Frostanbeter'");

    Creature* pCreature[2];
    GET_CREATURE_OUTRO(pCreature[0]);
    GET_CREATURE_LK(pCreature[1]);

    switch (GetState())
    {
        case ENCOUNTER_STATE_ENCOUNTER:
        {
            if (pCreature[1] && pCreature[1]->AI())
                pCreature[1]->AI()->DoAction(ENCOUNTER_ACTION_DESPAWN_ADDS);

            if (Creature* pCreatureWall = pInstanceMap->GetCreature(iceWallTargetGUID))
                pCreatureWall->DespawnOrUnsummon();

            if (GameObject* pObject = pInstanceMap->GetGameObject(pInstance->GetData64(GO_ICE_WALL)))
                pObject->Delete();
        }// No break

        case ENCOUNTER_STATE_INTRO:
        {
            pInstance->SetBossState(DATA_LICHKING_EVENT, NOT_STARTED);
            pInstance->DoAction(INSTANCE_ACTION_RESET_EVENT);
        }break;

        default:
            return;
    }

    pInstance->DoAction(INSTANCE_ACTION_ENCOUNTER_DONE);
}

void Encounter_TheLichKingHor::DoAction(int32 action)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TheLichKingHor::DoAction(%d)", action);

    switch (action)
    {
        case ENCOUNTER_ACTION_START_EVENT:
        {
            if (!pInstance->CheckRequiredBosses(DATA_LICHKING_EVENT))
            {
                OnFail();
                return;
            }

            pInstance->SetBossState(DATA_LICHKING_EVENT, IN_PROGRESS);

            // Start encounter
            startNext();
        } // No break
        case ENCOUNTER_ACTION_REACHED_WALL:
        case ENCOUNTER_ACTION_LICH_KING_WIN:
        case ENCOUNTER_ACTION_PREPARE_WALL:
        {
            GET_CREATURE_OUTRO(Creature* pOutro);
            if (!pOutro)
                return;

            if (pOutro->AI())
                pOutro->AI()->DoAction(action);

            if (GET_CREATURE_LK(Creature* pLichKing))
                if (pLichKing->AI())
                    pLichKing->AI()->DoAction(action);

            if (action == ENCOUNTER_ACTION_PREPARE_WALL)
            {
                uint32 data = NULL;
                if (!GetData(ENCOUNTER_DATA_WAVE_COUNT, data))
                    return;

                if (Creature* pCreatureWall = pInstanceMap->SummonCreatureOnMap(NPC_ICE_WALL, IceWallTargetPosition[data], TEMPSUMMON_MANUAL_DESPAWN, NULL))
                    iceWallTargetGUID = pCreatureWall->GetGUID();

                DoScriptText(JainaOrSylvanaWallText[pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O ? 0 : 1][data], pOutro);
            }
        }break;

        case ENCOUNTER_ACTION_NEXT_WALL:
        {
            if (GET_CREATURE_OUTRO(Creature* pOutro))
                if (pOutro->AI())
                    pOutro->AI()->DoAction(action);

            if (GET_CREATURE_LK(Creature* pLichKing))
                if (pLichKing->AI())
                    pLichKing->AI()->DoAction(action);

            uint32 data = NULL;
            if (!GetData(ENCOUNTER_DATA_WAVE_COUNT, data))
                return;

            if (GameObject* pObject = pInstanceMap->GetGameObject(pInstance->GetData64(GO_ICE_WALL)))
            {
                pObject->SetGoState(GO_STATE_ACTIVE);

                if (Creature* pCreatureWall = pInstanceMap->GetCreature(iceWallTargetGUID))
                    pCreatureWall->m_Events.AddEvent(new WallDeleteDelayEvent(pCreatureWall, pObject->GetGUID()),
                    pCreatureWall->m_Events.CalculateTime(3 * IN_MILLISECONDS));

                iceWallTargetGUID = NULL;
            }

            IncreaseData(ENCOUNTER_DATA_WAVE_COUNT, 1);
            SetData(ENCOUNTER_DATA_ADDS_DEFEATED, NULL);
        }break;

        case ENCOUNTER_ACTION_LAST_WALL:
        {
            if (GET_CREATURE_LK(Creature* pLichKing))
                if (pLichKing->AI())
                    pLichKing->AI()->DoAction(action);
        }break;

        case ENCOUNTER_ACTION_START_OUTRO:
        {
            // Start outro
            startNext();
        }break;

        case INSTANCE_EVENT_GUNSHIP_FIRE:
        {
            GET_CREATURE_OUTRO(Creature* pOutro);
            if (!pOutro)
                return;

            uint32 captainEnry = pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE ? NPC_BARTLETT : NPC_KORM;
            if (Creature* pCreature = pInstanceMap->GetCreature(captainEnry))
            {
                Player* pPlayer = pOutro->FindNearestPlayer(50.0f);
                DoScriptText(captainEnry == NPC_BARTLETT ? SAY_JAINA_FIRE : SAY_SYLVANA_FIRE, pCreature, pPlayer ? pPlayer : NULL);
            }

            eventMap.ScheduleEvent(2, 5 * IN_MILLISECONDS);
        }break;

        case INSTANCE_EVENT_GUNSHIP_ARRIVAL:
        {
            GET_CREATURE_OUTRO(Creature* pOutro);
            if (!pOutro)
                return;

            pOutro->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
            pOutro->SetFacingTo(4.0f);

            if (Creature* pCaptain = pInstanceMap->GetCreature(pInstance->GetData64(pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O ? NPC_BARTLETT : NPC_KORM)))
                DoScriptText(pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O ? SAY_JAINA_FINAL_1 : SAY_SYLVANA_FINAL_1, pCaptain);

            eventMap.ScheduleEvent(4, 6 * IN_MILLISECONDS);
        }break;
    }
}

void Encounter_TheLichKingHor::onDataChanged(uint32 type)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TheLichKingHor::onDataChanged(%u)", type);

    uint32 data = NULL;
    if (!GetData(type, data))
        ASSERT(false);

    switch (type)
    {
        case ENCOUNTER_DATA_ADDS_DEFEATED:
        {
            uint32 dataWaves = NULL;
            if (GetData(ENCOUNTER_DATA_WAVE_COUNT, dataWaves))
                if (data >= ((ENCOUNTER_DATA_VALUE_ADDS + (ENCOUNTER_DATA_VALUE_CALC * dataWaves))))
                    DoAction(ENCOUNTER_ACTION_NEXT_WALL);
        }break;
    }
}

void Encounter_TheLichKingHor::onStart()
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TheLichKingHor::onStart()");

    // Init data
    SetData(ENCOUNTER_DATA_ADDS_DEFEATED, NULL);
    SetData(ENCOUNTER_DATA_WAVE_COUNT, NULL);

    iceWallTargetGUID = NULL;

    pInstance->SetBossState(DATA_LICHKING_EVENT, SPECIAL);
}

void Encounter_TheLichKingHor::executeIntro(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TheLichKingHor::executeIntro(%u)", eventID);

    GET_CREATURE_OUTRO(Creature* pOutro);
    GET_CREATURE_LK(Creature* pLichKing);
    ASSERT(pOutro != NULL && pOutro->AI() != NULL && pLichKing != NULL);

    switch (eventID)
    {
        case 1:
        {
            pOutro->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

            // Complete Quests
            if (pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
                pOutro->CastSpell(pOutro, SPELL_CREDIT_FINDING_JAINA); // Credit 36955
            else
                pOutro->CastSpell(pOutro, SPELL_CREDIT_FINDING_SYLVANAS); // Credit 37554

            eventMap.ScheduleEvent(2, 100);
        }break;

        case 2:
        {
            if (pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O)
                DoScriptText(SAY_LICH_KING_AGGRO_A, pLichKing);
            else
                DoScriptText(SAY_LICH_KING_AGGRO_H, pLichKing);

            eventMap.ScheduleEvent(3, 8 * IN_MILLISECONDS);
        }break;

        case 3:
        {
            if (pOutro->GetEntry() == NPC_SYLVANAS_OUTRO)
            {
                pOutro->GetMotionMaster()->MovePoint(0, pOutro->GetPositionX() - 7.5f, pOutro->GetPositionY(), pOutro->GetPositionZ());
                eventMap.ScheduleEvent(4, 5 * IN_MILLISECONDS);
            }
            else
                eventMap.ScheduleEvent(5, 10 * IN_MILLISECONDS);
        }break;

        case 4:
        {
            pOutro->CastSpell(pLichKing, SPELL_SYLVANA_STEP, false);
            eventMap.ScheduleEvent(5, 3 * IN_MILLISECONDS);
        }break;

        case 5:
        {
            pLichKing->AttackStop();
            pLichKing->GetMotionMaster()->MovementExpired();
            pLichKing->SetFacingToObject(pOutro);

            if (pOutro->GetEntry() == NPC_SYLVANAS_OUTRO)
                pOutro->AI()->DoCast(SPELL_SYLVANA_JUMP);

            eventMap.ScheduleEvent(6, 1.5 * IN_MILLISECONDS);
        }break;

        case 6:
        {
            pOutro->AttackStop();
            pOutro->SetFacingToObject(pLichKing);
            pLichKing->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            pOutro->CastSpell((Unit*)NULL, pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O ? ENCOUNTER_THELICHKINGHOR::SPELL_ICE_PRISON
                              : ENCOUNTER_THELICHKINGHOR::SPELL_DARK_ARROW, true);

            eventMap.ScheduleEvent(7, 1 * IN_MILLISECONDS);
        }break;

        case 7:
        {
            if (pOutro->GetEntry() == NPC_SYLVANAS_OUTRO)
                DoScriptText(SAY_SYLVANA_AGGRO, pOutro);
            else
                DoScriptText(SAY_JAINA_AGGRO, pOutro);

            eventMap.ScheduleEvent(8, 1.5 * IN_MILLISECONDS);
        }break;

        case 8:
        {
            pOutro->GetMotionMaster()->MovePoint(ENCOUNTER_M_P_AFTER_INTRO, JainaOrSylvanasRunPosition);
        }break;
    }
}

void Encounter_TheLichKingHor::executeOutro(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TheLichKingHor::executeOutro(%u)", eventID);

    GET_CREATURE_OUTRO(Creature* pOutro);
    ASSERT(pOutro != NULL);

    switch (eventID)
    {
        case 1:
        {
            if (GET_CREATURE_LK(Creature* pLichKing))
            {
                DoScriptText(SAY_LICH_KING_END_DUN, pLichKing);
                pLichKing->CastSpell(pOutro, SPELL_HARVEST_SOUL, false);
            }

            pInstance->DoAction(INSTANCE_ACTION_START_GUNSHIP);
        }break;

        case 2:
        {
            if (Creature* pCreatureCave = pInstanceMap->GetCreature(pInstance->GetData64(NPC_ICE_WALL)))
                pCreatureCave->DespawnOrUnsummon();
            if (GameObject* pObjectCave = pInstanceMap->GetGameObject(pInstance->GetData64(GO_CAVE)))
                pObjectCave->SetGoState(GO_STATE_READY);
            if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(BOSS_LICH_KING)))
                pCreature->DisappearAndDie();

            eventMap.ScheduleEvent(3, 3 * IN_MILLISECONDS);
        }break;

        case 3:
        {
            // Complete quest - Credit 38211
            pOutro->CastSpell(pOutro, SPELL_CREDIT_ESCAPING_ARTHAS);

            pInstance->SetBossState(DATA_LICHKING_EVENT, DONE);
            pInstance->DoAction(INSTANCE_ACTION_RESTART_GUNSHIP);
        }break;

        case 4:
        {
            DoScriptText(pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O ? SAY_JAINA_FINAL_2 : SAY_SYLVANA_FINAL_2, pOutro);
            eventMap.ScheduleEvent(5, 10 * IN_MILLISECONDS);
        }break;

        case 5:
        {
            DoScriptText(pOutro->GetEntry() == NPC_JAINA_OR_SYLVANAS_O ? SAY_JAINA_FINAL_3 : NULL, pOutro);
            eventMap.ScheduleEvent(6, 4 * IN_MILLISECONDS);
        }break;

        case 6:
        {
            if (GameObject* pObjectCave = pInstanceMap->GetGameObject(pInstance->GetData64(GO_CAVE)))
                pObjectCave->SetGoState(GO_STATE_ACTIVE);

            pOutro->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            pInstance->DoAction(INSTANCE_ACTION_ENCOUNTER_DONE);
        }break;
    }
}