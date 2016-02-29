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

#include "Boss_Marwyn.h"
#include "../Encounter_FalricMarwynEvent.h"

using namespace BOSS_MARWYN;


typedef Boss_Marwyn::Boss_MarwynAI MarwynAI;

void MarwynAI::Reset()
{
    eventMap.Reset();
    me->SetVisible(false);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->SetReactState(REACT_PASSIVE);
}

void MarwynAI::MovementInform(uint32 type, uint32 point)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (point == 1)
    {
        me->SetFacingTo(ENCOUNTER_FALRIC_MARWYN_EVENT::MarwynEventPos.GetOrientation());
        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
    }
}

void MarwynAI::EnterCombat(Unit* /*who*/)
{
    DoScriptText(SAY_AGGRO, me);

    eventMap.ScheduleEvent(EVENT_OBLITERATE, 30 * IN_MILLISECONDS); // TODO Check timer
    eventMap.ScheduleEvent(EVENT_WELL_OF_CORRUPTION, 13 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_CORRUPTED_FLESH, 20 * IN_MILLISECONDS);
    // events.ScheduleEvent(EVENT_SHARED_SUFFERING, 20 * IN_MILLISECONDS); // TODO Check timer Spell Buggy

    if (pInstance)
        pInstance->SetBossState(DATA_MARWYN_EVENT, IN_PROGRESS);
}

void MarwynAI::KilledUnit(Unit* /*victim*/)
{
    DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
}

void MarwynAI::JustDied(Unit* /*killer*/)
{
    DoCast(me, SPELL_START_HALLS_OF_REFLECTION_QUEST_AE);
    DoScriptText(SAY_DEATH, me);

    if (pInstance)
        pInstance->SetBossState(DATA_MARWYN_EVENT, DONE);
}

void MarwynAI::EnterEvadeMode()
{
    ScriptedAI::EnterEvadeMode();

    if (pInstance)
        pInstance->SetBossState(DATA_MARWYN_EVENT, FAIL);
}

void MarwynAI::UpdateAI(const uint32 diff)
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

void MarwynAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_OBLITERATE:
        {
            DoCast(SPELL_OBLITERATE);
            timer = 30 * IN_MILLISECONDS;
        }break;

        case EVENT_WELL_OF_CORRUPTION:
        {
            DoCast(SPELL_WELL_OF_CORRUPTION);
            timer = 13 * IN_MILLISECONDS;
        }break;

        case EVENT_CORRUPTED_FLESH:
        {
            DoScriptText(RAND(SAY_CORRUPTED_FLESH_1, SAY_CORRUPTED_FLESH_2), me);
            DoCast(SPELL_CORRUPTED_FLESH);

            timer = 20 * IN_MILLISECONDS;
        }break;

        case EVENT_SHARED_SUFFERING:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_SHARED_SUFFERING);

            timer = 20 * IN_MILLISECONDS;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
