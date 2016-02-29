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

#ifndef _BOSS_KELIDANTHEBREAKER_H
#define _BOSS_KELIDANTHEBREAKER_H

#include "Data_KelidanTheBreaker.h"


class boss_kelidan_the_breaker : public CreatureScript
{
    public:
        boss_kelidan_the_breaker() : CreatureScript("boss_kelidan_the_breaker"){}

        struct boss_kelidan_the_breakerAI : public BossAI
        {
            boss_kelidan_the_breakerAI(Creature* creature) : BossAI(creature, DATA_KELIDAN_THE_BREAKER){}

            void Reset();
            void EnterCombat(Unit* who);
            void KilledUnit(Unit* victim);

            void ChannelerEngaged(Unit* who);
            void ChannelerDied(Unit* killer);
            uint64 GetChanneled(Creature* channeler1);
            void SummonChannelers();

            void JustDied(Unit* killer);
            void UpdateAI(const uint32 diff);
            void ExecuteEvents();

            private:
                InstanceScript* instance;
                bool addYell;
                uint64 Channelers[5];
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_kelidan_the_breakerAI(creature);
        }
};


#endif // _BOSS_KELIDANTHEBREAKER_H
