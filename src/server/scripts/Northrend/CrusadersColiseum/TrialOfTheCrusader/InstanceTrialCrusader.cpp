#include "InstanceTrialCrusader.h"
#include "NorthrendBeasts/NorthrendBeasts.h"
#include "LordJaraxxus/LordJaraxxus.h"
#include "FactionChampions/FactionChampions.h"
#include "TwinValkyr/TwinValkyr.h"
#include "Anubarak/Anubarak.h"

void InstanceTrialCrusaderMapScript::Save()
{
    OUT_SAVE_INST_DATA;

    std::ostringstream SaveStream;

    SaveStream << m_EncounterCurrent;
    SaveStream << ' ' << m_EncounterStatus;

    if (instance->IsHeroic())
        SaveStream << ' ' << m_TrialCounter;
    SaveDataBuffer = SaveStream.str();

    SaveToDB();
    OUT_SAVE_INST_DATA_COMPLETE;
}

std::string InstanceTrialCrusaderMapScript::GetSaveData()
{
    return SaveDataBuffer;
}

void InstanceTrialCrusaderMapScript::Load(const char* strIn)
{
    if (!strIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(strIn);

    std::istringstream loadStream(strIn);

    loadStream >> m_EncounterCurrent;
    loadStream >> m_EncounterStatus;

    if (instance->IsHeroic())
        loadStream >> m_TrialCounter;

    OUT_LOAD_INST_DATA_COMPLETE;
}

bool InstanceTrialCrusaderMapScript::IsEncounterInProgress() const
{
    return (m_EncounterStatus == IN_PROGRESS);
}

void InstanceTrialCrusaderMapScript::OnPlayerEnter(Player* player)
{
    UpdateCounter(player);
}

void InstanceTrialCrusaderMapScript::OnCreatureCreate(Creature* creature)
{
	auto entry = creature->GetEntry();

    switch (entry)
    {
        case NPC_THRALL:
        case NPC_PROUDMOORE:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
            break;
        case NPC_BARRETT:
        case NPC_TIRION:
        case NPC_GARROSH:
        case NPC_VARIAN:
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
        case NPC_FIZZLEBANG:
        case NPC_GORMOK:
        case NPC_ACIDMAW:
        case NPC_DREADSCALE:
        case NPC_ICEHOWL:
        case NPC_JARAXXUS:
        case NPC_CHAMPIONS_CONTROLLER:
        case NPC_DARKBANE:
        case NPC_LIGHTBANE:
        case NPC_ANUBARAK:
        case NPC_LICH_KING_TOCR:
            m_GUIDMap[creature->GetEntry()] = creature->GetGUID();
            break;
    }

	switch (entry)
	{
		case NPC_ALLIANCE_DEATH_KNIGHT:
		case NPC_ALLIANCE_DRUID_BALANCE:
		case NPC_ALLIANCE_DRUID_RESTORATION:
		case NPC_ALLIANCE_HUNTER:
		case NPC_ALLIANCE_MAGE:
		case NPC_ALLIANCE_PALADIN_HOLY:
		case NPC_ALLIANCE_PALADIN_RETRIBUTION:
		case NPC_ALLIANCE_PRIEST_DISCIPLINE:
		case NPC_ALLIANCE_PRIEST_SHADOW:
		case NPC_ALLIANCE_ROGUE:
		case NPC_ALLIANCE_SHAMAN_ENHANCEMENT:
		case NPC_ALLIANCE_SHAMAN_RESTORATION:
		case NPC_ALLIANCE_WARLOCK:
		case NPC_ALLIANCE_WARRIOR:
		case NPC_HORDE_DEATH_KNIGHT:
		case NPC_HORDE_DRUID_BALANCE:
		case NPC_HORDE_DRUID_RESTORATION:
		case NPC_HORDE_HUNTER:
		case NPC_HORDE_MAGE:
		case NPC_HORDE_PALADIN_HOLY:
		case NPC_HORDE_PALADIN_RETRIBUTION:
		case NPC_HORDE_PRIEST_DISCIPLINE:
		case NPC_HORDE_PRIEST_SHADOW:
		case NPC_HORDE_ROGUE:
		case NPC_HORDE_SHAMAN_ENHANCEMENT:
		case NPC_HORDE_SHAMAN_RESTORATION:
		case NPC_HORDE_WARLOCK:
		case NPC_HORDE_WARRIOR:
			if (ServerAllowsTwoSideGroups())
				creature->setFaction(FACTION_HOSTILE_FOR_ALL);
			break;
		default:
			break;
	}

    if (m_Encounter)
        m_Encounter->OnCreatureCreate(creature);
}

void InstanceTrialCrusaderMapScript::OnGameObjectCreate(GameObject* go)
{
    switch (go->GetEntry())
    {
        case GO_CRUSADERS_CACHE_10:
            if (instance->GetSpawnMode() == RAID_DIFFICULTY_10MAN_NORMAL)
                m_CrusadersCacheGUID = go->GetGUID();
            break;
        case GO_CRUSADERS_CACHE_25:
            if (instance->GetSpawnMode() == RAID_DIFFICULTY_25MAN_NORMAL)
                m_CrusadersCacheGUID = go->GetGUID();
            break;
        case GO_CRUSADERS_CACHE_10_H:
            if (instance->GetSpawnMode() == RAID_DIFFICULTY_10MAN_HEROIC)
                m_CrusadersCacheGUID = go->GetGUID();
            break;
        case GO_CRUSADERS_CACHE_25_H:
            if (instance->GetSpawnMode() == RAID_DIFFICULTY_25MAN_HEROIC)
                m_CrusadersCacheGUID = go->GetGUID();
            break;
        case GO_EAST_PORTCULLIS:
            go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
        case GO_ARGENT_COLISEUM_FLOOR:
        case GO_MAIN_GATE_DOOR:
        case GO_WEB_DOOR:
            m_GUIDMap[go->GetEntry()] = go->GetGUID();
            break;
    }
}

void InstanceTrialCrusaderMapScript::SetData(uint32 type, uint32 data)
{
    switch (type)
    {
        case TYPE_ENCOUNTER_STATUS:
        {
            if (data == IN_PROGRESS)
                m_Encounter->OnStart();
            else if (data == FAIL)
                m_Encounter->OnFail();
            else if (data == DONE)
                m_Encounter->OnDone();
            else if (data == SPECIAL)
                data = m_Encounter->OnSpecial();

            if (data == FAIL && m_EncounterStatus == IN_PROGRESS)
            {
                --m_TrialCounter;
                m_EncounterStatus = NOT_STARTED;
                Save();
                return;
            }

            m_EncounterStatus = data;

            if (data == DONE)
                Save();

            break;
        }
        case TYPE_ENCOUNTER_CURRENT:
        {
            if (data == ENCOUNTER_ANUBARAK)
                m_EventMap.ScheduleEvent(INSTANCE_DELAYED_ANUBARAK, 1000);
            else
                EncounterSwitch(data);
            break;
        }
        case GO_MAIN_GATE_DOOR:
        case GO_EAST_PORTCULLIS:
        case GO_WEB_DOOR:
        {
            if (data == OPEN)
                OpenDoor(m_GUIDMap[type]);
            else if (data == CLOSE)
                CloseDoor(m_GUIDMap[type]);
            break;
        }
    }
}

uint64 InstanceTrialCrusaderMapScript::GetData64(uint32 type)
{
    switch (type)
    {
        case NPC_TIRION:
        case NPC_FIZZLEBANG:
        case NPC_GARROSH:
        case NPC_VARIAN:
        case NPC_BARRETT:
        case NPC_GORMOK:
        case NPC_ACIDMAW:
        case NPC_DREADSCALE:
        case NPC_ICEHOWL:
        case NPC_JARAXXUS:
        case NPC_CHAMPIONS_CONTROLLER:
        case NPC_DARKBANE:
        case NPC_LIGHTBANE:
        case NPC_ANUBARAK:
        case NPC_LICH_KING_TOCR:
        case GO_ARGENT_COLISEUM_FLOOR:
            return m_GUIDMap[type];
        default:
            return 0;
    }
}

uint32 InstanceTrialCrusaderMapScript::GetData(uint32 type)
{
    switch (type)
    {
        case TYPE_TEAM:
        {
            Map::PlayerList const& players = instance->GetPlayers();
            if (players.isEmpty())
                return HORDE;

            int32 counter = 0;

            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (Player *player = itr->getSource())
                {
                    if (player->isGameMaster())
                        continue;
                    if (player->getFaction() == ALLIANCE)
                        counter++;
                    else
                        counter--;
                }

            if (counter <= 0)
                return HORDE;

            return ALLIANCE;
        }
        case TYPE_TRIAL_COUNTER:
            return m_TrialCounter;
        case TYPE_ENCOUNTER_CURRENT:
            return m_EncounterCurrent;
        case TYPE_ENCOUNTER_STATUS:
            return m_EncounterStatus;
    }
    return 0;
}

void InstanceTrialCrusaderMapScript::Update(uint32 diff)
{
    m_EventMap.Update(diff);

    while (uint32 EventId = m_EventMap.ExecuteEvent())
        switch (EventId)
        {
            case INSTANCE_COMBAT_CHECK:
            {
                CombatCheck();
                m_EventMap.ScheduleEvent(INSTANCE_COMBAT_CHECK, 2000);
                break;
            }
            case INSTANCE_DELAYED_ANUBARAK:
            {
                EncounterSwitch(ENCOUNTER_ANUBARAK);
                break;
            }
        }

    if (m_Encounter)
        m_Encounter->Update(diff);
}

void InstanceTrialCrusaderMapScript::EncounterSwitch(uint32 EncounterId)
{
    if (m_Encounter)
        delete m_Encounter;

    m_EncounterCurrent = EncounterId;
    m_EncounterStatus = NOT_STARTED;

    m_Encounter = GetNewEncounter();
}

void InstanceTrialCrusaderMapScript::CombatCheck()
{
    if (m_EncounterStatus != IN_PROGRESS)
        return;

    Map::PlayerList const& players = instance->GetPlayers();

    if (players.isEmpty())
    {
        SetData(TYPE_ENCOUNTER_STATUS, FAIL);
        return;
    }

    bool wipe = true;
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player *player = itr->getSource())
        {
            if (player->isGameMaster() || !player->isAlive())
                continue;

            wipe = false;
            player->SetInCombatFor();
        }

    if (wipe)
        SetData(TYPE_ENCOUNTER_STATUS, FAIL);
}

