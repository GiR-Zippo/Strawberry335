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

/*********************************************************************************
 * DEBUG MODE INSTRUCTIONS
 * This script includes a debug mode which affects the escort mob and the 
 * controller unit of the boss event for the boss - tribunal of ages
 * You can use it with the preprocessor define DEBUG_TRIBUNAL_OF_AGES
 * 
 * The debug interface is available at "Brann Bronzebeard" - gossip function
 * This NPC is located at his normal spawn position
 * You can switch to debug mode only if you are in gamemaster mode
 * If you switch it on, the npc will port him to the location
 * of the tribunal of ages
 * After that you don't need the gamemaster mode to control the event
 * If you stop the debug mode, the script will reset and the npc will
 * port himself to his normal spawn position
*********************************************************************************/
//#define DEBUG_TRIBUNAL_OF_AGES
/********************************************************************************/

/* Script Data Start
SDName: Boss Tribunal of the ages
SD%Complete: 100%
SDComment:
SDCategory: Ulduar/HallsOfStone
Script Data End */

#ifndef _DATA_TRIBUNALOFTHEAGES_H
#define _DATA_TRIBUNALOFTHEAGES_H

#include "../HallsOfStone.h"


enum Texts
{
    SAY_KILL_1                          = -1599016,
    SAY_KILL_2                          = -1599017,
    SAY_KILL_3                          = -1599018,
    SAY_LOW_HEALTH                      = -1599019,
    SAY_DEATH                           = -1599020,
    SAY_PLAYER_DEATH_1                  = -1599021,
    SAY_PLAYER_DEATH_2                  = -1599022,
    SAY_PLAYER_DEATH_3                  = -1599023,
    SAY_ESCORT_START                    = -1599024,

    SAY_EVENT_INTRO_1                   = -1599029,
    SAY_EVENT_INTRO_2                   = -1599030,
    SAY_EVENT_INTRO_3_ABED              = -1599031,

    SAY_EVENT_A_1                       = -1599032,
    SAY_EVENT_A_2_KADD                  = -1599033,
    SAY_EVENT_A_3                       = -1599034,

    SAY_EVENT_B_1                       = -1599035,
    SAY_EVENT_B_2_MARN                  = -1599036,
    SAY_EVENT_B_3                       = -1599037,

    SAY_EVENT_C_1                       = -1599038,
    SAY_EVENT_C_2_ABED                  = -1599039,
    SAY_EVENT_C_3                       = -1599040,

    SAY_EVENT_D_1                       = -1599041,
    SAY_EVENT_D_2_ABED                  = -1599042,
    SAY_EVENT_D_3                       = -1599043,
    SAY_EVENT_D_4_ABED                  = -1599044,

    SAY_EVENT_END_01                    = -1599045,
    SAY_EVENT_END_02                    = -1599046,
    SAY_EVENT_END_03_ABED               = -1599047,
    SAY_EVENT_END_04                    = -1599048,
    SAY_EVENT_END_05_ABED               = -1599049,
    SAY_EVENT_END_06                    = -1599050,
    SAY_EVENT_END_07_ABED               = -1599051,
    SAY_EVENT_END_08                    = -1599052,
    SAY_EVENT_END_09_KADD               = -1599053,
    SAY_EVENT_END_10                    = -1599054,
    SAY_EVENT_END_11_KADD               = -1599055,
    SAY_EVENT_END_12                    = -1599056,
    SAY_EVENT_END_13_KADD               = -1599057,
    SAY_EVENT_END_14                    = -1599058,
    SAY_EVENT_END_15_MARN               = -1599059,
    SAY_EVENT_END_16                    = -1599060,
    SAY_EVENT_END_17_MARN               = -1599061,
    SAY_EVENT_END_18                    = -1599062,
    SAY_EVENT_END_19_MARN               = -1599063,
    SAY_EVENT_END_20                    = -1599064,
    SAY_EVENT_END_21_ABED               = -1599065,

    SAY_ENTRANCE_MEET                   = -1599068,
};

enum BrannCreatures
{
    CREATURE_TRIBUNAL_OF_THE_AGES       = 28234,
    CREATURE_BRANN_BRONZEBEARD          = 28070,
    CREATURE_SEARING_GAZE_TARGET        = 28265,
    CREATURE_DARK_RUNE_PROTECTOR        = 27983,
    CREATURE_DARK_RUNE_STORMCALLER      = 27984,
    CREATURE_IRON_GOLEM_CUSTODIAN       = 27985,
};

