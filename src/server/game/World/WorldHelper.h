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

#ifndef _WORLD_HELPER_H
#define _WORLD_HELPER_H


namespace CART
{
    #define ConfigNameResetTimes "Custom.ArenaResetTimers"

    enum Misc
    {
        DAYS_PER_WEEK   = 7,
    };

    static const char* const IntToDay[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
};

class CustomArenaResetTimer
{
    public:

        // First item: Day of the week since sunday; Second item: Hours since 00:00
        typedef std::vector< std::pair<uint8, uint8> > ResetTimesStorage;

        CustomArenaResetTimer();
        ~CustomArenaResetTimer();

        bool Load();
        uint32 CalculateNextResetTime();

    private:

        void calculateNextResetTime();

        ResetTimesStorage resetTimes;
        time_t nextResetTime;
};


#endif // _WORLD_HELPER_H