bool InstanceTrialCrusaderMapScript::CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* source, Unit const* target, uint32 miscvalue1)
{
    switch (criteria_id)
    {
        case A_TRIBUTE_TO_SKILL_10_PLAYER:
        case A_TRIBUTE_TO_SKILL_25_PLAYER:
            return m_TrialCounter >= 25;
        case A_TRIBUTE_TO_MAD_SKILL_10_PLAYER:
        case A_TRIBUTE_TO_MAD_SKILL_25_PLAYER:
            return m_TrialCounter >= 45;
        case A_TRIBUTE_TO_INSANITY_10_PLAYER:
        case A_TRIBUTE_TO_INSANITY_25_PLAYER:
        case REALM_FIRST_GRAND_CRUSADER:
            return m_TrialCounter == 50;
        case A_TRIBUTE_TO_IMMORTALITY_HORDE:
        case A_TRIBUTE_TO_IMMORTALITY_ALLIANCE:
            return m_TrialCounter == 50 && m_Immortality && instance->Is25ManRaid();
        case A_TRIBUTE_TO_DEDICATED_INSANITY:
            return m_TrialCounter == 50 && m_Insanity && m_Immortality && instance->Is25ManRaid();
    }

    if (m_Encounter)
        return m_Encounter->CheckAchievementCriteriaMeet(criteria_id, source, target, miscvalue1);

    return false;
}

