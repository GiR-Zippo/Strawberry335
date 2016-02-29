/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 26.08.2014
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

#include "Encounter_GrandChampions.h"
#include"../TrialOfTheChampion.h"


void Encounter_GrandChampions::OnFail()
{
    // Check encounter data
    if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) == FAIL)
        return;

    switch (GetState())
    {
        case ENCOUNTER_STATE_INTRO:
            closeDoor();
        // No break;
        case ENCOUNTER_STATE_PRE_ENCOUNTER:
        {
            // Despawn adds
            for (uint8 i = 0; i < 3; ++i)
                for (uint8 j = 0; j < 3; ++j)
                    if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[i][j]))
                        pCreature->DespawnOrUnsummon();

            // Despawn boss
            for (uint8 i = 0; i < 3; ++i)
                if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[i]))
                    pCreature->DespawnOrUnsummon();

            // Set announcer
            if (GET_CREATURE_ANNOUNCER(Creature* pAnnouncer))
                pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }break;

        case ENCOUNTER_STATE_ENCOUNTER:
        {
            // Set Data
            SetData(ENCOUNTER_DATA_GC_BOSS_DEFEATED, 3);
        }break;
    }

    // Despawn trigger
    if (GET_CREATURE_TRIGGER(Creature* pTrigger))
        pTrigger->DespawnOrUnsummon();

    // Set encounter data
    pInstance->SetData(BOSS_GRAND_CHAMPIONS, FAIL);
}

void Encounter_GrandChampions::onDataChanged(uint32 type)
{
    uint32 data = 0;
    if (!GetData(type, data))
        ASSERT(false);

    if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) != IN_PROGRESS
        && pInstance->GetData(BOSS_GRAND_CHAMPIONS) != SPECIAL)
        return;

    switch (type)
    {
        case ENCOUNTER_DATA_GC_ADDS_DEFEATED:
        {
            switch (data)
            {
                case 3: // Start second wave
                    eventMap.ScheduleEvent(2, 3 * IN_MILLISECONDS);
                    break;

                case 6: // Start third wave
                    eventMap.ScheduleEvent(3, 3 * IN_MILLISECONDS);
                    break;

                case 9:// Start boss
                    eventMap.ScheduleEvent(4, 3 * IN_MILLISECONDS);
                    break;
            }
        }break;

        case ENCOUNTER_DATA_GC_BOSS_DEFEATED:
        {
            switch (data)
            {
                case 3: // Move player to safety position and boss to door
                    eventMap.ScheduleEvent(5, 3 * IN_MILLISECONDS);
                    break;

                case 6: // Encounter done
                {
                    // Move boss to despawn position
                    for (uint8 i = 0; i < 3; ++i)
                    {
                        if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[i]))
                        {
                            pCreature->RemoveAllAuras();
                            pCreature->GetMotionMaster()->MovePoint(ENCOUNTER_WP_DESPAWN_POSITION, SpawnPosition);
                        }
                    }

                    // Despawn trigger
                    if (GET_CREATURE_TRIGGER(Creature* pTrigger))
                        pTrigger->DespawnOrUnsummon();

                    // Set encounter done
                    pInstance->SetData(BOSS_GRAND_CHAMPIONS, DONE);

                    // Inform instance that encounter done
                    pInstance->DoAction(INSTANCE_ACTION_ENCOUNTER_DONE);
                }break;
            }
        }break;
    }
}

void Encounter_GrandChampions::onStart()
{
    // Spawn invisible orientation trigger
    SUMMON_CREATURE_TRIGGER(pInstance->GetData(DATA_TEAM) == ALLIANCE ? AllianceTriggerPosition : AnnouncerPosition[0]);

    // Set entries for champions
    setGrandChampionsForEncounter();

    // Set encounter data
    pInstance->SetData(BOSS_GRAND_CHAMPIONS, IN_PROGRESS);
}

