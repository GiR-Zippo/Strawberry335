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
#include "FalricMarwynEvent/Encounter_FalricMarwynEvent.h"
#include "TheLichKingHor/Encounter_TheLichKingHor.h"
#include "TransportMgr.h"

using namespace INSTANCE_HALLSOFREFLECTION;


typedef Instance_HallsOfReflection::Instance_HallsOfReflection_InstanceMapScript HorInstanceScript;

void HorInstanceScript::Initialize()
{
    dataMap.clear();
    data64Map.clear();
    cannonSet.clear();

    dataMap[DATA_TEAM_IN_INSTANCE] = TEAM_NEUTRAL;
    aliveCheckTimer = ALIVE_CHECK_TIMER;
    pCurrentEncounter = NULL;

    prepareEvent = false;
    delayedFail = false;

    SetBossNumber(MAX_ENCOUNTER);

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        SetBossState(i, NOT_STARTED);
}

bool HorInstanceScript::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (GetBossState(i) == IN_PROGRESS)
            return true;
    return false;
}

bool HorInstanceScript::SetBossState(uint32 bossId, EncounterState state)
{
    if (GetBossState(bossId) == state)
        return false;

    switch (bossId)
    {
        case DATA_FALRIC_EVENT:
        {
            if (state == DONE)
                if (pCurrentEncounter)
                    pCurrentEncounter->SetData(ENCOUNTER_FALRIC_MARWYN_EVENT::ENCOUNTER_DATA_FENRIC, 1);
        }break;

        case DATA_MARWYN_EVENT:
        {
            if (state == DONE)
            {
                HandleGameObject(GET_DATA64(GO_FROSTWORN_DOOR), true);
                HandleGameObject(GET_DATA64(GO_FRONT_DOOR), true);

                if (pCurrentEncounter)
                {
                    delete pCurrentEncounter;
                    pCurrentEncounter = NULL;

                    DoUpdateWorldState(WORLD_STATE_HOR, 0);
                }
            }
        }break;

        case DATA_FROSWORN_EVENT:
        {
            if (state == DONE)
            {
                HandleGameObject(GET_DATA64(GO_ARTHAS_DOOR), true);
                HandleGameObject(GET_DATA64(GO_RUN_DOOR), true);

                prepareLichKingEvent();
            }
        }break;

        case DATA_LICHKING_EVENT:
        {
            switch (state)
            {
                case IN_PROGRESS:
                {
                    if (instance->IsHeroic())
                        DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                }break;

                case DONE:
                {
                    if (pCurrentEncounter)
                    {
                        DoCompleteAchievement(ACHIEV_HALLS_OF_REFLECTION_N);

                        if (instance->IsHeroic())
                        {
                            DoCompleteAchievement(ACHIEV_HALLS_OF_REFLECTION_H);
                            DoCastSpellOnPlayers(SPELL_ACHIEV_CHECK);
                            DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                        }
                    }
                }break;
            }
        }break;
    }

    if (state == FAIL && pCurrentEncounter)
    {
        switch (bossId)
        {
            case DATA_FALRIC_EVENT:
            case DATA_MARWYN_EVENT:
                delayedFail = DELAYED_FAIL_NORMAL;
                break;

            case DATA_LICHKING_EVENT:
                delayedFail = DELAYED_FAIL_LICH_KING;
                break;
        }
    }

    return InstanceScript::SetBossState(bossId, state);
}

bool HorInstanceScript::CheckRequiredBosses(uint32 bossId, Player const* /*player*/) const
{
    if (GetBossState(bossId) == DONE)
        return false;

    bool state = true;

    // No breaks
    switch (bossId)
    {
        case DATA_LICHKING_EVENT:
            if (GetBossState(DATA_FROSWORN_EVENT) != DONE)
                state = false;

        case DATA_FROSWORN_EVENT:
            if (GetBossState(DATA_MARWYN_EVENT) != DONE)
                state = false;

        case DATA_MARWYN_EVENT:
            if (GetBossState(DATA_FALRIC_EVENT) != DONE)
                state = false;

        case DATA_FALRIC_EVENT:
            if (GetBossState(DATA_INTRO_EVENT) != DONE)
                state = false;

        case DATA_INTRO_EVENT:
            break;

        default:
            return false;
    }

    if (!state && pCurrentEncounter)
        pCurrentEncounter->OnFail(false);

    return state;
}

