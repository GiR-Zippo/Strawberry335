/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 10.10.2014
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

#include "Encounter_FalricMarwynEvent.h"

using namespace ENCOUNTER_FALRIC_MARWYN_EVENT;


void Encounter_FalricMarwynEvent::OnFail(bool handleReal)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::OnFail()");

    pInstance->DoUpdateWorldState(WORLD_STATE_HOR, 0);
    pInstance->DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, 0);

    switch (GetState())
    {
        case ENCOUNTER_STATE_INTRO:
        {
            if (pInstance->GetBossState(DATA_FALRIC_EVENT) == DONE)
                break;

            if (Creature* pCreatureUther = pInstanceMap->GetCreature(utherGUID))
                pCreatureUther->DespawnOrUnsummon();
            if (Creature* pCreatureLK = pInstanceMap->GetCreature(lichKingGUID))
                pCreatureLK->DespawnOrUnsummon();

            resetBoss(BOSS_INDEX_FALRIC);
            resetBoss(BOSS_INDEX_MARWYN);

            if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_JAINA_OR_SYLVANAS)))
            {
                pCreature->DisappearAndDie();
                pCreature->Relocate(pCreature->GetHomePosition());
                pCreature->Respawn();

                if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
                    pCreature->UpdateEntry(NPC_SYLVANAS, HORDE);

                pCreature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FROSTWORN_DOOR)))
                pGate->SetGoState(GO_STATE_READY);

            if (GameObject* pFrostmourne = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FROSTMOURNE)))
                pFrostmourne->SetPhaseMask(1, true);

            pInstance->SetBossState(DATA_INTRO_EVENT, FAIL);
        }break;

        case ENCOUNTER_STATE_PRE_ENCOUNTER:
        case ENCOUNTER_STATE_ENCOUNTER:
        {
            switch (eventMap.GetPhaseMask())
            {
                case ENCOUNTER_PHASE_FALRIC:
                {
                    if (!handleReal)
                        pInstance->DoSendNotifyToInstance(BOSS_REQUIREMENTS, "'Intro Event'");

                    resetBoss(BOSS_INDEX_FALRIC);
                    pInstance->SetBossState(DATA_FALRIC_EVENT, FAIL);
                } // No break

                case ENCOUNTER_PHASE_MARWYN:
                {
                    resetBoss(BOSS_INDEX_MARWYN);

                    if (eventMap.GetPhaseMask() == ENCOUNTER_PHASE_MARWYN)
                    {
                        if (!handleReal)
                            pInstance->DoSendNotifyToInstance(BOSS_REQUIREMENTS, "'Falric'");

                        pInstance->SetBossState(DATA_MARWYN_EVENT, FAIL);
                    }
                }break;
            }
        }
    }

    if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FRONT_DOOR)))
        pGate->SetGoState(GO_STATE_ACTIVE);

    pInstance->DoAction(INSTANCE_ACTION_ENCOUNTER_DONE);
}

void Encounter_FalricMarwynEvent::onDataChanged(uint32 type)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::onDataChanged(%u)", type);

    uint32 data = 0;
    if (!GetData(type, data))
        ASSERT(false);

    switch (type)
    {
        case ENCOUNTER_DATA_FENRIC:
            pInstance->SetBossState(DATA_MARWYN_EVENT, IN_PROGRESS);
            startNextPhase(ENCOUNTER_PHASE_MARWYN, ENCOUNTER_EVENT_WAIT_BOSS);
            break;
    }
}

void Encounter_FalricMarwynEvent::onStart()
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::onStart()");

    utherGUID = NULL;
    lichKingGUID = NULL;

    for (uint8 i = 0; i < 2; ++i)
        for (uint8 k = 0; k < ENCOUNTER_WAVE_NPC_COUNT; ++k)
            summonSet[i][k].clear();

    if (eventMap.IsEventActive(ENCOUNTER_EVENT_RESTART_WAVES))
    {
        if (!pInstance->CheckRequiredBosses(DATA_FALRIC_EVENT) && !pInstance->CheckRequiredBosses(DATA_MARWYN_EVENT))
        {
            sLog->outDebug(LOG_FILTER_TSCR, "CheckRequiredBosses => Failed");
            OnFail(false);
            return;
        }

        if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FRONT_DOOR)))
            pGate->SetGoState(GO_STATE_READY);
    }
    else
        pInstance->SetBossState(DATA_INTRO_EVENT, IN_PROGRESS);
}

