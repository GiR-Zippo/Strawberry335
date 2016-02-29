/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 23.09.2014
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

#ifndef _BOSS_THELICHKINGHOR_H
#define _BOSS_THELICHKINGHOR_H

#include "../HallsOfReflection.h"
#include "ScriptedEscortAI.h"


namespace BOSS_THELICHKINGHOR
{
    #define LICH_KING_SPEED                 1.0f
    #define LICH_KING_START_SPEED           1.8f
    #define LICH_KING_SPEED_BETWEEN_WALLS   1.6f
    #define LICH_KING_COMBAT_RANGE          5.0f

    enum Says
    {
        SAY_LICH_KING_WALL_01           = -1594486,
        SAY_LICH_KING_WALL_02           = -1594491,
        SAY_LICH_KING_GNOUL             = -1594482,
        SAY_LICH_KING_ABON              = -1594483,
        SAY_LICH_KING_WINTER            = -1594481,
        SAY_LICH_KING_WIN               = -1594485
    };

    enum Spells
    {
        SPELL_WINTER                    = 69780,
        SPELL_FURY_OF_FROSTMOURNE       = 70063,
        SPELL_SOUL_REAPER               = 73797,
        SPELL_RAISE_DEAD                = 69818,
        SPELL_SUMMON_ABO                = 69835,
        SPELL_SUMMON_WITCH_DOCTOR       = 69836
    };

    enum Events
    {
        EVENT_SPELL_WINTER              = 100,
        EVENT_START_MOVEMENT            = 101,
        EVENT_PUFFER                    = 102
    };

    enum EventTimers
    {
        EVENT_TIMER_ASAP                = 100,
        EVENT_TIMER_PUFFER              = 500,
        EVENT_TIMER_PUFFER_EXT          = 750
    };

    enum Phases
    {
        PHASE_INTRO_WAVE                = 1,
        PHASE_FIRST_WAVE                = 2,
        PHASE_SECOND_WAVE               = 3,
        PHASE_THIRD_WAVE                = 4,
        PHASE_FOURTH_WAVE               = 5,
        PHASE_OUTRO                     = 6,

        PHASE_MASK_INTRO_WAVE           = 1 << (PHASE_INTRO_WAVE - 1),
        PHASE_MASK_FIRST_WAVE           = 1 << (PHASE_FIRST_WAVE - 1),
        PHASE_MASK_SECOND_WAVE          = 1 << (PHASE_SECOND_WAVE - 1),
        PHASE_MASK_THIRD_WAVE           = 1 << (PHASE_THIRD_WAVE - 1),
        PHASE_MASK_FOURTH_WAVE          = 1 << (PHASE_FOURTH_WAVE - 1),
        PHASE_MASK_OUTRO                = 1 << (PHASE_OUTRO - 1)
    };
} // BOSS_THELICHKINGHOR

class Boss_TheLichKingHor : public CreatureScript
{
    public:

    Boss_TheLichKingHor() : CreatureScript("boss_lich_king_hor") { }

    struct Boss_TheLichKingHorAI : public npc_escortAI
    {
        public:

            Boss_TheLichKingHorAI(Creature* pCreature) : npc_escortAI(pCreature), summonList(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void DoAction(const int32 action) override;
            void Reset() override;

            void AttackStart(Unit* /*victim*/) override;
            void WaypointReached(uint32 /*point*/) override;

            void DamageTaken(Unit* /*pDoneBy*/, uint32& /*damage*/) override;
            void JustSummoned(Creature* /*summoned*/) override;
            void EnterEvadeMode() override { };

            void UpdateEscortAI(const uint32 diff) override;

        private:

            void setWave(uint32 wavePhase);
            bool applyPuffer(uint32 eventID);

            void executeGenericEvents(uint32 eventID);
            void executeFirstWave(uint32 eventID);
            void executeSecondWave(uint32 eventID);
            void executeThirdWave(uint32 eventID);
            void executeFourthWave(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;

            SummonList summonList;

            uint32 pufferEventID;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Boss_TheLichKingHorAI(pCreature);
    }
};


#endif // _BOSS_THELICHKINGHOR_H
