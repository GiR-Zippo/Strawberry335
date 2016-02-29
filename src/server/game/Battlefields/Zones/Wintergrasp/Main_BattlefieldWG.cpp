/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 29.07.2014
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

#include "Main_BattlefieldWG.h"
#include "Battleground.h"


BattlefieldWG::~BattlefieldWG()
{
    for (Workshop::const_iterator itr = workshopsList.begin(); itr != workshopsList.end(); ++itr)
        delete *itr;

    for (GameObjectBuilding::const_iterator itr = buildingsInZone.begin(); itr != buildingsInZone.end(); ++itr)
        delete *itr;
}

bool BattlefieldWG::SetupBattlefield()
{
    m_TypeId = BATTLEFIELD_WG; // See enum BattlefieldTypes
    m_BattleId = BATTLEFIELD_BATTLEID_WG;
    m_ZoneId = BATTLEFIELD_WG_ZONEID;
    m_MapId = BATTLEFIELD_WG_MAPID;
    m_Map = sMapMgr->FindMap(m_MapId, 0);

    m_MaxPlayer = sWorld->getIntConfig(CONFIG_WINTERGRASP_PLR_MAX);
    m_IsEnabled = sWorld->getBoolConfig(CONFIG_WINTERGRASP_ENABLE);
    m_MinPlayer = sWorld->getIntConfig(CONFIG_WINTERGRASP_PLR_MIN);
    m_MinLevel = sWorld->getIntConfig(CONFIG_WINTERGRASP_PLR_MIN_LVL);
    m_BattleTime = sWorld->getIntConfig(CONFIG_WINTERGRASP_BATTLETIME) * MINUTE * IN_MILLISECONDS;
    m_NoWarBattleTime = sWorld->getIntConfig(CONFIG_WINTERGRASP_NOBATTLETIME) * MINUTE * IN_MILLISECONDS;
    m_RestartAfterCrash = sWorld->getIntConfig(CONFIG_WINTERGRASP_RESTART_AFTER_CRASH) * MINUTE * IN_MILLISECONDS;

    m_TimeForAcceptInvite = 20;
    m_StartGroupingTimer = 15 * MINUTE * IN_MILLISECONDS;
    m_StartGrouping = false;

    tenacityStack = 0;

    KickPosition.Relocate(5728.117f, 2714.346f, 697.733f, 0);
    KickPosition.m_mapId = m_MapId;

    RegisterZone(m_ZoneId);

    m_Data32.resize(BATTLEFIELD_WG_DATA_MAX);

    saveTimer = TIMER_SAVE;
    fortressCheckTimer = TIMER_CHECK_FORTRESS;

    // Init GraveYards
    SetGraveyardNumber(BATTLEFIELD_WG_GRAVEYARD_MAX);

    // Load from db
    if ((sWorld->getWorldState(BATTLEFIELD_WG_WORLD_STATE_ACTIVE) == 0) && (sWorld->getWorldState(BATTLEFIELD_WG_WORLD_STATE_DEFENDER) == 0)
        && (sWorld->getWorldState(ClockWorldState[0]) == 0))
    {
        sWorld->setWorldState(BATTLEFIELD_WG_WORLD_STATE_ACTIVE, uint64(false));
        sWorld->setWorldState(BATTLEFIELD_WG_WORLD_STATE_DEFENDER, uint64(urand(0, 1)));
        sWorld->setWorldState(ClockWorldState[0], uint64(m_NoWarBattleTime));
    }

    m_isActive = bool(sWorld->getWorldState(BATTLEFIELD_WG_WORLD_STATE_ACTIVE));
    m_DefenderTeam = TeamId(sWorld->getWorldState(BATTLEFIELD_WG_WORLD_STATE_DEFENDER));
    notificationSent = false;

    // Restore state if server crash
    winCounter = 1;

    m_Timer = sWorld->getWorldState(ClockWorldState[0]);

    if (m_isActive)
    {
        m_isActive = false;
        m_Timer = m_RestartAfterCrash;
        notificationSent = true;
    }

    for (uint8 i = 0; i < BATTLEFIELD_WG_GRAVEYARD_MAX; ++i)
    {
        BfGraveyardWG* graveyard = new BfGraveyardWG(this);

        // When between games, the graveyard is controlled by the defending team
        if (WGGraveYard[i].startcontrol == TEAM_NEUTRAL)
            graveyard->Initialize(m_DefenderTeam, WGGraveYard[i].gyid);
        else
            graveyard->Initialize(WGGraveYard[i].startcontrol, WGGraveYard[i].gyid);

        graveyard->SetTextId(WGGraveYard[i].textid);
        m_GraveyardList[i] = graveyard;
    }

    // Spawn attacker base spirit keeper, link them to graveyard and hide them per default
    for (uint8 i = 0; i < 2; ++i)
    {
        uint32 entry = i ? WGAttackerBaseSpiritKeeper[i].entryHorde : WGAttackerBaseSpiritKeeper[i].entryAlliance;

        if (Creature* pCreature = SpawnCreature(entry, WGAttackerBaseSpiritKeeper[i].x, WGAttackerBaseSpiritKeeper[i].y,
            WGAttackerBaseSpiritKeeper[i].z, WGAttackerBaseSpiritKeeper[i].o, (TeamId)i))
        {
            if (BfGraveyard* pGrave = m_GraveyardList[BATTLEFIELD_WG_GY_ALLIANCE - i])
            {
                // Link him to graveyard and hide spirit keeper and 
                pGrave->SetSpirit(pCreature, (TeamId)i);
                HideNpc(pCreature);
            }
            else
            {
                sLog->outError("BattlefieldWG: Couldn't link attacker base spirit keeper %u to graveyard %u", pCreature->GetGUIDLow(), BATTLEFIELD_WG_GY_ALLIANCE - i);
                pCreature->DespawnOrUnsummon();
            }
        }
        else
            sLog->outError("BattlefieldWG: Couldn't spawn attacker base spirit keeper entry %u for team %s", entry, i ? "horde" : "alliance");
    }

    // Spawn workshop creatures and gameobjects
    for (uint8 i = 0; i < WG_MAX_WORKSHOP; ++i)
    {
        WintergraspWorkshopData* workshop = new WintergraspWorkshopData(this, i);
        if (i == BATTLEFIELD_WG_WORKSHOP_SE  || i == BATTLEFIELD_WG_WORKSHOP_SW)
            workshop->Init(GetAttackerTeam());
        else
            workshop->Init(GetDefenderTeam());

        workshopsList.insert(workshop);
    }

    // Spawn NPCs in the defender's keep, both Horde and Alliance
    for (uint8 i = 0; i < (sizeof(WGKeepNPC) / sizeof(WintergraspObjectPositionData)); ++i)
    {
        // Horde npc
        if (Creature* pCreature = SpawnCreature(WGKeepNPC[i].entryHorde, WGKeepNPC[i].x, WGKeepNPC[i].y, WGKeepNPC[i].z, WGKeepNPC[i].o, TEAM_HORDE))
            keepCreatures[TEAM_HORDE].insert(pCreature->GetGUID());
        // Alliance npc
        if (Creature* pCreature = SpawnCreature(WGKeepNPC[i].entryAlliance, WGKeepNPC[i].x, WGKeepNPC[i].y, WGKeepNPC[i].z, WGKeepNPC[i].o, TEAM_ALLIANCE))
            keepCreatures[TEAM_ALLIANCE].insert(pCreature->GetGUID());
    }

    // Hide Creatures from the Attackers team in the keep
    for (GuidSet::const_iterator itr = keepCreatures[GetAttackerTeam()].begin(); itr != keepCreatures[GetAttackerTeam()].end(); ++itr)
        if (Creature* pCreature = GetCreature(*itr))
            HideNpc(pCreature);

    // Spawn Horde NPCs outside the keep
    for (uint8 i = 0; i < WG_OUTSIDE_ALLIANCE_NPC; ++i)
        if (Creature* pCreature = SpawnCreature(WGOutsideNPC[i].entryHorde, WGOutsideNPC[i].x, WGOutsideNPC[i].y, WGOutsideNPC[i].z, WGOutsideNPC[i].o, TEAM_HORDE))
            outsideCreatures[TEAM_HORDE].insert(pCreature->GetGUID());

    // Spawn Alliance NPCs outside the keep
    for (uint8 i = WG_OUTSIDE_ALLIANCE_NPC; i < WG_MAX_OUTSIDE_NPC; ++i)
        if (Creature* pCreature = SpawnCreature(WGOutsideNPC[i].entryAlliance, WGOutsideNPC[i].x, WGOutsideNPC[i].y, WGOutsideNPC[i].z, WGOutsideNPC[i].o, TEAM_ALLIANCE))
            outsideCreatures[TEAM_ALLIANCE].insert(pCreature->GetGUID());

    // Hide units outside the keep that are defenders
    for (GuidSet::const_iterator itr = outsideCreatures[GetDefenderTeam()].begin(); itr != outsideCreatures[GetDefenderTeam()].end(); ++itr)
        if (Creature* pCreature = GetCreature(*itr))
            HideNpc(pCreature);

    // Spawn "Neutral" NPCs in Wintergrasp
    for (uint8 i = 0; i < (sizeof(WGNeutralNPC)/sizeof(WintergraspObjectPositionData)); ++i)
    {
        if (WGNeutralNPC[i].entryHorde != 0)
        {
            if(Creature* pGuard = SpawnCreature(WGNeutralNPC[i].entryHorde, WGNeutralNPC[i].x, WGNeutralNPC[i].y, WGNeutralNPC[i].z, WGNeutralNPC[i].o, TEAM_HORDE))
                ShowNpc(pGuard, true);
        }
        else if (WGNeutralNPC[i].entryAlliance != 0)
        {
            if (Creature* pGuard = SpawnCreature(WGNeutralNPC[i].entryAlliance, WGNeutralNPC[i].x, WGNeutralNPC[i].y, WGNeutralNPC[i].z, WGNeutralNPC[i].o, TEAM_ALLIANCE))
                ShowNpc(pGuard, true);
        }
    }

    // Spawn turrets and hide them per default
    for (uint8 i = 0; i < (sizeof(WGTurret) / sizeof(Position)); ++i)
    {
        Position towerCannonPos;
        WGTurret[i].GetPosition(&towerCannonPos);
        if (Creature* pCreature = SpawnCreature(BATTLEFIELD_WG_TOWER_CANNON, towerCannonPos, GetDefenderTeam()))
        {
            canonList.insert(pCreature->GetGUID());
            HideNpc(pCreature);
        }
    }

    // Spawn rocket launchers and hide them per default
    for (uint8 i = 0; i < (sizeof(WGKeepRocketLauncher) / sizeof(Position)); ++i)
    {
        Position rocketLauncherPos;
        WGKeepRocketLauncher[i].GetPosition(&rocketLauncherPos);
        if (Creature* pCreature = SpawnCreature(BATTLEFIELD_WG_NPC_ROCKET_LAUNCHER, rocketLauncherPos, GetDefenderTeam()))
        {
            rocketLaunchers.insert(pCreature->GetGUID());
            HideNpc(pCreature);
        }
    }

    // Spawn Banners in the defender's keep, both Horde and Alliance
    for (uint8 i = 0; i < (sizeof(WGKeepBanner)/sizeof(WintergraspObjectPositionData)); ++i)
    {
        // Horde Banner
        if (GameObject* pObj = SpawnGameObject(WGKeepBanner[i].entryHorde, WGKeepBanner[i].x, WGKeepBanner[i].y, WGKeepBanner[i].z, WGKeepBanner[i].o))
            keepBanner[TEAM_HORDE].insert(pObj->GetGUID());
        // Alliance Banner
        if (GameObject* pObj = SpawnGameObject(WGKeepBanner[i].entryAlliance, WGKeepBanner[i].x, WGKeepBanner[i].y, WGKeepBanner[i].z, WGKeepBanner[i].o))
            keepBanner[TEAM_ALLIANCE].insert(pObj->GetGUID());
    }

    // Hide Banners from the Attackers team in the keep
    for (GuidSet::const_iterator itr = keepBanner[GetAttackerTeam()].begin(); itr != keepBanner[GetAttackerTeam()].end(); ++itr)
        if (GameObject* pObj = GetGameObject(*itr))
            pObj->SetRespawnTime(RESPAWN_ONE_DAY);

    // Spawn all Buildings
    for (uint8 i = 0; i < (sizeof(WGGameObjectBuilding) / sizeof(WintergraspBuildingSpawnData)); ++i)
    {
        if (GameObject* go = SpawnGameObject(WGGameObjectBuilding[i].entry, WGGameObjectBuilding[i].x, WGGameObjectBuilding[i].y, WGGameObjectBuilding[i].z, WGGameObjectBuilding[i].o))
        {
            BfWGGameObjectBuilding* b = new BfWGGameObjectBuilding(this);
            b->Init(go, WGGameObjectBuilding[i].type, WGGameObjectBuilding[i].WorldState, WGGameObjectBuilding[i].nameId);
            if (!IsEnabled() && go->GetEntry() == BATTLEFIELD_WG_GO_VAULT_GATE)
                go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
            buildingsInZone.insert(b);
        }
    }

    // Spawning portal defender
    for (uint8 i = 0; i < (sizeof(WGPortalDefenderData) / sizeof(WintergraspTeleporterData)); ++i)
    {
        if (GameObject* go = SpawnGameObject(WGPortalDefenderData[i].entry, WGPortalDefenderData[i].x, WGPortalDefenderData[i].y, WGPortalDefenderData[i].z, WGPortalDefenderData[i].o))
        {
            defenderPortalList.insert(go->GetGUID());
            go->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[GetDefenderTeam()]);
        }
    }

    // Spawning portals in Dalaran (the defender portal will be shown)
    for (uint8 i = 0; i < (sizeof(WGPortals) / sizeof(Position)); ++i)
    {
        // Load grid
        if (!m_Map->IsGridLoaded(WGPortals[i].GetPositionX(), WGPortals[i].GetPositionY()))
            m_Map->LoadGrid(WGPortals[i].GetPositionX(), WGPortals[i].GetPositionY());

        if (GameObject* pObj = SpawnGameObject(BATTLEFIELD_WG_GO_WINTERGRASP_PORTAL, WGPortals[i].GetPositionX(),
            WGPortals[i].GetPositionY(), WGPortals[i].GetPositionZ(), WGPortals[i].GetOrientation()))
        {
            if (i == GetDefenderTeam())
                pObj->SetRespawnTime(RESPAWN_IMMEDIATELY);
            else
                pObj->SetRespawnTime(RESPAWN_ONE_DAY);

            dalaranPortals[i] = pObj->GetGUID();
        }
        else
            sLog->outError("BattlefieldWG: Couldn't spawn portal type: %u", i);
    }

    UpdateCounterVehicle(true);
    return true;
}

