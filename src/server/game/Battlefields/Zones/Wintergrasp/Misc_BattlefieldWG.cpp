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

#include "Misc_BattlefieldWG.h"
#include "Main_BattlefieldWG.h"


void BfWGGameObjectBuilding::Rebuild()
{
    switch (m_Type)
    {
        case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR:
        case BATTLEFIELD_WG_OBJECTTYPE_WALL:
            m_Team = m_WG->GetDefenderTeam(); // Objects that are part of the keep should be the defender's
            break;
        case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
            m_Team = m_WG->GetAttackerTeam(); // The towers in the south should be the attacker's
            break;
        default:
            m_Team = TEAM_NEUTRAL;
            break;
    }

    if (GameObject* build = m_WG->GetGameObject(m_BuildGUID))
    {
        // Rebuild gameobject
        if (build->IsDestructibleBuilding())
        {
            build->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, NULL, true);
            if (build->GetEntry() == BATTLEFIELD_WG_GO_VAULT_GATE)
                if (GameObject* go = build->FindNearestGameObject(BATTLEFIELD_WG_GO_KEEP_COLLISION_WALL, 50.0f))
                    go->SetGoState(GO_STATE_READY);

            // Update worldstate
            m_State = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT - (m_Team * 3);
            m_WG->SendUpdateWorldState(m_WorldState, m_State);
        }

        build->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[m_Team]);
        UpdateCreatureAndGo();
    }
}

void BfWGGameObjectBuilding::Damaged()
{
    // Update worldstate
    m_State = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE - (m_Team * 3);
    m_WG->SendUpdateWorldState(m_WorldState, m_State);

    // Send warning message
    if (m_NameId) // tower damage + name
        m_WG->SendWarningToAllInZone(LANG_WG_TOWER_DAMAGED, m_NameId);
    else if (m_Type == BATTLEFIELD_WG_OBJECTTYPE_WALL)
        m_WG->SendWarningToAllInZone(LANG_WG_KEEP_UNDER_ATTACK);

    for (GuidSet::const_iterator itr = m_CreatureTopList[m_WG->GetAttackerTeam()].begin(); itr != m_CreatureTopList[m_WG->GetAttackerTeam()].end(); ++itr)
        if (Creature* creature = m_WG->GetCreature(*itr))
            m_WG->HideNpc(creature);

    for (GuidSet::const_iterator itr = m_TurretTopList.begin(); itr != m_TurretTopList.end(); ++itr)
        if (Creature* creature = m_WG->GetCreature(*itr))
            m_WG->HideNpc(creature);

    switch (m_Type)
    {
        case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
        case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
            m_WG->GrantHonorForTower((TeamId)m_Team, false);
            m_WG->UpdateDamagedTowerCount((TeamId)m_Team);
            break;
    }
}

void BfWGGameObjectBuilding::Destroyed()
{
    // Update worldstate
    m_State = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY - (m_Team * 3);
    m_WG->SendUpdateWorldState(m_WorldState, m_State);

    // Warn players
    if (m_NameId)
        m_WG->SendWarningToAllInZone(LANG_WG_TOWER_DESTROYED, m_NameId);

    switch (m_Type)
    {
        // Inform the global wintergrasp script of the destruction of this object
        case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
        case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
            m_WG->GrantHonorForTower((TeamId)m_Team, true);
            m_WG->UpdatedDestroyedTowerCount(TeamId(m_Team));
            break;

        case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
            if (GameObject* build = m_WG->GetGameObject(m_BuildGUID))
                if (GameObject* go = build->FindNearestGameObject(BATTLEFIELD_WG_GO_KEEP_COLLISION_WALL, 50.0f))
                    go->SetGoState(GO_STATE_ACTIVE);
            m_WG->SetRelicInteractible(true);
            if (m_WG->GetRelic())
                m_WG->GetRelic()->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
            else
                sLog->outDebug(LOG_FILTER_NONE, "BattlefieldWG: Relic not found.");
            break;
    }

    m_WG->BrokenWallOrTower(TeamId(m_Team), m_BuildGUID);
}