void Encounter_GrandChampions::executeIntro(uint32 eventID)
{
    GET_CREATURE_ANNOUNCER(Creature* pAnnouncer);
    GET_CREATURE_TRIGGER(Creature* pTrigger);
    ASSERT(pAnnouncer != NULL && pTrigger != NULL);

    switch (eventID)
    {
        case 1: // Tirion intro
        {
            pAnnouncer->AI()->Talk(SAY_ANNOUNCER_INTRO_PLAYER);

            eventMap.ScheduleEvent(2, 7 * IN_MILLISECONDS);
        }break;

        case 2: // Announce depending on player
        {
            if (Player* pPlayer = pInstanceMap->GetPlayers().begin()->getSource())
                AnnounceChampion(pPlayer, pAnnouncer);

            eventMap.ScheduleEvent(3, 8 * IN_MILLISECONDS);
        }break;

        case 3: // Tirion say
        {
            if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_TIRION)))
                pCreature->AI()->Talk(SAY_TIRION_INTRO_CHAMPS_1);

            eventMap.ScheduleEvent(4, 8 * IN_MILLISECONDS);
        }break;

        case 4: // Thrall or Varian say
        {
            if (pInstance->GetData(DATA_TEAM) == HORDE)
            {
                if (Creature* pThrall = pInstanceMap->GetCreature(pInstance->GetData64(NPC_THRALL)))
                    pThrall->AI()->Talk(SAY_THRALL_INTRO_CHAMPS_H);

                eventMap.ScheduleEvent(5, 5 * IN_MILLISECONDS);
            }
            else
            {
                if (Creature* pVarian = pInstanceMap->GetCreature(pInstance->GetData64(NPC_VARIAN)))
                    pVarian->AI()->Talk(SAY_VARIAN_INTRO_CHAMPS_A);

                eventMap.ScheduleEvent(5, 8 * IN_MILLISECONDS);
            }
        }break;

        case 5: // Garrosh or Jaina say
        {
            if (pInstance->GetData(DATA_TEAM) == HORDE)
            {
                if (Creature* pGarrosh = pInstanceMap->GetCreature(pInstance->GetData64(NPC_GARROSH)))
                    pGarrosh->AI()->Talk(SAY_GARROSH_INTRO_CHAMPS_H);

                eventMap.ScheduleEvent(6, 8 * IN_MILLISECONDS);
            }
            else
            {
                if (Creature* pJaina = pInstanceMap->GetCreature(pInstance->GetData64(NPC_JAINA)))
                    pJaina->AI()->Talk(SAY_JAINA_INTRO_CHAMPS_A);

                eventMap.ScheduleEvent(6, 5 * IN_MILLISECONDS);
            }
        }break;

        case 6: // Garrosh or Varian say (Other team)
        {
            if (pInstance->GetData(DATA_TEAM) == HORDE)
            {
                if (Creature* pVarian = pInstanceMap->GetCreature(pInstance->GetData64(NPC_VARIAN)))
                    pVarian->AI()->Talk(SAY_VARIAN_INTRO_CHAMPS_H);

                eventMap.ScheduleEvent(7, 8 * IN_MILLISECONDS);
            }
            else
            {
                if (Creature* pGarrosh = pInstanceMap->GetCreature(pInstance->GetData64(NPC_GARROSH)))
                    pGarrosh->AI()->Talk(SAY_GARROSH_INTRO_CHAMPS_A);

                eventMap.ScheduleEvent(7, 8 * IN_MILLISECONDS);
            }
        }break;

        case 7: // Trall or Jaina say (Other team)
        {
            if (pInstance->GetData(DATA_TEAM) == HORDE)
            {
                if (Creature* pJaina = pInstanceMap->GetCreature(pInstance->GetData64(NPC_JAINA)))
                    pJaina->AI()->Talk(SAY_JAINA_INTRO_CHAMPS_H);
            }
            else
            {
                if (Creature* pThrall = pInstanceMap->GetCreature(pInstance->GetData64(NPC_THRALL)))
                    pThrall->AI()->Talk(SAY_THRALL_INTRO_CHAMPS_A);
            }

            eventMap.ScheduleEvent(8, 5 * IN_MILLISECONDS);
        }break;

        case 8: // Tirion say and open door
        {
            if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_TIRION)))
                pCreature->AI()->Talk(SAY_TIRION_INTRO_CHAMPS_2);

            openDoor();

            eventMap.ScheduleEvent(9, 7 * IN_MILLISECONDS);
        }break;

        case 9: // Summon 1st Boss and adds and make them follow him
        {
            spawnGroup(pAnnouncer, 0);
            eventMap.ScheduleEvent(10, 2000);
        }break;

        case 10: // Move first pBoss to the new position
        {
            if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[0]))
            {
                pCreature->SetTarget(NULL);
                pCreature->GetMotionMaster()->MovePoint(0, getBossPosition(0));
            }

            eventMap.ScheduleEvent(11, 5 * IN_MILLISECONDS);
        }break;

        case 11: // Refresh the adds position
        {
            refreshGroup(pTrigger, 1);
            eventMap.ScheduleEvent(12, 3 * IN_MILLISECONDS);
        }break;

        case 12: // Summon 2nd Boss and adds and make them follow him
        {
            spawnGroup(pAnnouncer, 1);
            eventMap.ScheduleEvent(13, 5 * IN_MILLISECONDS);
        }break;

        case 13:
        {
            // Move first boss to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[0]))
                pCreature->GetMotionMaster()->MovePoint(0, getBossPosition(1));

            // Move second boss to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[1]))
            {
                pCreature->SetTarget(NULL);
                pCreature->GetMotionMaster()->MovePoint(0, getBossPosition(0));
            }

            eventMap.ScheduleEvent(14, 5 * IN_MILLISECONDS);
        }break;

        case 14: // Refresh the adds position
        {
            refreshGroup(pTrigger, 2);
            eventMap.ScheduleEvent(15, 4 * IN_MILLISECONDS);
        }break;

        case 15: // Summon 3rd Boss and adds and make them follow him
        {
            spawnGroup(pAnnouncer, 2);
            eventMap.ScheduleEvent(16, 4 * IN_MILLISECONDS);
        }break;

        case 16:
        {
            // Move first pBoss to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[0]))
                pCreature->GetMotionMaster()->MovePoint(0, getBossPosition(2));

            // Move second pBoss to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[1]))
                pCreature->GetMotionMaster()->MovePoint(0, getBossPosition(1));

            // Move third pBoss to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID[2]))
            {
                pCreature->SetTarget(NULL);
                pCreature->GetMotionMaster()->MovePoint(0, getBossPosition(0));
            }

            eventMap.ScheduleEvent(17, 5 * IN_MILLISECONDS);
        }break;

        case 17: // Refresh positions
        {
            refreshGroup(pTrigger, 3);
            eventMap.ScheduleEvent(18, 4 * IN_MILLISECONDS);
        }break;

        case 18:
        {
            // Set home positions, in case of wipe, this avoids summons to go to the SpawnPos
            for (uint8 i = 0; i < 3; ++i)
            {
                if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID[i]))
                {
                    pBoss->CastSpell(pBoss, SPELL_MOUNT_LANCE_STAND, true);
                    pBoss->SetHomePosition(pBoss->GetPosition());
                }
            }

            for (uint8 i = 0; i < 3; ++i)
            {
                for (uint8 j = 0; j < 3; ++j)
                {
                    if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[i][j]))
                    {
                        pAdd->CastSpell(pAdd, SPELL_MOUNT_LANCE_STAND, true);
                        pAdd->SetHomePosition(pAdd->GetPosition());
                    }
                }
            }

            eventMap.ScheduleEvent(19, 5 * IN_MILLISECONDS);
        }break;

        case 19: // Close the door
        {
            closeDoor();
            eventMap.ScheduleEvent(20, 3 * IN_MILLISECONDS);
        }break;

        case 20: // Forward to state PreEncounter
        {
            startNext();
        }break;
    }
}