void Encounter_FalricMarwynEvent::executeIntro(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::executeIntro(%u)", eventID);

    if (eventMap.GetPhaseMask() & ENCOUNTER_PHASEMASK_INTRO_ALLIANCE)
        executeIntroAlliance(eventID);
    else if (eventMap.GetPhaseMask() & ENCOUNTER_PHASEMASK_INTRO_HORDE)
        executeIntroHorde(eventID);
    else if (eventMap.GetPhaseMask() & ENCOUNTER_PHASEMASK_INTRO_LICH_KING)
        executeIntroLichKing(eventID);
    else
        executeIntroCommon(eventID);
}

void Encounter_FalricMarwynEvent::executeIntroCommon(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::executeIntroCommon(%u)", eventID);

    GET_CREATURE_INTRO(Creature* pIntro);
    ASSERT(pIntro != NULL);

    switch (eventID)
    {
        case 1:
        {
            pIntro->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
            pIntro->GetMotionMaster()->MovePoint(0, MoveDoorPos);

            if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                eventMap.ScheduleEvent(2, 100);
            else
                eventMap.ScheduleEvent(4, 100);
        }break;

        case 2:
        {
            DoScriptText(SAY_JAINA_INTRO_1, pIntro);
            eventMap.ScheduleEvent(3, 9 * IN_MILLISECONDS);
        }break;

        case 3:
        {
            DoScriptText(SAY_JAINA_INTRO_2, pIntro);
            eventMap.ScheduleEvent(4, 7 * IN_MILLISECONDS);
        }break;

        case 4:
        {
            pIntro->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
            pIntro->GetMotionMaster()->MovePoint(0, MoveThronePos);

            eventMap.Reset();

            if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                eventMap.SetPhase(ENCOUNTER_PHASE_INTRO_ALLIANCE);
            else
                eventMap.SetPhase(ENCOUNTER_PHASE_INTRO_HORDE);

            eventMap.ScheduleEvent(1, 100);
        }break;
    }
}

void Encounter_FalricMarwynEvent::executeIntroAlliance(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::executeIntroAlliance(%u)", eventID);

    GET_CREATURE_INTRO(Creature* pIntro);
    ASSERT(pIntro != NULL && pIntro->AI() != NULL);

    switch (eventID)
    {
        case 1:
        {
            DoScriptText(SAY_JAINA_INTRO_3, pIntro);
            eventMap.ScheduleEvent(2, 5 * IN_MILLISECONDS);
        }break;

        case 2:
        {
            DoScriptText(SAY_JAINA_INTRO_4, pIntro);
            eventMap.ScheduleEvent(3, 10 * IN_MILLISECONDS);
        }break;

        case 3:
        {
            pIntro->AI()->DoCast(pIntro, SPELL_CAST_VISUAL);
            pIntro->CastSpell(pIntro, SPELL_FROSTMOURNE_SOUNDS, true);
            pInstance->HandleGameObject(pInstance->GetData64(GO_FROSTMOURNE), true);

            eventMap.ScheduleEvent(4, 10 * IN_MILLISECONDS);
        }break;

        case 4: // Spawn UTHER during speach 2
        {
            if (Creature* pUther = pInstanceMap->SummonCreatureOnMap(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN, NULL))
            {
                pUther->GetMotionMaster()->MoveIdle();
                pUther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
                utherGUID = pUther->GetGUID();
                pIntro->SetUInt64Value(UNIT_FIELD_TARGET, utherGUID);
                pUther->SetUInt64Value(UNIT_FIELD_TARGET, pIntro->GetGUID());
            }

            eventMap.ScheduleEvent(5, 2 * IN_MILLISECONDS);
        }break;

        case 5:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_A2_1, pUther);

            eventMap.ScheduleEvent(6, 3 * IN_MILLISECONDS);
        }break;

        case 6:
        {
            DoScriptText(SAY_JAINA_INTRO_5, pIntro);
            eventMap.ScheduleEvent(7, 6 * IN_MILLISECONDS);
        }break;

        case 7:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_A2_2, pUther);

            eventMap.ScheduleEvent(8, 7 * IN_MILLISECONDS);
        }break;

        case 8:
        {
            DoScriptText(SAY_JAINA_INTRO_6, pIntro);
            eventMap.ScheduleEvent(9, 2 * IN_MILLISECONDS);
        }break;

        case 9:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_A2_3, pUther);

            eventMap.ScheduleEvent(10, 9 * IN_MILLISECONDS);
        }break;

        case 10:
        {
            DoScriptText(SAY_JAINA_INTRO_7, pIntro);
            eventMap.ScheduleEvent(11, 5 * IN_MILLISECONDS);
        }break;

        case 11:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_A2_4, pUther);

            eventMap.ScheduleEvent(12, 11 * IN_MILLISECONDS);
        }break;

        case 12:
        {
            DoScriptText(SAY_JAINA_INTRO_8, pIntro);
            eventMap.ScheduleEvent(13, 4 * IN_MILLISECONDS);
        }break;

        case 13:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_A2_5, pUther);

            eventMap.ScheduleEvent(14, 13 * IN_MILLISECONDS);
        }break;

        case 14:
        {
            DoScriptText(SAY_JAINA_INTRO_9, pIntro);
            eventMap.ScheduleEvent(15, 10 * IN_MILLISECONDS);
        }break;

        case 15:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_A2_6, pUther);

            eventMap.ScheduleEvent(16, 22 * IN_MILLISECONDS);
        }break;

        case 16:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_A2_7, pUther);

            eventMap.ScheduleEvent(17, 4 * IN_MILLISECONDS);
        }break;

        case 17:
        {
            DoScriptText(SAY_JAINA_INTRO_10, pIntro);
            eventMap.ScheduleEvent(18, 2 * IN_MILLISECONDS);
        }break;

        case 18:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
            {
                pUther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                DoScriptText(SAY_UTHER_INTRO_A2_8, pUther);
            }
            eventMap.ScheduleEvent(19, 11 * IN_MILLISECONDS);
        }break;

        case 19:
        {
            DoScriptText(SAY_JAINA_INTRO_11, pIntro);

            eventMap.Reset();
            eventMap.SetPhase(ENCOUNTER_PHASE_INTRO_LICH_KING);
            eventMap.ScheduleEvent(1, 3 * IN_MILLISECONDS);
        }break;
    }
}