void HorInstanceScript::OnPlayerEnter(Player* pPlayer)
{
    if (GET_DATA(DATA_TEAM_IN_INSTANCE) == TEAM_NEUTRAL)
        dataMap[DATA_TEAM_IN_INSTANCE] = pPlayer->GetTeam();

    DoUpdateWorldState(WORLD_STATE_HOR, 0);
    DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, 0);
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
        {
            if (GET_DATA(DATA_TEAM_IN_INSTANCE) == HORDE)
                pCreature->UpdateEntry(NPC_SYLVANAS_OUTRO, HORDE);
        }break;

        case BOSS_LICH_KING:
        case NPC_FALRIC:
        case NPC_MARWYN:
        case NPC_SYLVANAS:
        case NPC_SYLVANAS_OUTRO:
        case NPC_BARTLETT:
        case NPC_KORM:
            break;

        default:
            return;
    }

	switch (entry)
	{
		case NPC_ARCHMAGE_KORELN:
		case NPC_DARK_RANGER_LORALEN:
		// case NPC_JAINA_OR_SYLVANAS:
		// case NPC_JAINA_OR_SYLVANAS_O:
		// case NPC_SYLVANAS:
		// case NPC_SYLVANAS_OUTRO:
		case NPC_BARTLETT:
		case NPC_KORM:
			if (ServerAllowsTwoSideGroups())
				pCreature->setFaction(FACTION_FRIENDLY_FOR_ALL);
			break;
		default:
			break;
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
            if (GetBossState(DATA_INTRO_EVENT) == DONE)
                pGameObject->SetPhaseMask(0, true);
            break;

        case GO_FROSTMOURNE_ALTAR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            HandleGameObject(0, true, pGameObject);
            break;

        case GO_FRONT_DOOR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            HandleGameObject(0, true, pGameObject);
            break;

        case GO_FROSTWORN_DOOR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            if (GetBossState(DATA_MARWYN_EVENT) == DONE)
                HandleGameObject(0, true, pGameObject);
            break;

        case GO_ARTHAS_DOOR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            if (GetBossState(DATA_FROSWORN_EVENT) == DONE)
                HandleGameObject(0, true, pGameObject);
            break;

        case GO_RUN_DOOR:
            pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
            if (GetBossState(DATA_FROSWORN_EVENT) == DONE)
                HandleGameObject(0, true, pGameObject);
            break;

        case GO_SKYBREAKER:
        case GO_ORGRIM_HAMMER:
        case GO_ICE_WALL:
        case GO_CAVE:
            break;

        default:
            return;
    }

    data64Map[entry] = pGameObject->GetGUID();
}

void HorInstanceScript::SetData(uint32 type, uint32 data)
{
    // Encounter related data
    if (type > ENCOUNTER_PREFIX)
    {
        if (pCurrentEncounter)
            pCurrentEncounter->IncreaseData(type, data);
    }
    else
    {
        switch (type)
        {
            case DATA_CURRENT_ENCOUNTER:
            {
                if (pCurrentEncounter != NULL)
                    delete pCurrentEncounter;

                switch (data)
                {
                    case ENCOUNTER_VALUE_F_M_E:
                    case ENCOUNTER_VALUE_F_M_E_FAST:
                    case ENCOUNTER_VALUE_F_M_E_WAVE_RESTART:
                        pCurrentEncounter = new Encounter_FalricMarwynEvent(this);
                        break;

                    case ENCOUNTER_VALUE_LICH_KING_EVENT:
                        pCurrentEncounter = new Encounter_TheLichKingHor(this);
                        break;

                    default:
                        ASSERT(false);
                }

                ASSERT(pCurrentEncounter != NULL);

                switch (data)
                {
                    case ENCOUNTER_VALUE_F_M_E_FAST:
                        pCurrentEncounter->Start(ENCOUNTER_EVENT_FAST_INTRO, ENCOUNTER_FALRIC_MARWYN_EVENT::ENCOUNTER_PHASE_INTRO_LICH_KING);
                        break;

                    case ENCOUNTER_VALUE_F_M_E_WAVE_RESTART:
                        pCurrentEncounter->Start(ENCOUNTER_EVENT_RESTART_WAVES, ENCOUNTER_FALRIC_MARWYN_EVENT::ENCOUNTER_PHASE_INTRO_LICH_KING);
                        break;

                    case ENCOUNTER_VALUE_F_M_E:
                    case ENCOUNTER_VALUE_LICH_KING_EVENT:
                        pCurrentEncounter->Start();
                        break;
                }
            }break;

            case DATA_TEAM_IN_INSTANCE:
                dataMap[type] = data;
                break;
        }

        if (pCurrentEncounter && data == FAIL)
            pCurrentEncounter->OnFail();

        if (data == DONE)
            SaveToDB();
    }
}

