/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 02.10.2014
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

#include "Instance_HallsOfReflection.h"

using namespace INSTANCE_HALLSOFREFLECTION;


typedef Instance_HallsOfReflection::Instance_HallsOfReflection_InstanceMapScript HorInstanceScript;

void HorInstanceScript::Initialize()
{
    eventMap.Reset();

    dataMap.clear();
    data64Map.clear();

    tempSummonSet.clear();

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        encounter[i] = NOT_STARTED;

    dataMap[DATA_TEAM_IN_INSTANCE] = TEAM_NEUTRAL;
}

void HorInstanceScript::OnPlayerEnter(Player* pPlayer)
{
    if (!eventMap.IsEventActive(EVENT_WIPE_CHECK))
        eventMap.ScheduleEvent(EVENT_WIPE_CHECK, 10 * IN_MILLISECONDS);

    if (GET_DATA(DATA_TEAM_IN_INSTANCE) == TEAM_NEUTRAL)
        dataMap[DATA_TEAM_IN_INSTANCE] = pPlayer->GetTeam();
}

void HorInstanceScript::OnCreatureCreate(Creature* pCreature)
{
    if (GET_DATA(DATA_TEAM_IN_INSTANCE) == TEAM_NEUTRAL)
    {
        const Map::PlayerList& playerList = instance->GetPlayers();
        if (!playerList.isEmpty())
            if (Player* pPlayer = playerList.begin()->getSource())
                dataMap[DATA_TEAM_IN_INSTANCE] = pPlayer->GetTeam();
    }

    uint32 entry = pCreature->GetEntry();

    switch (entry)
    {
        case NPC_ARCHMAGE_KORELN:
            if (GET_DATA(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                pCreature->UpdateEntry(NPC_DARK_RANGER_LORALEN);
            break;

        case NPC_FROSTWORN_GENERAL:
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            break;

        case NPC_JAINA_OR_SYLVANAS:
            if (GET_DATA(DATA_TEAM_IN_INSTANCE) == HORDE)
                pCreature->UpdateEntry(NPC_SYLVANAS, HORDE);
            break;

        case NPC_JAINA_OR_SYLVANAS_O:
            if (GET_DATA(DATA_TEAM_IN_INSTANCE) == HORDE)
                pCreature->UpdateEntry(NPC_SYLVANAS_OUTRO, HORDE);
            pCreature->SetHealth(252000);
            break;

        case BOSS_LICH_KING:
            pCreature->SetHealth(20917000);
            break;

        case NPC_FALRIC:
        case NPC_MARWYN:
        case NPC_SYLVANAS:
        case NPC_SYLVANAS_OUTRO:
            break;

        default:
            return;
    }

    data64Map[entry] = pCreature->GetGUID();
}

void HorInstanceScript::OnGameObjectCreate(GameObject* pGameObject)
{
    uint32 entry = pGameObject->GetEntry();

    switch (entry)
    {
        case GO_FROSTMOURNE:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            HandleGameObject(0, false, pGameObject);
            break;

        case GO_FROSTMOURNE_ALTAR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            HandleGameObject(0, true, pGameObject);
            break;

        case GO_FRONT_DOOR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            openDoor(pGameObject->GetGUID());
            break;

        case GO_FROSTWORN_DOOR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);

            if (encounter[1] == DONE)
                openDoor(pGameObject->GetGUID());
            else
                closeDoor(pGameObject->GetGUID());
            break;

        case GO_ARTHAS_DOOR:
            if (encounter[2] == DONE)
                openDoor(pGameObject->GetGUID());
            break;

        case GO_CHEST_HORDE:
        case GO_CHEST_HORDE_H:
        case GO_CHEST_ALLIANCE:
        case GO_CHEST_ALLIANCE_H:
        case GO_SKYBREAKER:
        case GO_ORGRIM_HAMMER:
        case GO_PORTAL:
            pGameObject->SetPhaseMask(2, true);
            break;

        case GO_RUN_DOOR:
        case GO_ICE_WALL_1:
        case GO_ICE_WALL_2:
        case GO_ICE_WALL_3:
        case GO_ICE_WALL_4:
        case GO_CAVE:
            break;

        default:
            return;
    }

    data64Map[entry] = pGameObject->GetGUID();
}

void HorInstanceScript::SetData(uint32 type, uint32 data)
{
    switch (type)
    {
        case DATA_UPDATE_WAVE_COUNT:
        {
            INCREASE_DATA(DATA_WAVE_COUNT, 1);
            DoUpdateWorldState(WORLD_STATE_HOR, 1);
            DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, GET_DATA(DATA_WAVE_COUNT));
        }break;

        case DATA_BEGIN_WAVE_EVENT:
        {
            if (GetData(DATA_FALRIC_EVENT) != DONE)
            {
                closeDoor(GET_DATA64(GO_FRONT_DOOR));

                if (Creature* pFalric = instance->GetCreature(GET_DATA64(NPC_FALRIC)))
                    if (pFalric->AI())
                        pFalric->GetAI()->DoAction(ACTION_ENTER_COMBAT);
            }
            else if (GetData(DATA_FALRIC_EVENT) == DONE)
            {
                closeDoor(GET_DATA64(GO_FRONT_DOOR));

                if (Creature* pMarwyn = instance->GetCreature(GET_DATA64(NPC_MARWYN)))
                    if (pMarwyn->AI())
                        pMarwyn->GetAI()->DoAction(ACTION_ENTER_COMBAT_CONT);
            }
        }break;

        case DATA_MARWYN_EVENT:
        {
            switch (data)
            {
                case SPECIAL:
                {
                    if (Creature* pMarwyn = instance->GetCreature(GET_DATA64(NPC_MARWYN)))
                    {
                        spawnWave(pMarwyn);

                        if (pMarwyn->AI())
                            pMarwyn->GetAI()->DoAction(ACTION_ENTER_COMBAT);
                    }
                }break;

                case DONE:
                {
                    encounter[1] = data;
                    openDoor(GET_DATA64(GO_FROSTWORN_DOOR));
                    openDoor(GET_DATA64(GO_FRONT_DOOR));
                }break;
            }
        }break;

        case DATA_FROSWORN_EVENT:
        {
            encounter[2] = data;

            if (data == DONE)
            {
                openDoor(GET_DATA64(GO_ARTHAS_DOOR));
                instance->SummonCreature(BOSS_LICH_KING, OutroSpawns[0]);
                instance->SummonCreature(NPC_JAINA_OR_SYLVANAS_O, OutroSpawns[1]);
            }
        }break;

        case DATA_LICHKING_EVENT:
        {
            encounter[3] = data;

            switch (data)
            {
                case  IN_PROGRESS:
                {
                    openDoor(GET_DATA64(GO_RUN_DOOR));

                    if (instance->IsHeroic())
                        DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                }break;

                case FAIL:
                {
                    openDoor(GET_DATA64(GO_ICE_WALL_1));
                    openDoor(GET_DATA64(GO_ICE_WALL_2));
                    openDoor(GET_DATA64(GO_ICE_WALL_3));
                    openDoor(GET_DATA64(GO_ICE_WALL_4));

                    closeDoor(GET_DATA64(GO_RUN_DOOR));

                    if (Creature* pLichKing = instance->GetCreature(GET_DATA64(BOSS_LICH_KING)))
                        pLichKing->DespawnOrUnsummon(10000);
                    if (Creature* pLider = instance->GetCreature(GET_DATA64(NPC_JAINA_OR_SYLVANAS_O)))
                        pLider->DespawnOrUnsummon(10000);

                    DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                    DoCastSpellOnPlayers(5); // Kill all players

                    instance->SummonCreature(BOSS_LICH_KING, OutroSpawns[0]);
                    instance->SummonCreature(NPC_JAINA_OR_SYLVANAS_O, OutroSpawns[1]);
                }break;

                case DONE:
                {
                    if (GameObject *pChest = instance->GetGameObject(uiChest))
                        pChest->SetPhaseMask(1, true);
                    if (GameObject *pPortal = instance->GetGameObject(uiPortal))
                        pPortal->SetPhaseMask(1, true);

                    DoCompleteAchievement(ACHIEV_HALLS_OF_REFLECTION_N);
                    if (instance->IsHeroic())
                    {
                        DoCompleteAchievement(ACHIEV_HALLS_OF_REFLECTION_H);
                        DoCastSpellOnPlayers(SPELL_ACHIEV_CHECK);
                        DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                    }
                }break;
            }
        }break;

        case DATA_SUMMONS:
        {
            if (data == 3)
                uiSummons = 0;
            else if (data == 1)
                ++uiSummons;
            else if (data == 0)
                --uiSummons;

            data = NOT_STARTED;
        }break;

        case DATA_SPAWN_ADDS_F:
            if (Creature* pFalric = instance->GetCreature(GET_DATA64(NPC_FALRIC)))
                spawnWave(pFalric);
            break;

        case DATA_SPAWN_ADDS_M:
            if (Creature* pMarwyn = instance->GetCreature(GET_DATA64(NPC_MARWYN)))
                spawnWave(pMarwyn);
            break;

        case DATA_SET_ADDS_INCOMBAT:
            setWaveInComabat();
            break;

        case DATA_INTRO_EVENT:
            uiIntroDone = data;
            break;

        case DATA_FALRIC_EVENT:
            if (data == DONE)
                encounter[0] = data;
            break;

        case DATA_ICE_WALL_1:
            uiWall[0] = data;
            break;

        case DATA_ICE_WALL_2:
            uiWall[1] = data;
            break;

        case DATA_ICE_WALL_3:
            uiWall[2] = data;
            break;

        case DATA_ICE_WALL_4:
            uiWall[3] = data;
            break;

        case DATA_PHASE:
            uiDataPhase = data;
            break;
    }

    if (data == DONE)
        SaveToDB();
}