void Encounter_GrandChampions::executePreEncounter(uint32 eventID)
{
    switch (eventID)
    {
        case 1: // Start event
        case 2: // Start next adds
        case 3: // Start next adds
        {
            startAdds(eventID - 1);
        }break;

        case 4: // Start boss + despawn previous adds
        {
            for (uint8 i = 0; i < 3; ++i)
            {
                if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[2][i]))
                    pAdd->DespawnOrUnsummon();

                if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID[i]))
                {
                    pBoss->RemoveAurasDueToSpell(SPELL_MOUNT_LANCE_STAND);
                    pBoss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    pBoss->SetReactState(REACT_AGGRESSIVE);
                    pBoss->setFaction(16);
                    AggroAllPlayers(pBoss);
                }
            }
        }break;

        case 5: // Dismount players, despawn mounts and spawn new boss
        {
            AreAllPlayersMounted(pInstanceMap, true);

            for (uint8 i = 0; i < 3; ++i)
                if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID[i]))
                    pBoss->DespawnOrUnsummon();

            // Set intro finished
            pInstance->SetData(BOSS_GRAND_CHAMPIONS, SPECIAL);

            Position DifferentSpawnPosition = GrandChampionSpawnPosition;

            for (uint8 i = 0; i < 3; ++i)
            {
                DifferentSpawnPosition.m_positionX = GrandChampionSpawnPosition.m_positionX;

                switch (i)
                {
                    case 1: { DifferentSpawnPosition.m_positionX -= 10.0f; }break;
                    case 2: { DifferentSpawnPosition.m_positionX += 10.0f; }break;
                }

                if (Creature* pBoss = pInstanceMap->SummonCreatureOnMap(bossEntry[i], DifferentSpawnPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
                {
                    if (GET_CREATURE_ANNOUNCER(Creature* pAnnouncer))
                        pBoss->SetTarget(pAnnouncer->GetGUID());

                    // Save current boss entry to inst for achievement check
                    pInstance->SetData(DATA_GRAND_CHAMPION_ENTRY, pBoss->GetEntry());

                    // Prevent bosses from falling down the ground
                    pBoss->SetPosition(pBoss->GetPositionX(), pBoss->GetPositionY(), pBoss->GetPositionZ() + 0.1f, pBoss->GetOrientation());
                    pBoss->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                    bossGUID[i] = pBoss->GetGUID();
                }
            }

            eventMap.ScheduleEvent(6, 3 * IN_MILLISECONDS);
        }break;

        case 6: // Set boss attackable
        {
            for (uint8 i = 0; i < 3; ++i)
            {
                if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID[i]))
                {
                    pBoss->SetTarget(NULL);
                    pBoss->SetFacingTo(4.7f);
                    pBoss->SetHomePosition(pBoss->GetPosition());
                    pBoss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    pBoss->SetReactState(REACT_AGGRESSIVE);
                }
            }

            // Encounter_GrandChampions finished
            startNext();
        }break;
    }
}