bool BattlefieldWG::Update(uint32 diff)
{
    bool m_return = Battlefield::Update(diff);

    if (saveTimer <= diff)
    {
        sWorld->setWorldState(BATTLEFIELD_WG_WORLD_STATE_ACTIVE, m_isActive);
        sWorld->setWorldState(BATTLEFIELD_WG_WORLD_STATE_DEFENDER, m_DefenderTeam);
        sWorld->setWorldState(ClockWorldState[0], m_Timer);
        saveTimer = TIMER_SAVE;
    }
    else
        saveTimer -= diff;

    if (!notificationSent && m_Timer < 30 * MINUTE * IN_MILLISECONDS)
    {
        SendWarningToAllWithLevelReq(LANG_WG_START_TIME_BATTLE, LANG_WG_START_TIME_30);
        notificationSent = true;
    }

    // Update fortress check
    if (!IsWarTime())
    {
        if (fortressCheckTimer <= diff)
        {
            checkFortress();
            fortressCheckTimer = TIMER_CHECK_FORTRESS;
        }
        else
            fortressCheckTimer -= diff;
    }
    else
        battleTimer += diff;

    return m_return;
}

void BattlefieldWG::OnBattleStart()
{
    // Update Fortress Creatures/GameObjects
    UpdateTeam();

    // Update Neutral Spirits (hide)
    UpdateNeutralSpirits();

    // Update DB spawned creatures (hide)
    UpdateDBSpawnedCreatures();

    // Spawn titan relic
    if (GameObject* relic = SpawnGameObject(BATTLEFIELD_WG_GO_TITAN_S_RELIC, 5440.0f, 2840.8f, 430.43f, 0))
    {
        // Update faction of relic, only attacker can click on
        relic->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[GetAttackerTeam()]);
        // Set in use (not allow to click on before last door is broken)
        relic->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
        titansRelicGUID = relic->GetGUID();
    }
    else
        sLog->outError("BattlefieldWG: Failed to spawn titan relic");

    // Update torret visibility and update faction
    for (GuidSet::const_iterator itr = canonList.begin(); itr != canonList.end(); ++itr)
    {
        if (Creature* pCreature = GetCreature(*itr))
        {
            ShowNpc(pCreature, true);
            pCreature->setFaction(WintergraspFaction[GetDefenderTeam()]);
        }
    }

    // Update rocket launcher faction + visibility
    for (GuidSet::const_iterator itr = rocketLaunchers.begin(); itr != rocketLaunchers.end(); ++itr)
    {
        if (Creature* pCreature = GetCreature(*itr))
        {
            ShowNpc(pCreature, false);
            pCreature->setFaction(WintergraspFaction[GetDefenderTeam()]);
        }
    }

    // Spawn Turrets (Tower) and update faction
    for (GameObjectBuilding::const_iterator itr = buildingsInZone.begin(); itr != buildingsInZone.end(); ++itr)
    {
        if (*itr)
            (*itr)->UpdateTurretAttack(false);
    }

    SetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT, 0);
    SetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_DEF, 0);
    SetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT, 0);
    SetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF, 0);

    // Update graveyard (in no war time all graveyard is to defender, in war time, depend of base)
    for (Workshop::const_iterator itr = workshopsList.begin(); itr != workshopsList.end(); ++itr)
    {
        if (*itr)
            (*itr)->OnBattleStart();
    }

    // Kick player in orb room (online)
    for (uint8 team = 0; team < 2; ++team)
    {
        for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                checkAndKickPlayerInOrbRoom(pPlayer);
        }
    }

    // Initialize vehicle counter
    UpdateCounterVehicle(true);

    // Relocate offline players from orb room
    relocateOfflinePlayersInOrbRoom();

    // Set portals in Dalaran
    setBFStartPortals();

    // Send start warning to all players
    SendWarningToAllInZone(LANG_WG_START_BATTLE);

    // Reset timer
    battleTimer = 0;
}

void BattlefieldWG::UpdateCounterVehicle(bool init)
{
    if (init)
    {
        SetData(BATTLEFIELD_WG_DATA_VEHICLE_H, 0);
        SetData(BATTLEFIELD_WG_DATA_VEHICLE_A, 0);
    }

    SetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H, 0);
    SetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A, 0);

    for (Workshop::const_iterator itr = workshopsList.begin(); itr != workshopsList.end(); ++itr)
    {
        if (WintergraspWorkshopData* workshop = (*itr))
        {
            if (workshop->GetTeam() == TEAM_ALLIANCE)
                UpdateData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A, 4);
            else if (workshop->GetTeam() == TEAM_HORDE)
                UpdateData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H, 4);
        }
    }

    UpdateVehicleCountWG();
}