uint32 HorInstanceScript::GetData(uint32 identifier)
{
    switch (identifier)
    {
        case DATA_FALRIC_EVENT:
            return encounter[0];
        case DATA_MARWYN_EVENT:
            return encounter[1];
        case DATA_FROSWORN_EVENT:
            return encounter[2];
        case DATA_LICHKING_EVENT:
            return encounter[3];

        default:
        {
            if (dataMap.find(identifier) != dataMap.end())
                return dataMap.at(identifier);
            else
                return NULL;
        }break;
    }
}

uint64 HorInstanceScript::GetData64(uint32 identifier)
{
    if (data64Map.find(identifier) != data64Map.end())
        return data64Map.at(identifier);
    else
        return NULL;
}

std::string HorInstanceScript::GetSaveData()
{
    OUT_SAVE_INST_DATA;

    std::ostringstream saveStream;
    saveStream << "H R 1 " << encounter[0] << " " << encounter[1] << " "
                << encounter[2] << " " << encounter[3] << " " << GET_DATA(DATA_INTRO_EVENT);

    OUT_SAVE_INST_DATA_COMPLETE;

    return saveStream.str();
}

void HorInstanceScript::Load(const char* in)
{
    if (!in)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(in);

    char dataHead1, dataHead2;
    uint16 version;
    uint16 data0, data1, data2, data3, data4;

    std::istringstream loadStream(in);
    loadStream >> dataHead1 >> dataHead2 >> version >> data0 >> data1 >> data2 >> data3 >> data4;

    if (dataHead1 == 'H' && dataHead2 == 'R')
    {
        encounter[0] = data0;
        encounter[1] = data1;
        encounter[2] = data2;
        encounter[3] = data3;
        dataMap[DATA_INTRO_EVENT] = data4;

        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (encounter[i] == IN_PROGRESS)
                encounter[i] = NOT_STARTED;
    }
    else
        OUT_LOAD_INST_DATA_FAIL;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void HorInstanceScript::Update(uint32 diff)
{
    eventMap.Update(diff);

    if (eventMap.ExecuteEvent() == EVENT_WIPE_CHECK)
        doWipeCheck();
}

void HorInstanceScript::doWipeCheck()
{
    if (GetData(DATA_FALRIC_EVENT) == DONE
        && GetData(DATA_MARWYN_EVENT) == DONE)
        return;

    uint8 alivePlayers = 0;
    const Map::PlayerList& playerList = instance->GetPlayers();

    if (playerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
    {
        if (Player* pPlayer = i->getSource())
        {
            if (pPlayer->isGameMaster())
                continue;

            if (pPlayer->isAlive())
                ++alivePlayers;
        }
    }

    if (alivePlayers == 0)
    {
        despawnWave();


        if (GetData(DATA_FALRIC_EVENT) != DONE)
        {
            //Stop and reset Falric
            dataMap[DATA_WAVE_COUNT] = 5;
            openDoor(GET_DATA64(GO_FRONT_DOOR));

            if (Creature* pFalric = instance->GetCreature(GET_DATA64(NPC_FALRIC)))
                pFalric->SetVisible(false);

            if (Creature* pFalric = instance->GetCreature(GET_DATA64(NPC_FALRIC)))
                if (pFalric->AI())
                    pFalric->GetAI()->DoAction(ACTION_STOP_COMBAT);
        }
        else if (GetData(DATA_FALRIC_EVENT) == DONE && GetData(DATA_MARWYN_EVENT) != DONE)
        {
            //Stop and reset Marwyn
            dataMap[DATA_WAVE_COUNT] = 0;
            openDoor(GET_DATA64(GO_FRONT_DOOR));

            if (Creature* pMarwyn = instance->GetCreature(GET_DATA64(NPC_MARWYN)))
                if (pMarwyn->AI())
                    pMarwyn->GetAI()->DoAction(ACTION_STOP_COMBAT);
        }

        //Respawn the NPCs
        if (uiTeamInInstance == ALLIANCE)
        {
            if (Creature* pJaina = instance->GetCreature(uiJainaPart1))
            {
                pJaina->Relocate(5262.540527f, 1949.693726f, 707.695007f, 0.808736f);
                pJaina->DisappearAndDie();
                pJaina->Respawn();
                pJaina->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        }
        else if (uiTeamInInstance == HORDE)
        {
            if (Creature* pSylvanas = instance->GetCreature(uiSylvanasPart1))
            {
                pSylvanas->Relocate(5262.540527f, 1949.693726f, 707.695007f, 0.808736f);
                pSylvanas->DisappearAndDie();
                pSylvanas->Respawn();
                pSylvanas->UpdateEntry(NPC_SYLVANAS, HORDE);
                pSylvanas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        }
    }
    else
        eventMap.ScheduleEvent(EVENT_WIPE_CHECK, 3000);
}

void HorInstanceScript::spawnWave(Creature* pCreature)
{
    uint32 index;

    pCreature->SetVisible(true);

    TempSummon* tempSummon = NULL;
    // TODO: do composition at random. # of spawn also depends on uiWaveCount
    // As of now, it is just one of each.
    index = urand(0, ENCOUNTER_WAVE_MERCENARY - 1);
    tempSummon = pCreature->SummonCreature(NPC_WAVE_MERCENARY, MercenarySpawnPos[index], TEMPSUMMON_MANUAL_DESPAWN, 0);
    setFlags(tempSummon);

    index = urand(0, ENCOUNTER_WAVE_FOOTMAN - 1);
    tempSummon = pCreature->SummonCreature(NPC_WAVE_FOOTMAN, FootmenSpawnPos[index], TEMPSUMMON_MANUAL_DESPAWN, 0);
    setFlags(tempSummon);

    index = urand(0, ENCOUNTER_WAVE_RIFLEMAN - 1);
    tempSummon = pCreature->SummonCreature(NPC_WAVE_RIFLEMAN, RiflemanSpawnPos[index], TEMPSUMMON_MANUAL_DESPAWN, 0);
    setFlags(tempSummon);

    index = urand(0, ENCOUNTER_WAVE_PRIEST - 1);
    tempSummon = pCreature->SummonCreature(NPC_WAVE_PRIEST, PriestSpawnPos[index], TEMPSUMMON_MANUAL_DESPAWN, 0);
    setFlags(tempSummon);

    index = urand(0, ENCOUNTER_WAVE_MAGE - 1);
    tempSummon = pCreature->SummonCreature(NPC_WAVE_MAGE, MageSpawnPos[index], TEMPSUMMON_MANUAL_DESPAWN, 0);
    setFlags(tempSummon);
}

void HorInstanceScript::setFlags(TempSummon* tempSummon)
{
    tempSummon->setFaction(35);
    tempSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    tempSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    tempSummonSet.insert(tempSummon->GetGUID());
}

void HorInstanceScript::setWaveInComabat()
{
    for (TempSummonSet::const_iterator itr = tempSummonSet.begin(); itr != tempSummonSet.end(); itr++)
    {
        if (Creature* pCreature = instance->GetCreature((*itr)))
        {
            pCreature->setFaction(16);
            pCreature->SetInCombatWithZone();
            pCreature->SetReactState(REACT_AGGRESSIVE);
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }
    }
}

void HorInstanceScript::despawnWave()
{
    for (TempSummonSet::const_iterator itr = tempSummonSet.begin(); itr != tempSummonSet.end(); itr++)
    {
        if (Creature* pCreature = instance->GetCreature((*itr)))
            pCreature->DespawnOrUnsummon();
    }

    tempSummonSet.clear();
}

void HorInstanceScript::openDoor(uint64 guid)
{
    if (GameObject* pGameObject = instance->GetGameObject(guid))
        pGameObject->SetGoState(GO_STATE_ACTIVE);
}

void HorInstanceScript::closeDoor(uint64 guid)
{
    if (GameObject* pGameObject = instance->GetGameObject(guid))
        pGameObject->SetGoState(GO_STATE_READY);
}