void InstanceTrialCrusaderMapScript::OpenDoor(uint64 guid)
{
    if (!guid)
        return;

    if (GameObject* go = instance->GetGameObject(guid))
        go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
}

void InstanceTrialCrusaderMapScript::CloseDoor(uint64 guid)
{
    if (!guid)
        return;

    if (GameObject* go = instance->GetGameObject(guid))
        go->SetGoState(GO_STATE_READY);
}

void InstanceTrialCrusaderMapScript::OnUnitDeath(Unit *unit)
{
    if (unit->GetTypeId() == TYPEID_PLAYER && IsEncounterInProgress())
        m_Immortality = false;

    if (m_Encounter)
        m_Encounter->OnUnitDeath(unit);
}

void InstanceTrialCrusaderMapScript::UpdateCounter(Player *player)
{
    if (instance->IsHeroic())
    {
        player->SendUpdateWorldState(UPDATE_STATE_UI_SHOW, 1);
        player->SendUpdateWorldState(UPDATE_STATE_UI_COUNT, m_TrialCounter);
    }
}

Encounter *InstanceTrialCrusaderMapScript::GetNewEncounter()
{
    switch (m_EncounterCurrent)
    {
        case ENCOUNTER_NORTHREND_BEASTS:
            return new NorthrendBeasts(this);
        case ENCOUNTER_LORD_JARAXXUS:
            return new LordJaraxxus(this);
        case ENCOUNTER_FACTION_CHAMPIONS:
            return new FactionChampions(this, m_CrusadersCacheGUID);
        case ENCOUNTER_TWIN_VALKYR:
            return new TwinValkyr(this);
        case ENCOUNTER_ANUBARAK:
            return new Anubarak(this);
    }
    return NULL;
}

void AddInstanceTrialCrusader()
{
    new InstanceTrialCrusader();
}