uint32 HorInstanceScript::GetData(uint32 identifier)
{
    if (identifier > ENCOUNTER_PREFIX)
    {
        uint32 data = 0;

        if (pCurrentEncounter)
            pCurrentEncounter->GetData(identifier, data);
        return data;
    }
    else
    {
        switch (identifier)
        {
            case DATA_CURRENT_ENCOUNTER:
            {
                return pCurrentEncounter != NULL;
            }break;;

            default:
            {
                if (dataMap.find(identifier) != dataMap.end())
                    return dataMap.at(identifier);
                else
                    return 0;
            }break;
        }
    }
}

uint64 HorInstanceScript::GetData64(uint32 identifier)
{
    if (data64Map.find(identifier) != data64Map.end())
        return data64Map.at(identifier);
    else
        return 0;
}

void HorInstanceScript::DoAction(int32 action)
{
    // Encounter related action
    if (action > ENCOUNTER_PREFIX)
    {
        // Check pointer and transmit action to encounter
        ASSERT(pCurrentEncounter != NULL);
        pCurrentEncounter->DoAction(action);
    }
    // Instance related action
    else
    {
        switch (action)
        {
            case INSTANCE_ACTION_ENCOUNTER_DONE:
            {
                if (pCurrentEncounter)
                {
                    delete pCurrentEncounter;
                    pCurrentEncounter = NULL;
                }
            }break;

            case INSTANCE_ACTION_RESET_EVENT:
                prepareLichKingEvent();
                break;

            case INSTANCE_ACTION_START_GUNSHIP:
                setupGunship();
                // No break
            case INSTANCE_ACTION_RESTART_GUNSHIP:
                if (Transport* pTransport = getCurrentGunship())
                    pTransport->EnableMovement(true);
                break;

            case INSTANCE_ACTION_STOP_GUNSHIP:
                if (Transport* pTransport = getCurrentGunship())
                    pTransport->EnableMovement(false);
                break;
        }
    }
}

std::string HorInstanceScript::GetSaveData()
{
    OUT_SAVE_INST_DATA;

    std::ostringstream saveStream;
    saveStream << "H R " << GetBossSaveData();

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
    uint16 data[5];

    std::istringstream loadStream(in);
    loadStream >> dataHead1 >> dataHead2 >> data[0] >> data[1] >> data[2] >> data[3] >> data[4];

    if (dataHead1 == 'H' && dataHead2 == 'R')
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (data[i] != NOT_STARTED && data[i] != DONE)
                data[i] = NOT_STARTED;
            SetBossState(i, EncounterState(data[i]));
        }

        if (CheckRequiredBosses(DATA_LICHKING_EVENT))
            prepareEvent = true;
    }
    else
        OUT_LOAD_INST_DATA_FAIL;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void HorInstanceScript::ProcessEvent(WorldObject* /*obj*/, uint32 eventID)
{
    switch (eventID)
    {
        case INSTANCE_EVENT_GUNSHIP_FIRE:
        {
            if (GetBossState(DATA_LICHKING_EVENT) != DONE)
            {
                DoAction(INSTANCE_ACTION_STOP_GUNSHIP);

                if (GameObject* pObjectCave = instance->GetGameObject(GET_DATA64(GO_CAVE)))
                    if (Creature* pCreatureCave = instance->SummonCreature(NPC_ICE_WALL, pObjectCave->GetPosition()))
                        pCreatureCave->SetObjectScale(4.0f);

                for (CannonSet::const_iterator itr = cannonSet.begin(); itr != cannonSet.end(); ++itr)
                    if (Creature* pCreature = instance->GetCreature((*itr)))
                        pCreature->CastSpell(pCreature, SPELL_GUNSHIP_CANNON_FIRE, true);

                if (pCurrentEncounter)
                    pCurrentEncounter->DoAction(eventID);
            }
            else
                DoAction(INSTANCE_ACTION_START_GUNSHIP);

        }break;

        case INSTANCE_EVENT_GUNSHIP_ARRIVAL:
        {
            DoAction(INSTANCE_ACTION_STOP_GUNSHIP);

            if (pCurrentEncounter)
            {
                spawnAfterGunshipFinished();
                pCurrentEncounter->DoAction(eventID);
            }
        }break;
    }
}

