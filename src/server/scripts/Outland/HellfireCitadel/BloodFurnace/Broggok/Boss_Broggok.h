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

#ifndef _BOSS_BROGGOK_H
#define _BOSS_BROGGOK_H

#include "Data_Broggok.h"


class boss_broggok : public CreatureScript
{
    public:

        boss_broggok() : CreatureScript("boss_broggok") {}

        struct boss_broggokAI : public BossAI
        {
            boss_broggokAI(Creature* creature) : BossAI(creature, DATA_BROGGOK) {}

            void Reset();

            void EnterCombat(Unit* who);
            void JustDied(Unit* /*killer*/);

            void UpdateAI(const uint32 diff);
            void ExecuteEvents();

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_broggokAI(creature);
        }
};


#endif // _BOSS_BROGGOK_H
