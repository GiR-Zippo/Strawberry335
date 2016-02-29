/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 19.01.2015
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
 * @copyright Copyright 2015 Laenalith-WoW. All rights reserved.
 */

#include "WorldHelper.h"
#include "World.h"
#include "Config.h"

using namespace CART;


CustomArenaResetTimer::CustomArenaResetTimer()
{
}

CustomArenaResetTimer::~CustomArenaResetTimer()
{
}

bool CustomArenaResetTimer::Load()
{
    std::string src = ConfigMgr::GetStringDefault(ConfigNameResetTimes, "");
    if (src.empty())
        return false;

    // Format: Day (Sunday 0 - 6 Saturday) Time (0-23) => 2:12,4:20...
    Tokenizer resetTokens(src, ',');
    for (Tokenizer::const_iterator itr = resetTokens.begin(); itr != resetTokens.end(); ++itr)
    {
        Tokenizer timeTokens((*itr), ':');
        ASSERT(timeTokens.size() == 2 && isNumeric(timeTokens[0]) && isNumeric(timeTokens[1]));

        uint8 data[2] = { static_cast<uint8>(atoi(timeTokens[0])), static_cast<uint8>((atoi(timeTokens[1]))) };
        resetTimes.push_back(std::make_pair(data[0], data[1]));
        sLog->outString(">> CustomArenaResetTimer: %s at %d o'clock", IntToDay[data[0]], data[1]);
    }
    return true;
}

uint32 CustomArenaResetTimer::CalculateNextResetTime()
{
    calculateNextResetTime();
    return nextResetTime;
}

void CustomArenaResetTimer::calculateNextResetTime()
{
    ASSERT(!resetTimes.empty());

    time_t curTime = time(NULL);
    tm localTm = *localtime(&curTime);
    localTm.tm_min = 0;
    localTm.tm_sec = 0;

    ResetTimesStorage::const_iterator itr = resetTimes.begin();

    // Find and calculate next reset time
    for (; itr != resetTimes.end(); ++itr)
    {
        if (((*itr).first == localTm.tm_wday && (*itr).second > localTm.tm_hour)
            || (*itr).first > localTm.tm_wday)
        {
            localTm.tm_mday += ((*itr).first - localTm.tm_wday);
            localTm.tm_hour = (*itr).second;
            nextResetTime = mktime(&localTm);
            return;
        }
    }

    // If this stage is reached, new week has to start
    itr = resetTimes.begin();
    localTm.tm_mday += ((DAYS_PER_WEEK - localTm.tm_wday) + (*itr).first);
    localTm.tm_hour = (*itr).second;
    nextResetTime = mktime(&localTm);
}
