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

#ifndef _CONTROLLER_BROGGOK_H
#define _CONTROLLER_BROGGOK_H

#include "Data_Broggok.h"


class mob_broggok_controller : public CreatureScript
{
    public:
        mob_broggok_controller() : CreatureScript("mob_broggok_controller") {}

        struct mob_broggok_controllerAI : public ScriptedAI
        {
            mob_broggok_controllerAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset();
            void JustSummoned(Creature* summoned);
            void DoAction(const int32 action);

            void UpdateAI(const uint32 diff);
            void ExecutePreEvents();

            void StartPreEvent();
            void SummonCreaturesForCell(uint8 cellId);
            void OpenCurrentCellDoor();
            void ModCreaturesAndStartCombat();

            bool CheckCreatureAliveOrCombat(bool onlyCheckCombat = false);
            void DespawnCreatures();

            void ResetPreEvent();
            void AggroAllPlayers(Creature* creature);

        private:
            typedef std::vector<uint64> GUIDStore;
            InstanceScript* instance;
            EventMap events;
            GUIDStore _cellCreatures[CELL_COUNT];
            uint8 _currentCell;
            uint8 _preEvent;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_broggok_controllerAI(creature);
        };
};


#endif // _CONTROLLER_BROGGOK_H
