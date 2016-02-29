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

/* Script Data Start
SDName: Boss Maiden of grief
SD%Complete: 100%
SDComment:
SDCategory: Ulduar/HallsOfStone
Script Data End */

#ifndef _BOSS_MAIDENOFGRIEF_H
#define _BOSS_MAIDENOFGRIEF_H

#include "../HallsOfStone.h"

#define BOUNDARY_POSITION_N                         896.0f

enum Spells
{
    H_SPELL_PARTING_SORROW                        = 59723,
    SPELL_STORM_OF_GRIEF                          = 50752,
    H_SPELL_STORM_OF_GRIEF                        = 59772,
    SPELL_SHOCK_OF_SORROW                         = 50760,
    H_SPELL_SHOCK_OF_SORROW                       = 59726,
    SPELL_PILLAR_OF_WOE                           = 50761,
    H_SPELL_PILLAR_OF_WOE                         = 59727
};

enum Yells
{
    SAY_AGGRO                                     = -1599000,
    SAY_SLAY_1                                    = -1599001,
    SAY_SLAY_2                                    = -1599002,
    SAY_SLAY_3                                    = -1599003,
    SAY_SLAY_4                                    = -1599004,
    SAY_DEATH                                     = -1599005,
    SAY_STUN                                      = -1599006
};

enum Events
{
    H_EVENT_SPELL_PARTING_SORROW                  = 1,
    EVENT_SPELL_STORM_OF_GRIEF                    = 2,
    EVENT_SPELL_SHOCK_OF_SORROW                   = 3,
    EVENT_SPELL_PILLAR_OF_WOE                     = 4,
    EVENT_CHECK_IN_ROOM                           = 5
};

enum Achievements
{
    ACHIEV_GOOD_GRIEF_START_EVENT                 = 20383,
};


class PartingSorrowFilter
{
    public:
        bool operator()(WorldObject* target) const
        {
            if (Unit* unit = target->ToUnit())
                return unit->getPowerType() != POWER_MANA;
            return false;
        }
};

class boss_maiden_of_grief : public CreatureScript
{
public:
    boss_maiden_of_grief() : CreatureScript("boss_maiden_of_grief") {}

    struct boss_maiden_of_griefAI : public BossAI
    {
        boss_maiden_of_griefAI(Creature* creature) : BossAI(creature, DATA_MAIDEN_OF_GRIEF)
        {
            instance = me->GetInstanceScript();
        };

        InstanceScript* instance;

        void Reset();
        void JustReachedHome();
        void EnterCombat(Unit* /*who*/);

        void UpdateAI(const uint32 diff);
        void ExecuteEvents();

        void JustDied(Unit* /*killer*/);
        void KilledUnit(Unit* victim);

        void CreateBoundaryMap();

    private:
        BossBoundaryMap _boundaryMap;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_maiden_of_griefAI (creature);
    };
};

#endif // _BOSS_MAIDENOFGRIEF_H
