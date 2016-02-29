/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
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

#include "Npc_ArgentumChallenge.h"
#include "Data_ArgentumChallenge.h"


typedef Npc_Memory::Npc_MemoryAI                            MemoryAI;
typedef Npc_ArgentMonk::Npc_ArgentMonkAI                    ArgentMonkAI;
typedef Npc_ArgentLightwielder::Npc_ArgentLightwielderAI    ArgentLightwielderAI;
typedef Npc_ArgentPriest::Npc_ArgentPriestAI                ArgentPriestAI;

void MemoryAI::Reset()
{
    if (pInstance)
        pInstance->SetData(DATA_MEMORY_ENTRY, me->GetEntry());

    DoCast(SPELL_SHADOW_FORM);
    eventMap.Reset();
}

void MemoryAI::EnterCombat(Unit* /*attacker*/)
{
    if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != IN_PROGRESS)
            me->DespawnOrUnsummon();

    eventMap.ScheduleEvent(EVENT_SPELL_OLD_WOUNDS, 12 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_SHADOWS_PAST, 5 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_WAKING_NIGHTMARE, 7 * IN_MILLISECONDS);
}

void MemoryAI::JustDied(Unit* /*killer*/)
{
    if (TempSummon* pSummon = me->ToTempSummon())
        if (Unit* pUnit = pSummon->GetSummoner())
            if (pUnit->isAlive() && pUnit->GetAI())
                pUnit->GetAI()->DoAction(1);
}

void MemoryAI::UpdateAI(const uint32 diff)
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

void MemoryAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_OLD_WOUNDS:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                if (pUnit && pUnit->isAlive())
                    DoCast(pUnit, SPELL_OLD_WOUNDS);

            timer = 12 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_SHADOWS_PAST:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                if (pUnit && pUnit->isAlive())
                    DoCast(pUnit, SPELL_SHADOWS_PAST);

            timer = 5 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_WAKING_NIGHTMARE:
        {
            DoScriptText(SAY_PALETRESS_NIGHTMARE_WARNING, me);
            DoCast(me, SPELL_WAKING_NIGHTMARE);

            timer = 15 * IN_MILLISECONDS;
        }break;
    }

    if (timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void ArgentMonkAI::Reset()
{
    shieldCasted = false;
    eventMap.Reset();
}

//void ArgentMonkAI::JustReachedHome()
//{
//    if (pInstance)
//        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) == IN_PROGRESS)
//            pInstance->SetData(BOSS_ARGENT_CHALLENGE, FAIL);
//}

void ArgentMonkAI::EnterCombat(Unit* /*attacker*/)
{
    eventMap.ScheduleEvent(EVENT_SPELL_FLURRY_OF_BLOWS, 2 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_PUMMEL, 1 * IN_MILLISECONDS);

    /*if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != IN_PROGRESS)
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, IN_PROGRESS);*/
}

void ArgentMonkAI::DamageTaken(Unit* /*attacker*/, uint32 & damage)
{
    if (damage >= me->GetHealth() && !shieldCasted)
    {
        damage = 0;
        DoCast(me, SPELL_DIVINE_SHIELD, true);
        DoCastVictim(SPELL_FINAL_MEDITATION);
        shieldCasted = true;
    }
}

void ArgentMonkAI::JustDied(Unit* /*killer*/)
{
    if (pInstance)
        pInstance->SetData(ENCOUNTER_DATA_AC_ADDS_DEFEATED, 1);
}

void ArgentMonkAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    eventMap.Update(diff);

    if (me->HasAura(SPELL_DIVINE_SHIELD) || me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        executeEvent(eventID);
        return;
    }

    DoMeleeAttackIfReady();
}

void ArgentMonkAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_FLURRY_OF_BLOWS:
        {
            DoCast(me, SPELL_FLURRY_OF_BLOWS);
            timer = urand(7 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_PUMMEL:
        {
            DoCastVictim(SPELL_PUMMEL);
            timer = urand(3 * IN_MILLISECONDS, 6 * IN_MILLISECONDS);
        }break;
    }

    if (timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void ArgentLightwielderAI::Reset()
{
    eventMap.Reset();
}

//void ArgentLightwielderAI::JustReachedHome()
//{
//    if (pInstance)
//        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) == IN_PROGRESS)
//            pInstance->SetData(BOSS_ARGENT_CHALLENGE, FAIL);
//}

void ArgentLightwielderAI::EnterCombat(Unit* /*attacker*/)
{
    eventMap.ScheduleEvent(EVENT_SPELL_BLAZING_LIGHT, 5 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_CLEAVE, 2 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_UNBALANCING_STRIKE, 3 * IN_MILLISECONDS);

    /*if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != IN_PROGRESS)
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, IN_PROGRESS);*/
}

void ArgentLightwielderAI::JustDied(Unit* /*killer*/)
{
    if (pInstance)
        pInstance->SetData(ENCOUNTER_DATA_AC_ADDS_DEFEATED, 1);
}

void ArgentLightwielderAI::UpdateAI(const uint32 diff)
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

void ArgentLightwielderAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_BLAZING_LIGHT:
        {
            Unit* pUnit = DoSelectLowestHpFriendly(40);
            if (!pUnit || pUnit->GetHealth() > me->GetHealth())
                pUnit = me;

            DoCast(pUnit, SPELL_BLAZING_LIGHT);
            timer = urand(8 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_CLEAVE:
        {
            DoCastVictim(SPELL_CLEAVE);
            timer = urand(7 * IN_MILLISECONDS, 9 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_UNBALANCING_STRIKE:
        {
            DoCastVictim(SPELL_UNBALANCING_STRIKE);
            timer = urand(3 * IN_MILLISECONDS, 6 * IN_MILLISECONDS);
        }break;
    }

    if (timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void ArgentPriestAI::Reset()
{
    eventMap.Reset();
}

//void ArgentPriestAI::JustReachedHome()
//{
//    if (pInstance)
//        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) == IN_PROGRESS)
//            pInstance->SetData(BOSS_ARGENT_CHALLENGE, FAIL);
//}

void ArgentPriestAI::EnterCombat(Unit* /*attacker*/)
{
    eventMap.ScheduleEvent(EVENT_SPELL_SMITE, 1 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_SHADOW_WORD_PAIN, 2 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_FOUNTAIN_OF_LIGHT, 9 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_MIND_CONTROL, 7 * IN_MILLISECONDS);

    if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != IN_PROGRESS)
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, IN_PROGRESS);
}

void ArgentPriestAI::JustDied(Unit* /*killer*/)
{
    if (pInstance)
        pInstance->SetData(ENCOUNTER_DATA_AC_ADDS_DEFEATED, 1);
}

void ArgentPriestAI::UpdateAI(const uint32 diff)
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

void ArgentPriestAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_SMITE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, IsHeroic() ? SPELL_HOLY_SMITE_H : SPELL_HOLY_SMITE);

            timer = urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_SHADOW_WORD_PAIN:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                if (!pUnit->HasAura(IsHeroic() ? SPELL_SHADOW_WORD_PAIN_H : SPELL_SHADOW_WORD_PAIN))
                    DoCast(pUnit, IsHeroic() ? SPELL_SHADOW_WORD_PAIN_H : SPELL_SHADOW_WORD_PAIN);

            timer = urand(3 * IN_MILLISECONDS, 5 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_MIND_CONTROL:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_MIND_CONTROL);

            timer = urand(12 * IN_MILLISECONDS, 16 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_FOUNTAIN_OF_LIGHT:
        {
            DoCast(SPELL_FOUNTAIN_OF_LIGHT);
            timer = urand(40 * IN_MILLISECONDS, 45 * IN_MILLISECONDS);
        }break;
    }

    if (timer)
        eventMap.ScheduleEvent(eventID, timer);
}