uint32 Encounter_GrandChampions::getAddEntry(uint32 bossEntry)
{
    switch (bossEntry)
    {
        // Alliance
        case NPC_JACOB:   return NPC_STORMWIND_CHAMPION;
        case NPC_AMBROSE: return NPC_GNOMEREGAN_CHAMPION;
        case NPC_COLOSOS: return NPC_EXODAR_CHAMPION;
        case NPC_JAELYNE: return NPC_DARNASSUS_CHAMPION;
        case NPC_LANA:    return NPC_IRONFORGE_CHAMPION;

        // Horde
        case NPC_MOKRA:   return NPC_ORGRIMMAR_CHAMPION;
        case NPC_ERESSEA: return NPC_SILVERMOON_CHAMPION;
        case NPC_RUNOK:   return NPC_THUNDER_BLUFF_CHAMPION;
        case NPC_ZULTORE: return NPC_SENJIN_CHAMPION;
        case NPC_VISCERI: return NPC_UNDERCITY_CHAMPION;
    }

    return 0;
}

const Position& Encounter_GrandChampions::getBossPosition(uint8 positionId) const
{
    ASSERT(positionId < 3);

    uint8 teamArray = pInstance->GetData(DATA_TEAM) == ALLIANCE ? 0 : 1;
    return FactionChampionPos[teamArray][positionId];
}