void Encounter_FalricMarwynEvent::executeIntroHorde(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::executeIntroHorde(%u)", eventID);

    GET_CREATURE_INTRO(Creature* pIntro);
    ASSERT(pIntro != NULL && pIntro->AI() != NULL);

    switch (eventID)
    {
        case 1:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_1, pIntro);
            eventMap.ScheduleEvent(2, 8 * IN_MILLISECONDS);
        }break;

        case 2:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_2, pIntro);
            eventMap.ScheduleEvent(3, 6 * IN_MILLISECONDS);
        }break;

        case 3:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_3, pIntro);
            pIntro->AI()->DoCast(pIntro, SPELL_CAST_VISUAL);
            pInstance->HandleGameObject(pInstance->GetData64(GO_FROSTMOURNE), true);

            pIntro->CastSpell(pIntro, SPELL_FROSTMOURNE_SOUNDS, true);
            eventMap.ScheduleEvent(4, 6 * IN_MILLISECONDS);
        }break;

        case 4: // Spawn UTHER during speach 2
        {
            if (Creature* pUther = pInstanceMap->SummonCreatureOnMap(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN, NULL))
            {
                pUther->GetMotionMaster()->MoveIdle();
                pUther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
                utherGUID = pUther->GetGUID();
                pIntro->SetUInt64Value(UNIT_FIELD_TARGET, utherGUID);
                pUther->SetUInt64Value(UNIT_FIELD_TARGET, pIntro->GetGUID());
            }
            eventMap.ScheduleEvent(5, 2 * IN_MILLISECONDS);
        }break;

        case 5:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_H2_1, pUther);

            eventMap.ScheduleEvent(6, 11 * IN_MILLISECONDS);
        }break;

        case 6:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_4, pIntro);
            eventMap.ScheduleEvent(7, 3 * IN_MILLISECONDS);
        }break;

        case 7:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_H2_2, pUther);

            eventMap.ScheduleEvent(8, 6 * IN_MILLISECONDS);
        }break;

        case 8:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_5, pIntro);
            eventMap.ScheduleEvent(9, 5 * IN_MILLISECONDS);
        }break;

        case 9:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_H2_3, pUther);

            eventMap.ScheduleEvent(10, 19 * IN_MILLISECONDS);
        }break;

        case 10:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_6, pIntro);
            eventMap.ScheduleEvent(11, 2 * IN_MILLISECONDS);
        }break;

        case 11:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_H2_4, pUther);

            eventMap.ScheduleEvent(12, 20 * IN_MILLISECONDS);
        }break;

        case 12:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_7, pIntro);
            eventMap.ScheduleEvent(13, 2 * IN_MILLISECONDS);
        }break;

        case 13:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
            {
                pUther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                DoScriptText(SAY_UTHER_INTRO_H2_5, pUther);
            }
            eventMap.ScheduleEvent(14, 12 * IN_MILLISECONDS);
        }break;

        case 14:
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
                DoScriptText(SAY_UTHER_INTRO_H2_6, pUther);

            eventMap.ScheduleEvent(15, 8 * IN_MILLISECONDS);
        }break;

        case 15:
        {
            DoScriptText(SAY_SYLVANAS_INTRO_8, pIntro);

            eventMap.Reset();
            eventMap.SetPhase(ENCOUNTER_PHASE_INTRO_LICH_KING);
            eventMap.ScheduleEvent(1, 3 * IN_MILLISECONDS);
        }break;
    }
}

