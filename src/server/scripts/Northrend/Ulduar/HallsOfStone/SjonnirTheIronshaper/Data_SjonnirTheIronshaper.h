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

/* Script Data Start
SDName: Boss Sjonnir the ironshaper
SD%Complete: 95%
SDComment: emote is missing
SDCategory: Ulduar/HallsOfStone
Script Data End */

#ifndef _DATA_SJONNIRTHEIRONSHAPER_H
#define _DATA_SJONNIRTHEIRONSHAPER_H

#include "../HallsOfStone.h"

#define BOUNDARY_POSITION_S                             1266.25f
#define BOUNDARY_POSITION_E                             643.48f
#define BOUNDARY_POSITION_W                             690.54f

enum Spells
{
    SPELL_LIGHTING_RING                               = 51849, // Periodic Trigger (interval 2s) spell = 50841
    H_SPELL_LIGHTING_RING                             = 59861, // Periodic Trigger (interval 2s) spell = 59849

    SPELL_STATIC_CHARGE                               = 50834, // Periodic Trigger 2s interval, spell =50835
    H_SPELL_STATIC_CHARGE                             = 59846, // Periodic Trigger 2s interval, spell =59847

    SPELL_CHAIN_LIGHTING                              = 50830,
    H_SPELL_CHAIN_LIGHTING                            = 59844,

    SPELL_LIGHTING_SHIELD                             = 50831,
    H_SPELL_LIGHTING_SHIELD                           = 59845,

    SPELL_FRENZY                                      = 28747
};

enum Yells
{
    SAY_AGGRO                                         = -1900011,
    SAY_SLAY                                          = -1900012,
    SAY_RANDOM_1                                      = -1900013,
    SAY_RANDOM_2                                      = -1900014,
    SAY_DEATH                                         = -1900015,

    SAY_BRANN_SPAWN_DWARF                             = -1599025,
    SAY_BRANN_SPAWN_TROGG                             = -1599026,
    SAY_BRANN_SPAWN_OOZE                              = -1599027,
    SAY_BRANN_SPAWN_EARTHEN                           = -1599028,
    SAY_VICTORY_SJONNIR_1                             = -1599066,
    SAY_VICTORY_SJONNIR_2                             = -1599067,
};

enum SjonnirCreatures
{
    NPC_FORGED_IRON_TROGG                             = 27979,
    NPC_MALFORMED_OOZE                                = 27981,
    NPC_FORGED_IRON_DWARF                             = 27982,
    NPC_IRON_SLUDGE                                   = 28165,
    NPC_EARTHEN_DWARF                                 = 27980,
};

enum Misc
{
    ACTION_OOZE_DEAD                                  = 1,
    DATA_ABUSE_THE_OOZE                               = 2,
};

enum Events
{
    EVENT_CHAIN_LIGHTNING                             = 1,
    EVENT_LIGHTNING_SHIELD,
    EVENT_STATIC_CHARGE,
    EVENT_LIGHTNING_RING,
    EVENT_SUMMON,
    EVENT_FRENZY,
    EVENT_CHECK_IN_ROOM
};

enum MalformedOozeEvents
{
    MOE_EVENT_SPAWN_IRON_SLUDGE                        = 1
};

enum BrannEvents
{
    BE_EVENT_WAIT_SAY_1                                = 1,
    BE_EVENT_WAIT_SAY_2                                = 2
};

enum Quests
{
    QUEST_HALLS_OF_STONE                               = 13207
};

const Position PipeLocations[] =
{
    { 1295.44f, 734.07f, 200.3f, 0.0f }, // left
    { 1295.70f, 595.6f,  199.9f, 0.0f }  // right
};

const Position AddPointLocations[] =
{
    { 1295.44f, 700.12f, 192.59f, 4.7f }, // left
    { 1295.7f,  633.72f, 192.61f, 1.5f }  // right
};

const Position CenterPoint      ={ 1289.210f, 667.157f, 189.691f, 0.0f };
const Position BrannSpawnPos    ={ 1229.641f, 667.025f, 189.608f, 0.0f };
const Position BrannPointPos    ={ 1256.641f, 667.025f, 189.608f, 0.0f };
const Position BrannEndPosition ={ 1304.326f, 667.053f, 189.608f, 0.0f };
const Position TriggerPosition  ={ 1236.565f, 667.157f, 189.691f, 0.0f };


#endif // _DATA_SJONNIRTHEIRONSHAPER_H
