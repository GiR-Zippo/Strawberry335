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

#include "Npc_KelidanTheBreaker.h"
#include "Boss_KelidanTheBreaker.h"
typedef mob_shadowmoon_channeler::mob_shadowmoon_channelerAI ChannelerAI;


void ChannelerAI::Reset()
{
    if (me->IsNonMeleeSpellCasted(false))
        me->InterruptNonMeleeSpells(true);

    events.ScheduleEvent(EVENT_CHECK_CAST, 0.5*IN_MILLISECONDS);
};

void ChannelerAI::EnterCombat(Unit* who)
{
    if (Creature* Kelidan = me->FindNearestCreature(ENTRY_KELIDAN, 100))
        CAST_AI(boss_kelidan_the_breaker::boss_kelidan_the_breakerAI, Kelidan->AI())->ChannelerEngaged(who);
    if (me->IsNonMeleeSpellCasted(false))
        me->InterruptNonMeleeSpells(true);
    DoStartMovement(who);

    events.ScheduleEvent(EVENT_SPELL_SHADOW_BOLT, urand(1*IN_MILLISECONDS, 2*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_SPELL_MARK, urand(5*IN_MILLISECONDS, 7*IN_MILLISECONDS));
};

void ChannelerAI::JustDied(Unit* killer)
{
    if (Creature* Kelidan = me->FindNearestCreature(ENTRY_KELIDAN, 100))
        CAST_AI(boss_kelidan_the_breaker::boss_kelidan_the_breakerAI, Kelidan->AI())->ChannelerDied(killer);
};

void ChannelerAI::UpdateAI(const uint32 diff)
{
    events.Update(diff);

    if (!UpdateVictim())
    {
        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_CHECK_CAST)
            {
                if (!me->IsNonMeleeSpellCasted(false))
                {
                    if (Creature* Kelidan = me->FindNearestCreature(ENTRY_KELIDAN, 100))
                    {
                        uint64 channeler = CAST_AI(boss_kelidan_the_breaker::boss_kelidan_the_breakerAI, Kelidan->AI())->GetChanneled(me);
                        if (Unit* channeled = Unit::GetUnit(*me, channeler))
                            DoCast(channeled, SPELL_CHANNELING);
                    }
                }

                events.ScheduleEvent(EVENT_CHECK_CAST, 5*IN_MILLISECONDS);
            }
        }
        return;
    }

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    ExecuteEvents();

    DoMeleeAttackIfReady();
};

void ChannelerAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SPELL_SHADOW_BOLT:
            {
                DoCast(me->getVictim(), DUNGEON_MODE(SPELL_SHADOW_BOLT, H_SPELL_SHADOW_BOLT));
                events.ScheduleEvent(EVENT_SPELL_SHADOW_BOLT, urand(5*IN_MILLISECONDS, 6*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_MARK:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_MARK_OF_SHADOW);

                events.ScheduleEvent(EVENT_SPELL_MARK, urand(15*IN_MILLISECONDS, 20*IN_MILLISECONDS));
            }break;
        }
    }
};
