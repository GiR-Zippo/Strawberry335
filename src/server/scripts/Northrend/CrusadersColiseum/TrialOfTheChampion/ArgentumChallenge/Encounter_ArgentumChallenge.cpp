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

#include "Encounter_ArgentumChallenge.h"
#include "../TrialOfTheChampion.h"


void Encounter_ArgentumChallenge::DoAction(int32 action)
{
    switch (action)
    {
        case ENCOUNTER_ACTION_AC_START:
        {
            if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_TIRION)))
                pCreature->AI()->Talk(SAY_TIRION_INTRO_ARGENT_2);

            closeDoor();

            for (uint8 i = 0; i < 3; ++i)
            {
                for (uint8 j = 0; j < 3; ++j)
                {
                    if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[i][j]))
                    {
                        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                        pCreature->SetReactState(REACT_AGGRESSIVE);
                        pCreature->setFaction(16);
                    }
                }
            }

            // Encounter_ArgentumChallenge finished
            startNext();
        }break;
    }
}

void Encounter_ArgentumChallenge::OnFail()
{
    // Check encounter data
    if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) == FAIL)
        return;

    switch (GetState())
    {
        case ENCOUNTER_STATE_INTRO:
        {
            closeDoor();

            // Despawn adds
            for (uint8 i = 0; i < 3; ++i)
                for (uint8 j = 0; j < 3; ++j)
                    if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[i][j]))
                        pCreature->DespawnOrUnsummon();

            // Despawn boss
            if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID))
                pCreature->DespawnOrUnsummon();

            // Set announcer
            if (GET_CREATURE_ANNOUNCER(Creature* pAnnouncer))
                pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }break;

        case ENCOUNTER_STATE_ENCOUNTER:
        {
            // Respawn adds
            for (uint8 i = 0; i < 3; ++i)
            {
                for (uint8 j = 0; j < 3; ++j)
                {
                    if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[i][j]))
                    {
                        pAdd->SetPosition(pAdd->GetHomePosition());
                        pAdd->Respawn(true);
                    }
                    else if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID))
                    {
                        // Only proceed if boss hasn't started
                        if (pBoss->getFaction() == 35)
                        {
                            // NPC has been despawned -> spawn new one
                            if (addsEntryAndPositionData[i][j].first != NULL)
                            {
                                if (Creature* pCreature = pInstanceMap->SummonCreatureOnMap(addsEntryAndPositionData[i][j].first,
                                    addsEntryAndPositionData[i][j].second, TEMPSUMMON_MANUAL_DESPAWN, 0))
                                {
                                    pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                                    pCreature->SetReactState(REACT_AGGRESSIVE);
                                    pCreature->setFaction(16);

                                    addsGUID[i][j] = pCreature->GetGUID();
                                }
                            }
                        }
                    }
                }
            }

            // Reset data
            SetData(ENCOUNTER_DATA_AC_ADDS_DEFEATED, 0);
        }break;
    }

    // Despawn trigger
    if (GET_CREATURE_TRIGGER(Creature* pTrigger))
        pTrigger->DespawnOrUnsummon();

    // Set encounter data
    pInstance->SetData(BOSS_ARGENT_CHALLENGE, FAIL);
}

void Encounter_ArgentumChallenge::onDataChanged(uint32 type)
{
    uint32 data = 0;
    if (!GetData(type, data))
        ASSERT(false);

    if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != IN_PROGRESS)
        return;

    switch (type)
    {
        case ENCOUNTER_DATA_AC_ADDS_DEFEATED:
        {
            if (data >= 9) // Start boss if 9 adds are defeated
            {
                // Despawn trigger
                if (GET_CREATURE_TRIGGER(Creature* pTrigger))
                    pTrigger->DespawnOrUnsummon();

                // Despawn adds
                for (uint8 i = 0; i < 3; ++i)
                    for (uint8 j = 0; j < 3; ++j)
                        if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[i][j]))
                            pCreature->DespawnOrUnsummon();

                if (Creature* pCreature = pInstanceMap->GetCreature(bossGUID))
                {
                    pCreature->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                    pCreature->GetMotionMaster()->MovePoint(1, ArgentSoldierPosition[0]);
                }
            }
        }break;
    }
}

