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

#include "Boss_TheLichKingHor.h"
#include "Encounter_TheLichKingHor.h"

using namespace BOSS_THELICHKINGHOR;


typedef Boss_TheLichKingHor::Boss_TheLichKingHorAI LichKingAI;


void LichKingAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_START_EVENT:
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->RemoveAllAuras();

            me->SetSpeed(MOVE_WALK, LICH_KING_START_SPEED, true);
            Start(false, false);
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_PREPARE_WALL:
        {
            // Only apply after first wave finished
            if (eventMap.GetPhaseMask() & PHASE_MASK_FIRST_WAVE)
                eventMap.ScheduleEvent(1, EVENT_TIMER_ASAP);
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_REACHED_WALL:
        {
            // Only apply at first wave
            if (!(eventMap.GetPhaseMask() & PHASE_MASK_FIRST_WAVE))
            {
                setWave(PHASE_INTRO_WAVE);
                eventMap.ScheduleEvent(1, EVENT_TIMER_ASAP);
            }
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_LICH_KING_WIN:
        {
            SetEscortPaused(true);
            me->setActive(false);
            me->StopMoving();
            me->AttackStop();

            DoScriptText(SAY_LICH_KING_WIN, me);

            me->RemoveAurasDueToSpell(SPELL_WINTER);
            me->CastSpell(me, SPELL_FURY_OF_FROSTMOURNE, false);

            if (pInstance)
                pInstance->SetBossState(DATA_LICHKING_EVENT, FAIL);
        } // No break
        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_NEXT_WALL:
        {
            me->RemoveAurasDueToSpell(SPELL_WINTER);
            me->SetSpeed(MOVE_WALK, LICH_KING_SPEED_BETWEEN_WALLS, true);
        } // No break
        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_DESPAWN_ADDS:
        {
            if (!summonList.empty())
                for (SummonList::const_iterator itr = summonList.begin(); itr != summonList.end(); ++itr)
                    if (Creature* pCreature = me->GetCreature(*me, (*itr)))
                        pCreature->DespawnOrUnsummon();
            summonList.clear();
        }break;

        case ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_LAST_WALL:
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetWalk(false);

            eventMap.SetPhase(PHASE_OUTRO);
        }break;
    }
}

void LichKingAI::Reset()
{
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

    eventMap.Reset();
    summonList.clear();

    pufferEventID = NULL;

    me->SetSpeed(MOVE_WALK, LICH_KING_START_SPEED, true);
}

void LichKingAI::AttackStart(Unit* pUnit)
{
    if (!pInstance)
        return;

    if (pInstance->GetBossState(DATA_LICHKING_EVENT) == IN_PROGRESS
        && pUnit->GetDistance(me) <= LICH_KING_COMBAT_RANGE && !(eventMap.GetPhaseMask() & PHASE_MASK_OUTRO))
    {
        pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_LICH_KING_WIN);
        return;
    }

    if (pUnit->GetTypeId() != TYPEID_PLAYER && pUnit->GetDistance(me) <= LICH_KING_COMBAT_RANGE)
        ScriptedAI::AttackStart(pUnit);
}

void LichKingAI::WaypointReached(uint32 point)
{
    if (!pInstance)
        return;

    switch (point)
    {
        case 66: // End
        {
            SetEscortPaused(true);
            me->setActive(false);
            pInstance->DoAction(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_ACTION_START_OUTRO);
        }break;
    }
}

void LichKingAI::DamageTaken(Unit* pUnit, uint32& damage)
{
    if (!pInstance)
        return;

    if (pUnit->GetTypeId() != TYPEID_PLAYER || damage >= me->GetHealth())
    {
        damage = NULL;
        return;
    }
}

void LichKingAI::JustSummoned(Creature* pCreature)
{
    if (!pInstance || !pCreature)
        return;

    if (pCreature->AI())
    {
        if (eventMap.GetPhaseMask() & PHASE_MASK_FOURTH_WAVE)
            pCreature->AI()->DoAction(2); // Move to fourth wave pos, then attack
        else if (eventMap.GetPhaseMask() & PHASE_MASK_THIRD_WAVE)
            pCreature->AI()->DoAction(1); // Move to third wave pos, then attack
    }

    summonList.Summon(pCreature);
}