void BattlefieldWG::UpdateTeam()
{
    // Update all npc in keep
    for (GuidSet::const_iterator itr = keepCreatures[GetAttackerTeam()].begin(); itr != keepCreatures[GetAttackerTeam()].end(); ++itr)
        if (Creature* pCreature = GetCreature(*itr))
            HideNpc(pCreature);

    for (GuidSet::const_iterator itr = keepCreatures[GetDefenderTeam()].begin(); itr != keepCreatures[GetDefenderTeam()].end(); ++itr)
        if (Creature* pCreature = GetCreature(*itr))
            ShowNpc(pCreature, true);

    // Update all npc out of keep
    for (GuidSet::const_iterator itr = outsideCreatures[GetDefenderTeam()].begin(); itr != outsideCreatures[GetDefenderTeam()].end(); ++itr)
        if (Creature* pCreature = GetCreature(*itr))
            HideNpc(pCreature);

    for (GuidSet::const_iterator itr = outsideCreatures[GetAttackerTeam()].begin(); itr != outsideCreatures[GetAttackerTeam()].end(); ++itr)
        if (Creature* pCreature = GetCreature(*itr))
            ShowNpc(pCreature, true);

    // Update all banner
    for (GuidSet::const_iterator itr = keepBanner[GetAttackerTeam()].begin(); itr != keepBanner[GetAttackerTeam()].end(); ++itr)
        if (GameObject* pObj = GetGameObject(*itr))
            pObj->SetRespawnTime(RESPAWN_ONE_DAY);

    for (GuidSet::const_iterator itr = keepBanner[GetDefenderTeam()].begin(); itr != keepBanner[GetDefenderTeam()].end(); ++itr)
        if (GameObject* pObj = GetGameObject(*itr))
            pObj->SetRespawnTime(RESPAWN_IMMEDIATELY);
}

void BattlefieldWG::UpdateNeutralSpirits()
{
    if (spiritKeeper.empty())
        return;

    // Update neutral spirit keeper
    for (GuidSet::const_iterator itr = spiritKeeper.begin(); itr != spiritKeeper.end(); ++itr)
    {
        if (Creature* pSpirit = GetCreature(*itr))
        {
            if (IsWarTime())
                HideNpc(pSpirit);
            else
                ShowNpc(pSpirit, false);
        }
    }

    // Update attacker base spirit keeper
    if (IsWarTime())
    {
        // For wartime - only attacker cemetery should be activated
        if (BfGraveyard* pGrave = m_GraveyardList[BATTLEFIELD_WG_GY_ALLIANCE - GetDefenderTeam()])
            pGrave->GiveControlTo(TEAM_NEUTRAL);
        if (BfGraveyard* pGrave = m_GraveyardList[BATTLEFIELD_WG_GY_ALLIANCE - GetAttackerTeam()])
            pGrave->GiveControlTo(GetAttackerTeam());
    }
    else
    {
        // At no wartime - both cemeterys should be activated with neutral keeper
        for (uint8 i = 0; i < 2; ++i)
            if (BfGraveyard* pGrave = m_GraveyardList[BATTLEFIELD_WG_GY_ALLIANCE - i])
                pGrave->GiveControlTo((TeamId)(TEAM_ALLIANCE + i));
    }
}

void BattlefieldWG::UpdateDBSpawnedCreatures()
{
    if (databaseSpawnedCreatures.empty())
        return;

    // Update DB spawned creatures
    for (GuidSet::const_iterator itr = databaseSpawnedCreatures.begin(); itr != databaseSpawnedCreatures.end();)
    {
        if (Creature* pCreature = GetCreature(*itr))
        {
            if (IsWarTime())
                HideNpc(pCreature);
            else
                ShowNpc(pCreature, true);

            ++itr;
        }
        else
            databaseSpawnedCreatures.erase(itr++);
    }
}

void BattlefieldWG::OnBattleEnd(bool endByTimer)
{
    // Remove relic
    if (titansRelicGUID)
    {
        if (GameObject* relic = GetGameObject(titansRelicGUID))
            relic->RemoveFromWorld();
        titansRelicGUID = 0;
    }

    // Grant bonus honor - has to be done before rebuild
    addBonusHonorAfterBattle(endByTimer);

    // Achievements / Quests and WG Buff
    for (GuidSet::const_iterator itr = m_PlayersInWar[GetDefenderTeam()].begin(); itr != m_PlayersInWar[GetDefenderTeam()].end(); ++itr)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
        {
            if (pPlayer->isDead())
            {
                pPlayer->ResurrectPlayer(1.f);

                if (Corpse* pCorpse = pPlayer->GetCorpse())
                    pCorpse->RemoveFromWorld();
            }
            else if (!isPlayerInFortress(pPlayer))
                pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_FORTRESS, true);

            pPlayer->CastSpell(pPlayer, SPELL_ESSENCE_OF_WINTERGRASP, true);
            pPlayer->CastSpell(pPlayer, SPELL_VICTORY_REWARD, true);

            // Send Wintergrasp victory achievement
            DoCompleteOrIncrementAchievement(BATTLEFIELD_WG_ACHIEVEMENT_WIN_WG, pPlayer);

            // Award achievement for succeeding in Wintergrasp in 10 minutes or less
            if (!endByTimer && battleTimer <= TIMER_ACHIEVEMENT_WIN_WG)
                DoCompleteOrIncrementAchievement(BATTLEFIELD_WG_ACHIEVEMENT_WIN_WG_TIMER_10, pPlayer);

            // Complete victory quest
            if (pPlayer->hasQuest(WintergraspQuest[GetDefenderTeam()][BATTLEFIELD_WG_QUEST_VICTORY_IN_WINTERGRASP]))
            {
                // Only complete if player rank is not recruit [1]
                if (pPlayer->GetAura(SPELL_RECRUIT))
                {
                    if (pPlayer->GetAura(SPELL_RECRUIT)->GetStackAmount() != 1)
                        pPlayer->CompleteQuest(WintergraspQuest[GetDefenderTeam()][BATTLEFIELD_WG_QUEST_VICTORY_IN_WINTERGRASP]);
                }
                else
                    pPlayer->CompleteQuest(WintergraspQuest[GetDefenderTeam()][BATTLEFIELD_WG_QUEST_VICTORY_IN_WINTERGRASP]);
            }
        }
    }

    if (!endByTimer) // win alli/horde
        SendWarningToAllInZone(LANG_WG_END_WON_BATTLE, (GetDefenderTeam() == TEAM_ALLIANCE) ? LANG_COMMAND_GRAVEYARD_ALLIANCE : LANG_COMMAND_GRAVEYARD_HORDE);
    else // defend alli/horde
        SendWarningToAllInZone(LANG_WG_END_LOOSE_BATTLE, (GetDefenderTeam() == TEAM_ALLIANCE) ? LANG_COMMAND_GRAVEYARD_ALLIANCE : LANG_COMMAND_GRAVEYARD_HORDE);

    // Check force defender change
    if (endByTimer)
    {
        if (++winCounter >= WINTERGRASP_MAX_WIN_IN_A_ROW)
        {
            SetDefenderTeam(GetAttackerTeam());
            SendWarningToAllInZone(LANG_WG_FORCE_CHANGE);
            winCounter = 0;
        }
    }
    else
        winCounter = 0;

    // Hide turrets
    for (GuidSet::const_iterator itr = canonList.begin(); itr != canonList.end(); ++itr)
    {
        if (Creature* pCreature = GetCreature(*itr))
        {
            if (!winCounter)
                pCreature->setFaction(WintergraspFaction[GetDefenderTeam()]);

            HideNpc(pCreature);
        }
    }

    // Hide rocket launchers
    for (GuidSet::const_iterator itr = rocketLaunchers.begin(); itr != rocketLaunchers.end(); ++itr)
    {
        if (Creature* pCreature = GetCreature(*itr))
        {
            if (!winCounter)
                pCreature->setFaction(WintergraspFaction[GetDefenderTeam()]);

            HideNpc(pCreature);
        }
    }

    if (!winCounter) // One player triggered the relic or force change
        UpdateTeam(); // Update the Creatures/Banners of the Keep

    // Update Neutral Spirits (show)
    UpdateNeutralSpirits();

    // Update DB spawned creatures (show)
    UpdateDBSpawnedCreatures();

    // Update all graveyard, control is to defender when no wartime
    for (uint8 i = 0; i < BATTLEFIELD_WG_GY_HORDE; ++i)
        if (BfGraveyard* graveyard = GetGraveyardById(i))
            graveyard->GiveControlTo(GetDefenderTeam());

    for (GuidSet::const_iterator itr = keepGameObject[GetDefenderTeam()].begin(); itr != keepGameObject[GetDefenderTeam()].end(); ++itr)
        if (GameObject* object = GetGameObject(*itr))
            object->SetRespawnTime(RESPAWN_IMMEDIATELY);

    for (GuidSet::const_iterator itr = keepGameObject[GetAttackerTeam()].begin(); itr != keepGameObject[GetAttackerTeam()].end(); ++itr)
        if (GameObject* object = GetGameObject(*itr))
            object->SetRespawnTime(RESPAWN_ONE_DAY);

    // Update portal defender faction
    for (GuidSet::const_iterator itr = defenderPortalList.begin(); itr != defenderPortalList.end(); ++itr)
        if (GameObject* portal = GetGameObject(*itr))
            portal->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[GetDefenderTeam()]);

    // Rebuild Buildings, remove Turrets (Tower), set faction and save data
    for (GameObjectBuilding::const_iterator itr = buildingsInZone.begin(); itr != buildingsInZone.end(); ++itr)
    {
        if (*itr)
        {
            (*itr)->Rebuild();
            (*itr)->UpdateTurretAttack(true);
            (*itr)->Save();
        }
    }

    // EventHook and Save
    for (Workshop::const_iterator itr = workshopsList.begin(); itr != workshopsList.end(); ++itr)
    {
        (*itr)->OnBattleEnd();
        (*itr)->Save();
    }

    // Clear CapturePoints
    m_capturePoints.clear();

    for (uint8 team = 0; team < 2; ++team)
    {
        for (GuidSet::const_iterator itr = vehicles[team].begin(); itr != vehicles[team].end(); ++itr)
            if (Creature* pCreature = GetCreature(*itr))
                if (pCreature->IsVehicle())
                    pCreature->DespawnOrUnsummon();

        vehicles[team].clear();
    }

    for (GuidSet::const_iterator itr = m_PlayersInWar[GetAttackerTeam()].begin(); itr != m_PlayersInWar[GetAttackerTeam()].end(); ++itr)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
        {
            if (pPlayer->isDead())
            {
                pPlayer->ResurrectPlayer(1.f);

                if (Corpse* pCorpse = pPlayer->GetCorpse())
                    pCorpse->RemoveFromWorld();
            }
            else
                pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_BRIDGE, true);

            pPlayer->CastSpell(pPlayer, SPELL_DEFEAT_REWARD, true);
        }
    }

    for (uint8 team = 0; team < 2; ++team)
    {
        for (GuidSet::const_iterator itr = m_PlayersInWar[team].begin(); itr != m_PlayersInWar[team].end(); ++itr)
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                RemoveAurasFromPlayer(pPlayer);
        }

        m_PlayersInWar[team].clear();
    }

    // Disband Groups
    for (uint8 team = 0; team < 2; ++team)
    {
        for (GuidSet::const_iterator itr = m_Groups[team].begin(); itr != m_Groups[team].end(); ++itr)
        {
            if (Group* group = sGroupMgr->GetGroupByGUID(*itr))
                group->Disband();
        }

        m_Groups[team].clear();
    }

    if (!winCounter)
    {
        for (uint8 team = 0; team < 2; ++team)
        {
            for (GuidSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            {
                if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                {
                    pPlayer->RemoveAurasDueToSpell(m_DefenderTeam == TEAM_ALLIANCE ? SPELL_HORDE_CONTROL_PHASE_SHIFT : SPELL_ALLIANCE_CONTROL_PHASE_SHIFT, pPlayer->GetGUID());
                    pPlayer->AddAura(m_DefenderTeam == TEAM_HORDE ? SPELL_HORDE_CONTROL_PHASE_SHIFT : SPELL_ALLIANCE_CONTROL_PHASE_SHIFT, pPlayer);
                }
            }
        }
    }

    // Reset message
    notificationSent = false;

    // Set Dalaran portals
    setBFEndPortals();

    // Delete saved ranks
    savedPlayerRanks.clear();
}