void Encounter_FalricMarwynEvent::executeIntroLichKing(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::executeIntroLichKing(%u)", eventID);

    GET_CREATURE_INTRO(Creature* pIntro);
    ASSERT(pIntro != NULL);

    switch (eventID)
    {
        case 1: // Spawn LK in front of door, and make him move to the sword
        {
            if (Creature* pLichKing = pInstanceMap->SummonCreatureOnMap(NPC_LICH_KING_EVENT, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN, NULL))
            {
                pLichKing->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
                pLichKing->SetReactState(REACT_PASSIVE);
                lichKingGUID = pLichKing->GetGUID();

                if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FROSTWORN_DOOR)))
                    pGate->SetGoState(GO_STATE_ACTIVE);

                pIntro->SetUInt64Value(UNIT_FIELD_TARGET, lichKingGUID);
                pLichKing->SetUInt64Value(UNIT_FIELD_TARGET, pIntro->GetGUID());
            }

            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
            {
                pUther->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
                if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                    DoScriptText(SAY_UTHER_INTRO_A2_9, pUther);
                else
                    DoScriptText(SAY_UTHER_INTRO_H2_7, pUther);
            }

            eventMap.ScheduleEvent(2, 11 * IN_MILLISECONDS);
        }break;

        case 2:
        {
            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
                DoScriptText(SAY_LK_INTRO_1, pLichKing);
            if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FROSTWORN_DOOR)))
                pGate->SetGoState(GO_STATE_READY);

            eventMap.ScheduleEvent(3, 2 * IN_MILLISECONDS);
        }break;

        case 3: // The Lich King banishes Uther to the abyss.
        {
            if (Creature* pUther = pIntro->GetCreature(*pIntro, utherGUID))
            {
                pUther->CastSpell(pUther, SPELL_UTHER_DESPAWN, true);
                pUther->DisappearAndDie();
                utherGUID = NULL;
            }
            eventMap.ScheduleEvent(4, 5 * IN_MILLISECONDS);
        }break;

        case ENCOUNTER_EVENT_FAST_INTRO: // Spawn LK in front of door, and make him move to the sword
        {
            if (Creature* pLichKing = pInstanceMap->SummonCreatureOnMap(NPC_LICH_KING_EVENT, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN, NULL))
            {
                pLichKing->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
                pLichKing->SetReactState(REACT_PASSIVE);
                lichKingGUID = pLichKing->GetGUID();

                pIntro->SetUInt64Value(UNIT_FIELD_TARGET, lichKingGUID);
                pLichKing->SetUInt64Value(UNIT_FIELD_TARGET, pIntro->GetGUID());

                if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FROSTWORN_DOOR)))
                    pGate->SetGoState(GO_STATE_ACTIVE);
            }

            pIntro->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
            pIntro->GetMotionMaster()->MovePoint(0, MoveThronePos);
            eventMap.ScheduleEvent(4, 20 * IN_MILLISECONDS);
        }break;

        case 4: // He steps forward and removes the runeblade from the heap of skulls.
        {
            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
                DoScriptText(SAY_LK_INTRO_2, pLichKing);
            if (GameObject *pFrostmourne = pIntro->FindNearestGameObject(GO_FROSTMOURNE, 11.0f))
                pFrostmourne->SetPhaseMask(0, true);

            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
            {
                pLichKing->CastSpell(pLichKing, SPELL_TAKE_FROSTMOURNE, true);
                pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_VISUAL, true);
            }

            pIntro->RemoveAllAuras();
            eventMap.ScheduleEvent(5, 10 * IN_MILLISECONDS);
        }break;

        case 5: // Summon Falric and Marwyn. Then go back to the door
        {
            if (Creature* pFalric = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_FALRIC)))
                eventSpawnBoss(pFalric);

            if (Creature* pMarwyn = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_MARWYN)))
                eventSpawnBoss(pMarwyn);

            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
                DoScriptText(SAY_LK_INTRO_3, pLichKing);

            eventMap.ScheduleEvent(6, 8 * IN_MILLISECONDS);
        }break;

        case 6:
        {
            if (Creature* pFalric = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_FALRIC)))
                DoScriptText(SAY_FALRIC_INTRO_1, pFalric);

            eventMap.ScheduleEvent(7, 2 * IN_MILLISECONDS);
        }break;

        case 7:
        {
            if (Creature* pMarwyn = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_MARWYN)))
                DoScriptText(SAY_MARWYN_INTRO_1, pMarwyn);

            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
            {
                if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FROSTWORN_DOOR)))
                    pGate->SetGoState(GO_STATE_ACTIVE);

                pLichKing->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);
            }

            eventMap.ScheduleEvent(8, 3 * IN_MILLISECONDS);
        }break;

        case 8: //Spawn the Army
        {
            if (Creature* pFalric = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_FALRIC)))
                DoScriptText(SAY_FALRIC_INTRO_2, pFalric);

            spawnWaves(BOSS_INDEX_FALRIC); // Falric waves
            spawnWaves(BOSS_INDEX_MARWYN); // Marwyn waves

            eventMap.ScheduleEvent(9, 5 * IN_MILLISECONDS);
        }break;

        case 9:
        {
            if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                DoScriptText(SAY_JAINA_INTRO_END, pIntro);
            else
                DoScriptText(SAY_SYLVANAS_INTRO_END, pIntro);

            pIntro->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);

            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
            {
                pLichKing->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);
            }

            eventMap.ScheduleEvent(10, 5 * IN_MILLISECONDS);
        }break;

        case 10:
        {
            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
                if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                    DoScriptText(SAY_LK_JAINA_INTRO_END, pLichKing);
                else
                    DoScriptText(SAY_LK_SYLVANAS_INTRO_END, pLichKing);

            eventMap.ScheduleEvent(11, 9 * IN_MILLISECONDS);
        }break;

        case 11:
        {
            if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FROSTWORN_DOOR)))
                pGate->SetGoState(GO_STATE_READY);

            pIntro->DisappearAndDie();
            if (Creature* pLichKing = pIntro->GetCreature(*pIntro, lichKingGUID))
                pLichKing->DisappearAndDie();

            pInstance->SetBossState(DATA_INTRO_EVENT, DONE);
            pInstance->SetBossState(DATA_FALRIC_EVENT, IN_PROGRESS);
        } // No break

        case ENCOUNTER_EVENT_RESTART_WAVES: // Start/Restart waves
        {
            startNext(1, 3 * IN_MILLISECONDS);

            if (pInstance->GetBossState(DATA_FALRIC_EVENT) == DONE)
            {
                if (Creature* pMarwyn = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_MARWYN)))
                    eventSpawnBoss(pMarwyn);

                spawnWaves(BOSS_INDEX_MARWYN); // Marwyn waves

                pInstance->SetBossState(DATA_MARWYN_EVENT, IN_PROGRESS);
                eventMap.SetPhase(ENCOUNTER_PHASE_MARWYN);

                pInstance->DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, 5);
            }
            else
            {
                if (pInstance->GetBossState(DATA_FALRIC_EVENT) != IN_PROGRESS)
                {
                    if (Creature* pFalric = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_FALRIC)))
                    {
                        eventSpawnBoss(pFalric);
                        DoScriptText(SAY_FALRIC_INTRO_2, pFalric);
                    }

                    if (Creature* pMarwyn = pIntro->GetCreature(*pIntro, pInstance->GetData64(NPC_MARWYN)))
                        eventSpawnBoss(pMarwyn);

                    spawnWaves(BOSS_INDEX_FALRIC); // Falric waves
                    spawnWaves(BOSS_INDEX_MARWYN); // Marwyn waves

                    pInstance->SetBossState(DATA_FALRIC_EVENT, IN_PROGRESS);
                }

                eventMap.SetPhase(ENCOUNTER_PHASE_FALRIC);
            }

            pInstance->DoUpdateWorldState(WORLD_STATE_HOR, 1);

            // Close door
            if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(GO_FRONT_DOOR)))
                pGate->SetGoState(GO_STATE_READY);
        }break;
    }
}

