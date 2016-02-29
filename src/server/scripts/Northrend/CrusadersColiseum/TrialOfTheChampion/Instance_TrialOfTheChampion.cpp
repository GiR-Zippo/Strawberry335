/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
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

#include "Instance_TrialOfTheChampion.h"
#include "Encounter_TrialOfTheChampion.h"
#include "GrandChampions/Encounter_GrandChampions.h"
#include "ArgentumChallenge/Encounter_ArgentumChallenge.h"
#include "BlackKnight/Encounter_BlackKnight.h"


#define GET_DATA64(x)   data64Map[x]

typedef Instance_TrialOfTheChampion::Instance_TrialOfTheChampion_InstanceMapScript TotC_InstanceMapScript;

void TotC_InstanceMapScript::Initialize()
{
    data64Map.clear();
    vehicleList.clear();

    aliveCheckTimer = ALIVE_CHECK_TIMER;
    achievementHadWorse = true;
    announcerPositionUpdate = false;
    teamInInstance = TEAM_NEUTRAL;

    memset(&encounter, NULL, sizeof(encounter));
    memset(&grandChampionEntry, NULL, sizeof(grandChampionEntry));
}

bool TotC_InstanceMapScript::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (encounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void TotC_InstanceMapScript::OnCreatureCreate(Creature* pCreature)
{
    uint32 entry = pCreature->GetEntry();

    // Default handling - save creature in map
    switch (entry)
    {
        case NPC_JAEREN:
        case NPC_TIRION:
        case NPC_THRALL:
        case NPC_GARROSH:
        case NPC_VARIAN:
        case NPC_JAINA:
        case NPC_EADRIC:
        case NPC_PALETRESS:
        case NPC_BLACK_KNIGHT:
        case VEHICLE_BLACK_KNIGHT:
        case NPC_ORIENTATION_TRIGGER:
        {
            data64Map[entry] = pCreature->GetGUID();
        }break;
    }

    // Special handling for some creatures
    switch (entry)
    {
        // Horde Npc will be spawned everytime, only change if alliance is in
        case NPC_JAEREN:
        {
            Map::PlayerList const &players = instance->GetPlayers();
            if (!players.isEmpty())
            {
                if (Player* pPlayer = players.begin()->getSource())
                {
                    if (pPlayer->GetTeam() == ALLIANCE)
                    {
                        pCreature->UpdateEntry(NPC_ARELAS);
                        teamInInstance = ALLIANCE;
                    }
                    else
                        teamInInstance = HORDE;
                }
            }

            if (encounter[2] == DONE)
            {
                pCreature->DespawnOrUnsummon();
                return;
            }
            else if (encounter[0] == DONE)
            {
                pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                announcerPositionUpdate = true;
            }
        }break;

        case VEHICLE_ARGENT_WARHORSE:
        case VEHICLE_ARGENT_BATTLEWORG:
        {
            if (encounter[BOSS_GRAND_CHAMPIONS] == DONE)
            {
                pCreature->DespawnOrUnsummon();
                return;
            }
            else
                vehicleList.push_back(pCreature->GetGUID());
        }break;

        case NPC_BLACK_KNIGHT:
        {
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->SetReactState(REACT_PASSIVE);
        }break;

        case VEHICLE_BLACK_KNIGHT:
        {
            pCreature->SetPhaseMask(2, true);
        }break;
    }

	switch (entry)
	{		
		case NPC_MOKRA:
		case NPC_ERESSEA:
		case NPC_RUNOK:
		case NPC_ZULTORE:
		case NPC_VISCERI:
		case NPC_JACOB:
		case NPC_AMBROSE:
		case NPC_COLOSOS:
		case NPC_JAELYNE:
		case NPC_LANA:
		case NPC_STORMWIND_CHAMPION:
		case NPC_IRONFORGE_CHAMPION:
		case NPC_GNOMEREGAN_CHAMPION:
		case NPC_DARNASSUS_CHAMPION:
		case NPC_EXODAR_CHAMPION:
		case NPC_ORGRIMMAR_CHAMPION:
		case NPC_THUNDER_BLUFF_CHAMPION:
		case NPC_UNDERCITY_CHAMPION:
		case NPC_SENJIN_CHAMPION:
		case NPC_SILVERMOON_CHAMPION:
		case VEHICLE_MARSHAL_JACOB_ALERIUS_MOUNT:
		case VEHICLE_AMBROSE_BOLTSPARK_MOUNT:
		case VEHICLE_COLOSOS_MOUNT:
		case VEHICLE_EVENSONG_MOUNT:
		case VEHICLE_DARNASSIA_NIGHTSABER:
		case VEHICLE_EXODAR_ELEKK:
		case VEHICLE_STORMWIND_STEED:
		case VEHICLE_GNOMEREGAN_MECHANOSTRIDER:
		case VEHICLE_IRONFORGE_RAM:
		case VEHICLE_MOKRA_SKILLCRUSHER_MOUNT:
		case VEHICLE_ERESSEA_DAWNSINGER_MOUNT:
		case VEHICLE_RUNOK_WILDMANE_MOUNT:
		case VEHICLE_ZUL_TORE_MOUNT:
		case VEHICLE_DEATHSTALKER_VESCERI_MOUNT:
		case VEHICLE_FORSAKE_WARHORSE:
		case VEHICLE_THUNDER_BLUFF_KODO:
		case VEHICLE_ORGRIMMAR_WOLF:
		case VEHICLE_SILVERMOON_HAWKSTRIDER:
		case VEHICLE_DARKSPEAR_RAPTOR:
			if (ServerAllowsTwoSideGroups())
				pCreature->setFaction(FACTION_HOSTILE_FOR_ALL);
			break;
		
		// case NPC_JAEREN:
		// case NPC_ARELAS:
		// 	if (ServerAllowsTwoSideGroups())
		// 		pCreature->setFaction(FACTION_FRIENDLY_FOR_ALL);
		// 	break;
		
		default:
			break;
	}
}

void TotC_InstanceMapScript::OnGameObjectCreate(GameObject* pObject)
{
    switch (uint32 entry = pObject->GetEntry())
    {
        case GO_MAIN_GATE:;
        case GO_PORTCULLIS:
        {
            data64Map[entry] = pObject->GetGUID();
        }break;

        case GO_CHAMPIONS_LOOT:
        case GO_CHAMPIONS_LOOT_H:
        case GO_EADRIC_LOOT:
        case GO_EADRIC_LOOT_H:
        case GO_PALETRESS_LOOT:
        case GO_PALETRESS_LOOT_H:
        {
            data64Map[DATA64_CHEST] = pObject->GetGUID();
        }break;
    }
}

void TotC_InstanceMapScript::SetData(uint32 type, uint32 data)
{
    // Encounter related data
    if (type > ENCOUNTER_PREFIX)
    {
        if (pCurrentEncounter)
            pCurrentEncounter->IncreaseData(type, data);
    }

    switch (type)
    {
        case BOSS_GRAND_CHAMPIONS:
        {
            encounter[0] = data;

            if (data == SPECIAL)
            {
                if (!vehicleList.empty())
                    for (std::vector<uint64>::const_iterator itr = vehicleList.begin(); itr != vehicleList.end(); ++itr)
                        if (Creature* pCreature = instance->GetCreature(*itr))
                            pCreature->RemoveFromWorld();
            }
            else if (data == DONE)
            {
                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT_CHAMPIONS);

                if (Creature* pAnnouncer = instance->GetCreature(GET_DATA64(NPC_JAEREN)))
                    pAnnouncer->SummonGameObject(instance->IsHeroic() ? GO_CHAMPIONS_LOOT_H : GO_CHAMPIONS_LOOT, StadiumPosition.GetPositionX(), StadiumPosition.GetPositionY(),
                                                 StadiumPosition.GetPositionZ(), StadiumPosition.GetOrientation(), 0, 0, 0, 0, 90000000);
            }
        }break;

        case BOSS_ARGENT_CHALLENGE:
        {
            encounter[1] = data;

            if (data == DONE)
            {
                if (Creature* pAnnouncer = instance->GetCreature(GET_DATA64(NPC_JAEREN)))
                {
                    if (GET_DATA64(NPC_EADRIC))
                    {
                        DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT_EADRIC);

                        pAnnouncer->SummonGameObject(instance->IsHeroic() ? GO_EADRIC_LOOT_H : GO_EADRIC_LOOT, StadiumPosition.GetPositionX(), StadiumPosition.GetPositionY(),
                                                     StadiumPosition.GetPositionZ(), StadiumPosition.GetOrientation(), 0, 0, 0, 0, 90000000);
                    }
                    else if (GET_DATA64(NPC_PALETRESS))
                    {
                        DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT_PALETRESS);
                        DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT_MEMORIES);

                        pAnnouncer->SummonGameObject(instance->IsHeroic() ? GO_PALETRESS_LOOT_H : GO_PALETRESS_LOOT, StadiumPosition.GetPositionX(), StadiumPosition.GetPositionY(),
                                                     StadiumPosition.GetPositionZ(), StadiumPosition.GetOrientation(), 0, 0, 0, 0, 90000000);
                    }
                }
            }
        }break;

        case BOSS_BLACK_KNIGHT:
        {
            encounter[2] = data;

            if (data == DONE)
                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT_BLACK_KNIGHT);
        }break;

        case DATA_GRAND_CHAMPION_ENTRY:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                if (grandChampionEntry[i] == 0)
                {
                    grandChampionEntry[i] = data;
                    return;
                }
            }
        }break;

        case DATA_MEMORY_ENTRY:
            memoryEntry = data;
            break;

        case DATA_I_VE_HAD_WORSE:
            achievementHadWorse = (bool)data;
            break;

        case DATA_CURRENT_ENCOUNTER:
        {
            if (pCurrentEncounter != NULL)
                delete pCurrentEncounter;

            switch (data)
            {
                case ENCOUNTER_VALUE_GRAND_CHAMPIONS:
                case ENCOUNTER_VALUE_GRAND_CHAMPIONS_FAST:
                    pCurrentEncounter = new Encounter_GrandChampions(this);
                    break;

                case ENCOUNTER_VALUE_ARGENTUM_CHALLENGE:
                    pCurrentEncounter = new Encounter_ArgentumChallenge(this);
                    break;

                case ENCOUNTER_VALUE_BLACK_KNIGHT:
                    pCurrentEncounter = new Encounter_BlackKnight(this);
                    break;

                default:
                    ASSERT(false);
            }

            ASSERT(pCurrentEncounter != NULL);

            if (data == ENCOUNTER_VALUE_GRAND_CHAMPIONS_FAST)
                pCurrentEncounter->Start(8); // Start without extended intro
            else
                pCurrentEncounter->Start();
        }break;
    }

    if (type <= BOSS_BLACK_KNIGHT)
    {
        if (data == DONE || data == FAIL)
            HandleGameObject(GET_DATA64(GO_PORTCULLIS), true);
        else if (data == IN_PROGRESS || data == SPECIAL)
            HandleGameObject(GET_DATA64(GO_PORTCULLIS), false);

        if (data == DONE)
        {
            Map::PlayerList const &PlList = instance->GetPlayers();

            if (!PlList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                    if (Player* pPlayer = i->getSource())
                        pPlayer->BindToInstance();
            SaveToDB();
        }
    }
}

