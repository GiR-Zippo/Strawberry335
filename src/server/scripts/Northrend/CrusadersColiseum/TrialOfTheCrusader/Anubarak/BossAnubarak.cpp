/*
 * Copyright (C) 2012 Zirkon
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

#include "BossAnubarak.h"

void BossAnubarakAI::Reset()
{
    submerged = false;        
    final = false;
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    summons.DespawnAll();
    events.Reset();
}

void BossAnubarakAI::JustReachedHome()
{
    if (instance)
        instance->SetData(GO_WEB_DOOR, OPEN);

    //Summon Scarab Swarms neutral at random places
    for (int i=0; i < 10; i++)
        if (Creature* temp = me->SummonCreature(NPC_SCARAB, AnubarakLoc[1].GetPositionX()+urand(0, 50)-25, AnubarakLoc[1].GetPositionY()+urand(0, 50)-25, AnubarakLoc[1].GetPositionZ()))
            temp->setFaction(31);
}

void BossAnubarakAI::JustDied(Unit* /*killer*/)
{
    summons.DespawnAll();
    events.Reset();
    DoScriptText(SAY_DEATH, me);
    if (instance)
    {
        instance->SetData(TYPE_ENCOUNTER_STATUS, DONE);
        instance->SetData(GO_WEB_DOOR, OPEN);
    }
}

void BossAnubarakAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (submerged)
        damage = 0;

    if (!HealthBelowPct(30) || final)
        return;

    final = true;

    if (submerged)
        return;
    
    events.DelayEvents(2000, GROUP_EMERGED);
    events.ScheduleEvent(EVENT_LEECHING_SWARM, 10);
}

void BossAnubarakAI::JustSummoned(Creature* summoned)
{
    summons.Summon(summoned);

    if (summoned->GetEntry() != NPC_BURROW)
        return;
    summoned->SetReactState(REACT_PASSIVE);
    summoned->CastSpell(summoned, SPELL_CHURNING_GROUND, true);
}

void BossAnubarakAI::SummonedCreatureDespawn(Creature* summoned)
{
    switch (summoned->GetEntry())
    {
        case NPC_SPIKE:
            if (submerged)
                events.ScheduleEvent(EVENT_PURSUING_SPIKE, TIME_PURSUING_SPIKE, GROUP_SUBMERGED);
            break;
        case NPC_FROST_SPHERE:
        {
            uint64 summ_guid = summoned->GetGUID();

            for (uint8 i = 0; i < 6; ++i)
                if (spheres[i] && spheres[i]->GetGUID() == summoned->GetGUID())
                {
                    spheres[i] = NULL;
                    break;
                }
            break;
        }
    }
}

void BossAnubarakAI::EnterCombat(Unit* /*who*/)
{
    DoScriptText(SAY_AGGRO, me);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    me->SetInCombatWithZone();

    instance->SetData(TYPE_ENCOUNTER_STATUS, IN_PROGRESS);

    if (instance)
        instance->SetData(GO_WEB_DOOR, CLOSE);

    //Despawn Scarab Swarms neutral
    EntryCheckPredicate pred(NPC_SCARAB);
    summons.DoAction(ACTION_SCARAB_SUBMERGE, pred);

    //Spawn Burrow
    for (int i=0; i < 4; i++)
        burrow[i] = me->SummonCreature(NPC_BURROW, AnubarakLoc[i+2]);

    //Spawn Frost Spheres
    for (int i = 0; i < 6; i++)
        spheres[i] = me->SummonCreature(NPC_FROST_SPHERE, SphereSpawn[i]);

    events.ScheduleEvent(EVENT_SUMMON_NERUB, TIME_SUMMON_NERUB_INIT, GROUP_EMERGED);
    events.ScheduleEvent(EVENT_PENETRATING_COLD, TIME_PENETRATING_COLD, GROUP_EMERGED);
    events.ScheduleEvent(EVENT_FREEZE_SLASH, TIME_FREEZE_SLASH, GROUP_EMERGED);
    events.ScheduleEvent(EVENT_SUBMERGE, TIME_SUBMERGE);
    events.ScheduleEvent(EVENT_BERSERK, TIME_BERSERK);

    if (IsHeroic())
        events.ScheduleEvent(EVENT_NERUB_SHADOW_STRIKE, TIME_NERUB_SHADOW_STRIKE);
    else
        events.ScheduleEvent(EVENT_SUMMON_FROST_SPHERE, TIME_SUMMON_FROST_SPHERE);
}