void Encounter_FalricMarwynEvent::executePreEncounter(uint32 eventID)
{
    switch (eventMap.GetPhaseMask())
    {
        case ENCOUNTER_PHASE_FALRIC:
            executePreEncounterFalric(eventID);
            break;

        case ENCOUNTER_PHASE_MARWYN:
            executePreEncounterMarwyn(eventID);
            break;
    }
}

void Encounter_FalricMarwynEvent::executePreEncounterFalric(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::executePreEncounterFalric(%u)", eventID);

    switch (eventID)
    {
        case 1: // Despawn killed and Activate new Wave
        case 2:
        case 3:
        case 4:
        {
            if (!pInstance->CheckRequiredBosses(DATA_FALRIC_EVENT))
            {
                sLog->outDebug(LOG_FILTER_TSCR, "CheckRequiredBosses => Failed");
                OnFail(false);
                return;
            }

            despawnWave(BOSS_INDEX_FALRIC);
            activateWave(BOSS_INDEX_FALRIC);

            pInstance->DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, eventID);
            eventMap.ScheduleEvent(eventID + 1, ENCOUNTER_TIMER_WAVE);
        }break;

        case 5: // Boss
        {
            despawnWave(BOSS_INDEX_FALRIC);
            activateBoss();

            pInstance->DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, (ENCOUNTER_WAVE_NPC_COUNT));

            // Part of Encounter finished
            startNext();
        }break;
    }
}