void LichKingAI::UpdateEscortAI(const uint32 diff)
{
    if (!pInstance)
        return;

    eventMap.Update(diff);

    if (pInstance->GetBossState(DATA_LICHKING_EVENT) == IN_PROGRESS)
    {
        if (me->HasUnitState(UNIT_STATE_CASTING))
        {
            if (!HasEscortState(STATE_ESCORT_PAUSED))
                SetEscortPaused(true);
            return;
        }
    }

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        switch (eventID)
        {
            case EVENT_SPELL_WINTER:
            case EVENT_START_MOVEMENT:
            case EVENT_PUFFER:
            {
                executeGenericEvents(eventID);
            }break;

            default:
            {
                if (uint32 eventPhase = eventMap.GetPhaseMask())
                {
                    if (eventID == 1 && eventMap.GetPhaseMask() & PHASE_FIRST_WAVE)
                    {
                        if (!(eventMap.GetPhaseMask() & PHASE_MASK_SECOND_WAVE))
                            setWave(PHASE_SECOND_WAVE);
                        else if (!(eventMap.GetPhaseMask() & PHASE_MASK_THIRD_WAVE))
                            setWave(PHASE_THIRD_WAVE);
                        else if (!(eventMap.GetPhaseMask() & PHASE_MASK_FOURTH_WAVE))
                            setWave(PHASE_FOURTH_WAVE);

                        DoCast(me, SPELL_RAISE_DEAD, true);
                        eventMap.ScheduleEvent(2, EVENT_TIMER_ASAP);
                    }
                    else
                    {
                        if (eventPhase & PHASE_MASK_FOURTH_WAVE)
                            executeFourthWave(eventID);
                        else if (eventPhase & PHASE_MASK_THIRD_WAVE)
                            executeThirdWave(eventID);
                        else if (eventPhase & PHASE_MASK_SECOND_WAVE)
                            executeSecondWave(eventID);
                        else if (eventPhase & PHASE_MASK_INTRO_WAVE)
                            executeFirstWave(eventID);
                    }
                }break;
            }
        }
    }

    if (pInstance->GetBossState(DATA_LICHKING_EVENT) == IN_PROGRESS)
        UpdateVictim();
    else
        DoMeleeAttackIfReady();
}

void LichKingAI::executeGenericEvents(uint32 eventID)
{
    switch (eventID)
    {
        case EVENT_SPELL_WINTER:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(me, SPELL_WINTER);
            eventMap.ScheduleEvent(EVENT_START_MOVEMENT, EVENT_TIMER_ASAP);
        }break;

        case EVENT_START_MOVEMENT:
        {
            if (applyPuffer(eventID))
                return;

            me->SetSpeed(MOVE_WALK, LICH_KING_SPEED, true);
            SetEscortPaused(false);
        }break;

        case EVENT_PUFFER:
        {
            if (pufferEventID)
                eventMap.ScheduleEvent(pufferEventID, EVENT_TIMER_PUFFER);
        }break;
    }
}

void LichKingAI::executeFirstWave(uint32 eventID)
{
    switch (eventID)
    {
        case 1:
        {
            DoScriptText(SAY_LICH_KING_WALL_01, me);
            eventMap.ScheduleEvent(2, 3 * IN_MILLISECONDS);
        }break;

        case 2:
        {
            DoCast(me, SPELL_RAISE_DEAD, true);
            DoScriptText(SAY_LICH_KING_GNOUL, me);

            eventMap.ScheduleEvent(3, EVENT_TIMER_ASAP);
        }break;

        case 3:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(me, SPELL_WINTER);
            DoScriptText(SAY_LICH_KING_WINTER, me);

            eventMap.ScheduleEvent(4, EVENT_TIMER_ASAP);
        }break;

        case 4:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(SPELL_SUMMON_WITCH_DOCTOR);
            eventMap.ScheduleEvent(EVENT_START_MOVEMENT, EVENT_TIMER_ASAP);
            eventMap.SetPhase(PHASE_FIRST_WAVE); // Intro wave finished
        }break;
    }
}

