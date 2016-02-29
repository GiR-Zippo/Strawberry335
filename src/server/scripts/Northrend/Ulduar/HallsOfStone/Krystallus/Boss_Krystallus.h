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

#ifndef _BOSS_KRYSTALLUS_H
#define _BOSS_KRYSTALLUS_H

#include "Data_Krystallus.h"


class boss_krystallus : public CreatureScript
{
public:
    boss_krystallus() : CreatureScript("boss_krystallus") {}

    struct boss_krystallusAI : public BossAI
    {
        boss_krystallusAI(Creature* creature) : BossAI(creature, DATA_KRYSTALLUS)
        {
            instance = creature->GetInstanceScript();
        };

        InstanceScript* instance;

        void Reset();
        void JustReachedHome();
        void EnterCombat(Unit* who);

        void UpdateAI(const uint32 diff);
        void ExecuteEvents();

        void JustDied(Unit* killer);
        void KilledUnit(Unit* victim);

        void SpellHitTarget(Unit* /*target*/, const SpellInfo* pSpell);

        void CreateBoundaryMap();

    private:
        BossBoundaryMap _boundaryMap;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_krystallusAI (creature);
    };
};

#endif // _BOSS_KRYSTALLUS_H