// *******************************************************
// ******************* Reward System *********************
// *******************************************************
void BattlefieldWG::DoCompleteOrIncrementAchievement(uint32 achievement, Player* pPlayer, uint8 /*incrementNumber*/)
{
    AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(achievement);

    if (!achievementEntry)
        return;

    switch (achievement)
    {
        case BATTLEFIELD_WG_ACHIEVEMENT_WIN_WG_100:
        {
            // pPlayer->UpdateAchievementCriteria();
        }
        default:
        {
            if (pPlayer)
                pPlayer->CompletedAchievement(achievementEntry);
            break;
        }
    }

}

void BattlefieldWG::OnStartGrouping()
{
    SendWarningToAllWithLevelReq(LANG_WG_START_TIME_BATTLE, LANG_WG_START_TIME_15);
    SendWarningToAllWithLevelReq(LANG_WG_PORTAL_SPAWN);
    SetClientQueueEnabled();
}

uint8 BattlefieldWG::GetSpiritGraveyardId(uint32 areaId) const
{
    switch (areaId)
    {
        case AREA_WINTERGRASP_FORTRESS:
            return BATTLEFIELD_WG_GY_KEEP;
        case AREA_THE_SUNKEN_RING:
            return BATTLEFIELD_WG_GY_WORKSHOP_NE;
        case AREA_THE_BROKEN_TEMPLATE:
            return BATTLEFIELD_WG_GY_WORKSHOP_NW;
        case AREA_WESTPARK_WORKSHOP:
            return BATTLEFIELD_WG_GY_WORKSHOP_SW;
        case AREA_EASTPARK_WORKSHOP:
            return BATTLEFIELD_WG_GY_WORKSHOP_SE;
        case AREA_WINTERGRASP:
            return BATTLEFIELD_WG_GY_ALLIANCE;
        case AREA_THE_CHILLED_QUAGMIRE:
            return BATTLEFIELD_WG_GY_HORDE;
        default:
            sLog->outError("BattlefieldWG: Unexpected Area Id %u", areaId);
            break;
    }

    return 0;
}

void BattlefieldWG::OnCreatureCreate(Creature* pCreature)
{
    // Accessing to db spawned creatures
    if (pCreature->GetDBTableGUIDLow() != 0)
    {
        switch (pCreature->GetEntry())
        {
            // Store the DB spawned neutral Spirit Keepers
            case NPC_SPIRIT_KEEPER:
            {
                spiritKeeper.insert(pCreature->GetGUID());

                // If there is a war in progress, hide the npc, else show
                if (IsWarTime())
                    HideNpc(pCreature);
                else
                    ShowNpc(pCreature, false);
            }break;

            // Don't store this NPCs - always visible
            case BATTLEFIELD_WG_NPC_DISCOVER_UNIT:
            case BATTLEFIELD_WG_NPC_CONTROL_ARMS:
            case NPC_FLIGHT_MASTER_ALLIANCE:
            case NPC_FLIGHT_MASTER_HORDE:
            case NPC_WORLD_TRIGGER_LARGE_AOI_NOT_IMMUNE_PC_NPC:
            {
            }break;

            // Show or hide all other NPCs depending on wartime and store them
            default:
            {
                if (IsWarTime())
                    HideNpc(pCreature);
                else
                    ShowNpc(pCreature, true);

                // Set checks itself if it's already stored
                databaseSpawnedCreatures.insert(pCreature->GetGUID());
            }
        }

        return;
    }

    if (IsWarTime())
    {
        switch (pCreature->GetEntry())
        {
            case BATTLEFIELD_WG_SIEGE_ENGINE_CANNON_ALLIANCE:
            case BATTLEFIELD_WG_SIEGE_ENGINE_CANNON_HORDE:
            {
                // Add spell immunity aura - has to be done because its possible that nobody is inside 
                // this cannon (no spell immunity because of wgdrivevehicle aura)
                for (uint8 i = 0; i < (sizeof(BattlegroundVehicleSpellImmunity) / sizeof(uint32)); ++i)
                    pCreature->ApplySpellImmune(BattlegroundVehicleSpellImmunity[i], IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            }break;

            case BATTLEFIELD_WG_SIEGE_ENGINE_ALLIANCE:
            case BATTLEFIELD_WG_SIEGE_ENGINE_HORDE:
            case BATTLEFIELD_WG_CATAPULT:
            case BATTLEFIELD_WG_DEMOLISHER:
            {
                if (!pCreature->ToTempSummon() || !pCreature->ToTempSummon()->GetSummonerGUID() || !sObjectAccessor->FindPlayer(pCreature->ToTempSummon()->GetSummonerGUID()))
                {
                    pCreature->DespawnOrUnsummon();
                    return;
                }

                Player* pCreator = sObjectAccessor->FindPlayer(pCreature->ToTempSummon()->GetSummonerGUID());
                TeamId team = pCreator->GetTeamId();

                if (team == TEAM_HORDE)
                {
                    if (GetData(BATTLEFIELD_WG_DATA_VEHICLE_H) < GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H))
                    {
                        UpdateData(BATTLEFIELD_WG_DATA_VEHICLE_H, 1);
                        pCreature->AddAura(SPELL_HORDE_FLAG, pCreature);
                        vehicles[team].insert(pCreature->GetGUID());
                        UpdateVehicleCountWG();
                    }
                    else
                    {
                        pCreature->DespawnOrUnsummon();
                        return;
                    }
                }
                else
                {
                    if (GetData(BATTLEFIELD_WG_DATA_VEHICLE_A) < GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A))
                    {
                        UpdateData(BATTLEFIELD_WG_DATA_VEHICLE_A, 1);
                        pCreature->AddAura(SPELL_ALLIANCE_FLAG, pCreature);
                        vehicles[team].insert(pCreature->GetGUID());
                        UpdateVehicleCountWG();
                    }
                    else
                    {
                        pCreature->DespawnOrUnsummon();
                        return;
                    }
                }

                // Build delayed grab passenger object - only for non teleported vehicles
                if (!pCreator->HasAura(SPELL_WINTERGRASP_VEHICLE_TELEPORT))
                {
                    // Set unit not selectable to be sure, owner gets into with grab passenger
                    pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    // Delay Grab Passenger
                    pCreature->DelayedCastSpell(TIMER_GRAB_PASSENGER_DELAY, pCreator, SPELL_GRAB_PASSENGER);
                }

                break;
            }
        }
    }
}

