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

#include "Encounter_BlackKnight.h"
#include "../TrialOfTheChampion.h"


void Encounter_BlackKnight::DoAction(int32 action)
{
    switch (action)
    {
        case ENCOUNTER_ACTION_BK_GRYPHON_FINISHED:
        {
            eventMap.ScheduleEvent(3, 2 * IN_MILLISECONDS);
        }break;
    }
}

void Encounter_BlackKnight::onStart()
{
    // Set encounter data
    pInstance->SetData(BOSS_BLACK_KNIGHT, IN_PROGRESS);
}

void Encounter_BlackKnight::executeIntro(uint32 eventID)
{
    switch (eventID)
    {
        case 1: // Tirion intro
        {
            if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_TIRION)))
                pCreature->AI()->Talk(SAY_TIRION_INTRO_BK_1);

            eventMap.ScheduleEvent(2, 3 * IN_MILLISECONDS);
        }break;

        case 2: // Set Black Knight and Gryphon visible
        {
            if (GET_CREATURE_ANNOUNCER(Creature* pAnnouncer))
            {
                if (Creature* pMount = pInstanceMap->GetCreature(pInstance->GetData64(VEHICLE_BLACK_KNIGHT)))
                {
                    pAnnouncer->AI()->Talk(SAY_ANNOUNCER_BK);

                    pMount->SetPhaseMask(1, true);
                    pMount->SetVisible(true);
                    pMount->SetCanFly(true);
                    pMount->AI()->DoAction(1); // Start escort
                    pAnnouncer->SetTarget(pMount->GetGUID());

                    if (Creature* pKnight = pInstanceMap->GetCreature(pInstance->GetData64(NPC_BLACK_KNIGHT)))
                    {
                        pKnight->SetPhaseMask(1, true);
                        pKnight->SetVisible(true);
                        pKnight->SetCanFly(true);
                        pKnight->SetReactState(REACT_PASSIVE);
                        pKnight->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
            }
        }break;

        case 3: // Black Knight landed -> Cast on announcer (Called by action)
        {
            if (GET_CREATURE_ANNOUNCER(Creature* pAnnouncer))
            {
                if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_BLACK_KNIGHT)))
                {
                    pCreature->SetCanFly(false);
                    pCreature->SetTarget(pAnnouncer->GetGUID());

                    pCreature->AI()->Talk(SAY_BK_INTRO_1);
                    pCreature->AI()->DoCast(SPELL_DEATH_RESPITE_INTRO);
                }
            }

            eventMap.ScheduleEvent(4, 4 * IN_MILLISECONDS);
        }break;

        case 4: // Black Knight "pushes" announcer away
        {
            if (GET_CREATURE_ANNOUNCER(Creature* pAnnouncer))
                if (Creature* pCreature = pInstanceMap->GetCreature(pInstance->GetData64(NPC_BLACK_KNIGHT)))
                    pCreature->AI()->DoCast(pAnnouncer, SPELL_DEATH_PUSH_INTRO);

            eventMap.ScheduleEvent(5, 2 * IN_MILLISECONDS);
        }break;

        case 5: // Announcer casts feign death
        {
            if (GET_CREATURE_ANNOUNCER(Creature* pAnnouncer))
            {
                if (Creature* pTirion = pInstanceMap->GetCreature(pInstance->GetData64(NPC_TIRION)))
                {
                    pTirion->AI()->Talk(SAY_TIRION_INTRO_BK_2);

                    if (Creature* pKnight = pInstanceMap->GetCreature(pInstance->GetData64(NPC_BLACK_KNIGHT)))
                    {
                        pKnight->SetTarget(pTirion->GetGUID());
                        pKnight->Kill(pAnnouncer);
                    }
                }
            }

            eventMap.ScheduleEvent(6, 3 * IN_MILLISECONDS);
        }break;

        case 6: // More texts
        {
            if (Creature* pKnight = pInstanceMap->GetCreature(pInstance->GetData64(NPC_BLACK_KNIGHT)))
                pKnight->AI()->Talk(SAY_BK_INTRO_2);

            eventMap.ScheduleEvent(7, 9 * IN_MILLISECONDS);
        }break;

        case 7: // More texts
        {
            if (Creature* pKnight = pInstanceMap->GetCreature(pInstance->GetData64(NPC_BLACK_KNIGHT)))
                pKnight->AI()->Talk(SAY_BK_INTRO_3);

            eventMap.ScheduleEvent(8, 5 * IN_MILLISECONDS);
        }break;

        case 8: // Garrosh or Varian say and start fight of Black Knight
        {
            if (pInstance->GetData(DATA_TEAM) == HORDE)
            {
                if (Creature* pGarrosh = pInstanceMap->GetCreature(pInstance->GetData64(NPC_GARROSH)))
                    pGarrosh->AI()->Talk(SAY_GARROSH_INTRO_BK);
            }
            else
            {
                if (Creature* pVarian = pInstanceMap->GetCreature(pInstance->GetData64(NPC_VARIAN)))
                    pVarian->AI()->Talk(SAY_VARIAN_INTRO_BK);
            }

            if (Creature* pKnight = pInstanceMap->GetCreature(pInstance->GetData64(NPC_BLACK_KNIGHT)))
            {
                pKnight->SetHomePosition(pKnight->GetPosition());
                pKnight->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pKnight->SetReactState(REACT_AGGRESSIVE);
            }

            // Encounter_BlackKnight finished
            pInstance->DoAction(INSTANCE_ACTION_ENCOUNTER_DONE);
        }break;
    }
}
