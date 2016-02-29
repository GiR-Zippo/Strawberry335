/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "Misc_TribunalOfTheAges.h"
typedef achievement_brann_spankin_new SpankinNew;


bool SpankinNew::OnCheck(Player* /*player*/, Unit* target)
{
    if (!target)
        return false;

    if (Creature* Brann = target->ToCreature())
        if (Brann->AI()->GetData(DATA_BRANN_SPARKLIN_NEWS))
            return true;

    return false;
};
