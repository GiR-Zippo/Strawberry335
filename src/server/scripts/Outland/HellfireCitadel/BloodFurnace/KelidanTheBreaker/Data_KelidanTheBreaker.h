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
SDName: Boss_Kelidan_The_Breaker
SD%Complete: 100
SDComment:
SDCategory: Hellfire Citadel/Blood Furnace
EndScriptData */

#ifndef _DATA_KELIDANTHEBREAKER_H
#define _DATA_KELIDANTHEBREAKER_H

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "../BloodFurnace.h"


enum Events
{
    EVENT_SPELL_CORRUPTION         = 1,
    EVENT_SPELL_FIRE_NOVA          = 2,
    EVENT_SPELL_SHADOW_BOLT_VOLLEY = 3,
    EVENT_SPELL_BURNING_NOVA       = 4,
    EVENT_SPELL_VORTEX             = 5,
    EVENT_CHECK_CAST_EVOCATION     = 6
};

enum Say
{
    SAY_WAKE                       = -1542000,
    SAY_ADD_AGGRO_1                = -1542001,
    SAY_ADD_AGGRO_2                = -1542002,
    SAY_ADD_AGGRO_3                = -1542003,
    SAY_KILL_1                     = -1542004,
    SAY_KILL_2                     = -1542005,
    SAY_NOVA                       = -1542006,
    SAY_DIE                        = -1542007
};

enum Spells
{
    SPELL_CORRUPTION               = 30938,
    SPELL_EVOCATION                = 30935,

    SPELL_FIRE_NOVA                = 33132,
    H_SPELL_FIRE_NOVA              = 37371,

    SPELL_SHADOW_BOLT_VOLLEY       = 28599,
    H_SPELL_SHADOW_BOLT_VOLLEY     = 40070,

    SPELL_BURNING_NOVA             = 30940,
    SPELL_VORTEX                   = 37370 // very slow pull, use DoTeleportAll() instead
};

enum CreatureEntry
{
    ENTRY_KELIDAN                  = 17377,
    ENTRY_CHANNELER                = 17653
};

const float ShadowmoonChannelers[5][4]=
{
    {302.0f, -87.0f, -24.4f, 0.157f},
    {321.0f, -63.5f, -24.6f, 4.887f},
    {346.0f, -74.5f, -24.6f, 3.595f},
    {344.0f, -103.5f, -24.5f, 2.356f},
    {316.0f, -109.0f, -24.6f, 1.257f}
};

enum AddEvents
{
    EVENT_SPELL_SHADOW_BOLT        = 1,
    EVENT_SPELL_MARK               = 2,
    EVENT_CHECK_CAST               = 3
};

enum AddSpells
{
    SPELL_SHADOW_BOLT              = 12739,
    H_SPELL_SHADOW_BOLT            = 15472,

    SPELL_MARK_OF_SHADOW           = 30937,
    SPELL_CHANNELING               = 39123
};


#endif // _DATA_KELIDANTHEBREAKER_H
