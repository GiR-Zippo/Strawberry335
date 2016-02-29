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

#include "Npc_BlackKnight.h"
#include "Vehicle.h"


typedef Npc_RisenGhoul::Npc_RisenGhoulAI RisenGhoulAI;
typedef Npc_BlackKnight_SkeletalGryphon::Npc_BlackKnight_SkeletalGryphonAI SkeletalGryphonAI;

void RisenGhoulAI::Reset()
{
    eventMap.Reset();

    if (pInstance)
        if (Creature* pCreature = me->GetCreature(*me, pInstance->GetData64(NPC_BLACK_KNIGHT)))
            pCreature->AI()->JustSummoned(me);
}

void RisenGhoulAI::EnterCombat(Unit* pUnit)
{
    eventMap.ScheduleEvent(EVENT_SPELL_CLAW, 3.5 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_LEAP, 1 * IN_MILLISECONDS);
};

void RisenGhoulAI::SpellHitTarget(Unit* /*victim*/, const SpellInfo* spell)
{
    if (spell->Id == SPELL_EXPLODE_H && pInstance)
        pInstance->SetData(DATA_I_VE_HAD_WORSE, (uint32)false);
}

void RisenGhoulAI::UpdateAI(const uint32 diff)
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

void RisenGhoulAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_CLAW:
        {
            DoCastVictim(SPELL_CLAW);
            timer = urand(1 * IN_MILLISECONDS, 3.5 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_LEAP:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_FARTHEST, 0, 30, true))
            {
                DoResetThreat();
                me->AddThreat(pUnit, 5.0f);
                DoCast(pUnit, SPELL_LEAP);
            }

            timer = urand(7 * IN_MILLISECONDS, 10 * IN_MILLISECONDS);
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}

void SkeletalGryphonAI::DoAction(int32 action)
{
    if (action == 1)
        Start(false, true, 0, NULL);
}

void SkeletalGryphonAI::WaypointReached(uint32 id)
{
    switch (id)
    {
        case 13:
        {
            if (Creature* pCreature = me->GetCreature(*me, pInstance->GetData64(NPC_JAEREN)))
                me->SetFacingToObject(pCreature);

            if (Unit* pUnit = pVehicleKit->GetPassenger(0))
                pUnit->ExitVehicle();

            if (pInstance)
                pInstance->DoAction(ENCOUNTER_ACTION_BK_GRYPHON_FINISHED);
        }break;
    }
}
