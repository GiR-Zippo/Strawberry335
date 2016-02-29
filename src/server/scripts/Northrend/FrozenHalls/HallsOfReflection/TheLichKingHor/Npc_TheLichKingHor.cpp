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

#include "Npc_TheLichKingHor.h"
#include "Encounter_TheLichKingHor.h"

using namespace NPC_THELICHKINGHOR;


typedef Npc_RagingGhoul::Npc_RagingGhoulAI              RagingGhoulAI;
typedef Npc_RisenWitchDoctor::Npc_RisenWitchDoctorAI    WitchDoctorAI;
typedef Npc_Abon::Npc_AbonAI                            AbonAI;


void Npc_TheLichKingHorAI::DoAction(int32 action)
{
    switch (action)
    {
        case ACTION_MOVEMENT_THIRD_WALL:
        {
            eventMap.CancelEvent(EVENT_EMERGE);
            me->GetMotionMaster()->MovePoint(ACTION_MOVEMENT_THIRD_WALL, ThirdWaveMovePosition);
        }break;

        case ACTION_MOVEMENT_FOURTH_WALL:
        {
            eventMap.CancelEvent(EVENT_EMERGE);
            me->GetMotionMaster()->MovePoint(ACTION_MOVEMENT_FOURTH_WALL, FourthWaveMovePosition);
        }break;
    }
}

void Npc_TheLichKingHorAI::Reset()
{
    if (!pInstance)
        return;

    DoCast(me, SPELL_EMERGE_VISUAL);

    eventMap.Reset();
    eventMap.ScheduleEvent(EVENT_EMERGE, ((me->GetEntry() == NPC_RAGING_GHOUL ? 3 : 0.5) * IN_MILLISECONDS));
}

void Npc_TheLichKingHorAI::MovementInform(uint32 type, uint32 point)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (point == ACTION_MOVEMENT_THIRD_WALL || point == ACTION_MOVEMENT_FOURTH_WALL)
    {
        if (pInstance && pInstance->GetBossState(DATA_LICHKING_EVENT) == IN_PROGRESS)
        {
            me->SetInCombatWithZone();

            if (Creature* pCreature = sObjectAccessor->GetCreature(*me, pInstance->GetData64(NPC_JAINA_OR_SYLVANAS_O)))
            {
                me->AddThreat(pCreature, 100.0f);
                me->AI()->AttackStart(pCreature);
            }
        }
    }
}

void Npc_TheLichKingHorAI::JustDied(Unit* pUnit)
{
    if (!pInstance)
        return;

    pInstance->SetData(ENCOUNTER_THELICHKINGHOR::ENCOUNTER_DATA_ADDS_DEFEATED, 1);
}

void Npc_TheLichKingHorAI::UpdateAI(const uint32 diff)
{
    eventMap.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        if (eventID == EVENT_EMERGE)
        {
            if (pInstance && pInstance->GetBossState(DATA_LICHKING_EVENT) == IN_PROGRESS)
            {
                me->SetInCombatWithZone();

                if (Creature* pCreature = sObjectAccessor->GetCreature(*me, pInstance->GetData64(NPC_JAINA_OR_SYLVANAS_O)))
                {
                    me->AddThreat(pCreature, 100.0f);
                    me->AI()->AttackStart(pCreature);
                }
            }
        }
        else
            executeEvent(eventID);
        return;
    }

    if (!eventMap.IsEventActive(EVENT_EMERGE))
        if (UpdateVictim())
            DoMeleeAttackIfReady();
}

void RagingGhoulAI::EnterCombat(Unit* /*attacker*/)
{
    eventMap.ScheduleEvent(EVENT_SPELL_GNOUL_JUMP, 2 * IN_MILLISECONDS);
}

void RagingGhoulAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_GNOUL_JUMP:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f))
            {
                if (me->GetDistance(pUnit) >= 5.0f)
                {
                    DoCast(pUnit, SPELL_GNOUL_JUMP);
                    return;
                }
            }

            timer = 1 * IN_MILLISECONDS;
        }

        default:
            return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void WitchDoctorAI::EnterCombat(Unit* /*attacker*/)
{
    eventMap.ScheduleEvent(EVENT_COURSE_OF_DOOM, 7 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_SHADOW_BOLT_VOLLEY, 15 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_SHADOW_BOLT, 6 * IN_MILLISECONDS);
}

void WitchDoctorAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_COURSE_OF_DOOM:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f))
                DoCast(pUnit, SPELL_COURSE_OF_DOOM);

            timer = urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_SHADOW_BOLT_VOLLEY:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 30.0f))
                DoCast(pUnit, SPELL_SHADOW_BOLT_VOLLEY);

            timer = urand(15 * IN_MILLISECONDS, 22 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_SHADOW_BOLT:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 30.0f))
                DoCast(pUnit, SPELL_SHADOW_BOLT);

            timer = urand(2 * IN_MILLISECONDS, 3 * IN_MILLISECONDS);
        }break;

        default:
            return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void AbonAI::EnterCombat(Unit* /*attacker*/)
{
    eventMap.ScheduleEvent(EVENT_SPELL_ABON_STRIKE, 6 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_VOMIT_SPRAY, 15 * IN_MILLISECONDS);
}

void AbonAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_ABON_STRIKE:
        {
            if (Unit *target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 5.0f))
                DoCast(target, SPELL_ABON_STRIKE);

            timer = urand(7 * IN_MILLISECONDS, 9 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_VOMIT_SPRAY:
        {
            if (Unit *target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 5.0f))
                DoCast(target, SPELL_VOMIT_SPRAY);

            timer = urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS);
        }break;

        default:
            return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