void BossAnubarakAI::HandleEvent(const uint32 Event)
{
    switch (Event)
    {
        case EVENT_BERSERK:
            DoCast(me, SPELL_BERSERK);
            break;
        case EVENT_LEECHING_SWARM:
            DoScriptText(EMOTE_LEECHING_SWARM, me);
            DoScriptText(SAY_LEECHING_SWARM, me);
            DoCastAOE(SPELL_LEECHING_SWARM);
            break;
        case EVENT_SUMMON_FROST_SPHERE:
        {
            uint8 startAt = urand(0, 5);
            uint8 i = startAt;
            do
            {
                if (!spheres[i])
                {
                    spheres[i] = me->SummonCreature(NPC_FROST_SPHERE, SphereSpawn[i]);
                    break;
                }
                i = (i+1)%6;
            } while (i != startAt);
            events.ScheduleEvent(EVENT_SUMMON_FROST_SPHERE, TIME_SUMMON_FROST_SPHERE);

            break;
        }
        /*
         * GROUP_EMERGED
         */
        case EVENT_FREEZE_SLASH:
            DoCastVictim(SPELL_FREEZE_SLASH);
            events.ScheduleEvent(EVENT_FREEZE_SLASH, TIME_FREEZE_SLASH, GROUP_EMERGED);
            break;
        case EVENT_PENETRATING_COLD:
            me->CastCustomSpell(SPELL_PENETRATING_COLD, SPELLVALUE_MAX_TARGETS, RAID_MODE(2, 5, 2, 5));
            events.ScheduleEvent(EVENT_PENETRATING_COLD, TIME_PENETRATING_COLD, GROUP_EMERGED);
            break;
        case EVENT_SUMMON_NERUB:
        {
            if (!IsHeroic() && final)
                break;
            DoCast(SPELL_SUMMON_BURROWER);
            events.ScheduleEvent(EVENT_SUMMON_NERUB, TIME_SUMMON_NERUB, GROUP_EMERGED);
            break;
        }
        case EVENT_NERUB_SHADOW_STRIKE:
        {
            if (!submerged)
            {
                EntryCheckPredicate pred(NPC_BURROWER);
                summons.DoAction(ACTION_SHADOW_STRIKE, pred);
            }
            events.ScheduleEvent(EVENT_NERUB_SHADOW_STRIKE, TIME_NERUB_SHADOW_STRIKE);
            break;
        }
        /*
         *GROUP_SUBMERGED
         */
        case EVENT_PURSUING_SPIKE:
            if (submerged)
                DoCast(SPELL_SUMMON_SPIKE);
            break;
        case EVENT_SUMMON_SCARAB:
        {
            DoCast(me, SPELL_SUMMON_BEATLES, true);
            break;
        }
    }
}

void BossAnubarakAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (int Event = events.ExecuteEvent())
    {
        switch (Event)
        {
        case EVENT_SUBMERGE:
            if (final || me->HasAura(SPELL_BERSERK))
                break;

            submerged = true;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            DoScriptText(SAY_BURROWER, me);
            DoCast(me, SPELL_SUBMERGE_ANUBARAK);

            events.DelayEvents(TIME_EMERGE + 2000, GROUP_EMERGED);
            events.ScheduleEvent(EVENT_SUMMON_SCARAB, TIME_SUMMON_SCARAB, GROUP_SUBMERGED);
            events.ScheduleEvent(EVENT_PURSUING_SPIKE, TIME_PURSUING_SPIKE, GROUP_SUBMERGED);
            events.ScheduleEvent(EVENT_EMERGE, TIME_EMERGE);
            break;
        case EVENT_EMERGE:
            submerged = false;
            summons.DespawnEntry(NPC_SPIKE);

            me->RemoveAurasDueToSpell(SPELL_SUBMERGE_ANUBARAK);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            DoCast(me, SPELL_EMERGE_ANUBARAK);
            me->GetMotionMaster()->MoveChase(me->getVictim());

            if (final)

            events.CancelEventGroup(GROUP_SUBMERGED);

            if (final)
            {
                events.DelayEvents(3000, GROUP_EMERGED);
                events.ScheduleEvent(EVENT_LEECHING_SWARM, 1000);
            }
            else
                events.ScheduleEvent(EVENT_SUBMERGE, TIME_SUBMERGE);
            break;
        default:
            HandleEvent(Event);
        }

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
    }

    DoMeleeAttackIfReady();
}