void HorInstanceScript::Update(uint32 diff)
{
    if (pCurrentEncounter)
        pCurrentEncounter->Update(diff);

    if (aliveCheckTimer <= diff)
    {
        if (prepareEvent && GET_DATA(DATA_TEAM_IN_INSTANCE) != TEAM_NEUTRAL)
        {
            prepareLichKingEvent();
            prepareEvent = false;
        }

        if (!arePlayersAlive())
        {
            if (pCurrentEncounter)
            {
                pCurrentEncounter->OnFail();

                delete pCurrentEncounter;
                pCurrentEncounter = NULL;
            }
        }

        aliveCheckTimer = ALIVE_CHECK_TIMER;
    }
    else
        aliveCheckTimer -= diff;
}

bool HorInstanceScript::arePlayersAlive()
{
    if (delayedFail)
        if (--delayedFail == DELAYED_EXECUTE)
            return false;

    const Map::PlayerList& playerList = instance->GetPlayers();

    if (playerList.isEmpty())
        return false;

    for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
    {
        if (Player* pPlayer = itr->getSource())
        {
            if (pPlayer->isGameMaster())
                continue;

            if (pPlayer->isAlive())
                return true;
        }
    }

    return false;
}

void HorInstanceScript::prepareLichKingEvent()
{
    sLog->outDebug(LOG_FILTER_TSCR, "HorInstanceScript::prepareLichKingEvent()");

    if (Creature* pCreature = instance->GetCreature(GET_DATA64(BOSS_LICH_KING)))
        pCreature->DespawnOrUnsummon();

    if (GetBossState(DATA_LICHKING_EVENT) != DONE)
        instance->SummonCreatureOnMap(BOSS_LICH_KING, ENCOUNTER_THELICHKINGHOR::OutroSpawns[GET_DATA(DATA_TEAM_IN_INSTANCE) == ALLIANCE ? 1 : 0],
                                        TEMPSUMMON_MANUAL_DESPAWN, 0);

    if (Creature* pCreature = instance->GetCreature(GET_DATA64(NPC_JAINA_OR_SYLVANAS_O)))
        pCreature->DespawnOrUnsummon();

    if (GetBossState(DATA_LICHKING_EVENT) != DONE)
        if (Creature* pCreatureJainaOrSylvanas = instance->SummonCreatureOnMap(NPC_JAINA_OR_SYLVANAS_O,
            ENCOUNTER_THELICHKINGHOR::OutroSpawns[GET_DATA(DATA_TEAM_IN_INSTANCE) == ALLIANCE ? 0 : 1], TEMPSUMMON_MANUAL_DESPAWN, 0))
            if (pCreatureJainaOrSylvanas->AI())
                pCreatureJainaOrSylvanas->AI()->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_START_COMBAT);

    if (Transport* pTransport = getCurrentGunship())
        pTransport->Delete();

    if (Transport* pTransport = sTransportMgr->CreateTransport(GET_DATA(DATA_TEAM_IN_INSTANCE) == HORDE ? GO_ORGRIM_HAMMER : GO_SKYBREAKER, 0, instance))
        pTransport->EnableMovement(GetBossState(DATA_LICHKING_EVENT) == DONE);
}