void LichKingAI::executeSecondWave(uint32 eventID)
{
    switch (eventID)
    {
        case 2:
        {
            if (applyPuffer(eventID))
                return;

            me->SetSpeed(MOVE_WALK, LICH_KING_SPEED, true);

            DoCast(SPELL_SUMMON_ABO);
            DoCast(SPELL_SUMMON_WITCH_DOCTOR);
            eventMap.ScheduleEvent(3, EVENT_TIMER_ASAP);
        }break;

        case 3:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(SPELL_SUMMON_WITCH_DOCTOR);
            eventMap.ScheduleEvent(EVENT_SPELL_WINTER, EVENT_TIMER_ASAP);
        }break;
    }
}

void LichKingAI::executeThirdWave(uint32 eventID)
{
    switch (eventID)
    {
        case 2:
        {
            if (applyPuffer(eventID))
                return;

            me->SetSpeed(MOVE_WALK, LICH_KING_SPEED, true);
            DoScriptText(SAY_LICH_KING_ABON, me);

            DoCast(SPELL_SUMMON_ABO);
            DoCast(SPELL_SUMMON_ABO);
            DoCast(SPELL_SUMMON_WITCH_DOCTOR);

            eventMap.ScheduleEvent(3, EVENT_TIMER_ASAP);
        }break;

        case 3:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(SPELL_SUMMON_WITCH_DOCTOR);
            eventMap.ScheduleEvent(4, EVENT_TIMER_ASAP);
        }break;

        case 4:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(SPELL_SUMMON_WITCH_DOCTOR);
            eventMap.ScheduleEvent(EVENT_SPELL_WINTER, EVENT_TIMER_ASAP);
        }break;

    }
}

void LichKingAI::executeFourthWave(uint32 eventID)
{
    switch (eventID)
    {
        case 2:
        {
            if (applyPuffer(eventID))
                return;

            me->SetSpeed(MOVE_WALK, LICH_KING_SPEED, true);

            DoCast(SPELL_SUMMON_ABO);
            DoCast(SPELL_SUMMON_ABO);
            DoCast(SPELL_SUMMON_WITCH_DOCTOR);

            eventMap.ScheduleEvent(3, EVENT_TIMER_ASAP);
        }break;

        case 3:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(SPELL_SUMMON_WITCH_DOCTOR);
            eventMap.ScheduleEvent(4, EVENT_TIMER_ASAP);
        }break;

        case 4:
        {
            if (applyPuffer(eventID))
                return;

            DoCast(SPELL_SUMMON_WITCH_DOCTOR);

            eventMap.ScheduleEvent(EVENT_SPELL_WINTER, EVENT_TIMER_ASAP);
            eventMap.ScheduleEvent(5, 15 * IN_MILLISECONDS);
        }break;

        case 5:
        {
            DoScriptText(SAY_LICH_KING_ABON, me);
            DoCast(SPELL_SUMMON_WITCH_DOCTOR);

            eventMap.ScheduleEvent(6, EVENT_TIMER_ASAP);
        }break;

        case 6:
        {
            DoCast(SPELL_SUMMON_WITCH_DOCTOR);
            eventMap.ScheduleEvent(EVENT_START_MOVEMENT, EVENT_TIMER_ASAP);
        }break;
    }
}

void LichKingAI::setWave(uint32 wavePhase)
{
    SetEscortPaused(true);
    me->StopMoving();

    pufferEventID = NULL;
    eventMap.AddPhase(wavePhase);
}

bool LichKingAI::applyPuffer(uint32 eventID)
{
    if (pufferEventID != eventID)
    {
        pufferEventID = eventID;
        eventMap.ScheduleEvent(pufferEventID, pufferEventID == EVENT_SPELL_WINTER ? EVENT_TIMER_PUFFER_EXT : EVENT_TIMER_PUFFER);
        return true;
    }

    return false;
}
