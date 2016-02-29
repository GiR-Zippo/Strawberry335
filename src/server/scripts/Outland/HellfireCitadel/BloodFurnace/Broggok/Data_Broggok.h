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
SDName: Boss_Broggok
SD%Complete: 100
SDComment:
SDCategory: Hellfire Citadel/Blood Furnace
EndScriptData */

#ifndef _DATA_BROGGOK_H
#define _DATA_BROGGOK_H

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "../BloodFurnace.h"


#define TIME_BETWEEN_CELLS          40*IN_MILLISECONDS

enum Events
{
    EVENT_SPELL_SLIME_SPRAY       = 1,
    EVENT_SPELL_POISON_CLOUD      = 2,
    EVENT_SPELL_POISON_BOLT       = 3
};

enum PreEvents
{
    PREEVENT_NOT_STARTED          = 0,
    PREEVENT_CELLS                = 1,
    PREEVENT_BROGGOK_START        = 2
};

enum Say
{
    SAY_AGGRO                     = -1542008,
};

enum Spells
{
    SPELL_SLIME_SPRAY             = 30913,
    H_SPELL_SLIME_SPRAY           = 38458,

    SPELL_POISON_BOLT             = 30917,
    H_SPELL_POISON_BOLT           = 38459,

    SPELL_POISON_CLOUD            = 30916
};

enum CellId
{
    CELL_FRONT_RIGHT              = 0,
    CELL_BACK_RIGHT               = 1,
    CELL_FRONT_LEFT               = 2,
    CELL_BACK_LEFT                = 3,
    CELL_COUNT                    = 4
};

enum PrisonCount
{
    COUNT_PRISON_0                = 3,
    COUNT_PRISON_1                = 4,
    COUNT_PRISON_2                = 5,
    COUNT_PRISON_3                = 6,
    COUNT_PRISON_MAX
};

enum Actions
{
    ACTION_START_PREEVENT         = 1
};

const Position SpawnPositonCells[4] =
{
    {    413.1f,   81.278f,  9.652664f, 6.264f    },
    {    413.1f,   111.974f, 9.652428f, 6.264f    },
    {    498.919f, 81.278f,  9.651441f, 3.174f    },
    {    498.919f, 111.974f, 9.652776f, 3.174f    }
};

const Position BroggokCombatPosition = { 456.308f, 45.887f, 9.615795f, 1.55f };


#endif // _DATA_BROGGOK_H