void HorInstanceScript::setupGunship()
{
    sLog->outDebug(LOG_FILTER_TSCR, "HorInstanceScript::setupGunship()");

    Transport* pTransport = getCurrentGunship();
    if (!pTransport)
        return;

    cannonSet.clear();
    uint8 index = pTransport->GetEntry() == GO_SKYBREAKER ? 0 : 1;

    for (uint8 i = 0; i < 4; ++i)
    {
        Position spawnPosition;
        spawnPosition.Relocate(GunshipCreatureSpawnData[index][i].positionX, GunshipCreatureSpawnData[index][i].positionY,
                               GunshipCreatureSpawnData[index][i].positionZ, GunshipCreatureSpawnData[index][i].orientation);

        if (Creature* pCreature = pTransport->SummonPassenger(GunshipCreatureSpawnData[index][i].entry, spawnPosition, TEMPSUMMON_MANUAL_DESPAWN))
            if (i != 0) // Commander
                cannonSet.insert(pCreature->GetGUID());
    }
}

void HorInstanceScript::spawnAfterGunshipFinished()
{
    sLog->outDebug(LOG_FILTER_TSCR, "HorInstanceScript::spawnAfterGunshipFinished()");

    Transport* pTransport = getCurrentGunship();
    if (!pTransport)
        return;

    uint8 index = pTransport->GetEntry() == GO_SKYBREAKER ? 0 : 1;

    // Portal
    pTransport->SummonGameObject(GunshipGameObjectSpawnData[index][0].entry, GunshipGameObjectSpawnData[index][0].positionX,
                                 GunshipGameObjectSpawnData[index][0].positionY, GunshipGameObjectSpawnData[index][0].positionZ,
                                 GunshipGameObjectSpawnData[index][0].orientation, 0, 0, 0, 0, RESPAWN_IMMEDIATELY);
    // Chest
    uint8 heroic = instance->IsHeroic() ? 2 : 1;
    instance->SummonGameObjectOnMap(GunshipGameObjectSpawnData[index][heroic].entry, GunshipGameObjectSpawnData[index][heroic].positionX,
                                 GunshipGameObjectSpawnData[index][heroic].positionY, GunshipGameObjectSpawnData[index][heroic].positionZ,
                                 GunshipGameObjectSpawnData[index][heroic].orientation, 0, 0, 0, 0, 1 * DAY);

    // Stairs
    if (index == TEAM_ALLIANCE) // Alliance
    {
        instance->SummonGameObjectOnMap(GO_STAIRS_SKYBREAKER, 5216.49f, 1627.18f, 797.313f, 5.58189f, 0, 0, 0.343505f, -0.939151f, 1 * DAY);
        instance->SummonGameObjectOnMap(GO_STAIRS_SKYBREAKER, 5228.88f, 1642.02f, 784.955f, 5.58189f, 0, 0, 0.343505f, -0.939151f, 1 * DAY);
    }
    else // Horde
    {
        instance->SummonGameObjectOnMap(GO_STAIRS_ORGRIM_HAMMER, 5224.71f, 1589.8f, 808.235f, 5.80093f, 0, 0, 0.238798f, -0.971069f, 1 * DAY);
        instance->SummonGameObjectOnMap(GO_STAIRS_ORGRIM_HAMMER, 5234.12f, 1607.4f, 795.842f, 5.80093f, 0, 0, 0.238798f, -0.971069f, 1 * DAY);
        instance->SummonGameObjectOnMap(GO_STAIRS_ORGRIM_HAMMER, 5243.62f, 1625.24f, 783.797f, 5.80093f, 0, 0, 0.238798f, -0.971069f, 1 * DAY);
    }
}

Transport* HorInstanceScript::getCurrentGunship()
{
    sLog->outDebug(LOG_FILTER_TSCR, "HorInstanceScript::getCurrentGunship()");

    uint32 gunshipEntry = GET_DATA(DATA_TEAM_IN_INSTANCE) == ALLIANCE ? GO_SKYBREAKER : GO_ORGRIM_HAMMER;
    uint64 gunshipGUID = GET_DATA64(gunshipEntry);

    if (Transport* pTransport = instance->GetTransport(gunshipGUID))
        return pTransport;
    return NULL;
}