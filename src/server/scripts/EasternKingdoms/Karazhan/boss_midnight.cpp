/*
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

/* ScriptData
SDName: Boss_Midnight
SD%Complete: 100
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "karazhan.h"
#include "boss_midnight.h"

// midnightAI functions
void boss_midnightAI::Reset()
{
    instance = me->GetInstanceScript();

    if (instance && (!me->FindNearestCreature(CREATURE_ATTUMEN, 50.0f) || !me->FindNearestCreature(CREATURE_ATTUMEN_MOUNTED, 50.0f)))
        instance->SetData(TYPE_ATTUMEN, NOT_STARTED);
};

void boss_midnightAI::EnterCombat(Unit* who) 
{
    if (!instance->CheckRequiredBosses(DATA_ATTUMEN, who->ToPlayer()))
    {
        EnterEvadeMode();
        return;
    }

    instance->SetData(TYPE_ATTUMEN, IN_PROGRESS);

    me->setActive(true);
    events.Reset();

    DoZoneInCombat(me);
};

void boss_midnightAI::DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
{
    if (!WithAttumen && HealthAbovePct(25) && HealthBelowPct(95))
    {
        DoCast(SUMMON_ATTUMEN);
        WithAttumen = true;
    }
    else if (WithAttumen && HealthBelowPct(25))
    {
        WithAttumen = false;
        DoAction(MIDNIGHT_ACTION_MOUNT_START);
    }
};

void boss_midnightAI::KilledUnit(Unit* /*victim*/)
{
    if (WithAttumen)
    {
        if (Creature* Attumen = Creature::GetCreature((*me), instance->GetData64(DATA_ATTUMEN)))
            DoScriptText(SAY_MIDNIGHT_KILL, Attumen);
    }
};

void boss_midnightAI::EnterEvadeMode()
{
    BossAI::EnterEvadeMode();
    me->SetReactState(REACT_PASSIVE);
    me->SetVisible(false);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    events.ScheduleEvent(RESET, RESET_TIME);
};

void boss_midnightAI::DoAction(const int32 action)
{
    switch(action)
    {
        case MIDNIGHT_ACTION_MOUNT_START:
        {
            if (Creature* Attumen = Creature::GetCreature((*me), instance->GetData64(DATA_ATTUMEN)))
            {
                DoScriptText(SAY_MOUNT, Attumen);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                float newX = Attumen->GetPositionX();
                float newY = Attumen->GetPositionY();
                float newZ = 50;

                me->SetReactState(REACT_PASSIVE);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(0, newX, newY, newZ);

                events.ScheduleEvent(MOUNTING, 2.5*IN_MILLISECONDS);
            }
            else
                EnterEvadeMode();
        }
        break;
        case MIDNIGHT_ACTION_MOUNT:
        {
            me->SetVisible(false);
            if (Creature* Attumen = Creature::GetCreature((*me), instance->GetData64(DATA_ATTUMEN)))
            {
                Attumen->AI()->DoCast(SUMMON_ATTUMEN_MOUNTED);
                Attumen->DespawnOrUnsummon(1);
            }
        }
        break;
        default:
            break;
    }
}

