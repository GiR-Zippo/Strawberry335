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

#include "Boss_MaidenOfGrief.h"
typedef boss_maiden_of_grief::boss_maiden_of_griefAI MaidenAI;


void MaidenAI::Reset()
{
    if (instance)
    {
        instance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, NOT_STARTED);
        instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_GOOD_GRIEF_START_EVENT);
    }

    CreateBoundaryMap();
};

void MaidenAI::JustReachedHome()
{
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
    me->SetVisible(true);
};

void MaidenAI::EnterCombat(Unit* /*who*/)
{
    DoScriptText(SAY_AGGRO, me);

    if (instance)
    {
        if (GameObject* pDoor = instance->instance->GetGameObject(instance->GetData64(DATA_MAIDEN_DOOR)))
            if (pDoor->GetGoState() == GO_STATE_READY)
            {
                EnterEvadeMode();
                return;
            }

            instance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, IN_PROGRESS);
            instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_GOOD_GRIEF_START_EVENT);
    }

    if (IsHeroic())
        events.ScheduleEvent(H_EVENT_SPELL_PARTING_SORROW, urand(25000, 30000));
    events.ScheduleEvent(EVENT_SPELL_STORM_OF_GRIEF, 10000);
    events.ScheduleEvent(EVENT_SPELL_SHOCK_OF_SORROW, urand(20000, 25000));
    events.ScheduleEvent(EVENT_SPELL_PILLAR_OF_WOE, urand(5000, 15000));
    events.ScheduleEvent(EVENT_CHECK_IN_ROOM, 5*IN_MILLISECONDS);
};

void MaidenAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    ExecuteEvents();

    DoMeleeAttackIfReady();
};

void MaidenAI::JustDied(Unit* /*killer*/)
{
    DoScriptText(SAY_DEATH, me);

    if (instance)
        instance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, DONE);
};

void MaidenAI::KilledUnit(Unit* victim)
{
    if (victim == me)
        return;

    DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3, SAY_SLAY_4), me);
};

void MaidenAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case H_EVENT_SPELL_PARTING_SORROW:
            {
                std::list<Player*> PlList = me->GetNearestPlayersList(60.0f);
                if (PlList.empty())
                    EnterEvadeMode();

                PlList.remove_if(PartingSorrowFilter());
                if (!PlList.empty())
                {
                    std::list<Player*>::const_iterator itr = PlList.begin();

                    if (PlList.size() > 1)
                        advance(itr, rand()%PlList.size());

                    if (Player* target = (*itr))
                        DoCast(target, H_SPELL_PARTING_SORROW);
                }

                events.ScheduleEvent(H_EVENT_SPELL_PARTING_SORROW, urand(30*IN_MILLISECONDS, 40*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_STORM_OF_GRIEF:
            {
                DoCastVictim(DUNGEON_MODE(SPELL_STORM_OF_GRIEF, H_SPELL_STORM_OF_GRIEF));
                events.ScheduleEvent(EVENT_SPELL_STORM_OF_GRIEF, urand(15*IN_MILLISECONDS, 20*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_SHOCK_OF_SORROW:
            {
                DoResetThreat();
                DoScriptText(SAY_STUN, me);
                DoCast(me, DUNGEON_MODE(SPELL_SHOCK_OF_SORROW, H_SPELL_SHOCK_OF_SORROW));

                events.ScheduleEvent(EVENT_SPELL_SHOCK_OF_SORROW, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_PILLAR_OF_WOE:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, DUNGEON_MODE(SPELL_PILLAR_OF_WOE, H_SPELL_PILLAR_OF_WOE));
        
                events.ScheduleEvent(EVENT_SPELL_PILLAR_OF_WOE, urand(5*IN_MILLISECONDS, 25*IN_MILLISECONDS));
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

void MaidenAI::CreateBoundaryMap()
{
    _boundaryMap.clear();
    _boundaryMap[BOUNDARY_N] = BOUNDARY_POSITION_N;
};

void AddSC_boss_maiden_of_grief()
{
    new boss_maiden_of_grief();
};