void BfWGGameObjectBuilding::Init(GameObject* go, uint32 type, uint32 worldstate, uint32 nameid)
{
    if (!go)
        return;

    // Set object active, fix to show it without normal behaviour
    go->setActive(true);

    // GameObject associated to object
    m_BuildGUID = go->GetGUID();

    // Type of building (WALL/TOWER/DOOR)
    m_Type = type;

    // WorldState for client (icon on map)
    m_WorldState = worldstate;

    // NameId for Warning text
    m_NameId = nameid;

    switch (m_Type)
    {
        case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR:
        case BATTLEFIELD_WG_OBJECTTYPE_WALL:
            m_Team = m_WG->GetDefenderTeam(); // Objects that are part of the keep should be the defender's
            break;
        case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
            m_Team = m_WG->GetAttackerTeam(); // The towers in the south should be the attacker's
            break;
        default:
            m_Team = TEAM_NEUTRAL;
            break;
    }

    // Set faction of this gameobject
    go->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[m_Team]);

    m_State = sWorld->getWorldState(m_WorldState);
    switch (m_State)
    {
        case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT:
        case BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT:
            go->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, NULL, true);
            break;
        case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY:
        case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DESTROY:
            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
            break;
        case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE:
        case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DAMAGE:
            go->SetDestructibleState(GO_DESTRUCTIBLE_DAMAGED);
            break;
    }

    int32 towerid = -1;
    switch (go->GetEntry())
    {
        case BATTLEFIELD_WG_GO_FORTRESS_TOWER_1:
            towerid = 0;
            break;
        case BATTLEFIELD_WG_GO_FORTRESS_TOWER_2:
            towerid = 1;
            break;
        case BATTLEFIELD_WG_GO_FORTRESS_TOWER_3:
            towerid = 2;
            break;
        case BATTLEFIELD_WG_GO_FORTRESS_TOWER_4:
            towerid = 3;
            break;
        case BATTLEFIELD_WG_GO_SHADOWSIGHT_TOWER:
            towerid = 4;
            break;
        case BATTLEFIELD_WG_GO_WINTER_S_EDGE_TOWER:
            towerid = 5;
            break;
        case BATTLEFIELD_WG_GO_FLAMEWATCH_TOWER:
            towerid = 6;
            break;
    }

    if (towerid > 3) // Attacker towers
    {
        // Spawn associate gameobjects
        for (uint8 i = 0; i < AttackTowers[towerid - 4].nbObject; i++)
        {
            WintergraspObjectPositionData gobData = AttackTowers[towerid - 4].GameObject[i];
            if (GameObject* go = m_WG->SpawnGameObject(gobData.entryHorde, gobData.x, gobData.y, gobData.z, gobData.o))
                m_GameObjectList[TEAM_HORDE].insert(go->GetGUID());
            if (GameObject* go = m_WG->SpawnGameObject(gobData.entryAlliance, gobData.x, gobData.y, gobData.z, gobData.o))
                m_GameObjectList[TEAM_ALLIANCE].insert(go->GetGUID());
        }

        // Spawn associate npc bottom
        for (uint8 i = 0; i < AttackTowers[towerid - 4].nbCreatureBottom; i++)
        {
            WintergraspObjectPositionData creatureData = AttackTowers[towerid - 4].CreatureBottom[i];
            if (Creature* creature = m_WG->SpawnCreature(creatureData.entryHorde, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_HORDE))
                m_CreatureBottomList[TEAM_HORDE].insert(creature->GetGUID());
            if (Creature* creature = m_WG->SpawnCreature(creatureData.entryAlliance, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_ALLIANCE))
                m_CreatureBottomList[TEAM_ALLIANCE].insert(creature->GetGUID());
        }

        // Spawn associate npc top
        for (uint8 i = 0; i < AttackTowers[towerid - 4].nbCreatureTop; i++)
        {
            WintergraspObjectPositionData creatureData = AttackTowers[towerid - 4].CreatureTop[i];
            if (Creature* creature = m_WG->SpawnCreature(creatureData.entryHorde, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_HORDE))
                m_CreatureTopList[TEAM_HORDE].insert(creature->GetGUID());
            if (Creature* creature = m_WG->SpawnCreature(creatureData.entryAlliance, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_ALLIANCE))
                m_CreatureTopList[TEAM_ALLIANCE].insert(creature->GetGUID());
        }
    }

    if (towerid >= 0)
    {
        // Spawn Turret bottom
        for (uint8 i = 0; i < TowerCannon[towerid].nbTowerCannonBottom; i++)
        {
            Position turretPos;
            TowerCannon[towerid].TowerCannonBottom[i].GetPosition(&turretPos);
            if (Creature* turret = m_WG->SpawnCreature(BATTLEFIELD_WG_TOWER_CANNON, turretPos, TEAM_ALLIANCE))
            {
                m_TowerCannonBottomList.insert(turret->GetGUID());
                switch (go->GetEntry())
                {
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_1:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_2:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_3:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_4:
                        turret->setFaction(WintergraspFaction[m_WG->GetDefenderTeam()]);
                        break;
                    case BATTLEFIELD_WG_GO_SHADOWSIGHT_TOWER:
                    case BATTLEFIELD_WG_GO_WINTER_S_EDGE_TOWER:
                    case BATTLEFIELD_WG_GO_FLAMEWATCH_TOWER:
                        turret->setFaction(WintergraspFaction[m_WG->GetAttackerTeam()]);
                        break;
                }
                m_WG->HideNpc(turret);
            }
        }

        // Spawn Turret top
        for (uint8 i = 0; i < TowerCannon[towerid].nbTurretTop; i++)
        {
            Position towerCannonPos;
            TowerCannon[towerid].TurretTop[i].GetPosition(&towerCannonPos);
            if (Creature* turret = m_WG->SpawnCreature(BATTLEFIELD_WG_TOWER_CANNON, towerCannonPos, TeamId(0)))
            {
                m_TurretTopList.insert(turret->GetGUID());
                switch (go->GetEntry())
                {
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_1:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_2:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_3:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_4:
                        turret->setFaction(WintergraspFaction[m_WG->GetDefenderTeam()]);
                        break;
                    case BATTLEFIELD_WG_GO_SHADOWSIGHT_TOWER:
                    case BATTLEFIELD_WG_GO_WINTER_S_EDGE_TOWER:
                    case BATTLEFIELD_WG_GO_FLAMEWATCH_TOWER:
                        turret->setFaction(WintergraspFaction[m_WG->GetAttackerTeam()]);
                        break;
                }
                m_WG->HideNpc(turret);
            }
        }
        UpdateCreatureAndGo();
    }
}