void Encounter_ArgentumChallenge::onStart()
{
    // Initialize
    for (uint8 i = 0; i < 3; ++i)
        for (uint8 j = 0; j < 3; ++j)
            addsEntryAndPositionData[i][j].first = NULL;

    // Spawn invisible orientation trigger
    SUMMON_CREATURE_TRIGGER(StadiumPosition);

    // Set encounter data
    pInstance->SetData(BOSS_ARGENT_CHALLENGE, IN_PROGRESS);
}

void Encounter_ArgentumChallenge::executeIntro(uint32 eventID)
{
    GET_CREATURE_ANNOUNCER(Creature* pAnnouncer);
    GET_CREATURE_TRIGGER(Creature* pTrigger);
    ASSERT(pAnnouncer != NULL && pTrigger != NULL);

    switch (eventID)
    {
        case 1:
        {
            if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_TIRION)))
                pCreature->AI()->Talk(SAY_TIRION_INTRO_ARGENT_1);

            openDoor();
            eventMap.ScheduleEvent(2, 2000);
        }break;

        case 2: // Summon 1st wave
        {
            spawnAdds(pTrigger, 0);
            eventMap.ScheduleEvent(3, 2000);
        }break;

        case 3: // Move first wave to his position
        {
            if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[0][0]))
            {
                pCreature->SetTarget(NULL);
                pCreature->GetMotionMaster()->MovePoint(0, ArgentSoldierPosition[0]);
            }

            eventMap.ScheduleEvent(4, 3000);
        }break;

        case 4: // Refresh adds position
        {
            refreshAdds(pTrigger, 1);
            eventMap.ScheduleEvent(5, 3000);
        }break;

        case 5: // Summon 2nd wave
        {
            spawnAdds(pTrigger, 1);
            eventMap.ScheduleEvent(6, 5000);
        }break;

        case 6:
        {
            // Move first wave to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[0][0]))
                pCreature->GetMotionMaster()->MovePoint(0, ArgentSoldierPosition[1]);

            // Move second wave to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[1][0]))
            {
                pCreature->SetTarget(NULL);
                pCreature->GetMotionMaster()->MovePoint(0, ArgentSoldierPosition[0]);
            }

            eventMap.ScheduleEvent(7, 4000);
        }break;

        case 7: // Refresh adds position
        {
            refreshAdds(pTrigger, 2);
            eventMap.ScheduleEvent(8, 4000);
        }break;

        case 8: // Summon 3rd wave
        {
            spawnAdds(pTrigger, 2);
            eventMap.ScheduleEvent(9, 4000);
        }break;

        case 9:
        {
            // Move second wave to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[1][0]))
                pCreature->GetMotionMaster()->MovePoint(0, ArgentSoldierPosition[2]);

            // Move third wave to the new position
            if (Creature* pCreature = pInstanceMap->GetCreature(addsGUID[2][0]))
            {
                pCreature->SetTarget(NULL);
                pCreature->GetMotionMaster()->MovePoint(0, ArgentSoldierPosition[0]);
            }

            eventMap.ScheduleEvent(10, 4000);
        }break;

        case 10: // Refresh adds position
        {
            refreshAdds(pTrigger, 3);
            eventMap.ScheduleEvent(11, 4000);
        }break;

        case 11: // Summon the boss
        {
            bool chance;
            chance = urand(0, 1);

            if (Creature* pCreature = pInstanceMap->SummonCreatureOnMap(chance ? NPC_EADRIC : NPC_PALETRESS, SpawnPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
            {
                AnnounceChampion(pCreature, pAnnouncer);
                bossGUID = pCreature->GetGUID();

                pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                pCreature->SetReactState(REACT_PASSIVE);
                pCreature->setFaction(35);
            }

            eventMap.ScheduleEvent(12, 4000);
        }break;

        case 12: // Set home positions, in case of wipe, this avoids summons goin back to the SpawnPos
        {
            if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID))
            {
                pBoss->SetHomePosition(pBoss->GetPosition());

                switch (pBoss->GetEntry())
                {
                    case NPC_EADRIC:
                        pBoss->AI()->Talk(SAY_EADRIC_INTRO);
                        break;
                    case NPC_PALETRESS:
                        pBoss->AI()->Talk(SAY_PALETRESS_INTRO_1);
                        break;
                }
            }

            for (uint8 i = 0; i < 3; i++)
            {
                for (uint8 j = 0; j < 3; j++)
                {
                    if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[i][j]))
                    {
                        pAdd->SetHomePosition(pAdd->GetPosition());

                        // Save entry and position in case of despawn
                        addsEntryAndPositionData[i][j].first = pAdd->GetEntry();
                        addsEntryAndPositionData[i][j].second = pAdd->GetPosition();
                    }
                }
            }

            eventMap.ScheduleEvent(13, 4000);
        }break;

        case 13: // Paletress Say
        {
            if (Creature* pBoss = pInstanceMap->GetCreature(bossGUID))
            {
                switch (pBoss->GetEntry())
                {
                    case NPC_PALETRESS:
                        pBoss->AI()->Talk(SAY_PALETRESS_INTRO_2);
                        break;
                }
            }

            eventMap.ScheduleEvent(14, 1500);
        }break;

        case 14: // Move Announcer
        {
            pAnnouncer->AI()->DoAction(ANNOUNCER_ACTION_MOVE_HOME);
        }break;
    }
}