enum Spells
{
    SPELL_STEALTH                       = 58506,
    //Kadrak
    SPELL_GLARE_OF_THE_TRIBUNAL         = 50988,
    H_SPELL_GLARE_OF_THE_TRIBUNAL       = 59870,
    //Marnak
    SPELL_DARK_MATTER                   = 51012,
    H_SPELL_DARK_MATTER                 = 59868,
    //Abedneum
    SPELL_SEARING_GAZE                  = 51136,
    H_SPELL_SEARING_GAZE                = 59867,

    SPELL_REWARD_ACHIEVEMENT            = 59046,

    SPELL_VISUAL_SHADOW_ORB             = 46235,
    SPELL_VISUAL_LIGHTNING_TRIGGER      = 45213,
    SPELL_VISUAL_LIGHTNING              = 56868,
    SPELL_KILL_TRIBUNAL_ADD             = 51288
};

enum Events
{
    EVENT_KADDRAK_CAST_SPELL            = 1,
    EVENT_MARNAK_CAST_SPELL             = 2,
    EVENT_ABEDNEUM_CAST_SPELL           = 3,
    EVENT_CHECK_PLAYERS                 = 4
};

enum Timer
{
    TIMER_SPELL_KADDRAK                 = 1500,
    TIMER_SPELL_MARNAK                  = 30000,
    TIMER_SPELL_ABEDNEUM                = 15000,
    TIMER_EVENT_CHECK_PLAYERS           = 10000
};

enum BossId
{
    BOSS_KADDRAK                        = 0,
    BOSS_MARNAK                         = 1,
    BOSS_ABEDNEUM                       = 2,
    BOSS_ALL                            = 3,
    BOSS_CONTROLLER                     = 4
};

enum BossGOState
{
    BOSS_GO_STATE_NOTHING               = 0,
    BOSS_GO_STATE_ACTIVE_RED            = 1,
    BOSS_GO_STATE_ACTIVE_WHITE          = 2
};

enum BrannGossip
{
    GOSSIP_BRANN_OPTIONID_START         = 0,
    GOSSIP_BRANN_OPTIONID_PROGRESS      = 1,
};

enum BrannActions
{
    ACTION_BRANN_START_ESCORT           = 0
};

enum ControllerActions
{
    ACTION_START_EVENT                  = 0,
    ACTION_RESET_EVENT                  = 1,
    ACTION_ENABLE_KADDRAK               = 2,
    ACTION_ENABLE_MARNAK                = 3,
    ACTION_ENABLE_ABEDNEUM              = 4,
    ACTION_SET_ALL_GO_WHITE             = 5
};

#define SAY_CUSTOM_BRANN_RESPAWN_FIRST        "Uh... Ah... Es hat mich erwischt!"
#define SAY_CUSTOM_BRANN_RESPAWN_SECOND       "Keine Sorge, ich bin nicht tot. Meine Getreuen bringen mir in einer Minute einen Verbandskofer! Dann bin ich wieder wie der Alte!"
#ifdef DEBUG_TRIBUNAL_OF_AGES
#define GOSSIP_ITEM_DEBUG               "[Debug] Modus starten"
#define GOSSIP_ITEM_DEBUG_TITLE         "[Debug] Benutzerinterface"
#define GOSSIP_ITEM_DEBUG_LINE          "--------------------------------------------"
#define GOSSIP_ITEM_DEBUG_0             "[Debug] Boss Kaddrak"
#define GOSSIP_ITEM_DEBUG_1             "[Debug] Boss Marnak"
#define GOSSIP_ITEM_DEBUG_2             "[Debug] Boss Abedneum"
#define GOSSIP_ITEM_DEBUG_3             "[Debug] SpellTimer Kaddrak setzen"
#define GOSSIP_ITEM_DEBUG_4             "[Debug] SpellTimer Marnak setzen"
#define GOSSIP_ITEM_DEBUG_5             "[Debug] SpellTimer Abedneum setzen"
#define GOSSIP_ITEM_RESET               "[Debug] Modus beenden"
#endif
#define DATA_BRANN_SPARKLIN_NEWS        1


static Position SpawnLocations[]=
{
    { 944.895f, 400.764f, 205.995f, 3.80482f },
    { 964.300f, 381.942f, 206.078f, 3.80482f }
};

const Position TribunalShadowOrb ={ 909.979f, 345.107f, 223.047f, 5.51f  };
const Position TribunalDebugPos  ={ 897.167f, 332.890f, 203.706f, 0.849f };
const Position AddPointPos       ={ 943.079f, 379.341f, 207.424f, 3.973f };


#endif // _DATA_TRIBUNALOFTHEAGES_H