uint32 TotC_InstanceMapScript::GetData(uint32 data)
{
    switch (data)
    {
        case BOSS_GRAND_CHAMPIONS:      return encounter[0];
        case BOSS_ARGENT_CHALLENGE:     return encounter[1];
        case BOSS_BLACK_KNIGHT:         return encounter[2];

        case DATA_TEAM:                 return teamInInstance;

        default:
            return 0;
    }
}

uint64 TotC_InstanceMapScript::GetData64(uint32 data)
{
    if (data64Map.find(data) != data64Map.end())
        return GET_DATA64(data);
    else
        return NULL;
}

void TotC_InstanceMapScript::DoAction(int32 action)
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

                if (Creature* pAnnouncer = instance->GetCreature(GetData64(NPC_JAEREN)))
                    pAnnouncer->AI()->DoAction(ANNOUNCER_ACTION_MOVE_CHEST);
            }break;
        }
    }
}

bool TotC_InstanceMapScript::CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/, uint32 /*miscvalue1*/)
{
    switch (criteria_id)
    {
        case CRITERIA_CHAMPION_JACOB:
        case CRITERIA_CHAMPION_LANA:
        case CRITERIA_CHAMPION_COLOSOS:
        case CRITERIA_CHAMPION_AMBROSE:
        case CRITERIA_CHAMPION_JAELYNE:
        case CRITERIA_CHAMPION_MOKRA:
        case CRITERIA_CHAMPION_VISCERI:
        case CRITERIA_CHAMPION_RUNOK:
        case CRITERIA_CHAMPION_ERESSEA:
        case CRITERIA_CHAMPION_ZULTORE:
        case CRITERIA_CHAMPION_JACOB_H:
        case CRITERIA_CHAMPION_LANA_H:
        case CRITERIA_CHAMPION_COLOSOS_H:
        case CRITERIA_CHAMPION_AMBROSE_H:
        case CRITERIA_CHAMPION_JAELYNE_H:
        case CRITERIA_CHAMPION_MOKRA_H:
        case CRITERIA_CHAMPION_VISCERI_H:
        case CRITERIA_CHAMPION_RUNOK_H:
        case CRITERIA_CHAMPION_ERESSEA_H:
        case CRITERIA_CHAMPION_ZULTORE_H:
            for (uint8 i = 0; i < 3; i++)
                if (grandChampionEntry[i] == getRelatedCreatureEntry(criteria_id))
                    return true;
            return false;
        case CRITERIA_MEMORY_HOGGER:
        case CRITERIA_MEMORY_VANCLEEF:
        case CRITERIA_MEMORY_MUTANUS:
        case CRITERIA_MEMORY_HEROD:
        case CRITERIA_MEMORY_LUCIFROM:
        case CRITERIA_MEMORY_THUNDERAAN:
        case CRITERIA_MEMORY_CHROMAGGUS:
        case CRITERIA_MEMORY_HAKKAR:
        case CRITERIA_MEMORY_VEKNILASH:
        case CRITERIA_MEMORY_KALITHRESH:
        case CRITERIA_MEMORY_MALCHEZAAR:
        case CRITERIA_MEMORY_GRUUL:
        case CRITERIA_MEMORY_VASHJ:
        case CRITERIA_MEMORY_ARCHIMONDE:
        case CRITERIA_MEMORY_ILLIDAN:
        case CRITERIA_MEMORY_DELRISSA:
        case CRITERIA_MEMORY_MURU:
        case CRITERIA_MEMORY_INGVAR:
        case CRITERIA_MEMORY_CYANIGOSA:
        case CRITERIA_MEMORY_ECK:
        case CRITERIA_MEMORY_ONYXIA:
        case CRITERIA_MEMORY_HEIGAN:
        case CRITERIA_MEMORY_IGNIS:
        case CRITERIA_MEMORY_VEZAX:
        case CRITERIA_MEMORY_ALGALON:
            return (memoryEntry == getRelatedCreatureEntry(criteria_id));
        case CRITERIA_I_VE_HAD_WORSE:
            return achievementHadWorse;
    }

    return false;
}

