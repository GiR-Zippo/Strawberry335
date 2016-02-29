/*
* Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
* Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Npc_SjonnirTheIronshaper.h"
typedef npc_malformed_ooze::npc_malformed_oozeAI   MalformedOozeAI;
typedef npc_iron_sludge::npc_iron_sludgeAI         IronSludgeAI;
typedef npc_brann_hos_final::npc_brann_hos_finalAI BrannHosFinalAI;
typedef npc_trigger_sjonnir::npc_trigger_sjonnirAI TriggerSjonnirAI;
typedef npc_brann_adds::npc_brann_addsAI           BrannAddsAI;


void MalformedOozeAI::UpdateAI(uint32 const diff)
{
    if (!events.Empty())
    {
        events.Update(diff);
        ExecuteEvents();
    }

    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();
};

void MalformedOozeAI::EnterCombat(Unit* /*who*/)
{
    events.ScheduleEvent(MOE_EVENT_SPAWN_IRON_SLUDGE, 3*IN_MILLISECONDS);
};

void MalformedOozeAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == 2)
    {
        Unit* target = NULL;
        if (target = me->SelectNearestTarget())
        {
            me->AI()->AttackStart(target);
            me->AddThreat(target, 0.0f);
        }
    }
};

void MalformedOozeAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case MOE_EVENT_SPAWN_IRON_SLUDGE:
            {
                if (Creature* temp = me->FindNearestCreature(NPC_MALFORMED_OOZE, 5.0f))
                {
                    if (temp == me)
                    {
                        events.ScheduleEvent(MOE_EVENT_SPAWN_IRON_SLUDGE, 3*IN_MILLISECONDS);
                        return;
                    }

                    DoSpawnCreature(NPC_IRON_SLUDGE, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20*IN_MILLISECONDS);
                    temp->DisappearAndDie();
                    me->DisappearAndDie();
                }
                else
                    events.ScheduleEvent(MOE_EVENT_SPAWN_IRON_SLUDGE, 3*IN_MILLISECONDS);
            }break;
        }
    }
};

void IronSludgeAI::JustDied(Unit* /*killer*/)
{
    if (Creature* sjonnir = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SJONNIR)))
        sjonnir->AI()->DoAction(ACTION_OOZE_DEAD);
};


void BrannHosFinalAI::EnterEvadeMode()
{
    _EnterEvadeMode();
    events.ScheduleEvent(BE_EVENT_WAIT_SAY_1, 13*IN_MILLISECONDS);
};

void BrannHosFinalAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == 1)
        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY_UNARMED);
};

void BrannHosFinalAI::DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
{
    if (me->FindNearestPlayer(40.0f))
        me->SetFullHealth();
    else
    {
        if (me->getVictim())
            me->getVictim()->Kill(me);
        else
            me->Kill(me);
    }
};

void BrannHosFinalAI::UpdateAI(const uint32 diff)
{
    if (!events.Empty() && me->isAlive())
    {
        events.Update(diff);
        ExecuteEvents();
    }

    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();
};

void BrannHosFinalAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case BE_EVENT_WAIT_SAY_1:
            {
                DoScriptText(SAY_VICTORY_SJONNIR_1, me);
                me->GetMotionMaster()->MovePoint(2, BrannEndPosition);
                events.ScheduleEvent(BE_EVENT_WAIT_SAY_2, 15*IN_MILLISECONDS);
            }break;

            case BE_EVENT_WAIT_SAY_2:
            {
                DoScriptText(SAY_VICTORY_SJONNIR_2, me);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                me->SetFacingTo(3.13f);
            }break;
        }
    }
};


void TriggerSjonnirAI::MoveInLineOfSight(Unit* who)
{
    if (!who)
        return;

    if (who->GetTypeId() != TYPEID_PLAYER)
        return;

    if (me->FindNearestCreature(CREATURE_BRANN_FINAL, 100.0f))
        return;

    if (me->GetDistance2d(who) < 10.0f)
    {
        Creature* pCreature = NULL;
        if (pCreature = me->SummonCreature(CREATURE_BRANN_FINAL, BrannSpawnPos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10*IN_MILLISECONDS))
        {
            pCreature->GetMotionMaster()->MovePoint(1, BrannPointPos);
            DoScriptText(SAY_BRANN_SPAWN_DWARF, pCreature);
        }
    }
};

void BrannAddsAI::DoAfterMovement()
{
    Unit* target = NULL;
    if (target = me->SelectNearestTarget())
    {
        me->AI()->AttackStart(target);
        me->AddThreat(target, 0.0f);
    }
};