void Encounter_ArgentumChallenge::spawnAdds(Creature* pCreature, uint8 index)
{
    ASSERT(index <= 2);

    if (Creature* pFirstAdd = pInstanceMap->SummonCreatureOnMap(NPC_ARGENT_LIGHWIELDER, SpawnPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
    {
        addsGUID[index][0] = pFirstAdd->GetGUID();
        pFirstAdd->SetTarget(pCreature->GetGUID());

        pFirstAdd->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pFirstAdd->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        pFirstAdd->SetReactState(REACT_PASSIVE);
        pFirstAdd->setFaction(35);

        Position AddSpawnPosition = SpawnPosition;
        AddSpawnPosition.m_positionX += 5.0f;
        AddSpawnPosition.m_positionY -= 5.0f;

        for (uint8 i = 1; i < 3; ++i)
        {
            uint32 addEntry = 0;

            switch (i)
            {
                case 1:
                    addEntry = NPC_ARGENT_MONK;
                    break;
                case 2:
                    addEntry = NPC_ARGENT_PRIEST;
                    break;
            }

            if (Creature* pAdd = pInstanceMap->SummonCreatureOnMap(addEntry, AddSpawnPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
            {
                addsGUID[index][i] = pAdd->GetGUID();
                pAdd->SetTarget(pCreature->GetGUID());

                pAdd->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pAdd->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                pAdd->SetReactState(REACT_PASSIVE);
                pAdd->setFaction(35);

                switch (i)
                {
                    case 1:
                        pAdd->GetMotionMaster()->MoveFollow(pFirstAdd, 1.0f, (M_PI / 2) + 0.5f);
                        AddSpawnPosition.m_positionX -= 10.0f;
                        break;
                    case 2:
                        pAdd->GetMotionMaster()->MoveFollow(pFirstAdd, 1.0f, (M_PI / 2 + M_PI) - 0.5f);
                        break;
                }
            }
        }
    }
}

void Encounter_ArgentumChallenge::refreshAdds(Creature* pCreature, uint8 maxIndex)
{
    ASSERT(maxIndex <= 3);

    for (uint8 i = 0; i < maxIndex; ++i)
    {
        for (uint8 j = 0; j < 3; ++j)
        {
            if (Creature* pAdd = pInstanceMap->GetCreature(addsGUID[i][j]))
            {
                switch (j)
                {
                    case 0:
                        pAdd->SetFacingToObject(pCreature);
                        break;
                    case 1:
                        pAdd->GetMotionMaster()->MoveFollow(pAdd, 1.0f, (M_PI / 2) + 0.5f);
                        break;
                    case 2:
                        pAdd->GetMotionMaster()->MoveFollow(pAdd, 1.0f, (M_PI / 2 + M_PI) - 0.5f);
                        break;
                }
            }
        }
    }
}
