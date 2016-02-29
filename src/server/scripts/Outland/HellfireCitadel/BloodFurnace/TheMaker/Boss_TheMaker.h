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

/* ScriptData
SDName: Boss_The_Maker
SD%Complete: 100
SDComment:
SDCategory: Hellfire Citadel/Blood Furnace
EndScriptData */

#ifndef _BOSS_THEMAKER_H
#define _BOSS_THEMAKER_H

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "../BloodFurnace.h"


enum Events
{
    EVENT_SPELL_ACID_SPRAY        = 1,
    EVENT_SPELL_EXPLODING_BREAKER = 2,
    EVENT_SPELL_KNOCKDOWN         = 3,
    EVENT_SPELL_DOMINATION        = 4
};

enum Say
{
    SAY_AGGRO_1                   = -1542009,
    SAY_AGGRO_2                   = -1542010,
    SAY_AGGRO_3                   = -1542011,
    SAY_KILL_1                    = -1542012,
    SAY_KILL_2                    = -1542013,
    SAY_DIE                       = -1542014,
};

enum Spells
{
    SPELL_EXPLODING_BREAKER       = 30925,
    H_SPELL_EXPLODING_BREAKER     = 40059,

    SPELL_ACID_SPRAY              = 38153,
    SPELL_KNOCKDOWN               = 20276,
    SPELL_DOMINATION              = 30923
};


class boss_the_maker : public CreatureScript
{
    public:
        boss_the_maker() : CreatureScript("boss_the_maker"){}

        struct boss_the_makerAI : public BossAI
        {
            boss_the_makerAI(Creature* creature) : BossAI(creature, DATA_THE_MAKER){}

            InstanceScript* instance;

            void Reset();
            void EnterCombat(Unit* /*who*/);

            void KilledUnit(Unit* /*victim*/);
            void JustDied(Unit* /*killer*/);

            void UpdateAI(const uint32 diff);
            void ExecuteEvents();
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_the_makerAI(creature);
        }
};


#endif // _BOSS_THEMAKER_H
