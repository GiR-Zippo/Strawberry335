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

#include "Misc_HallsOfReflection.h"

using namespace MISC_HALLSOFREFLECTION;


typedef Npc_Queldelar::Npc_QueldelarAI QueldelarAI;
typedef NpcFrostmourneHor::NpcFrostmourneHorAI FrostmourneHorAI;

bool At_WavesRestarterHor::OnTrigger(Player* pPlayer, AreaTriggerEntry const* /*trigger*/)
{
    InstanceScript* pInstance = pPlayer->GetInstanceScript();
    if(!pInstance)
        return true;

    if (pPlayer->isGameMaster())
        return true;

    if (pInstance->GetBossState(DATA_INTRO_EVENT) != DONE)
        return true;

    if (pInstance->GetBossState(DATA_FALRIC_EVENT) == DONE && pInstance->GetBossState(DATA_MARWYN_EVENT) == DONE)
        return true;

    if (!pInstance->GetData(DATA_CURRENT_ENCOUNTER))
        pInstance->SetData(DATA_CURRENT_ENCOUNTER, ENCOUNTER_VALUE_F_M_E_WAVE_RESTART);

    return true;
}


void QueldelarAI::MoveInLineOfSight(Unit* pUnit)
{
    if (!pUnit)
        return;

    if (me->IsWithinDistInMap(pUnit, 20) && pUnit->HasAura(SPELL_QUELDELAR_AURA))
    {
        me->SummonCreature(NPC_QUELDELAR, me->GetPosition(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
        me->DisappearAndDie();
    }
}


void FrostmourneHorAI::Reset()
{
    eventMap.Reset();
}

void FrostmourneHorAI::MoveInLineOfSight(Unit* pUnit)
{
    if (pUnit->HasAura(SPELL_QUELDELAR_AURA))
    {
        pUnit->CastSpell(pUnit, SPELL_UNKNOW_1);

        if (Creature* pCreatureUther = me->SummonCreature(NPC_UTHER_LIGHTBRINGER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
        {
            pCreatureUther->AI()->Talk(SAY_UTHER_2);
            pCreatureUther->AI()->Talk(SAY_UTHER_1);
        }

        eventMap.ScheduleEvent(EVENT_QUELDELAR, 4 * IN_MILLISECONDS);
    }
}

void FrostmourneHorAI::UpdateAI(const uint32 diff)
{
    eventMap.Update(diff);

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        switch (eventID)
        {
            case EVENT_QUELDELAR:
            {
                me->CastSpell(me, SPELL_UNKNOW_2, true);

                if (Creature* pCreatureUther = me->FindNearestCreature(NPC_UTHER_LIGHTBRINGER, 100))
                    pCreatureUther->AI()->Talk(SAY_UTHER_3);
            }break;
        }
    }
}