void BfWGGameObjectBuilding::UpdateCreatureAndGo()
{
    for (GuidSet::const_iterator itr = m_CreatureTopList[m_WG->GetDefenderTeam()].begin(); itr != m_CreatureTopList[m_WG->GetDefenderTeam()].end(); ++itr)
        if (Creature* creature = m_WG->GetCreature(*itr))
            m_WG->HideNpc(creature);

    for (GuidSet::const_iterator itr = m_CreatureTopList[m_WG->GetAttackerTeam()].begin(); itr != m_CreatureTopList[m_WG->GetAttackerTeam()].end(); ++itr)
        if (Creature* creature = m_WG->GetCreature(*itr))
            m_WG->ShowNpc(creature, true);

    for (GuidSet::const_iterator itr = m_CreatureBottomList[m_WG->GetDefenderTeam()].begin(); itr != m_CreatureBottomList[m_WG->GetDefenderTeam()].end(); ++itr)
        if (Creature* creature = m_WG->GetCreature(*itr))
            m_WG->HideNpc(creature);

    for (GuidSet::const_iterator itr = m_CreatureBottomList[m_WG->GetAttackerTeam()].begin(); itr != m_CreatureBottomList[m_WG->GetAttackerTeam()].end(); ++itr)
        if (Creature* creature = m_WG->GetCreature(*itr))
            m_WG->ShowNpc(creature, true);

    for (GuidSet::const_iterator itr = m_GameObjectList[m_WG->GetDefenderTeam()].begin(); itr != m_GameObjectList[m_WG->GetDefenderTeam()].end(); ++itr)
        if (GameObject* object = m_WG->GetGameObject(*itr))
            object->SetRespawnTime(RESPAWN_ONE_DAY);

    for (GuidSet::const_iterator itr = m_GameObjectList[m_WG->GetAttackerTeam()].begin(); itr != m_GameObjectList[m_WG->GetAttackerTeam()].end(); ++itr)
        if (GameObject* object = m_WG->GetGameObject(*itr))
            object->SetRespawnTime(RESPAWN_IMMEDIATELY);
}

void BfWGGameObjectBuilding::UpdateTurretAttack(bool disable)
{
    for (GuidSet::const_iterator itr = m_TowerCannonBottomList.begin(); itr != m_TowerCannonBottomList.end(); ++itr)
    {
        if (Creature* creature = m_WG->GetCreature(*itr))
        {
            if (GameObject* build = m_WG->GetGameObject(m_BuildGUID))
            {
                if (disable)
                    m_WG->HideNpc(creature);
                else
                    m_WG->ShowNpc(creature, true);

                switch (build->GetEntry())
                {
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_1:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_2:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_3:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_4:
                    {
                        creature->setFaction(WintergraspFaction[m_WG->GetDefenderTeam()]);
                        break;
                    }
                    case BATTLEFIELD_WG_GO_SHADOWSIGHT_TOWER:
                    case BATTLEFIELD_WG_GO_WINTER_S_EDGE_TOWER:
                    case BATTLEFIELD_WG_GO_FLAMEWATCH_TOWER:
                    {
                        creature->setFaction(WintergraspFaction[m_WG->GetAttackerTeam()]);
                        break;
                    }
                }
            }
        }
    }

    for (GuidSet::const_iterator itr = m_TurretTopList.begin(); itr != m_TurretTopList.end(); ++itr)
    {
        if (Creature* creature = m_WG->GetCreature(*itr))
        {
            if (GameObject* build = m_WG->GetGameObject(m_BuildGUID))
            {
                if (disable)
                    m_WG->HideNpc(creature);
                else
                    m_WG->ShowNpc(creature, true);

                switch (build->GetEntry())
                {
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_1:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_2:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_3:
                    case BATTLEFIELD_WG_GO_FORTRESS_TOWER_4:
                    {
                        creature->setFaction(WintergraspFaction[m_WG->GetDefenderTeam()]);
                        break;
                    }
                    case BATTLEFIELD_WG_GO_SHADOWSIGHT_TOWER:
                    case BATTLEFIELD_WG_GO_WINTER_S_EDGE_TOWER:
                    case BATTLEFIELD_WG_GO_FLAMEWATCH_TOWER:
                    {
                        creature->setFaction(WintergraspFaction[m_WG->GetAttackerTeam()]);
                        break;
                    }
                }
            }
        }
    }
}

void BfWGGameObjectBuilding::Save()
{
    sWorld->setWorldState(m_WorldState, m_State);
}

BfGraveyardWG::BfGraveyardWG(BattlefieldWG* battlefield) : BfGraveyard(battlefield)
{
    m_Bf = battlefield;
    m_GossipTextId = 0;
}