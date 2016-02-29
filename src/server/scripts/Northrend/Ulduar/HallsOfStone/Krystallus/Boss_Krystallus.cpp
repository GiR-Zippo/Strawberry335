/*
* Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "Boss_Krystallus.h"
typedef boss_krystallus::boss_krystallusAI KrystallusAI;


void KrystallusAI::Reset()
{
    CreateBoundaryMap();

    events.Reset();

    if (instance)
        instance->SetData(DATA_KRYSTALLUS_EVENT, NOT_STARTED);
};

void KrystallusAI::JustReachedHome()
{
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
    me->SetVisible(true);
};

void KrystallusAI::EnterCombat(Unit* who)
{
    DoScriptText(SAY_AGGRO, me);

    events.ScheduleEvent(EVENT_SPELL_BOULDER, urand(3*IN_MILLISECONDS, 9*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_SPELL_GROUND_SPIKE, urand(9*IN_MILLISECONDS, 14*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_SPELL_GROUND_SLAM, urand(15*IN_MILLISECONDS, 18*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_SPELL_STOMP, urand(20*IN_MILLISECONDS, 29*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_CHECK_IN_ROOM, 5*IN_MILLISECONDS);

    if (instance)
        instance->SetData(DATA_KRYSTALLUS_EVENT, IN_PROGRESS);
};

void KrystallusAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    ExecuteEvents();

    DoMeleeAttackIfReady();
};

void KrystallusAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SPELL_BOULDER:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, DUNGEON_MODE(SPELL_BOULDER_TOSS, H_SPELL_BOULDER_TOSS));

                events.ScheduleEvent(EVENT_SPELL_BOULDER, urand(9*IN_MILLISECONDS, 15*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_GROUND_SPIKE:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_GROUND_SPIKE);

                events.ScheduleEvent(EVENT_SPELL_GROUND_SPIKE, urand(12*IN_MILLISECONDS, 17*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_GROUND_SLAM:
            {
                DoCast(me, SPELL_GROUND_SLAM);
                events.ScheduleEvent(EVENT_SPELL_GROUND_SLAM, urand(15*IN_MILLISECONDS, 18*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SPELL_SHATTER, 10*IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_SHATTER:
            {
                DoCast(me, DUNGEON_MODE(SPELL_SHATTER, H_SPELL_SHATTER));
                DoScriptText(SAY_SHATTER, me);
            }break;

            case EVENT_SPELL_STOMP:
            {
                DoCast(me, DUNGEON_MODE(SPELL_STOMP, H_SPELL_STOMP));
                events.ScheduleEvent(EVENT_SPELL_STOMP,  urand(20*IN_MILLISECONDS, 29*IN_MILLISECONDS));
            }break;

            case EVENT_CHECK_IN_ROOM:
            {
                if (CheckBoundaryMap(&_boundaryMap))
                    events.ScheduleEvent(EVENT_CHECK_IN_ROOM, 3*IN_MILLISECONDS);
                else
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                    me->SetVisible(false);
                    EnterEvadeMode();
                }
            }break;
        }
    }
};

void KrystallusAI::JustDied(Unit* killer)
{
    DoScriptText(SAY_DEATH, me);

    if (instance)
        instance->SetData(DATA_KRYSTALLUS_EVENT, DONE);
};

void KrystallusAI::KilledUnit(Unit* victim)
{
    if (victim == me)
        return;
    DoScriptText(SAY_KILL, me);
};

void KrystallusAI::SpellHitTarget(Unit* /*target*/, const SpellInfo* pSpell)
{
    //this part should be in the core
    if (pSpell->Id == SPELL_SHATTER || pSpell->Id == H_SPELL_SHATTER)
    {
        //and correct movement, if not already
        if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() != CHASE_MOTION_TYPE)
        {
            if (me->getVictim())
                me->GetMotionMaster()->MoveChase(me->getVictim());
        }
    }
};

void KrystallusAI::CreateBoundaryMap()
{
    _boundaryMap.clear();
    _boundaryMap[BOUNDARY_N] = BOUNDARY_POSITION_N;
    _boundaryMap[BOUNDARY_S] = BOUNDARY_POSITION_S;
    _boundaryMap[BOUNDARY_W] = BOUNDARY_POSITION_W;
};
