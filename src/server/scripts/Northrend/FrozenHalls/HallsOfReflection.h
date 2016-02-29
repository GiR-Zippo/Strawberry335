/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 02.10.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#ifndef _HALLS_OF_REFLECTION_H
#define _HALLS_OF_REFLECTION_H


#define GET_INSTANCESCRIPT_SAFE(x) x = me->GetInstanceScript() ? me->GetInstanceScript() : NULL

enum Data
{
    MAX_ENCOUNTER           = 4,

    DATA_FALRIC_EVENT       = 1,
    DATA_MARWYN_EVENT       = 2,
    DATA_FROSWORN_EVENT     = 3,
    DATA_LICHKING_EVENT     = 4,

    DATA_INTRO_EVENT        = 5,
    DATA_TEAM_IN_INSTANCE   = 6,
    DATA_WAVE_COUNT         = 7,

    DATA_SUMMONS            = 8,
    DATA_ICE_WALL_1         = 9,
    DATA_ICE_WALL_2         = 10,
    DATA_ICE_WALL_3         = 11,
    DATA_ICE_WALL_4         = 12,

    DATA_PHASE              = 13,
    DATA_SPAWN_ADDS_F       = 14,
    DATA_SPAWN_ADDS_M       = 15,
    DATA_SET_ADDS_INCOMBAT  = 16,
    DATA_UPDATE_WAVE_COUNT  = 17,
    DATA_BEGIN_WAVE_EVENT   = 18,
};

enum Creatures
{
    NPC_FALRIC              = 38112,
    NPC_MARWYN              = 38113,
    NPC_FROSTWORN_GENERAL   = 36723,
    NPC_REFLECTION          = 37107,
    NPC_LICH_KING_EVENT     = 36954,
    NPC_QUELDELAR           = 37158,
    BOSS_LICH_KING          = 37226,

    NPC_ARCHMAGE_KORELN     = 37582,
    NPC_DARK_RANGER_LORALEN = 37779,
    NPC_UTHER               = 37225,
    NPC_JAINA_OR_SYLVANAS   = 37221,
    NPC_JAINA_OR_SYLVANAS_O = 36955,
    NPC_SYLVANAS            = 37223,
    NPC_SYLVANAS_OUTRO      = 37554,
    NPC_BARTLETT            = 37182,
    NPC_KORM                = 37833,

    NPC_WAVE_MERCENARY      = 38177,
    NPC_WAVE_FOOTMAN        = 38173,
    NPC_WAVE_RIFLEMAN       = 38176,
    NPC_WAVE_PRIEST         = 38175,
    NPC_WAVE_MAGE           = 38172,
    NPC_WAVE_LOOT_DUMMY     = 37906,

    NPC_ICE_WALL            = 37014,
    NPC_RAGING_GNOUL        = 36940,
    NPC_RISEN_WITCH_DOCTOR  = 36941,
    NPC_ABON                = 37069

};

enum GameObjects
{
    GO_FROSTMOURNE          = 202302,
    GO_FROSTMOURNE_ALTAR    = 202236,
    GO_FRONT_DOOR           = 201976,
    GO_FROSTWORN_DOOR       = 197341,
    GO_ARTHAS_DOOR          = 197342,
    GO_RUN_DOOR             = 197343,

    GO_ICE_WALL_1           = 201385,
    GO_ICE_WALL_2           = 201885,
    GO_ICE_WALL_3           = 202396,
    GO_ICE_WALL_4           = 500001,
    GO_CAVE                 = 201596,

    GO_STAIRS_SKYBREAKER    = 201709,
    GO_SKYBREAKER           = 500002,
    GO_STAIRS_ORGRIM_HAMMER = 202211,
    GO_ORGRIM_HAMMER        = 500003,
    GO_PORTAL               = 202079,
    GO_CHEST_HORDE          = 202212, //3145
    GO_CHEST_ALLIANCE       = 201710, //30357
    GO_CHEST_HORDE_H        = 202337, //3246
    GO_CHEST_ALLIANCE_H     = 202336  //3333
};

enum Achievements
{
    ACHIEV_HALLS_OF_REFLECTION_N   = 4518,
    ACHIEV_HALLS_OF_REFLECTION_H   = 4521,
    ACHIEV_NOT_RETREATING_EVENT    = 22615,
    SPELL_ACHIEV_CHECK             = 72830,
};

const Position OutroSpawns[2] =
{
    {5564.25f, 2274.69f, 733.01f, 3.93f}, // Lich King
    {5556.27f, 2266.28f, 733.01f, 0.8f}   // Jaina/Sylvana
};

enum HorWorldStates
{
    WORLD_STATE_HOR             = 4884,
    WORLD_STATE_HOR_WAVE_COUNT  = 4882
};

// Common actions from Instance Script to Boss Script
enum Actions
{
    ACTION_ENTER_COMBAT         = 1,
    ACTION_ENTER_COMBAT_CONT    = 2,
    ACTION_STOP_COMBAT          = 3
};

enum Spells
{
    SPELL_BOSS_SPAWN_AURA = 72712 // Falric and Marwyn
};

enum Phases
{
    PHASE_SPAWN_WAVES           = 1
};

// Base class for FALRIC and MARWYN
// handled the summonList and the notification events to/from the InstanceScript
struct Boss_HorAI : ScriptedAI
{
    public:

        Boss_HorAI(Creature* pCreature) : ScriptedAI(pCreature), summonList(pCreature)
        {
            GET_INSTANCESCRIPT_SAFE(pInstance);
        }

        void Reset()
        {
            eventMap.Reset();
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        void DamageTaken(Unit* /*who*/, uint32& damage)
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                damage = 0;
        }

        void DoAction(const int32 actionID)
        {
            switch (actionID)
            {
                case ACTION_ENTER_COMBAT: // called by InstanceScript when boss shall enter in combat.
                {
                    // Just in case. Should have been done by InstanceScript
                    me->SetVisible(true);

                    // Reset flags
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->SetReactState(REACT_AGGRESSIVE);

                    if (Unit* pUnit = me->SelectNearestTarget())
                        AttackStart(pUnit);

                    DoZoneInCombat();
                }break;
            }
        }

        void JustSummoned(Creature* pCreature)
        {
            summonList.Summon(pCreature);

            if (Unit* pUnitTarget = pCreature->SelectNearestTarget())
            {
                if (pCreature->AI())
                    pCreature->AI()->AttackStart(pUnitTarget);
                else
                {
                    pCreature->GetMotionMaster()->MoveChase(pUnitTarget);
                    pCreature->Attack(pUnitTarget, true);
                }
            }

            if (pCreature->AI())
                pCreature->AI()->DoZoneInCombat();
        }

        void SummonedCreatureDespawn(Creature* pCreature)
        {
            summonList.Despawn(pCreature);
            if (summonList.empty())
            {
                if (pCreature->isAlive())
                    pInstance->SetData(DATA_WAVE_COUNT, NOT_STARTED);
                else
                    pInstance->SetData(DATA_WAVE_COUNT, SPECIAL);
            }
        }

    protected:

        InstanceScript* pInstance;
        EventMap eventMap;
        SummonList summonList;
};


#endif // _HALLS_OF_REFLECTION_H
