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

#include "Workshop_BattlefieldWG.h"
#include "Main_BattlefieldWG.h"


WintergraspWorkshopData::WintergraspWorkshopData(BattlefieldWG* WG, uint8 workshopId) : m_WG(WG), m_workshopId(workshopId)
{
    m_FactoryGUID = 0;
    m_State = BATTLEFIELD_WG_OBJECTSTATE_NONE;
    m_TeamControl = BATTLEFIELD_WG_TEAM_NEUTRAL;
}

void WintergraspWorkshopData::AddCreature(const WintergraspObjectPositionData& obj)
{
    if (Creature* pCreature = m_WG->SpawnCreature(obj.entryHorde, obj.x, obj.y, obj.z, obj.o, TEAM_HORDE))
        m_CreatureOnPoint[TEAM_HORDE].insert(pCreature->GetGUID());
    if (Creature* pCreature = m_WG->SpawnCreature(obj.entryAlliance, obj.x, obj.y, obj.z, obj.o, TEAM_ALLIANCE))
        m_CreatureOnPoint[TEAM_ALLIANCE].insert(pCreature->GetGUID());
}

void WintergraspWorkshopData::AddGameObject(const WintergraspObjectPositionData& obj)
{
    if (GameObject* gameobject = m_WG->SpawnGameObject(obj.entryHorde, obj.x, obj.y, obj.z, obj.o))
        m_GameObjectOnPoint[TEAM_HORDE].insert(gameobject->GetGUID());
    if (GameObject* gameobject = m_WG->SpawnGameObject(obj.entryAlliance, obj.x, obj.y, obj.z, obj.o))
        m_GameObjectOnPoint[TEAM_ALLIANCE].insert(gameobject->GetGUID());
}

void WintergraspWorkshopData::AddRocketLauncher(const WintergraspObjectPositionData& obj)
{
    if (Creature* pCreature = m_WG->SpawnCreature(obj.entryHorde, obj.x, obj.y, obj.z, obj.o, TEAM_NEUTRAL))
    {
        m_RocketLauncherOnPoint.insert(pCreature->GetGUID());
        m_WG->HideNpc(pCreature); // Hide per default
    }
}