void Encounter_GrandChampions::setGrandChampionsForEncounter()
{
    // Clear entries
    memset(&bossEntry, NULL, sizeof(bossEntry));
    uint32 team = pInstance->GetData(DATA_TEAM);

    if (team == HORDE)
        bossEntry[0] = RAND(NPC_JACOB, NPC_LANA);
    else
        bossEntry[0] = RAND(NPC_MOKRA, NPC_VISCERI);

    while (bossEntry[1] == bossEntry[0] || bossEntry[2] == bossEntry[0] || bossEntry[2] == bossEntry[1])
    {
        if (team == HORDE)
        {
            bossEntry[1] = RAND(NPC_JACOB, NPC_AMBROSE, NPC_COLOSOS, NPC_JAELYNE, NPC_LANA);
            bossEntry[2] = RAND(NPC_JACOB, NPC_AMBROSE, NPC_COLOSOS, NPC_JAELYNE, NPC_LANA);
        }
        else
        {
            bossEntry[1] = RAND(NPC_MOKRA, NPC_ERESSEA, NPC_RUNOK, NPC_ZULTORE, NPC_VISCERI);
            bossEntry[2] = RAND(NPC_MOKRA, NPC_ERESSEA, NPC_RUNOK, NPC_ZULTORE, NPC_VISCERI);
        }
    }
}

void Encounter_GrandChampions::spawnGroup(Creature* pAnnouncer, uint8 index)
{
    ASSERT(index <= 2);

    if (Creature* pBoss = pInstanceMap->SummonCreatureOnMap(bossEntry[index], SpawnPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
    {
        bossGUID[index] = pBoss->GetGUID();
        pBoss->SetTarget(pAnnouncer->GetGUID());
        AnnounceChampion(pBoss->ToUnit(), pAnnouncer);

        pBoss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pBoss->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        pBoss->SetReactState(REACT_PASSIVE);
        pBoss->setFaction(35);

        Position AddSpawnPosition = SpawnPosition;
        AddSpawnPosition.m_positionX += 5.0f;
        AddSpawnPosition.m_positionY -= 5.0f;

        for (uint8 i = 0; i < 3; ++i)
        {
            if (Creature* pAdd = pInstanceMap->SummonCreatureOnMap(getAddEntry(pBoss->GetEntry()), AddSpawnPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
            {
                pAdd->SetTarget(pAnnouncer->GetGUID());
                addsGUID[index][i] = pAdd->GetGUID();

                pAdd->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pAdd->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                pAdd->SetReactState(REACT_PASSIVE);
                pAdd->setFaction(35);

                switch (i)
                {
                    case 0:
                        pAdd->GetMotionMaster()->MoveFollow(pBoss, 2.0f, M_PI);
                        AddSpawnPosition.m_positionX -= 5.0f;
                        break;
                    case 1:
                        pAdd->GetMotionMaster()->MoveFollow(pBoss, 2.0f, M_PI / 2);
                        AddSpawnPosition.m_positionX -= 5.0f;
                        break;
                    case 2:
                        pAdd->GetMotionMaster()->MoveFollow(pBoss, 2.0f, M_PI / 2 + M_PI);
                        break;
                }
            }
        }
    }
}

void Encounter_GrandChampions::refreshGroup(Creature* pCreature, uint8 maxIndex)
{
    ASSERT(maxIndex <= 3);

    for (uint8 i = 0; i < maxIndex; ++i)
    {
        if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID[i]))
        {
            pBoss->SetFacingToObject(pCreature);

            for (uint8 j = 0; j < 3; ++j)
            {
                if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[i][j]))
                {
                    switch (j)
                    {
                        case 0:
                            pAdd->GetMotionMaster()->MoveFollow(pBoss, 2.0f, M_PI);
                            break;
                        case 1:
                            pAdd->GetMotionMaster()->MoveFollow(pBoss, 2.0f, M_PI / 2);
                            break;
                        case 2:
                            pAdd->GetMotionMaster()->MoveFollow(pBoss, 2.0f, M_PI / 2 + M_PI);
                            break;
                    }
                }
            }
        }
    }
}

void Encounter_GrandChampions::startAdds(uint8 bossId)
{
    ASSERT(bossId < 3);

    // Despawn previous wave
    if (bossId != 0)
    {
        for (uint8 i = 0; i < 3; ++i)
            if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[bossId - 1][i]))
                pAdd->DespawnOrUnsummon();
    }

    for (uint8 i = 0; i < 3; ++i)
    {
        if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[bossId][i]))
        {
            pAdd->RemoveAurasDueToSpell(SPELL_MOUNT_LANCE_STAND);
            pAdd->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            pAdd->SetReactState(REACT_AGGRESSIVE);
            pAdd->setFaction(16);
            AggroAllPlayers(pAdd);
        }
    }
}
