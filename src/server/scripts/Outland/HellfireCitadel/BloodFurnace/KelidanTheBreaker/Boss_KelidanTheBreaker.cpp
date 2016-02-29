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

#include "Boss_KelidanTheBreaker.h"
typedef boss_kelidan_the_breaker::boss_kelidan_the_breakerAI KelidanAI;


void KelidanAI::Reset()
{
    instance = me->GetInstanceScript();
    for (uint8 i=0; i<5; ++i)
        Channelers[i] = 0;

    addYell = false;
    SummonChannelers();
    if (instance)
        instance->SetData(DATA_KELIDAN_THE_BREAKER, NOT_STARTED);

    events.ScheduleEvent(EVENT_CHECK_CAST_EVOCATION, 0.5*IN_MILLISECONDS);
};

void KelidanAI::EnterCombat(Unit* who)
{
    DoScriptText(SAY_WAKE, me);
    if (me->IsNonMeleeSpellCasted(false))
        me->InterruptNonMeleeSpells(true);
    DoStartMovement(who);

    events.ScheduleEvent(EVENT_SPELL_CORRUPTION, 5*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_SPELL_SHADOW_BOLT_VOLLEY, 1*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_SPELL_BURNING_NOVA, 15*IN_MILLISECONDS);

    if (instance)
        instance->SetData(DATA_KELIDAN_THE_BREAKER, IN_PROGRESS);
};

void KelidanAI::KilledUnit(Unit* victim)
{
    if (rand()%2)
        return;

    DoScriptText(RAND(SAY_KILL_1, SAY_KILL_2), me);
};

void KelidanAI::ChannelerEngaged(Unit* who)
{
    if (who && !addYell)
    {
        addYell = true;
        DoScriptText(RAND(SAY_ADD_AGGRO_1, SAY_ADD_AGGRO_2, SAY_ADD_AGGRO_3), me);
    }
    for (uint8 i=0; i<5; ++i)
    {
        Creature* channeler = Unit::GetCreature(*me, Channelers[i]);
        if (who && channeler && !channeler->isInCombat())
            channeler->AI()->AttackStart(who);
    }
};

void KelidanAI::ChannelerDied(Unit* killer)
{
    for (uint8 i=0; i<5; ++i)
    {
        Creature* channeler = Unit::GetCreature(*me, Channelers[i]);
        if (channeler && channeler->isAlive())
            return;
    }

    if (killer)
        me->AI()->AttackStart(killer);
};

uint64 KelidanAI::GetChanneled(Creature* channeler1)
{
    SummonChannelers();
    if (!channeler1)
        return 0;

    uint8 i;
    for (i=0; i<5; ++i)
    {
        Creature* channeler = Unit::GetCreature(*me, Channelers[i]);
        if (channeler && channeler->GetGUID() == channeler1->GetGUID())
            break;
    }
    return Channelers[(i+2)%5];
};

void KelidanAI::SummonChannelers()
{
    for (uint8 i=0; i<5; ++i)
    {
        Creature* channeler = Unit::GetCreature(*me, Channelers[i]);
        if (!channeler || channeler->isDead())
            channeler = me->SummonCreature(ENTRY_CHANNELER, ShadowmoonChannelers[i][0], ShadowmoonChannelers[i][1], ShadowmoonChannelers[i][2], ShadowmoonChannelers[i][3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300000);
        if (channeler)
            Channelers[i] = channeler->GetGUID();
        else
            Channelers[i] = 0;
    }
};

void KelidanAI::JustDied(Unit* killer)
{
    DoScriptText(SAY_DIE, me);

    if (!instance)
        return;

    instance->SetData(DATA_KELIDAN_THE_BREAKER, DONE);
    instance->HandleGameObject(instance->GetData64(DATA64_FINAL_EXIT_DOOR), true);
    instance->HandleGameObject(instance->GetData64(DATA64_KELIDAN_EXIT_DOOR), true);
};

void KelidanAI::UpdateAI(const uint32 diff)
{
    events.Update(diff);

    if (!UpdateVictim())
    {
        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_CHECK_CAST_EVOCATION)
            {
                if (!me->IsNonMeleeSpellCasted(false))
                    DoCast(me, SPELL_EVOCATION);

                events.ScheduleEvent(EVENT_CHECK_CAST_EVOCATION, 5*IN_MILLISECONDS);
            }
        }
        return;
    }

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    ExecuteEvents();

    DoMeleeAttackIfReady();
};

void KelidanAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SPELL_CORRUPTION:
            {
                DoCast(me, SPELL_CORRUPTION);
                events.ScheduleEvent(EVENT_SPELL_CORRUPTION, urand(30*IN_MILLISECONDS, 50*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_FIRE_NOVA:
            {
                DoCast(me, DUNGEON_MODE(SPELL_FIRE_NOVA, H_SPELL_FIRE_NOVA));

                events.CancelEvent(EVENT_SPELL_SHADOW_BOLT_VOLLEY);
                events.ScheduleEvent(EVENT_SPELL_SHADOW_BOLT_VOLLEY, 2*IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_SHADOW_BOLT_VOLLEY:
            {
                DoCast(me, DUNGEON_MODE(SPELL_SHADOW_BOLT_VOLLEY, H_SPELL_SHADOW_BOLT_VOLLEY));
                events.ScheduleEvent(EVENT_SPELL_SHADOW_BOLT_VOLLEY, urand(5*IN_MILLISECONDS, 13*IN_MILLISECONDS));
            }break;

            case EVENT_SPELL_BURNING_NOVA:
            {
                if (me->IsNonMeleeSpellCasted(false))
                    me->InterruptNonMeleeSpells(true);

                DoScriptText(SAY_NOVA, me);

                if (SpellInfo const* nova = sSpellMgr->GetSpellInfo(SPELL_BURNING_NOVA))
                {
                    if (Aura* aura = Aura::TryRefreshStackOrCreate(nova, MAX_EFFECT_MASK, me, me))
                        aura->ApplyForTargets();
                }

                if (IsHeroic())
                    DoTeleportAll(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

                events.CancelEvent(EVENT_SPELL_FIRE_NOVA);
                events.ScheduleEvent(EVENT_SPELL_FIRE_NOVA, 5*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_SPELL_BURNING_NOVA, urand(20*IN_MILLISECONDS, 28*IN_MILLISECONDS));
            }break;
        }
    }
};
