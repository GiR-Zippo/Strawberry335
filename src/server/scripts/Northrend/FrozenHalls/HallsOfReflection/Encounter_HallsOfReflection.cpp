/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 10.10.2014
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
 * Base class for encounters in the instance "Halls of Reflection"
 * Used to handle intro and pre-encounters
 */

#include "Encounter_HallsOfReflection.h"
#include "HallsOfReflection.h"


void Encounter_HallsOfReflection::Start(uint32 eventID, uint32 eventPhase)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_HallsOfReflection::Start(%u, %u)", eventID, eventPhase);

    if (currentState != ENCOUNTER_STATE_NOTHING)
        ASSERT(false);

    currentState = ENCOUNTER_STATE_INTRO;

    // Set phase
    if (eventPhase)
        eventMap.SetPhase(eventPhase);

    // Start intro
    eventMap.ScheduleEvent(eventID, EVENT_SCHEDULE_START_TIMER);

    // Call event
    onStart();
}

bool Encounter_HallsOfReflection::GetData(uint32 type, uint32& data)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_HallsOfReflection::GetData(%u, %u)", type, data);

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

void Encounter_HallsOfReflection::SetData(uint32 type, uint32 data)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_HallsOfReflection::SetData(%u, %u)", type, data);

    // Set data in dataMap
    dataMap[type] = data;

    // Call event
    onDataChanged(type);
}

void Encounter_HallsOfReflection::IncreaseData(uint32 type, uint32 data)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_HallsOfReflection::IncreaseData(%u, %u)", type, data);

    // Increase data in dataMap
    dataMap[type] += data;

    // Call event
    onDataChanged(type);
}

void Encounter_HallsOfReflection::Update(const uint32 diff)
{
    if (!IsActive())
        return;

    eventMap.Update(diff);

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        switch (currentState)
        {
            case ENCOUNTER_STATE_INTRO:
                executeIntro(eventID);
                break;

            case ENCOUNTER_STATE_PRE_ENCOUNTER:
                executePreEncounter(eventID);
                break;

            case ENCOUNTER_STATE_OUTRO:
                executeOutro(eventID);
                break;

            default:
                return;
        }
    }
}

void Encounter_HallsOfReflection::startNextPhase(uint32 eventPhase, uint32 eventID)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_HallsOfReflection::startNextPhase(%u, %u)", eventPhase, eventID);

    if (currentState != ENCOUNTER_STATE_ENCOUNTER)
        ASSERT(false);

    currentState = ENCOUNTER_STATE_PRE_ENCOUNTER;

    // Set phase
    eventMap.SetPhase(eventPhase);

    // Start preencounter
    eventMap.ScheduleEvent(eventID, EVENT_SCHEDULE_START_TIMER);
}

bool Encounter_HallsOfReflection::startNext(uint32 eventID, uint32 eventTime)
{
    sLog->outDebug(LOG_FILTER_TSCR, "Call: Encounter_HallsOfReflection::startNext()");

    if (!IsActive() && !hasOutro)
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
                eventMap.ScheduleEvent(eventID, eventTime);

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

        case ENCOUNTER_STATE_ENCOUNTER:
        {
            sLog->outDebug(LOG_FILTER_TSCR, "Proceed: ENCOUNTER_STATE_ENCOUNTER");

            currentState = ENCOUNTER_STATE_OUTRO;

            // Start preencounter
            eventMap.ScheduleEvent(eventID, eventTime);

            sLog->outDebug(LOG_FILTER_TSCR, "Proceed: ENCOUNTER_STATE_ENCOUNTER -> ENCOUNTER_STATE_OUTRO");
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