void BattlefieldWG::OnCreatureRemove(Creature* /*pCreature*/)
{
    // Not used
}

void BattlefieldWG::OnGameObjectCreate(GameObject* /*go*/)
{
    // Not used
}

// Called when player kill a unit in wg zone
void BattlefieldWG::HandleKill(Player* pPlayer, Unit* pVictim)
{
    Battlefield::HandleKill(pPlayer, pVictim);

    if (!m_isActive)
        return;

    if (pPlayer == pVictim)
        return;

    bool again = false;
    TeamId killerTeam = pPlayer->GetTeamId();

    if (pVictim->GetTypeId() == TYPEID_PLAYER || pVictim->IsVehicle())
    {
        for (GuidSet::const_iterator itr = m_PlayersInWar[killerTeam].begin(); itr != m_PlayersInWar[killerTeam].end(); ++itr)
        {
            if (Player* pNearPlayer = sObjectAccessor->FindPlayer(*itr))
            {
                if (pPlayer->GetDistance2d(pNearPlayer) < WINTERGRASP_PROMOTE_PLAYER_RANGE)
                {
                    PromotePlayer(pNearPlayer);

                    // Increment quests
                    if (pVictim->GetTypeId() == TYPEID_PLAYER)
                    {
                        incrementPlayerQuest(BATTLEFIELD_WG_QUEST_KILL_THEM_ALL, pNearPlayer);
                        incrementPlayerQuest(BATTLEFIELD_WG_QUEST_KILL_THEM_ALL_BASE, pNearPlayer);
                    }
                }
            }
        }
    }
    else
    {
        for (GuidSet::const_iterator itr = keepCreatures[GetOtherTeam(killerTeam)].begin(); itr != keepCreatures[GetOtherTeam(killerTeam)].end(); ++itr)
        {
            if (Creature* pCreature = GetCreature(*itr))
            {
                if (pVictim->GetEntry() == pCreature->GetEntry() && !again)
                {
                    again = true;
                    for (GuidSet::const_iterator iter = m_PlayersInWar[killerTeam].begin(); iter != m_PlayersInWar[killerTeam].end(); ++iter)
                    {
                        if (Player* pNearPlayer = sObjectAccessor->FindPlayer(*iter))
                        {
                            if (pPlayer->GetDistance2d(pNearPlayer) < WINTERGRASP_PROMOTE_PLAYER_RANGE)
                            {
                                PromotePlayer(pNearPlayer);
                                incrementPlayerQuest(BATTLEFIELD_WG_QUEST_KILL_THEM_ALL, pNearPlayer);
                                incrementPlayerQuest(BATTLEFIELD_WG_QUEST_KILL_THEM_ALL_BASE, pNearPlayer);
                            }
                        }
                    }
                }
            }
        }
    }
    /// @todoRecent PvP activity worldstate
}

bool BattlefieldWG::FindAndRemoveVehicleFromList(Unit* vehicle)
{
    for (uint32 itr = 0; itr < 2; ++itr)
    {
        if (vehicles[itr].find(vehicle->GetGUID()) != vehicles[itr].end())
        {
            vehicles[itr].erase(vehicle->GetGUID());
            if (itr == TEAM_HORDE)
                UpdateData(BATTLEFIELD_WG_DATA_VEHICLE_H, -1);
            else
                UpdateData(BATTLEFIELD_WG_DATA_VEHICLE_A, -1);
            return true;
        }
    }
    return false;
}

void BattlefieldWG::OnUnitDeath(Unit* unit)
{
    if (IsWarTime())
        if (unit->IsVehicle())
            if (FindAndRemoveVehicleFromList(unit))
                UpdateVehicleCountWG();
}

// Update rank for player
void BattlefieldWG::PromotePlayer(Player* pPlayer)
{
    if (!m_isActive)
        return;

    // Updating rank of player
    if (Aura* pAura = pPlayer->GetAura(SPELL_RECRUIT))
    {
        if (pAura->GetStackAmount() >= 5)
        {
            pPlayer->RemoveAura(SPELL_RECRUIT);
            pPlayer->CastSpell(pPlayer, SPELL_CORPORAL, true);
            SendWarningToPlayer(pPlayer, LANG_WG_RANK_CORPORAL_REACHED);
        }
        else
            pPlayer->CastSpell(pPlayer, SPELL_RECRUIT, true);
    }
    else if (Aura* pAura = pPlayer->GetAura(SPELL_CORPORAL))
    {
        if (pAura->GetStackAmount() >= 5)
        {
            pPlayer->RemoveAura(SPELL_CORPORAL);
            pPlayer->CastSpell(pPlayer, SPELL_LIEUTENANT, true);
            SendWarningToPlayer(pPlayer, LANG_WG_RANK_LIEUTNANT_REACHED);
        }
        else
            pPlayer->CastSpell(pPlayer, SPELL_CORPORAL, true);
    }
}

void BattlefieldWG::RemoveAurasFromPlayer(Player* pPlayer)
{
    pPlayer->RemoveAurasDueToSpell(SPELL_RECRUIT);
    pPlayer->RemoveAurasDueToSpell(SPELL_CORPORAL);
    pPlayer->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
    pPlayer->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
    pPlayer->RemoveAurasDueToSpell(BATTLEFIELD_SPELL_SPIRITUAL_IMMUNITY);
    pPlayer->RemoveAurasDueToSpell(SPELL_TENACITY);
    pPlayer->RemoveAurasDueToSpell(SPELL_ESSENCE_OF_WINTERGRASP);
    pPlayer->RemoveAurasDueToSpell(SPELL_WINTERGRASP_RESTRICTED_FLIGHT_AREA);
    pPlayer->RemoveAurasDueToSpell(SPELL_ARGENTUM_TOURNAMENT_ARMISTICE);
}

void BattlefieldWG::OnPlayerJoinWar(Player* pPlayer)
{
    RemoveAurasFromPlayer(pPlayer);

    if (!restorePlayerRank(pPlayer))
        pPlayer->CastSpell(pPlayer, SPELL_RECRUIT, true);

    // Only add to teleport list when player hasn't joined with dalaran portal
    if (!pPlayer->HasAura(SPELL_TELEPORT_DALARAN_TO_WG))
        if (uint32 teleport = getTeleportSpell(pPlayer))
            pPlayer->DelayedCastSpell(TIMER_PLAYER_TELEPORT_DELAY, pPlayer, teleport);

    UpdateTenacity();

    if (pPlayer->GetTeamId() == GetAttackerTeam())
    {
        if (GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT) < 3)
            pPlayer->SetAuraStack(SPELL_TOWER_CONTROL, pPlayer, 3 - GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT));
    }
    else
    {
        if (GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT) > 0)
            pPlayer->SetAuraStack(SPELL_TOWER_CONTROL, pPlayer, GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT));
    }

    pPlayer->RemoveAurasDueToSpell(SPELL_TELEPORT_DALARAN_TO_WG);
    SendInitWorldStatesTo(pPlayer);
}

void BattlefieldWG::OnPlayerLeaveWar(Player* pPlayer)
{
    // Save rank
    savePlayerRank(pPlayer);

    pPlayer->RemoveAurasDueToSpell(SPELL_HORDE_CONTROLS_FACTORY_PHASE_SHIFT);
    pPlayer->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROLS_FACTORY_PHASE_SHIFT);
    pPlayer->RemoveAurasDueToSpell(SPELL_HORDE_CONTROL_PHASE_SHIFT);
    pPlayer->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROL_PHASE_SHIFT);

    // Remove rocket launcher
    if (Item* pItem = pPlayer->GetItemByEntry(BATTLEFIELD_WG_ROCKET_LAUNCHER))
        pPlayer->DestroyItemCount(BATTLEFIELD_WG_ROCKET_LAUNCHER, pItem->GetCount(), true);

    if (!pPlayer->GetSession()->PlayerLogout())
    {
        // Remove vehicle of player if he leaves
        if (Creature* pCreature = pPlayer->GetVehicleCreatureBase())
            pCreature->Kill(pCreature);

        RemoveAurasFromPlayer(pPlayer);
    }

    UpdateTenacity();
}

void BattlefieldWG::OnPlayerLeaveZone(Player* pPlayer)
{
    // Remove vehicle of player if he leaves
    if (Creature* pCreature = pPlayer->GetVehicleCreatureBase())
        pCreature->Kill(pCreature);

    if (!m_isActive)
    {
        RemoveAurasFromPlayer(pPlayer);

        // Remove rocket launcher
        if (Item* pItem = pPlayer->GetItemByEntry(BATTLEFIELD_WG_ROCKET_LAUNCHER))
            pPlayer->DestroyItemCount(BATTLEFIELD_WG_ROCKET_LAUNCHER, pItem->GetCount(), true);
    }

    pPlayer->RemoveAurasDueToSpell(SPELL_HORDE_CONTROLS_FACTORY_PHASE_SHIFT);
    pPlayer->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROLS_FACTORY_PHASE_SHIFT);
    pPlayer->RemoveAurasDueToSpell(SPELL_HORDE_CONTROL_PHASE_SHIFT);
    pPlayer->RemoveAurasDueToSpell(SPELL_ALLIANCE_CONTROL_PHASE_SHIFT);
}

void BattlefieldWG::OnPlayerEnterZone(Player* pPlayer)
{
    if (!m_isActive)
        RemoveAurasFromPlayer(pPlayer);

    pPlayer->AddAura(m_DefenderTeam == TEAM_HORDE ? SPELL_HORDE_CONTROL_PHASE_SHIFT : SPELL_ALLIANCE_CONTROL_PHASE_SHIFT, pPlayer);
}