std::string TotC_InstanceMapScript::GetSaveData()
{
    OUT_SAVE_INST_DATA;

    std::ostringstream saveStream;

    saveStream << "T C " << encounter[0]
        << ' ' << encounter[1]
        << ' ' << encounter[2]
        << ' ' << encounter[3];

    OUT_SAVE_INST_DATA_COMPLETE;
    return  saveStream.str();
}

void TotC_InstanceMapScript::Load(const char* in)
{
    if (!in)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(in);

    char dataHead1, dataHead2;
    uint16 data0, data1, data2, data3;

    std::istringstream loadStream(in);
    loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3;

    if (dataHead1 == 'T' && dataHead2 == 'C')
    {
        encounter[0] = data0;
        encounter[1] = data1;
        encounter[2] = data2;
        encounter[3] = data3;

        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (encounter[i] == IN_PROGRESS)
                encounter[i] = NOT_STARTED;
    }
    else
        OUT_LOAD_INST_DATA_FAIL;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void TotC_InstanceMapScript::Update(const uint32 diff)
{
    if (pCurrentEncounter)
        pCurrentEncounter->Update(diff);

    if (aliveCheckTimer <= diff)
    {
        if (!arePlayersAlive())
        {
            if (pCurrentEncounter)
            {
                pCurrentEncounter->OnFail();

                if (pCurrentEncounter->GetState() != ENCOUNTER_STATE_ENCOUNTER)
                {
                    delete pCurrentEncounter;
                    pCurrentEncounter = NULL;
                }
            }
        }

        aliveCheckTimer = ALIVE_CHECK_TIMER;
    }
    else
        aliveCheckTimer -= diff;

    if (announcerPositionUpdate)
    {
        if (Creature* pCreature = instance->GetCreature(GET_DATA64(NPC_JAEREN)))
        {
            pCreature->AI()->DoAction(ANNOUNCER_ACTION_MOVE_CHEST);
            announcerPositionUpdate = false;
        }
    }
}

uint32 TotC_InstanceMapScript::getRelatedCreatureEntry(uint32 criteria_id)
{
    switch (criteria_id)
    {
        case CRITERIA_CHAMPION_JACOB:
        case CRITERIA_CHAMPION_JACOB_H:
            return NPC_JACOB;
        case CRITERIA_CHAMPION_LANA:
        case CRITERIA_CHAMPION_LANA_H:
            return NPC_LANA;
        case CRITERIA_CHAMPION_COLOSOS:
        case CRITERIA_CHAMPION_COLOSOS_H:
            return NPC_COLOSOS;
        case CRITERIA_CHAMPION_AMBROSE:
        case CRITERIA_CHAMPION_AMBROSE_H:
            return NPC_AMBROSE;
        case CRITERIA_CHAMPION_JAELYNE:
        case CRITERIA_CHAMPION_JAELYNE_H:
            return NPC_JAELYNE;
        case CRITERIA_CHAMPION_MOKRA:
        case CRITERIA_CHAMPION_MOKRA_H:
            return NPC_MOKRA;
        case CRITERIA_CHAMPION_VISCERI:
        case CRITERIA_CHAMPION_VISCERI_H:
            return NPC_VISCERI;
        case CRITERIA_CHAMPION_RUNOK:
        case CRITERIA_CHAMPION_RUNOK_H:
            return NPC_RUNOK;
        case CRITERIA_CHAMPION_ERESSEA:
        case CRITERIA_CHAMPION_ERESSEA_H:
            return NPC_ERESSEA;
        case CRITERIA_CHAMPION_ZULTORE:
        case CRITERIA_CHAMPION_ZULTORE_H:
            return NPC_ZULTORE;

        case CRITERIA_MEMORY_HOGGER:     return NPC_MEMORY_HOGGER;
        case CRITERIA_MEMORY_VANCLEEF:   return NPC_MEMORY_VANCLEEF;
        case CRITERIA_MEMORY_MUTANUS:    return NPC_MEMORY_MUTANUS;
        case CRITERIA_MEMORY_HEROD:      return NPC_MEMORY_HEROD;
        case CRITERIA_MEMORY_LUCIFROM:   return NPC_MEMORY_LUCIFROM;
        case CRITERIA_MEMORY_THUNDERAAN: return NPC_MEMORY_THUNDERAAN;
        case CRITERIA_MEMORY_CHROMAGGUS: return NPC_MEMORY_CHROMAGGUS;
        case CRITERIA_MEMORY_HAKKAR:     return NPC_MEMORY_HAKKAR;
        case CRITERIA_MEMORY_VEKNILASH:  return NPC_MEMORY_VEKNILASH;
        case CRITERIA_MEMORY_KALITHRESH: return NPC_MEMORY_KALITHRESH;
        case CRITERIA_MEMORY_MALCHEZAAR: return NPC_MEMORY_MALCHEZAAR;
        case CRITERIA_MEMORY_GRUUL:      return NPC_MEMORY_GRUUL;
        case CRITERIA_MEMORY_VASHJ:      return NPC_MEMORY_VASHJ;
        case CRITERIA_MEMORY_ARCHIMONDE: return NPC_MEMORY_ARCHIMONDE;
        case CRITERIA_MEMORY_ILLIDAN:    return NPC_MEMORY_ILLIDAN;
        case CRITERIA_MEMORY_DELRISSA:   return NPC_MEMORY_DELRISSA;
        case CRITERIA_MEMORY_MURU:       return NPC_MEMORY_MURU;
        case CRITERIA_MEMORY_INGVAR:     return NPC_MEMORY_INGVAR;
        case CRITERIA_MEMORY_CYANIGOSA:  return NPC_MEMORY_CYANIGOSA;
        case CRITERIA_MEMORY_ECK:        return NPC_MEMORY_ECK;
        case CRITERIA_MEMORY_ONYXIA:     return NPC_MEMORY_ONYXIA;
        case CRITERIA_MEMORY_HEIGAN:     return NPC_MEMORY_HEIGAN;
        case CRITERIA_MEMORY_IGNIS:      return NPC_MEMORY_IGNIS;
        case CRITERIA_MEMORY_VEZAX:      return NPC_MEMORY_VEZAX;
        case CRITERIA_MEMORY_ALGALON:    return NPC_MEMORY_ALGALON;
    }

    return 0;
}

bool TotC_InstanceMapScript::arePlayersAlive()
{
    Map::PlayerList const &PlList = instance->GetPlayers();

    if (PlList.isEmpty())
        return true;

    for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
    {
        if (Player* pPlayer = i->getSource())
        {
            if (pPlayer->isGameMaster())
                continue;

            if (pPlayer->isAlive())
                return true;
        }
    }

    return false;
}