void boss_midnightAI::UpdateAI(const uint32 diff)
{
    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case MOUNTING:
                DoAction(MIDNIGHT_ACTION_MOUNT);
                break;
            case RESET:
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetVisible(true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
};

// attumenAI functions
void boss_attumenAI::Reset()
{
    events.Reset();
    instance = me->GetInstanceScript();
};

void boss_attumenAI::EnterCombat(Unit* /*who*/)
{
    DoScriptText(RAND(SAY_APPEAR1, SAY_APPEAR2, SAY_APPEAR3), me);
    me->setActive(true);
    events.Reset();

    DoZoneInCombat();
    events.ScheduleEvent(SPELL_CLEAVE, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
    events.ScheduleEvent(SPELL_CURSE, 30*IN_MILLISECONDS);
    events.ScheduleEvent(RANDOM_YELL, urand(30*IN_MILLISECONDS, 60*IN_MILLISECONDS));
    WithAttumen = false;
};

void boss_attumenAI::DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
{
    if (!WithAttumen && HealthBelowPct(25))
    {
        WithAttumen = true;
        me->GetMotionMaster()->Clear();
        me->SetReactState(REACT_PASSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        if (Creature* Midnight = Creature::GetCreature((*me), instance->GetData64(DATA_MIDNIGHT)))
            Midnight->AI()->DoAction(MIDNIGHT_ACTION_MOUNT_START);
    }
};

void boss_attumenAI::KilledUnit(Unit* /*victim*/)
{
    DoScriptText(RAND(SAY_KILL1, SAY_KILL2), me);
};

void boss_attumenAI::EnterEvadeMode()
{
    if (!me->isAlive())
        return;

    if (!me->FindNearestCreature(CREATURE_ATTUMEN_MOUNTED, 50.0f))
        if (Creature* Midnight = Creature::GetCreature((*me), instance->GetData64(DATA_MIDNIGHT)))
            Midnight->AI()->EnterEvadeMode();

    me->DespawnOrUnsummon();
};

void boss_attumenAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case SPELL_CLEAVE:
            DoCastVictim(SPELL_SHADOWCLEAVE);
            events.ScheduleEvent(SPELL_CLEAVE, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
            break;
        case SPELL_CURSE:
            DoCastVictim(SPELL_INTANGIBLE_PRESENCE);
            events.ScheduleEvent(SPELL_CURSE, 30*IN_MILLISECONDS);
            break;
        case RANDOM_YELL:
            DoScriptText(RAND(SAY_RANDOM1, SAY_RANDOM2), me);
            events.ScheduleEvent(RANDOM_YELL, urand(30*IN_MILLISECONDS, 60*IN_MILLISECONDS));
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
};

void boss_attumenAI::SpellHit(Unit* /*source*/, const SpellInfo* spell)
{
    if (spell->Mechanic == MECHANIC_DISARM)
        DoScriptText(SAY_DISARMED, me);
};


// attumen_mountedAI functions
void boss_attumen_mountedAI::Reset()
{
    events.Reset();
    instance = me->GetInstanceScript();
};

void boss_attumen_mountedAI::EnterCombat(Unit* /*who*/)
{
    DoScriptText(RAND(SAY_APPEAR1, SAY_APPEAR2, SAY_APPEAR3), me);
    me->setActive(true);
    events.Reset();

    DoZoneInCombat();
    events.ScheduleEvent(SPELL_CLEAVE, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
    events.ScheduleEvent(SPELL_CURSE, 30*IN_MILLISECONDS);
    events.ScheduleEvent(RANDOM_YELL, urand(30*IN_MILLISECONDS, 60*IN_MILLISECONDS));
    events.ScheduleEvent(CHARGE_MOUNT, 20*IN_MILLISECONDS);
};

void boss_attumen_mountedAI::KilledUnit(Unit* /*victim*/)
{
    DoScriptText(RAND(SAY_KILL1, SAY_KILL2), me);
};

void boss_attumen_mountedAI::JustDied(Unit* /*killer*/)
{
    DoScriptText(SAY_DEATH, me);
    if (Creature* Midnight = Creature::GetCreature((*me), instance->GetData64(DATA_MIDNIGHT)))
        Midnight->DespawnOrUnsummon();

    if (instance)
        instance->SetData(TYPE_ATTUMEN, DONE);
};

void boss_attumen_mountedAI::EnterEvadeMode()
{
    if (!me->isAlive())
        return;

    if (Creature* Midnight = Creature::GetCreature((*me), instance->GetData64(DATA_MIDNIGHT)))
        Midnight->AI()->EnterEvadeMode();

    me->DespawnOrUnsummon();
};

void boss_attumen_mountedAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE))
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case SPELL_CLEAVE:
                DoCast(SPELL_SHADOWCLEAVE);
                events.ScheduleEvent(SPELL_CLEAVE, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                break;
            case SPELL_CURSE:
                DoCast(SPELL_INTANGIBLE_PRESENCE);
                events.ScheduleEvent(SPELL_CURSE, 30*IN_MILLISECONDS);
                break;
            case RANDOM_YELL:
                DoScriptText(RAND(SAY_RANDOM1, SAY_RANDOM2), me);
                events.ScheduleEvent(RANDOM_YELL, urand(30*IN_MILLISECONDS, 60*IN_MILLISECONDS));
                break;
            case CHARGE_MOUNT:
            {
                Unit* target = NULL;
                std::list<HostileReference*> t_list = me->getThreatManager().getThreatList();
                std::vector<Unit*> target_list;

                for (std::list<HostileReference*>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    target = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                    if (target && !target->IsWithinDist(me, ATTACK_DISTANCE, false))
                        target_list.push_back(target);

                    target = NULL;
                }

                if (!target_list.empty())
                    target = *(target_list.begin()+rand()%target_list.size());

                DoCast(target, SPELL_BERSERKER_CHARGE);
                events.ScheduleEvent(CHARGE_MOUNT, 20*IN_MILLISECONDS);
            }
            break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
};

void boss_attumen_mountedAI::SpellHit(Unit* /*source*/, const SpellInfo* spell)
{
    if (spell->Mechanic == MECHANIC_DISARM)
        DoScriptText(SAY_DISARMED, me);
};

void AddSC_boss_attumen()
{
    new boss_midnight();
    new boss_attumen();
    new boss_attumen_mounted();
};