uint32 BattlefieldWG::GetData(uint32 data) const
{
    switch (data)
    {
        // Used to determine when the phasing spells must be cast
        // See: SpellArea::IsFitToRequirements
        case AREA_THE_SUNKEN_RING:
        case AREA_THE_BROKEN_TEMPLATE:
        case AREA_WESTPARK_WORKSHOP:
        case AREA_EASTPARK_WORKSHOP:
            // Graveyards and Workshops are controlled by the same team.
            if (BfGraveyard const* graveyard = GetGraveyardById(GetSpiritGraveyardId(data)))
                return graveyard->GetControlTeamId();
    }

    return Battlefield::GetData(data);
}

BfCapturePoint* BattlefieldWG::RemoveCapturePointForWorkshop(WintergraspWorkshopData* workshop)
{
    if (m_capturePoints.empty())
        return NULL;

    for (BfCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
    {
        if (itr->second)
        {
            if (itr->second->GetCapturePointEntry() == workshop->GetCapturePointEntry())
            {
                BfCapturePoint* tmp = itr->second;
                DelCapturePoint(itr->second);
                return tmp;
            }
        }
    }

    sLog->outError("BattlefieldWG: Couldn't find BfCapturePoint for workshop with id: %u", workshop->GetId());
    return NULL;
}

// @param entry = trinity_string.entry for the main message
// @param additionalEntry = trinity_string.entry for the item message (Alliance / Horde or south-tower/north-tower as an example)
// If the param additionalEntry is given, the trinity_string(entry) must contain a format string '%s' to parse the trinity_string(additionalEntry)
void BattlefieldWG::SendWarningToAllWithLevelReq(uint32 entry, uint32 additionalEntry)
{
    SessionMap sMap = sWorld->GetAllSessions();

    if(sMap.empty())
        return;

    for (SessionMap::const_iterator itr = sMap.begin(); itr != sMap.end(); ++itr)
    {
        if (itr->second)
        {
            if (Player* pPlayer = itr->second->GetPlayer())
            {
                if (pPlayer->IsInWorld()
                    && pPlayer->getLevel() >= BATTLEFIELD_WG_NOTIFICATION_LEVEL
                    && pPlayer->GetMapId() == 571) // Northrend
                {
                    std::string core = itr->second->GetTrinityString(entry);

                    if (additionalEntry != 0)
                    {
                        std::string additional = itr->second->GetTrinityString(additionalEntry);

                        ChatHandler(itr->second).PSendSysMessage(core.c_str(), additional.c_str());
                        itr->second->SendAreaTriggerMessage(core.c_str(), additional.c_str());
                    }
                    else
                    {
                        ChatHandler(itr->second).PSendSysMessage(core.c_str());
                        itr->second->SendAreaTriggerMessage(core.c_str());
                    }
                }
            }
        }
    }
}

// DO NOT USE! ONLY FOR TESTING
void BattlefieldWG::SetClientQueueEnabled()
{
    SessionMap sMap = sWorld->GetAllSessions();

    if(sMap.empty())
        return;

    for (SessionMap::const_iterator itr = sMap.begin(); itr != sMap.end(); ++itr)
    {
        if (itr->second)
        {
            if (itr->second->GetPlayer()
                && itr->second->GetPlayer()->IsInWorld()
                && itr->second->GetPlayer()->getLevel() >= BATTLEFIELD_WG_NOTIFICATION_LEVEL)
            {
                itr->second->SendBfStateChange(m_BattleId, 1);
            }
        }
    }
}

void BattlefieldWG::FillInitialWorldStates(WorldPacket& data)
{
    data << uint32(BATTLEFIELD_WG_WORLD_STATE_ATTACKER) << uint32(IsWarTime() ? 0 : GetAttackerTeam());
    data << uint32(BATTLEFIELD_WG_WORLD_STATE_DEFENDER) << uint32(IsWarTime() ? 0 : GetDefenderTeam());
    data << uint32(BATTLEFIELD_WG_WORLD_STATE_ACTIVE) << uint32(IsWarTime() ? 0 : 1);
    data << uint32(BATTLEFIELD_WG_WORLD_STATE_SHOW_WORLDSTATE) << uint32(IsWarTime() ? 1 : 0);

    for (uint32 i = 0; i < 2; ++i)
        data << ClockWorldState[i] << uint32(time(NULL) + (m_Timer / 1000));

    data << uint32(BATTLEFIELD_WG_WORLD_STATE_VEHICLE_H) << uint32(GetData(BATTLEFIELD_WG_DATA_VEHICLE_H));
    data << uint32(BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_H) << GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H);
    data << uint32(BATTLEFIELD_WG_WORLD_STATE_VEHICLE_A) << uint32(GetData(BATTLEFIELD_WG_DATA_VEHICLE_A));
    data << uint32(BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_A) << GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A);

    for (GameObjectBuilding::const_iterator itr = buildingsInZone.begin(); itr != buildingsInZone.end(); ++itr)
        if (*itr)
            data << (*itr)->m_WorldState << (*itr)->m_State;

    for (Workshop::const_iterator itr = workshopsList.begin(); itr != workshopsList.end(); ++itr)
        if (*itr)
            data << (*itr)->GetWorldState() << (*itr)->GetState();
}

void BattlefieldWG::SendInitWorldStatesTo(Player* pPlayer)
{
    WorldPacket data(SMSG_INIT_WORLD_STATES, (4 + 4 + 4 + 2 + (buildingsInZone.size() * 8) + (workshopsList.size() * 8)));

    data << uint32(m_MapId);
    data << uint32(m_ZoneId);
    data << uint32(0);
    data << uint16(10 + buildingsInZone.size() + workshopsList.size()); // Number of fields

    FillInitialWorldStates(data);

    pPlayer->GetSession()->SendPacket(&data);
}

void BattlefieldWG::SendInitWorldStatesToAll(bool toServer)
{
    if (toServer)
    {
        for (SessionMap::const_iterator itr = sWorld->GetAllSessions().begin(); itr != sWorld->GetAllSessions().end(); ++itr)
        {
            if (itr->second)
            {
                if (Player* pPlayer = itr->second->GetPlayer())
                {
                    // Only send to player who are in northrend
                    if (pPlayer->GetMapId() == 571)
                    {
                        if (pPlayer->GetZoneId() != BATTLEFIELD_WG_ZONEID)
                        {
                            WorldPacket data(SMSG_INIT_WORLD_STATES, (4 + 4 + 4 + 2 + (buildingsInZone.size() * 8) + (workshopsList.size() * 8)));

                            data << uint32(pPlayer->GetMapId());
                            data << uint32(pPlayer->GetZoneId());
                            data << uint32(pPlayer->GetAreaId());
                            data << uint16(10 + buildingsInZone.size() + workshopsList.size()); // Number of fields

                            FillInitialWorldStates(data);

                            itr->second->SendPacket(&data);
                        }
                        else
                            SendInitWorldStatesTo(pPlayer);
                    }
                }
            }
        }

        return;
    }

    for (uint8 team = 0; team < 2; team++)
    {
        for (GuidSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                SendInitWorldStatesTo(pPlayer);
        }
    }
}

void BattlefieldWG::BrokenWallOrTower(TeamId team, uint64 guid)
{
    GameObject* pObject = GetGameObject(guid);
    if (!pObject)
        return;

    // Quest/achievement reward system for team (opposite of building team)
    checkBuildingPlayerQuestAndAchievements(team ? TEAM_ALLIANCE : TEAM_HORDE, pObject);
}

// Called when a tower is broken
void BattlefieldWG::UpdatedDestroyedTowerCount(TeamId team)
{
    // Destroy an attack tower
    if (team == GetAttackerTeam())
    {
        // Update counter
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT, -1);
        UpdateData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT, 1);

        // Remove buff stack on attackers
        for (GuidSet::const_iterator itr = m_PlayersInWar[GetAttackerTeam()].begin(); itr != m_PlayersInWar[GetAttackerTeam()].end(); ++itr)
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                pPlayer->RemoveAuraFromStack(SPELL_TOWER_CONTROL);

            // Add buff stack to defenders
            for (GuidSet::const_iterator itr = m_PlayersInWar[GetDefenderTeam()].begin(); itr != m_PlayersInWar[GetDefenderTeam()].end(); ++itr)
            {
                if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                    pPlayer->CastSpell(pPlayer, SPELL_TOWER_CONTROL, true);
            }
        }

        // If all three south towers are destroyed (ie. all attack towers), remove ten minutes from battle time
        if (GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT) == 3)
        {
            if (int32(m_Timer - (TIMER_DECREASE_TOWER_DEFENDED)) < 0)
                m_Timer = 0;
            else
                m_Timer -= TIMER_DECREASE_TOWER_DEFENDED;

            SendInitWorldStatesToAll();
        }
    }
    else
    {
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF, -1);
        UpdateData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_DEF, 1);
    }
}

void BattlefieldWG::ProcessEvent(WorldObject* obj, uint32 eventId)
{
    if (!obj || !IsWarTime())
        return;

    // We handle only gameobjects here
    GameObject* go = obj->ToGameObject();
    if (!go)
        return;

    // On click on titan relic
    if (go->GetEntry() == BATTLEFIELD_WG_GO_TITAN_S_RELIC)
    {
        if (CanInteractWithRelic())
            EndBattle(false);
        else if (GameObject* relic = GetRelic())
            relic->SetRespawnTime(RESPAWN_IMMEDIATELY);
    }

    // if destroy or damage event, search the wall/tower and update worldstate/send warning message
    for (GameObjectBuilding::const_iterator itr = buildingsInZone.begin(); itr != buildingsInZone.end(); ++itr)
    {
        if (GameObject* build = GetGameObject((*itr)->m_BuildGUID))
        {
            if (go->GetEntry() == build->GetEntry())
            {
                if (build->GetGOInfo()->building.damagedEvent == eventId)
                    (*itr)->Damaged();

                if (build->GetGOInfo()->building.destroyedEvent == eventId)
                    (*itr)->Destroyed();

                break;
            }
        }
    }
}

