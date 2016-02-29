/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 22.09.2014
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

#include "Boss_Falric.h"
#include "../Encounter_FalricMarwynEvent.h"

using namespace BOSS_FALRIC;


typedef Boss_Falric::Boss_FalricAI FalricAI;

void FalricAI::Reset()
{
    eventMap.Reset();
    me->SetVisible(false);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->SetReactState(REACT_PASSIVE);
}

void FalricAI::MovementInform(uint32 type, uint32 point)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (point == 1)
    {
        me->SetFacingTo(ENCOUNTER_FALRIC_MARWYN_EVENT::FalricEventPos.GetOrientation());
        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
    }
}

void FalricAI::EnterCombat(Unit* /*who*/)
{
    DoScriptText(SAY_AGGRO, me);

    eventMap.ScheduleEvent(EVENT_QUIVERING_STRIKE, 23 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_IMPENDING_DESPAIR, 9 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_DEFILING_HORROR, urand(25 * IN_MILLISECONDS, 45 * IN_MILLISECONDS)); // TODO adjust timer
}

void FalricAI::DamageTaken(Unit* /*pDoneBy*/, uint32& /*damage*/)
{
    if ((eventMap.GetPhaseMask() & PHASE_MASK_HOPELESSNESS_FIRST) == NULL && HealthBelowPct(66))
        eventMap.AddPhase(PHASE_HOPELESSNESS_FIRST);
    else if ((eventMap.GetPhaseMask() & PHASE_MASK_HOPELESSNESS_SECOND) == NULL && HealthBelowPct(33))
        eventMap.AddPhase(PHASE_HOPELESSNESS_SECOND);
    else if ((eventMap.GetPhaseMask() & PHASE_MASK_HOPELESSNESS_THIRD) == NULL && HealthBelowPct(10))
        eventMap.AddPhase(PHASE_HOPELESSNESS_THIRD);
    else
        return;

    DoCast(DUNGEON_MODE(SPELL_HOPELESSNESS, H_SPELL_HOPELESSNESS));
}

void FalricAI::KilledUnit(Unit* /*victim*/)
{
    DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
}

void FalricAI::JustDied(Unit* /*killer*/)
{
    DoScriptText(SAY_DEATH, me);

    if (pInstance)
        pInstance->SetBossState(DATA_FALRIC_EVENT, DONE);
}

void FalricAI::EnterEvadeMode()
{
    ScriptedAI::EnterEvadeMode();

    if (pInstance)
        pInstance->SetBossState(DATA_FALRIC_EVENT, FAIL);
}

void FalricAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    eventMap.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        executeEvent(eventID);
        return;
    }

    DoMeleeAttackIfReady();
}

void FalricAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_QUIVERING_STRIKE:
        {
            DoCast(SPELL_QUIVERING_STRIKE);
            timer = 10 * IN_MILLISECONDS;
        }break;

        case EVENT_IMPENDING_DESPAIR:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
            {
                DoScriptText(SAY_IMPENDING_DESPAIR, me);
                DoCast(pUnit, SPELL_IMPENDING_DESPAIR);
            }

            timer = 13 * IN_MILLISECONDS;
        }break;

        case EVENT_DEFILING_HORROR:
        {
            DoCast(SPELL_DEFILING_HORROR);
            timer = urand(25 * IN_MILLISECONDS, 45 * IN_MILLISECONDS); // TODO adjust timer.
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
