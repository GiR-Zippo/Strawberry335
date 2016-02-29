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

#include "Boss_Broggok.h"
typedef boss_broggok::boss_broggokAI BroggokAI;


void BroggokAI::Reset()
{
    if (instance = me->GetInstanceScript())
    {
        if (instance->GetData(DATA_BROGGOK_EVENT))
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        else
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            
        instance->SetData(DATA_BROGGOK, NOT_STARTED);
        instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_FRONT_DOOR), true);
        return;
    }

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
};

void BroggokAI::EnterCombat(Unit* who)
{
    DoScriptText(SAY_AGGRO, me);

    events.ScheduleEvent(EVENT_SPELL_SLIME_SPRAY, 10*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_SPELL_POISON_CLOUD, 5*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_SPELL_POISON_BOLT, 7*IN_MILLISECONDS);

    if (instance)
    {
        instance->SetData(DATA_BROGGOK, IN_PROGRESS);
        instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_FRONT_DOOR), false);
    }
};

void BroggokAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    ExecuteEvents();

    DoMeleeAttackIfReady();
};

void BroggokAI::JustDied(Unit* /*killer*/)
{
    if (instance)
    {
        instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_FRONT_DOOR), true);
        instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_REAR_DOOR), true);
        instance->SetData(DATA_BROGGOK, DONE);
    }
};

void BroggokAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (uint32 eventId = events.ExecuteEvent())
        {
            case EVENT_SPELL_SLIME_SPRAY:
            {
                DoCast(me->getVictim(), DUNGEON_MODE(SPELL_SLIME_SPRAY, H_SPELL_SLIME_SPRAY));
                events.ScheduleEvent(EVENT_SPELL_SLIME_SPRAY, urand(4*IN_MILLISECONDS, 12*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_POISON_CLOUD:
            {
                DoCast(me, SPELL_POISON_CLOUD);
                events.ScheduleEvent(EVENT_SPELL_POISON_CLOUD, 20*IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_POISON_BOLT:
            {
                DoCast(me->getVictim(), DUNGEON_MODE(SPELL_POISON_BOLT, H_SPELL_POISON_BOLT));
                events.ScheduleEvent(EVENT_SPELL_POISON_BOLT, urand(4*IN_MILLISECONDS, 12*IN_MILLISECONDS));
            }break;
        }
    }
};