// Called when a tower is damaged, used for honor reward calcul
void BattlefieldWG::UpdateDamagedTowerCount(TeamId team)
{
    if (team == GetAttackerTeam())
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT, 1);
    else
        UpdateData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF, 1);
}

// Update vehicle count WorldState to player
void BattlefieldWG::UpdateVehicleCountWG()
{
    SendUpdateWorldState(BATTLEFIELD_WG_WORLD_STATE_VEHICLE_H, GetData(BATTLEFIELD_WG_DATA_VEHICLE_H));
    SendUpdateWorldState(BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_H, GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_H));
    SendUpdateWorldState(BATTLEFIELD_WG_WORLD_STATE_VEHICLE_A, GetData(BATTLEFIELD_WG_DATA_VEHICLE_A));
    SendUpdateWorldState(BATTLEFIELD_WG_WORLD_STATE_MAX_VEHICLE_A, GetData(BATTLEFIELD_WG_DATA_MAX_VEHICLE_A));
}

void BattlefieldWG::UpdateTenacity()
{
    TeamId team = TEAM_NEUTRAL;
    float players[2] = { m_PlayersInWar[TEAM_ALLIANCE].size(), m_PlayersInWar[TEAM_HORDE].size() };
    uint8 newStack = 0;

    if (players[TEAM_ALLIANCE] && players[TEAM_HORDE] && (players[TEAM_ALLIANCE] != players[TEAM_HORDE]))
    {
        float stackCalculate = 0.0f;

        if (players[TEAM_ALLIANCE] < players[TEAM_HORDE])
            team = TEAM_ALLIANCE;
        else
            team = TEAM_HORDE;

        // Calculate stacks
        stackCalculate = ((std::max(players[TEAM_HORDE], players[TEAM_ALLIANCE]) / std::min(players[TEAM_HORDE], players[TEAM_ALLIANCE])) - 1.0f) * 4.0f;
        newStack = int8(stackCalculate);

        if (float(stackCalculate - newStack) >= 0.5f)
            ++newStack;

        if (newStack > 20)
            newStack = 20;
    }

    if (tenacityStack == newStack)
        return;

    // Remove all buffs only if stacks changed to zero
    if (tenacityStack != 0 && newStack == 0)
    {
        for (uint8 i = 0; i < 2; ++i)
        {
            for (GuidSet::const_iterator itr = m_PlayersInWar[i].begin(); itr != m_PlayersInWar[i].end(); ++itr)
                if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
                    pPlayer->RemoveAurasDueToSpell(SPELL_TENACITY);

            for (GuidSet::const_iterator itr = vehicles[i].begin(); itr != vehicles[i].end(); ++itr)
                if (Creature* pCreature = GetCreature(*itr))
                    pCreature->RemoveAurasDueToSpell(SPELL_TENACITY_VEHICLE);
        }
    }

    tenacityStack = newStack;

    // No apply, return (remove has been already made above)
    if (tenacityStack == 0)
        return;

    // Remove old buff
    for (GuidSet::const_iterator itr = m_PlayersInWar[GetOtherTeam(team)].begin(); itr != m_PlayersInWar[GetOtherTeam(team)].end(); ++itr)
        if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
            pPlayer->RemoveAurasDueToSpell(SPELL_TENACITY);

    for (GuidSet::const_iterator itr = vehicles[GetOtherTeam(team)].begin(); itr != vehicles[GetOtherTeam(team)].end(); ++itr)
        if (Creature* pCreature = GetCreature(*itr))
            pCreature->RemoveAurasDueToSpell(SPELL_TENACITY_VEHICLE);

    // Set honor buff
    uint32 buff_honor = SPELL_GREATEST_HONOR;
    if (tenacityStack < 5)
        buff_honor = 0;
    else if (tenacityStack < 10)
        buff_honor = SPELL_GREAT_HONOR;
    else if (tenacityStack < 15)
        buff_honor = SPELL_GREATER_HONOR;

    // Apply buffs (player)
    for (GuidSet::const_iterator itr = m_PlayersInWar[team].begin(); itr != m_PlayersInWar[team].end(); ++itr)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
        {
            pPlayer->SetAuraStack(SPELL_TENACITY, pPlayer, tenacityStack);

            if (buff_honor)
                pPlayer->CastSpell(pPlayer, buff_honor, true);
        }
    }

    // Apply buffs (vehicles)
    for (GuidSet::const_iterator itr = vehicles[team].begin(); itr != vehicles[team].end(); ++itr)
    {
        if (Creature* pCreature = GetCreature(*itr))
            pCreature->SetAuraStack(SPELL_TENACITY_VEHICLE, pCreature, tenacityStack);

        if (Creature* pCreature = GetCreature(*itr))
            pCreature->CastSpell(pCreature, buff_honor, true);
    }
}

void BattlefieldWG::GrantHonorForTower(TeamId team, bool destroyed)
{
    TeamId teamToProceed = team ? TEAM_ALLIANCE : TEAM_HORDE;

    if (m_PlayersInWar[teamToProceed].empty())
        return;

    for (GuidSet::const_iterator itr = m_PlayersInWar[teamToProceed].begin(); itr != m_PlayersInWar[teamToProceed].end(); ++itr)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
        {
            if (destroyed)
                pPlayer->CastSpell(pPlayer, SPELL_DESTROYED_TOWER, TRIGGERED_FULL_MASK);
            else
                pPlayer->CastSpell(pPlayer, SPELL_DAMAGED_TOWER, TRIGGERED_FULL_MASK);
        }
    }
}

void BattlefieldWG::setBFStartPortals()
{
    // Get portals in Dalaran and show them
    for (uint8 i = 0; i < (sizeof(dalaranPortals) / sizeof(uint64)); ++i)
    {
        if (GameObject* pObj = GetGameObject(dalaranPortals[i]))
            pObj->SetRespawnTime(RESPAWN_IMMEDIATELY);
        else
            sLog->outError("BattlefieldWG: Couldn't get portal type: %u guid: %u", i, dalaranPortals[i]);
    }
}

void BattlefieldWG::setBFEndPortals()
{
    // Get portal of the loserteam in Dalaran and hide it
    if (GameObject* pObj = GetGameObject(dalaranPortals[GetAttackerTeam()]))
        pObj->SetRespawnTime(RESPAWN_ONE_DAY);
    else
        sLog->outError("BattlefieldWG: Couldn't get portal type: %u guid: %u", GetDefenderTeam(), dalaranPortals[GetAttackerTeam()]);
}

void BattlefieldWG::teleportPlayer(Player* pPlayer)
{
    if (!pPlayer)
        return;

    if (pPlayer->isInCombat())
        pPlayer->ClearInCombat();

    if (pPlayer->GetTeamId() == GetDefenderTeam() && !isPlayerInFortress(pPlayer))
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_FORTRESS, true);
    else if (pPlayer->GetTeamId() == GetAttackerTeam())
    {
        if (pPlayer->GetTeamId() == TEAM_HORDE)
            pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_CAMP_HORDE, true);
        else
            pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_CAMP_ALLIANCE, true);
    }
}

uint32 BattlefieldWG::getTeleportSpell(Player* pPlayer)
{
    if (!pPlayer)
        return 0;

    if (pPlayer->GetTeamId() == GetDefenderTeam() && !isPlayerInFortress(pPlayer))
        return SPELL_TELEPORT_FORTRESS;
    else if (pPlayer->GetTeamId() == GetAttackerTeam())
    {
        if (pPlayer->GetTeamId() == TEAM_HORDE)
            return SPELL_TELEPORT_CAMP_HORDE;
        else
            return SPELL_TELEPORT_CAMP_ALLIANCE;
    }

    return 0;
}

bool BattlefieldWG::isPlayerInFortress(Player* pPlayer)
{
    if (!pPlayer)
        return false;

    if (pPlayer->GetAreaId() == AREA_WINTERGRASP_FORTRESS)
        return true;
    return false;
}

bool BattlefieldWG::isPlayerInOrbRoom(Player* pPlayer)
{
    if (!pPlayer)
        return false;

    if (pPlayer->GetPositionX() > WGFortressInside[1][0] && pPlayer->GetPositionX() < WGFortressInside[0][0]
        && pPlayer->GetPositionY() > WGFortressInside[1][1] && pPlayer->GetPositionY() < WGFortressInside[0][1])
        return true;
    return false;
}

void BattlefieldWG::checkAndKickPlayerInOrbRoom(Player* pPlayer)
{
    if (!pPlayer)
        return;

    if (isPlayerInOrbRoom(pPlayer))
    {
        if (pPlayer->GetTeamId() == m_DefenderTeam)
            pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_FORTRESS, true);
        else
        {
            if (pPlayer->GetTeamId() == TEAM_HORDE)
                pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_CAMP_HORDE, true);
            else
                pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_CAMP_ALLIANCE, true);
        }
    }
}

