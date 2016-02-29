/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MISC_SJONNIRTHEIRONSHAPER_H
#define _MISC_SJONNIRTHEIRONSHAPER_H

#include "Data_SjonnirTheIronshaper.h"


class achievement_abuse_the_ooze : public AchievementCriteriaScript
{
    public:
        achievement_abuse_the_ooze() : AchievementCriteriaScript("achievement_abuse_the_ooze") {}

        bool OnCheck(Player* /*player*/, Unit* target);
};


#endif // _MISC_SJONNIRTHEIRONSHAPER_H
