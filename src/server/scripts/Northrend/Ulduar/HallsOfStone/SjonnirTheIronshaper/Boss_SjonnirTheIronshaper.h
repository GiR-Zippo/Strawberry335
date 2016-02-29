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

#ifndef _BOSS_SJONNIRTHEIRONSHAPER_H
#define _BOSS_SJONNIRTHEIRONSHAPER_H

#include "Data_SjonnirTheIronshaper.h"


class boss_sjonnir_the_ironshaper : public CreatureScript
{
    public:
        boss_sjonnir_the_ironshaper() : CreatureScript("boss_sjonnir") {}

        struct boss_sjonnir_the_ironshaperAI : public BossAI
        {
            boss_sjonnir_the_ironshaperAI(Creature* creature) : BossAI(creature, DATA_SJONNIR) {}

            void Reset();
            void JustReachedHome();
            void EnterCombat(Unit* who);

            void JustSummoned(Creature* summon);
            void JustDied(Unit* /*killer*/);
            void KilledUnit(Unit* who);

            void DoAction(int32 const action);
            uint32 GetData(uint32 type) const;

            void UpdateAI(uint32 const diff);
            void ExecuteEvents();

            void CreateBoundaryMap();

            private:
                uint8 abuseTheOoze;
                uint8 currentPipeLoc;
                bool messageSent[4];
                BossBoundaryMap _boundaryMap;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sjonnir_the_ironshaperAI (creature);
        };
};

#endif // _BOSS_SJONNIRTHEIRONSHAPER_H