void BattlefieldWG::relocateOfflinePlayersInOrbRoom()
{
    // Select all offline player within orb room
    if (QueryResult result = CharacterDatabase.PQuery("SELECT `guid` FROM characters WHERE position_x > '%f' AND position_x < '%f'"
        " AND position_y > '%f' AND position_y < '%f'", WGFortressInside[1][0], WGFortressInside[0][0],
        WGFortressInside[1][1], WGFortressInside[0][1]))
    {
        do
        {
            Field* field = result->Fetch();

            // Change save position in database
            Player::SavePositionInDB(BATTLEFIELD_WG_MAPID, WGDalaranPortPosition.GetPositionX(), WGDalaranPortPosition.GetPositionY(),
                                     WGDalaranPortPosition.GetPositionZ(), WGDalaranPortPosition.GetOrientation(), BATTLEFIELD_WG_ZONEID,
                                     field->GetUInt64());

        } while (result->NextRow());
    }
}

void BattlefieldWG::savePlayerRank(Player* pPlayer)
{
    if (!pPlayer)
        return;

    uint32 auraId = 0;
    uint8 stackAmount = 0;

    if (pPlayer->HasAura(SPELL_RECRUIT))
    {
        auraId = SPELL_RECRUIT;
        stackAmount = pPlayer->GetAura(SPELL_RECRUIT)->GetStackAmount();
    }
    else if (pPlayer->HasAura(SPELL_CORPORAL))
    {
        auraId = SPELL_CORPORAL;
        stackAmount = pPlayer->GetAura(SPELL_CORPORAL)->GetStackAmount();
    }
    else if (pPlayer->HasAura(SPELL_LIEUTENANT))
    {
        auraId = SPELL_LIEUTENANT;
        stackAmount = 1; // Lieutenant can not stack
    }
    else
        return;

    SavedRankObject rankObject;
    rankObject.auraId = auraId;
    rankObject.stackAmount = stackAmount;

    savedPlayerRanks[pPlayer->GetGUID()] = rankObject;
}

bool BattlefieldWG::restorePlayerRank(Player* pPlayer)
{
    if (!pPlayer)
        return false;

    if (savedPlayerRanks.find(pPlayer->GetGUID()) == savedPlayerRanks.end())
        return false;

    // Cast rank spell and set aura amount
    SavedRankObject* pRObject = &savedPlayerRanks.find(pPlayer->GetGUID())->second;
    if (!pRObject)
        return false;

    pPlayer->AddAura(pRObject->auraId, pPlayer);
    pPlayer->GetAura(pRObject->auraId)->SetStackAmount(pRObject->stackAmount);

    // Delete from map
    savedPlayerRanks.erase(pPlayer->GetGUID());
    return true;
}

void BattlefieldWG::checkBuildingPlayerQuestAndAchievements(TeamId team, GameObject* pObject)
{
    std::vector<Player*> playerInRange;
    bool isVehicleInRange = false;

    // Get players in range
    for (GuidSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
        {
            if (pPlayer->GetDistance(pObject) < WINTERGRASP_BUILDING_QUEST_OR_ACHIEVEMENT_RANGE)
            {
                // Save player - proceed later
                playerInRange.push_back(pPlayer);

                // Check condition for quest "Defend The Siege"
                if (!isVehicleInRange && pPlayer->GetVehicle())
                    isVehicleInRange = true;
            }
        }
    }

    if (playerInRange.empty())
        return;

    // Increment or reward quests
    if (isVehicleInRange)
        incrementPlayerQuest(BATTLEFIELD_WG_QUEST_DEFEND_THE_SIEGE, playerInRange);

    switch (pObject->GetEntry())
    {
        case BATTLEFIELD_WG_GO_SHADOWSIGHT_TOWER:
        case BATTLEFIELD_WG_GO_WINTER_S_EDGE_TOWER:
        case BATTLEFIELD_WG_GO_FLAMEWATCH_TOWER:
            handlePlayerQuestAndAchievement(BATTLEFIELD_WG_QUEST_TOPPLING_THE_TOWERS, BATTLEFIELD_WG_ACHIEVEMENT_WG_TOWER_DESTROY, playerInRange);
            break;

        default:
            break;
    }
}

void BattlefieldWG::incrementPlayerQuest(WintergraspQuestType questType, Player* pPlayer)
{
    if (questType >= BATTLEFIELD_WG_QUEST_MAX)
    {
        sLog->outError("BattlefieldWG: incrementPlayerQuest has been called with unknow questType %u for Player %u", questType, pPlayer->GetGUIDLow());
        return;
    }

    if (!pPlayer)
    {
        sLog->outError("BattlefieldWG: incrementPlayerQuest has been called with invalid pointer");
        return;
    }

    uint32 questId = WintergraspQuest[pPlayer->GetTeamId()][questType];
    uint32 killCredit = WintergraspQuestKillCredit[pPlayer->GetTeamId()][questType];
    if (!killCredit)
        return;

    if (pPlayer->hasQuest(questId) && pPlayer->GetQuestStatus(questId) != QUEST_STATUS_COMPLETE)
        pPlayer->KilledMonsterCredit(killCredit, 0);
}

void BattlefieldWG::incrementPlayerQuest(WintergraspQuestType questType, std::vector<Player*>& playerList)
{
    if (playerList.empty())
        return;

    for (std::vector<Player*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
        if ((*itr))
            incrementPlayerQuest(questType, *itr);
}

/*void BattlefieldWG::completePlayerQuest(uint32 questId, std::vector<Player*>& playerList)
{
    if (playerList.empty())
        return;

    for (std::vector<Player*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
        if ((*itr))
            if ((*itr)->hasQuest(questId) && (*itr)->GetQuestStatus(questId) != QUEST_STATUS_COMPLETE)
                (*itr)->CompleteQuest(questId);
}*/

void BattlefieldWG::completePlayerAchievement(WintergraspAchievements achievement, std::vector<Player*>& playerList)
{
    if (playerList.empty())
        return;

    for (std::vector<Player*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
        if ((*itr))
            DoCompleteOrIncrementAchievement(achievement, (*itr));
}

void BattlefieldWG::handlePlayerQuestAndAchievement(WintergraspQuestType questType, WintergraspAchievements achievement, std::vector<Player*>& playerList)
{
    if (playerList.empty())
        return;

    for (std::vector<Player*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
    {
        if ((*itr))
        {
            incrementPlayerQuest(questType, *itr);
            DoCompleteOrIncrementAchievement(achievement, (*itr));
        }
    }
}

void BattlefieldWG::addBonusHonorAfterBattle(bool endByTimer)
{
    // Get prev defender and attacker team
    TeamId attackerTeam = GetAttackerTeam();
    TeamId defenderTeam = GetDefenderTeam();

    if (!endByTimer)
    {
        attackerTeam = GetDefenderTeam();
        defenderTeam = GetAttackerTeam();
    }

    // Player should be honored for own damaged and intact buildings
    uint8 damagedAttackerTowers = GetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT);
    uint8 damagedDefenderTowers = GetData(BATTLEFIELD_WG_DATA_DAMAGED_TOWER_DEF);
    uint8 intactAttackerTowers = WINTERGRASP_ATTACKER_TOWER_MAX - damagedAttackerTowers - GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT);
    uint8 intactDefenderTowers = WINTERGRASP_DEFENDER_TOWER_MAX - damagedDefenderTowers - GetData(BATTLEFIELD_WG_DATA_BROKEN_TOWER_DEF);

    for (uint8 team = 0; team < 2; ++team)
    {
        for (GuidSet::const_iterator itr = m_PlayersInWar[team].begin(); itr != m_PlayersInWar[team].end(); ++itr)
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
            {
                // Grant honor depending on tower
                if (team == attackerTeam)
                {
                    for (uint8 i = 0; i < damagedAttackerTowers; ++i)
                        pPlayer->CastSpell(pPlayer, SPELL_DAMAGED_BUILDING, TRIGGERED_FULL_MASK);

                    for (uint8 i = 0; i < intactAttackerTowers; ++i)
                        pPlayer->CastSpell(pPlayer, SPELL_INTACT_BUILDING, TRIGGERED_FULL_MASK);
                }
                else
                {
                    for (uint8 i = 0; i < damagedDefenderTowers; ++i)
                        pPlayer->CastSpell(pPlayer, SPELL_DAMAGED_BUILDING, TRIGGERED_FULL_MASK);

                    for (uint8 i = 0; i < intactDefenderTowers; ++i)
                        pPlayer->CastSpell(pPlayer, SPELL_INTACT_BUILDING, TRIGGERED_FULL_MASK);
                }

                // Grant honor depending on rank
                if (pPlayer->HasAura(SPELL_CORPORAL))
                    pPlayer->CastSpell(pPlayer, SPELL_DAMAGED_BUILDING, TRIGGERED_FULL_MASK);
                else if (pPlayer->HasAura(SPELL_LIEUTENANT))
                    pPlayer->CastSpell(pPlayer, SPELL_INTACT_BUILDING, TRIGGERED_FULL_MASK);
            }
        }
    }
}

void BattlefieldWG::checkFortress()
{
    if (!m_players[GetAttackerTeam()].empty())
    {
        for (GuidSet::const_iterator itr = m_players[GetAttackerTeam()].begin(); itr != m_players[GetAttackerTeam()].end(); ++itr)
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayer(*itr))
            {
                if (pPlayer->isGameMaster())
                    continue;

                if (isPlayerInOrbRoom(pPlayer) && !pPlayer->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_TAXI_FLIGHT))
                    if (uint32 teleport = getTeleportSpell(pPlayer))
                        pPlayer->DelayedCastSpell(TIMER_PLAYER_TELEPORT_DELAY, pPlayer, teleport);
            }
        }
    }
}