void Encounter_FalricMarwynEvent::executePreEncounterMarwyn(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::executePreEncounterMarwyn(%u)", eventID);

    switch (eventID)
    {
        case ENCOUNTER_EVENT_WAIT_BOSS:
        {
            eventMap.ScheduleEvent(1, ENCOUNTER_TIMER_BOSS);
        }break;

        case 1: // Despawn killed and Activate new Wave
        case 2:
        case 3:
        case 4:
        {
            if (!pInstance->CheckRequiredBosses(DATA_MARWYN_EVENT))
            {
                sLog->outDebug(LOG_FILTER_TSCR, "CheckRequiredBosses => Failed");
                OnFail(false);
                return;
            }

            despawnWave(BOSS_INDEX_MARWYN);
            activateWave(BOSS_INDEX_MARWYN);

            pInstance->DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, (eventID + ENCOUNTER_WAVE_NPC_COUNT));
            eventMap.ScheduleEvent(eventID + 1, ENCOUNTER_TIMER_WAVE);
        }break;

        case 5: // Boss
        {
            despawnWave(BOSS_INDEX_MARWYN);
            activateBoss();

            pInstance->DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, (ENCOUNTER_WAVE_NPC_COUNT* 2));

            // Encounter finished
            startNext();
        }break;
    }
}

