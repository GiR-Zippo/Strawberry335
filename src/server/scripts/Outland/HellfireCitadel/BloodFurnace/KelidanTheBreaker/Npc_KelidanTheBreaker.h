/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#ifndef _NPC_KELIDANTHEBREAKER_H
#define _NPC_KELIDANTHEBREAKER_H

#include "Data_KelidanTheBreaker.h"


class mob_shadowmoon_channeler : public CreatureScript
{
    public:
        mob_shadowmoon_channeler() : CreatureScript("mob_shadowmoon_channeler"){}

        struct mob_shadowmoon_channelerAI : public ScriptedAI
        {
            mob_shadowmoon_channelerAI(Creature* creature) : ScriptedAI(creature){}

            void Reset();
            void EnterCombat(Unit* who);
            void JustDied(Unit* killer);

            void UpdateAI(const uint32 diff);
            void ExecuteEvents();

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_shadowmoon_channelerAI(creature);
        }
};


#endif // _NPC_KELIDANTHEBREAKER_H
