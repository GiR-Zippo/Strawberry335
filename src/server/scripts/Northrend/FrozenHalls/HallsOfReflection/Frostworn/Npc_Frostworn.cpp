/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.09.2014
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

#include "Npc_Frostworn.h"

using namespace NPC_FROSTWORN;


typedef Npc_FrostwornGeneral::Npc_FrostwornGeneralAI        GeneralAI;
typedef Npc_SpiritualReflection::Npc_SpiritualReflectionAI  ReflectionAI;

void GeneralAI::Reset()
{
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    eventMap.Reset();

    if (pInstance)
        pInstance->SetBossState(DATA_FROSWORN_EVENT, NOT_STARTED);
}

void GeneralAI::JustDied(Unit* pKiller)
{
    DoScriptText(SAY_DEATH, me);

    if (pInstance)
        pInstance->SetBossState(DATA_FROSWORN_EVENT, DONE);
}

void GeneralAI::MoveInLineOfSight(Unit* pUnit)
{
    if (!pInstance)
        return;

    if (me->getVictim())
        return;

    if (pUnit->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(pUnit, 20.0f))
        return;

    if (!pInstance->CheckRequiredBosses(DATA_FROSWORN_EVENT))
    {
        pInstance->DoSendNotifyToInstance(BOSS_REQUIREMENTS, "'Marwyn'");
        return;
    }

    if (Player* pPlayer = pUnit->ToPlayer())
        if (pPlayer->isGameMaster())
            return;

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    AttackStart(pUnit);
}

void GeneralAI::EnterCombat(Unit* /*victim*/)
{
    DoScriptText(SAY_AGGRO, me);

    eventMap.ScheduleEvent(EVENT_SPELL_SHIELD_THROWN, 5 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_SPIKE, 14 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_CLONE, 22 * IN_MILLISECONDS);

    if (pInstance)
        pInstance->SetBossState(DATA_FROSWORN_EVENT, IN_PROGRESS);
}

void GeneralAI::UpdateAI(const uint32 diff)
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

void GeneralAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_SHIELD_THROWN:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pUnit, SPELL_SHIELD_THROWN);

            timer = urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_SPIKE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pUnit, SPELL_SPIKE);

            timer = urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS);
        }break;

        case EVENT_CLONE:
        {
            DoCast(SPELL_SUMMON_REFLECTIONS);
            timer = 60 * IN_MILLISECONDS;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void ReflectionAI::Reset()
{
    eventMap.Reset();
}

void ReflectionAI::EnterCombat(Unit* /*victim*/)
{
    eventMap.ScheduleEvent(EVENT_SPELL_BALEFUL_STRIKE, urand(1 * IN_MILLISECONDS, 3 * IN_MILLISECONDS));
}

void ReflectionAI::JustDied(Unit* pUnit)
{
    DoCast(pUnit, SPELL_SPIRIT_BURST);
}

void ReflectionAI::UpdateAI(const uint32 diff)
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

void ReflectionAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_BALEFUL_STRIKE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pUnit, SPELL_BALEFUL_STRIKE);

            timer = urand(3 * IN_MILLISECONDS, 8 * IN_MILLISECONDS);
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