void Encounter_FalricMarwynEvent::spawnWaves(uint8 bossID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::spawnWaves(%u)", bossID);

    int8 positionCounter = NULL;

    for (uint8 npcIndex = NULL; npcIndex < ENCOUNTER_WAVE_NPC_COUNT; ++npcIndex)
    {
        for (uint8 waveIndex = NULL; waveIndex < ENCOUNTER_WAVE_COUNT; ++waveIndex)
        {
            // "Random" position in wave
            positionCounter = npcIndex + waveIndex + bossID;
            if (positionCounter >= ENCOUNTER_WAVE_NPC_COUNT)
                positionCounter -= ENCOUNTER_WAVE_NPC_COUNT;

            Position spawnPosition;

            spawnPosition.m_positionX = EncounterWaveSpawnPosition.GetPositionX() + (cos(TO_RADIAN(EncounterWaveDegree[waveIndex][positionCounter])) * EncounterWaveRadius[bossID]);
            spawnPosition.m_positionY = EncounterWaveSpawnPosition.GetPositionY() + (sin(TO_RADIAN(EncounterWaveDegree[waveIndex][positionCounter])) * EncounterWaveRadius[bossID]);
            spawnPosition.m_positionZ = EncounterWaveSpawnPosition.GetPositionZ();
            spawnPosition.m_orientation = spawnPosition.GetAngle(EncounterWaveSpawnPosition.GetPositionX(), EncounterWaveSpawnPosition.GetPositionY());

            if (Creature* pCreature = pInstanceMap->SummonCreatureOnMap(EncounterWaveEntries[npcIndex], spawnPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
            {
                pCreature->setFaction(35);
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                pCreature->m_Events.AddEvent(new SpawnWaveEvent(pCreature), pCreature->m_Events.CalculateTime(500));
                summonSet[bossID][npcIndex].insert(pCreature->GetGUID());
            }
        }
    }
}

void Encounter_FalricMarwynEvent::despawnWave(uint8 bossID, bool force)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::despawnWave(%u)", bossID);

    for (uint8 npcIndex = NULL; npcIndex < ENCOUNTER_WAVE_NPC_COUNT; ++npcIndex)
    {
        if (summonSet[bossID][npcIndex].empty())
            continue;

        for (SummonSet::iterator itr = summonSet[bossID][npcIndex].begin(); itr != summonSet[bossID][npcIndex].end();)
        {
            if (Creature* pCreature = pInstanceMap->GetCreature((*itr)))
            {
                if (!pCreature->isAlive() || force)
                {
                    pCreature->DespawnOrUnsummon();
                    summonSet[bossID][npcIndex].erase(itr++);
                }
                else
                    ++itr;
            }
        }
    }
}

void Encounter_FalricMarwynEvent::activateWave(uint8 bossID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::activateWave(%u)", bossID);

    for (uint8 npcIndex = NULL; npcIndex < ENCOUNTER_WAVE_NPC_COUNT; ++npcIndex)
    {
        if (summonSet[bossID][npcIndex].empty())
            continue;

        for (uint8 maxNpcIndex = NULL; maxNpcIndex < ENCOUNTER_WAVE_NPC_COUNT; ++maxNpcIndex)
        {
            uint64 guid = Trinity::Containers::SelectRandomContainerElement(summonSet[bossID][npcIndex]);
            if (Creature* pCreature = pInstanceMap->GetCreature(guid))
            {
                if (pCreature->isAlive() && !pCreature->HasAura(SPELL_SPIRIT_ACTIVATE))
                {
                    pCreature->CastSpell(pCreature, SPELL_SPIRIT_ACTIVATE);
                    break;
                }
            }
        }
    }
}

void Encounter_FalricMarwynEvent::activateBoss()
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::activateBoss()");

    uint32 bossEntry = NULL;

    switch (eventMap.GetPhaseMask())
    {
        case ENCOUNTER_PHASE_FALRIC:
            bossEntry = NPC_FALRIC;
            break;

        case ENCOUNTER_PHASE_MARWYN:
            bossEntry = NPC_MARWYN;
            break;

        default:
            return;
    }

    if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(bossEntry)))
    {
        pCreature->setFaction(16);
        pCreature->SetReactState(REACT_AGGRESSIVE);
        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        pCreature->SetInCombatWithZone();
    }
}

void Encounter_FalricMarwynEvent::eventSpawnBoss(Creature* pCreature)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_FalricMarwynEvent::eventSpawnBoss(%u)", &pCreature);

    pCreature->CastSpell(pCreature, SPELL_BOSS_SPAWN_AURA, true);
    pCreature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
    pCreature->SetVisible(true);

    switch (pCreature->GetEntry())
    {
        case NPC_FALRIC:
            pCreature->GetMotionMaster()->MovePoint(1, FalricEventPos);
            break;

        case NPC_MARWYN:
            pCreature->GetMotionMaster()->MovePoint(1, MarwynEventPos);
            break;
    }
}

void Encounter_FalricMarwynEvent::resetBoss(uint8 bossId)
{
    if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(bossId == BOSS_INDEX_FALRIC ? NPC_FALRIC : NPC_MARWYN)))
    {
        pCreature->SetVisible(false);
        pCreature->DisappearAndDie();
        pCreature->Relocate(pCreature->GetHomePosition());
        pCreature->Respawn();
    }

    despawnWave(bossId, true);
}
