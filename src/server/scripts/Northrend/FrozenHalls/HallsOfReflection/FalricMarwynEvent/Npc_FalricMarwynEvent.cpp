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

#include "Npc_FalricMarwynEvent.h"
#include "Encounter_FalricMarwynEvent.h"

using namespace NPC_FALRICMARWYNEVENT;


typedef Npc_GhostlyPriest::Npc_GhostlyPriestAI                  GhostlyPriestAI;
typedef Npc_PhantomMage::Npc_PhantomMageAI                      PhantomMageAI;
typedef Npc_PhantomHallucination::Npc_PhantomHallucinationAI    HallucinationAI;
typedef Npc_ShadowyMercenary::Npc_ShadowyMercenaryAI            ShadowyMercenaryAI;
typedef Npc_SpectralFootman::Npc_SpectralFootmanAI              SpectralFootmanAI;
typedef Npc_TorturedRifleman::Npc_TorturedRiflemanAI            TorturedRiflemanAI;


void Npc_FalricMarwynEventAI::Reset()
{
    eventMap.Reset();
}

void Npc_FalricMarwynEventAI::SpellHit(Unit* /*pUnit*/, const SpellInfo* spellInfo)
{
    if (spellInfo->Id == ENCOUNTER_FALRIC_MARWYN_EVENT::SPELL_SPIRIT_ACTIVATE)
    {
        me->setFaction(16);
        me->SetReactState(REACT_AGGRESSIVE);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetInCombatWithZone();
    }
}

void Npc_FalricMarwynEventAI::UpdateAI(const uint32 diff)
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


void GhostlyPriestAI::EnterCombat(Unit* /*who*/)
{
    eventMap.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8 * IN_MILLISECONDS); // TODO: adjust timers
    eventMap.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_COWER_IN_FEAR, 10 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_DARK_MENDING, 20 * IN_MILLISECONDS);
}

void GhostlyPriestAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SHADOW_WORD_PAIN:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_SHADOW_WORD_PAIN);

            timer = 8 * IN_MILLISECONDS;
        }return;

        case EVENT_CIRCLE_OF_DESTRUCTION:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_CIRCLE_OF_DESTRUCTION);

            timer = 12 * IN_MILLISECONDS;
        }return;

        case EVENT_COWER_IN_FEAR:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_COWER_IN_FEAR);

            timer = 10 * IN_MILLISECONDS;
        }return;

        case EVENT_DARK_MENDING:
        {
            // find an ally with missing HP
            if (Unit* pUnit = DoSelectLowestHpFriendly(40, DUNGEON_MODE(30 * IN_MILLISECONDS, 50 * IN_MILLISECONDS)))
            {
                DoCast(pUnit, SPELL_DARK_MENDING);
                timer = 20 * IN_MILLISECONDS;
            }
            else // no friendly unit with missing hp. re-check in just 5 sec.
                timer = 5 * IN_MILLISECONDS;
        }return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void PhantomMageAI::EnterCombat(Unit* /*who*/)
{
    eventMap.ScheduleEvent(EVENT_FIREBALL, 3 * IN_MILLISECONDS); // TODO: adjust timers
    eventMap.ScheduleEvent(EVENT_FLAMESTRIKE, 6 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_FROSTBOLT, 9 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_CHAINS_OF_ICE, 12 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_HALLUCINATION, 40 * IN_MILLISECONDS);
}

void PhantomMageAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_FIREBALL:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_FIREBALL);

            timer = 15 * IN_MILLISECONDS;
        }return;

        case EVENT_FLAMESTRIKE:
        {
            DoCast(SPELL_FLAMESTRIKE);
            timer = 15 * IN_MILLISECONDS;
        }return;

        case EVENT_FROSTBOLT:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_FROSTBOLT);

            timer = 15 * IN_MILLISECONDS;
        }return;

        case EVENT_CHAINS_OF_ICE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_CHAINS_OF_ICE);

            timer = 15 * IN_MILLISECONDS;
        }return;

        case EVENT_HALLUCINATION:
        {
            DoCast(SPELL_HALLUCINATION);
        }return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void HallucinationAI::JustDied(Unit* /*killer*/)
{
    DoCast(SPELL_HALLUCINATION_2);
}


void ShadowyMercenaryAI::EnterCombat(Unit* /*who*/)
{
    eventMap.ScheduleEvent(EVENT_SHADOW_STEP, 8 * IN_MILLISECONDS); // TODO: adjust timers
    eventMap.ScheduleEvent(EVENT_DEADLY_POISON, 5 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_KIDNEY_SHOT, 12 * IN_MILLISECONDS);
}

void ShadowyMercenaryAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SHADOW_STEP:
        {
            DoCast(SPELL_SHADOW_STEP);
            timer = 8 * IN_MILLISECONDS;
        }return;

        case EVENT_DEADLY_POISON:
        {
            DoCast(me->getVictim(), SPELL_DEADLY_POISON);
            timer = 10 * IN_MILLISECONDS;
        }return;

        case EVENT_ENVENOMED_DAGGER_THROW:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_ENVENOMED_DAGGER_THROW);

            timer = 10 * IN_MILLISECONDS;
        }return;

        case EVENT_KIDNEY_SHOT:
        {
            DoCast(me->getVictim(), SPELL_KIDNEY_SHOT);
            timer = 10 * IN_MILLISECONDS;
        }return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void SpectralFootmanAI::EnterCombat(Unit* /*who*/)
{
    eventMap.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5 * IN_MILLISECONDS); // TODO: adjust timers
    eventMap.ScheduleEvent(EVENT_SHIELD_BASH, 10 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15 * IN_MILLISECONDS);
}

void SpectralFootmanAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPECTRAL_STRIKE:
        {
            DoCast(me->getVictim(), SPELL_SPECTRAL_STRIKE);
            timer = 5 * IN_MILLISECONDS;
        }return;

        case EVENT_SHIELD_BASH:
        {
            DoCast(me->getVictim(), SPELL_SHIELD_BASH);
            timer = 5 * IN_MILLISECONDS;
        }return;

        case EVENT_TORTURED_ENRAGE:
        {
            DoCast(SPELL_TORTURED_ENRAGE);
            timer = 15 * IN_MILLISECONDS;
        }return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void TorturedRiflemanAI::EnterCombat(Unit* /*who*/)
{
    eventMap.ScheduleEvent(EVENT_SHOOT, 2 * IN_MILLISECONDS); // TODO: adjust timers
    eventMap.ScheduleEvent(EVENT_CURSED_ARROW, 10 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_FROST_TRAP, 1 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_ICE_SHOT, 15 * IN_MILLISECONDS);
}

void TorturedRiflemanAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SHOOT:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_SHOOT);

            timer = 2 * IN_MILLISECONDS;
        }return;

        case EVENT_CURSED_ARROW:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_CURSED_ARROW);

            timer = 10 * IN_MILLISECONDS;
        }return;

        case EVENT_FROST_TRAP:
        {
            DoCast(SPELL_FROST_TRAP);
            timer = 30 * IN_MILLISECONDS;
        }return;

        case EVENT_ICE_SHOT:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM))
                DoCast(pUnit, SPELL_ICE_SHOT);

            timer = 15 * IN_MILLISECONDS;
        }return;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
