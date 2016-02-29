/*
* Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "Boss_TheMaker.h"
typedef boss_the_maker::boss_the_makerAI TheMakerAI;


void TheMakerAI::Reset()
{
    if (instance = me->GetInstanceScript())
    {
        instance->SetData(DATA_THE_MAKER, NOT_STARTED);
        instance->HandleGameObject(instance->GetData64(DATA64_THE_MAKER_FRONT_DOOR), true);
    }
};

void TheMakerAI::EnterCombat(Unit* /*who*/)
{
    DoScriptText(RAND(SAY_AGGRO_1, SAY_AGGRO_2, SAY_AGGRO_3), me);

    events.ScheduleEvent(EVENT_SPELL_ACID_SPRAY, 15*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_SPELL_EXPLODING_BREAKER, 6*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_SPELL_KNOCKDOWN, 120*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_SPELL_DOMINATION, 10*IN_MILLISECONDS);

    if (!instance)
        return;

    instance->SetData(DATA_THE_MAKER, IN_PROGRESS);
    instance->HandleGameObject(instance->GetData64(DATA64_THE_MAKER_FRONT_DOOR), false);
};

void TheMakerAI::KilledUnit(Unit* /*victim*/)
{
    DoScriptText(RAND(SAY_KILL_1, SAY_KILL_2), me);
};

void TheMakerAI::JustDied(Unit* /*killer*/)
{
    DoScriptText(SAY_DIE, me);

    if (!instance)
        return;

    instance->SetData(DATA_THE_MAKER, DONE);
    instance->HandleGameObject(instance->GetData64(DATA64_THE_MAKER_FRONT_DOOR), true);
    instance->HandleGameObject(instance->GetData64(DATA64_THE_MAKER_REAR_DOOR), true);
};

void TheMakerAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    ExecuteEvents();

    DoMeleeAttackIfReady();
};

void TheMakerAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SPELL_ACID_SPRAY:
            {
                DoCast(me->getVictim(), SPELL_ACID_SPRAY);
                events.ScheduleEvent(EVENT_SPELL_ACID_SPRAY, urand(15*IN_MILLISECONDS, 23*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_EXPLODING_BREAKER:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, DUNGEON_MODE(SPELL_EXPLODING_BREAKER, H_SPELL_EXPLODING_BREAKER));

                events.ScheduleEvent(EVENT_SPELL_EXPLODING_BREAKER, urand(4*IN_MILLISECONDS, 12*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_KNOCKDOWN:
            {
                DoCast(me->getVictim(), SPELL_KNOCKDOWN);
                events.ScheduleEvent(EVENT_SPELL_KNOCKDOWN, 120*IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_DOMINATION:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_DOMINATION);

                events.ScheduleEvent(EVENT_SPELL_DOMINATION, urand(4*IN_MILLISECONDS, 12*IN_MILLISECONDS));
            }break;
        }
    }
};

void AddSC_boss_the_maker()
{
    new boss_the_maker();
};
