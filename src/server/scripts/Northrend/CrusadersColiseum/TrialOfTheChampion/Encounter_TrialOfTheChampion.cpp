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
 *
 * @description
 *
 * Base class for all encounters in the instance "Trial Of The Champion"
 * Used to handle intro and pre-encounters
 */

#include "Encounter_TrialOfTheChampion.h"
#include "TrialOfTheChampion.h"


void Encounter_TrialOfTheChampion::Start(uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TrialOfTheChampion::Start(%u)", eventID);

    if (currentState != ENCOUNTER_STATE_NOTHING)
        ASSERT(false);

    currentState = ENCOUNTER_STATE_INTRO;

    // Start intro
    eventMap.ScheduleEvent(eventID, EVENT_SCHEDULE_START_TIMER);

    // Call event
    onStart();
}

bool Encounter_TrialOfTheChampion::GetData(uint32 type, uint32& data)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TrialOfTheChampion::GetData(%u, %u)", type, data);

    // Get data from dataMap
    if (dataMap.find(type) != dataMap.end())
    {
        data = dataMap.at(type);

        sLog->outDebug(LOG_FILTER_TSCR, "Return: true with data %u", data);
        return true;
    }
    else
    {
        sLog->outDebug(LOG_FILTER_TSCR, "Return: false for type %u", type);
        return false;
    }
}

void Encounter_TrialOfTheChampion::SetData(uint32 type, uint32 data)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TrialOfTheChampion::SetData(%u, %u)", type, data);

    // Set data in dataMap
    dataMap[type] = data;

    // Call event
    onDataChanged(type);
}

void Encounter_TrialOfTheChampion::IncreaseData(uint32 type, uint32 data)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TrialOfTheChampion::IncreaseData(%u, %u)", type, data);

    // Increase data in dataMap
    dataMap[type] += data;

    // Call event
    onDataChanged(type);
}

void Encounter_TrialOfTheChampion::Update(const uint32 diff)
{
    if (!IsActive())
        return;

    eventMap.Update(diff);

    if (uint32 eventId = eventMap.ExecuteEvent())
    {
        switch (currentState)
        {
            case ENCOUNTER_STATE_INTRO:
                executeIntro(eventId);
                break;

            case ENCOUNTER_STATE_PRE_ENCOUNTER:
                executePreEncounter(eventId);
                break;

            default:
                return;
        }
    }
}

bool Encounter_TrialOfTheChampion::startNext()
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TrialOfTheChampion::startNext()");

    if (!IsActive())
    {
        sLog->outDebug(LOG_FILTER_TSCR, "Return: false");
        return false;
    }

    // Reset EventMap
    eventMap.Reset();

    switch (currentState)
    {
        case ENCOUNTER_STATE_INTRO:
        {
            sLog->outDebug(LOG_FILTER_TSCR, "Proceed: ENCOUNTER_STATE_INTRO");

            if (hasPreEncounter)
            {
                currentState = ENCOUNTER_STATE_PRE_ENCOUNTER;

                // Start preencounter
                eventMap.ScheduleEvent(EVENT_SCHEDULE_START, EVENT_SCHEDULE_START_TIMER);

                sLog->outDebug(LOG_FILTER_TSCR, "Proceed: ENCOUNTER_STATE_INTRO -> ENCOUNTER_STATE_PRE_ENCOUNTER");
                break;
            }
        } // No break
        case ENCOUNTER_STATE_PRE_ENCOUNTER:
        {
            sLog->outDebug(LOG_FILTER_TSCR, "Proceed: ENCOUNTER_STATE_PRE_ENCOUNTER");
            currentState = ENCOUNTER_STATE_ENCOUNTER;
            sLog->outDebug(LOG_FILTER_TSCR, "Proceed: ENCOUNTER_STATE_PRE_ENCOUNTER -> ENCOUNTER_STATE_ENCOUNTER");
        }break;

        default:
        {
            sLog->outDebug(LOG_FILTER_TSCR, "Proceed: return false");
            return false;
        }
    }

    sLog->outDebug(LOG_FILTER_TSCR, "Return: true");
    return true;
}

void Encounter_TrialOfTheChampion::openDoor()
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TrialOfTheChampion::openDoor()");

    if (GameObject* pObject = pInstanceMap->GetGameObject(pInstance->GetData64(GO_MAIN_GATE)))
        pInstance->HandleGameObject(pObject->GetGUID(), true);
}

void Encounter_TrialOfTheChampion::closeDoor()
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_TrialOfTheChampion::closeDoor()");

    if (GameObject* pObject = pInstanceMap->GetGameObject(pInstance->GetData64(GO_MAIN_GATE)))
        pInstance->HandleGameObject(pObject->GetGUID(), false);
}
