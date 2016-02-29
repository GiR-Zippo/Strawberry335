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

#include "Vehicle_GrandChampions.h"


void MountedChampionAI::Reset()
{
    me->Mount(getMountId());
    defeated = false;

    if (pInstance)
        SetEquipmentSlots(false, pInstance->GetData(DATA_TEAM) == ALLIANCE ? 46070 : 46069);
}

void MountedChampionAI::EnterCombat(Unit* /*who*/)
{
    defeated = false;

    eventMap.ScheduleEvent(EVENT_SPELL_CHARGE, urand(1 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_SHIELD_BREAKER, 8 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_SHIELD, urand(4 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));

    // Set Defend to 3 charges at start
    for (uint8 i = 0; i < 3; ++i)
        DoCast(me, SPELL_SHIELD, true);
}

void MountedChampionAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (!isGrandChampion())
        return;

    if (defeated)
    {
        damage = 0;
        return;
    }

    if (damage >= me->GetHealth())
    {
        damage = 0;
        defeated = true;
        me->GetMotionMaster()->MoveIdle();
        me->Dismount();
        me->RemoveAurasDueToSpell(SPELL_SHIELD);
        me->CastSpell(me, SPELL_KNEE, true);
        me->SetTarget(0);

        if (pInstance)
            pInstance->SetData(ENCOUNTER_DATA_GC_BOSS_DEFEATED, 1);
    }
}

void MountedChampionAI::JustDied(Unit* /*killer*/)
{
    if (!isGrandChampion())
        pInstance->SetData(ENCOUNTER_DATA_GC_ADDS_DEFEATED, 1);
}

void MountedChampionAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim() || defeated)
        return;

    eventMap.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        executeEvent(eventID);
        return;
    }

    // Use Thrust instead of melee attack
    if (me->isAttackReady() && me->IsWithinMeleeRange(me->getVictim()))
    {
        me->AddUnitState(UNIT_STATE_ONVEHICLE);
        DoCast(me->getVictim(), SPELL_THRUST);
        me->ResetAttackTimer();
        me->ClearUnitState(UNIT_STATE_ONVEHICLE);
    }
}

void MountedChampionAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_CHARGE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_FARTHEST))
            {
                DoResetThreat();
                me->AddThreat(pUnit, 5.0f);
                DoCast(pUnit, SPELL_CHARGE, true);
            }

            timer = 5000;
        }break;

        case EVENT_SPELL_SHIELD_BREAKER:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_FARTHEST))
                DoCast(pUnit, SPELL_SHIELD_BREAKER, true);

            timer = 7 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_SHIELD:
        {
            Aura* pAura = me->GetAura(SPELL_SHIELD);
            if (!pAura || pAura->GetStackAmount() < 3)
            {
                DoCast(SPELL_SHIELD);
                timer = urand(4 * IN_MILLISECONDS, 5 * IN_MILLISECONDS);
            }
            else
                timer = urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS);
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}

uint32 MountedChampionAI::getMountId()
{
    switch (me->GetEntry())
    {
        // Alliance
        case NPC_STORMWIND_CHAMPION:
            return 28912;
        case NPC_IRONFORGE_CHAMPION:
            return 29258;
        case NPC_GNOMEREGAN_CHAMPION:
            return 28571;
        case NPC_DARNASSUS_CHAMPION:
            return 29256;
        case NPC_EXODAR_CHAMPION:
            return 29255;
        case NPC_JACOB:
            return 29284;
        case NPC_AMBROSE:
            return 28571;
        case NPC_COLOSOS:
            return 29255;
        case NPC_JAELYNE:
            return 9991;
        case NPC_LANA:
            return 2787;

        // Horde
        case NPC_ORGRIMMAR_CHAMPION:
            return 29260;
        case NPC_THUNDER_BLUFF_CHAMPION:
            return 29259;
        case NPC_UNDERCITY_CHAMPION:
            return 29257;
        case NPC_SENJIN_CHAMPION:
            return 29261;
        case NPC_SILVERMOON_CHAMPION:
            return 29262;
        case NPC_MOKRA:
            return 29879;
        case NPC_ERESSEA:
            return 28607;
        case NPC_RUNOK:
            return 29880;
        case NPC_ZULTORE:
            return 29261;
        case NPC_VISCERI:
            return 10718;
    }

    return 0;
}

bool MountedChampionAI::isGrandChampion()
{
    return (me->GetEntry() == NPC_MOKRA   || me->GetEntry() == NPC_JACOB   ||
            me->GetEntry() == NPC_ERESSEA || me->GetEntry() == NPC_AMBROSE ||
            me->GetEntry() == NPC_RUNOK   || me->GetEntry() == NPC_COLOSOS ||
            me->GetEntry() == NPC_ZULTORE || me->GetEntry() == NPC_JAELYNE ||
            me->GetEntry() == NPC_VISCERI || me->GetEntry() == NPC_LANA);
}

bool MountedChampionAI::isInMountedGauntlet()
{
    if (pInstance)
        if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) == IN_PROGRESS)
            return true;
    return false;
}