void WintergraspWorkshopData::Init(uint8 team)
{
    // Outside Workshops
    if (m_workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
    {
        // Set worldstate and nameId
        m_WorldState = CapturableWorkshops[m_workshopId].worldState;
        m_NameId = CapturableWorkshops[m_workshopId].nameId;

        // Spawning Workshop
        WintergraspObjectPositionData workshop = CapturableWorkshops[m_workshopId].GameObjectMain[0];
        GameObject* pWorkshop = m_WG->SpawnGameObject(workshop.entryHorde, workshop.x, workshop.y, workshop.z, workshop.o);
        if (!pWorkshop)
            return;

        // Spawning Engineer, save them separate
        WintergraspObjectPositionData engineer = CapturableWorkshops[m_workshopId].Engineer;
        if (Creature* pEngineer = m_WG->SpawnCreature(engineer.entryHorde, engineer.x, engineer.y, engineer.z, engineer.o, TEAM_HORDE))
            m_Engineer[TEAM_HORDE] = pEngineer->GetGUID();
        if (Creature* pEngineer = m_WG->SpawnCreature(engineer.entryAlliance, engineer.x, engineer.y, engineer.z, engineer.o, TEAM_ALLIANCE))
            m_Engineer[TEAM_ALLIANCE] = pEngineer->GetGUID();

        // Spawning Spirit Keeper and link him to the graveyard
        if (BfGraveyard* pGrave = m_WG->GetGraveyardById(m_workshopId))
        {
            WintergraspObjectPositionData spirit = CapturableWorkshops[m_workshopId].SpiritGuide;
            if (Creature* pSpirit = m_WG->SpawnCreature(spirit.entryHorde, spirit.x, spirit.y, spirit.z, spirit.o, TEAM_HORDE))
                // Link spirit to graveyard
                pGrave->SetSpirit(pSpirit, TEAM_HORDE);
            if (Creature* pSpirit = m_WG->SpawnCreature(spirit.entryAlliance, spirit.x, spirit.y, spirit.z, spirit.o, TEAM_ALLIANCE))
                // Link spirit to graveyard
                pGrave->SetSpirit(pSpirit, TEAM_ALLIANCE);
        }

        // Spawning Creature and NPC
        for (uint8 i = 0; i < CapturableWorkshops[m_workshopId].nbCreature; ++i)
            AddCreature(CapturableWorkshops[m_workshopId].Creature[i]);

        // Spawning Banner
        for (uint8 i = 0; i < CapturableWorkshops[m_workshopId].nbBanner; ++i)
            AddGameObject(CapturableWorkshops[m_workshopId].GameObjectBanner[i]);

        // Spawning Rocket Launcher
        for (uint8 i = 0; i < CapturableWorkshops[m_workshopId].nbRocketLauncher; ++i)
            AddRocketLauncher(CapturableWorkshops[m_workshopId].RocketLauncher[i]);
    }
    // Fortress Workshops
    else
    {
        // Array support (only 2 elements - 4, 5 are m_workshopIds)
        uint8 workshopId = m_workshopId - WG_CAPTURABLE_WORKSHOP;

        // Set worldstate
        m_WorldState = FortressWorkshops[workshopId].worldState;

        // Spawning Workshop
        WintergraspObjectPositionData workshop = FortressWorkshops[workshopId].Workshop;
        GameObject* pWorkshop = m_WG->SpawnGameObject(workshop.entryHorde, workshop.x, workshop.y, workshop.z, workshop.o);
        if (!pWorkshop)
            return;

        // Spawning Engineer
        WintergraspObjectPositionData engineer = FortressWorkshops[workshopId].Engineer;
        if (Creature* pEngineer = m_WG->SpawnCreature(engineer.entryHorde, engineer.x, engineer.y, engineer.z, engineer.o, TEAM_HORDE))
            m_Engineer[TEAM_HORDE] = pEngineer->GetGUID();
        if (Creature* pEngineer = m_WG->SpawnCreature(engineer.entryAlliance, engineer.x, engineer.y, engineer.z, engineer.o, TEAM_ALLIANCE))
            m_Engineer[TEAM_ALLIANCE] = pEngineer->GetGUID();

        // Spawning Spirit Keeper and link him to the graveyard (only one graveyard in the keep - linked to the west)
        if (m_workshopId == BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
        {
            if (BfGraveyard* pGrave = m_WG->GetGraveyardById(m_workshopId))
            {
                WintergraspObjectPositionData spirit = FortressWorkshops[workshopId].SpiritGuide;
                if (Creature* pSpirit = m_WG->SpawnCreature(spirit.entryHorde, spirit.x, spirit.y, spirit.z, spirit.o, TEAM_HORDE))
                    // Link spirit to graveyard
                    pGrave->SetSpirit(pSpirit, TEAM_HORDE);
                if (Creature* pSpirit = m_WG->SpawnCreature(spirit.entryAlliance, spirit.x, spirit.y, spirit.z, spirit.o, TEAM_ALLIANCE))
                    // Link spirit to graveyard
                    pGrave->SetSpirit(pSpirit, TEAM_ALLIANCE);
            }
        }
    }

    // Finally change faction
    GiveControlTo(team);
}

void WintergraspWorkshopData::GiveControlTo(uint8 team)
{
    // Find associate graveyard and update it
    if (m_workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_EAST)
        if (m_WG->GetGraveyardById(m_workshopId))
            m_WG->GetGraveyardById(m_workshopId)->GiveControlTo(TeamId(team));

    if (team != TEAM_NEUTRAL)
    {
        // Hide current creatures
        for (GuidSet::const_iterator itr = m_CreatureOnPoint[m_WG->GetOtherTeam(TeamId(team))].begin(); itr != m_CreatureOnPoint[m_WG->GetOtherTeam(TeamId(team))].end(); ++itr)
            if (Creature* pCreature = m_WG->GetCreature(*itr))
                m_WG->HideNpc(pCreature);

        // Show controller creatures
        for (GuidSet::const_iterator itr = m_CreatureOnPoint[TeamId(team)].begin(); itr != m_CreatureOnPoint[TeamId(team)].end(); ++itr)
        {
            if (Creature* pCreature = m_WG->GetCreature(*itr))
            {
                m_WG->ShowNpc(pCreature, (pCreature->GetEntry() != BATTLEFIELD_WG_NPC_GNOMISH_MECHANIC_ENGINEER
                    && pCreature->GetEntry() != BATTLEFIELD_WG_NPC_GOBLIN_MECHANIC_ENGINEER));
                pCreature->SetPosition(pCreature->GetHomePosition());
            }
        }

        // Hide current gameobjects
        for (GuidSet::const_iterator itr = m_GameObjectOnPoint[m_WG->GetOtherTeam(TeamId(team))].begin(); itr != m_GameObjectOnPoint[m_WG->GetOtherTeam(TeamId(team))].end(); ++itr)
            if (GameObject* object = m_WG->GetGameObject(*itr))
                object->SetRespawnTime(RESPAWN_ONE_DAY);

        // Show controller gameobjects
        for (GuidSet::const_iterator itr = m_GameObjectOnPoint[TeamId(team)].begin(); itr != m_GameObjectOnPoint[TeamId(team)].end(); ++itr)
            if (GameObject* object = m_WG->GetGameObject(*itr))
                object->SetRespawnTime(RESPAWN_IMMEDIATELY);
    }

    switch (team)
    {
        case BATTLEFIELD_WG_TEAM_NEUTRAL:
        {
            // Send warning message to all player for inform a faction attack a workshop
            // alliance / horde attacking workshop
            m_WG->SendWarningToAllInZone(m_TeamControl ? LANG_WG_WORKSHOP_ATTACK_ALLIANCE : LANG_WG_WORKSHOP_ATTACK_HORDE, m_NameId);

            // Updating worldstate
            m_State = BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_INTACT;
            m_WG->SendUpdateWorldState(m_WorldState, m_State);

            // Hide current Engineer
            if (Creature* pEngineer = m_WG->GetCreature(m_Engineer[m_TeamControl]))
                m_WG->HideNpc(pEngineer);

            // Update GameObject ArtKit of the Factory Banner (only outside workshops)
            if (m_FactoryGUID != 0 && m_WG->IsWarTime())
            {
                GameObject* pFactory = m_WG->GetGameObject(m_FactoryGUID);
                if (pFactory)
                    GameObject::SetGoArtKit(GO_ARTKIT_FACTORY_NEUTRAL, pFactory, pFactory->GetGUIDLow());
            }
        }break;

        case BATTLEFIELD_WG_TEAM_ALLIANCE:
        {
            // Hide Horde Engineer - has to be done at init
            if (Creature* pEngineer = m_WG->GetCreature(m_Engineer[TEAM_HORDE]))
                m_WG->HideNpc(pEngineer);

            // Show Alliance Engineer - If it isn't wartime, hide engineer per default
            if (Creature* pEngineer = m_WG->GetCreature(m_Engineer[TEAM_ALLIANCE]))
            {
                if (m_WG->IsWarTime())
                    m_WG->ShowNpc(pEngineer, false);
                else
                    m_WG->HideNpc(pEngineer);
            }

            // Updating worldstate
            m_State = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT;
            m_WG->SendUpdateWorldState(m_WorldState, m_State);

            // Warning message
            if (m_WG->IsWarTime() && m_workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST) // workshop taken - alliance
                m_WG->SendWarningToAllInZone(LANG_WG_WORKSHOP_TAKEN_ALLIANCE, m_NameId);

            // Update GameObject ArtKit of the Factory Banner (only outside workshops)
            if (m_FactoryGUID != 0 && m_WG->IsWarTime())
            {
                GameObject* pFactory = m_WG->GetGameObject(m_FactoryGUID);
                if (pFactory)
                    GameObject::SetGoArtKit(GO_ARTKIT_FACTORY_ALLIANCE, pFactory, pFactory->GetGUIDLow());
            }
        }break;

        case BATTLEFIELD_WG_TEAM_HORDE:
        {
            // Hide Alliance Engineer - has to be done at init
            if (Creature* pEngineer = m_WG->GetCreature(m_Engineer[TEAM_ALLIANCE]))
                m_WG->HideNpc(pEngineer);

            // Show Horde Engineer - If it isn't wartime, hide engineer per default
            if (Creature* pEngineer = m_WG->GetCreature(m_Engineer[TEAM_HORDE]))
            {
                if (m_WG->IsWarTime())
                    m_WG->ShowNpc(pEngineer, false);
                else
                    m_WG->HideNpc(pEngineer);
            }

            // Update worldstate
            m_State = BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT;
            m_WG->SendUpdateWorldState(m_WorldState, m_State);

            // Warning message
            if (m_WG->IsWarTime() && m_workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST) // workshop taken - horde
                m_WG->SendWarningToAllInZone(LANG_WG_WORKSHOP_TAKEN_HORDE, m_NameId);

            // Update GameObject ArtKit of the Factory Banner (only outside workshops)
            if (m_FactoryGUID != 0 && m_WG->IsWarTime())
            {
                GameObject* pFactory = m_WG->GetGameObject(m_FactoryGUID);
                if (pFactory)
                    GameObject::SetGoArtKit(GO_ARTKIT_FACTORY_HORDE, pFactory, pFactory->GetGUIDLow());
            }
        }break;
    }

    m_TeamControl = team;

    if (!m_RocketLauncherOnPoint.empty())
        for (GuidSet::const_iterator itr = m_RocketLauncherOnPoint.begin(); itr != m_RocketLauncherOnPoint.end(); ++itr)
            if (Creature* pCreature = m_WG->GetCreature(*itr))
                pCreature->setFaction(WintergraspFaction[team]);

    if (m_WG->IsWarTime())
        m_WG->UpdateCounterVehicle(false);
}

void WintergraspWorkshopData::OnBattleStart()
{
    // Update graveyard
    UpdateGraveyard();

    // Show Engineer
    if (Creature* pEngineer = m_WG->GetCreature(m_Engineer[m_TeamControl]))
        m_WG->ShowNpc(pEngineer, false);

    if (m_workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
    {
        // Reset creatures
        for (GuidSet::const_iterator itr = m_CreatureOnPoint[m_TeamControl].begin(); itr != m_CreatureOnPoint[m_TeamControl].end(); ++itr)
            if (Creature* pCreature = m_WG->GetCreature(*itr))
                pCreature->SetPosition(pCreature->GetHomePosition());

        // Show rocket launchers
        for (GuidSet::const_iterator itr = m_RocketLauncherOnPoint.begin(); itr != m_RocketLauncherOnPoint.end(); ++itr)
        {
            if (Creature* pCreature = m_WG->GetCreature(*itr))
            {
                m_WG->ShowNpc(pCreature, false);
                pCreature->setFaction(WintergraspFaction[GetTeam()]);
            }
        }

        // Spawning Factory Banner
        WintergraspObjectPositionData factory = CapturableWorkshops[m_workshopId].GameObjectMain[1];
        GameObject* pFactory = m_WG->SpawnGameObject(factory.entryHorde, factory.x, factory.y, factory.z, factory.o);
        if (!pFactory)
            return;

        // Create and Add Capturable Point
        WintergraspCapturePoint* capturePoint = new WintergraspCapturePoint(m_WG, TeamId(m_TeamControl));
        if (!capturePoint)
            return;

        capturePoint->SetCapturePointData(pFactory);
        capturePoint->LinkToWorkshop(this);
        m_WG->AddCapturablePoint(capturePoint);

        // Save factory banner guid and entry
        m_FactoryGUID = pFactory->GetGUID();
        m_FactoryEntry = pFactory->GetEntry();

        // Update GameObject ArtKit of the Factory Banner (only outside workshops)
        GameObject::SetGoArtKit(m_TeamControl ? GO_ARTKIT_FACTORY_HORDE : GO_ARTKIT_FACTORY_ALLIANCE, pFactory, pFactory->GetGUIDLow());
    }
}

void WintergraspWorkshopData::OnBattleEnd()
{
    if (m_workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
    {
        // Delete Capturable Point
        if (BfCapturePoint* pCp = m_WG->RemoveCapturePointForWorkshop(this))
            delete pCp;
    }

    if (m_workshopId == BATTLEFIELD_WG_WORKSHOP_SE  || m_workshopId == BATTLEFIELD_WG_WORKSHOP_SW)
        GiveControlTo(m_WG->GetAttackerTeam());
    else
        GiveControlTo(m_WG->GetDefenderTeam());

    // Hide rocket launchers
    for (GuidSet::const_iterator itr = m_RocketLauncherOnPoint.begin(); itr != m_RocketLauncherOnPoint.end(); ++itr)
        if (Creature* pCreature = m_WG->GetCreature(*itr))
            m_WG->HideNpc(pCreature);
}

void WintergraspWorkshopData::UpdateGraveyard()
{
    // East Fortress Workshops - no own graveyard
    if (m_workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_EAST)
        m_WG->GetGraveyardById(m_workshopId)->GiveControlTo(TeamId(m_TeamControl));
}

void WintergraspWorkshopData::Save()
{
    sWorld->setWorldState(m_WorldState, m_State);
}

WintergraspCapturePoint::WintergraspCapturePoint(BattlefieldWG* battlefield, TeamId teamInControl) : BfCapturePoint(battlefield)
{
    m_Bf = battlefield;
    m_team = teamInControl;
    m_Workshop = NULL;
}

void WintergraspCapturePoint::ChangeTeam(TeamId /*oldTeam*/)
{
    ASSERT(m_Workshop);
    m_Workshop->GiveControlTo(m_team);
}