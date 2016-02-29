/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
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

#ifndef _BOSS_MIDNIGHT_H
#define _BOSS_MIDNIGHT_H

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

#define RESET_TIME              5*IN_MILLISECONDS

enum Texts
{
    SAY_MIDNIGHT_KILL           = -1532000,
    SAY_APPEAR1                 = -1532001,
    SAY_APPEAR2                 = -1532002,
    SAY_APPEAR3                 = -1532003,
    SAY_MOUNT                   = -1532004,
    SAY_KILL1                   = -1532005,
    SAY_KILL2                   = -1532006,
    SAY_DISARMED                = -1532007,
    SAY_DEATH                   = -1532008,
    SAY_RANDOM1                 = -1532009,
    SAY_RANDOM2                 = -1532010
};

enum Phases
{
    MIDNIGHT_ALONE              = 1,
    MIDNIGHT_AND_ATTUMEN        = 2,
    ATTUMEN_MOUNTED             = 3
};

enum Spells
{
    SPELL_SHADOWCLEAVE          = 29832,
    SPELL_INTANGIBLE_PRESENCE   = 29833,
    SUMMON_ATTUMEN              = 29714,
    SUMMON_ATTUMEN_MOUNTED      = 29799,
    SPELL_BERSERKER_CHARGE      = 26561                   //Only when mounted
};

enum Events
{
    SPELL_CLEAVE                = 1,
    SPELL_CURSE                 = 2,
    RANDOM_YELL                 = 3,
    CHARGE_MOUNT                = 4,
    MOUNTING                    = 5,
    RESET                       = 6
};

enum Groups
{
    GROUP_ALL                   = 0,
    GROUP_ATTUMEN               = 1
};

enum Creatures
{
   CREATURE_MIDNIGHT            = 16151,
   CREATURE_ATTUMEN             = 15550,
   CREATURE_ATTUMEN_MOUNTED     = 16152
};

enum Actions
{
    MIDNIGHT_ACTION_MOUNT_START = 1,
    MIDNIGHT_ACTION_MOUNT       = 2
};

struct boss_midnightAI : public BossAI
{
    boss_midnightAI(Creature* creature) : BossAI(creature, TYPE_ATTUMEN), WithAttumen(false){}

    void Reset();
    void EnterCombat(Unit* /*who*/);
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);
    void KilledUnit(Unit* /*victim*/);
    void EnterEvadeMode();
    void UpdateAI(const uint32 diff);
    void DoAction(const int32 action);

private:
    InstanceScript* instance;
    bool WithAttumen;
};

class boss_midnight : public CreatureScript
{
public:
    boss_midnight() : CreatureScript("boss_midnight") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_midnightAI(creature);
    }
};

struct boss_attumenAI : public ScriptedAI
{
    boss_attumenAI(Creature* creature) : ScriptedAI(creature), WithAttumen(false), events() {}

    void Reset();
    void EnterCombat(Unit* /*who*/);
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);
    void KilledUnit(Unit* /*victim*/);
    void EnterEvadeMode();
    void UpdateAI(const uint32 diff);
    void SpellHit(Unit* /*source*/, const SpellInfo* spell);

private:
    InstanceScript* instance;
    EventMap events;
    bool WithAttumen;
};

class boss_attumen : public CreatureScript
{
public:
    boss_attumen() : CreatureScript("boss_attumen") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_attumenAI (creature);
    }
};

struct boss_attumen_mountedAI : public ScriptedAI
{
    boss_attumen_mountedAI(Creature* creature) : ScriptedAI(creature), events() {}

    void Reset();
    void EnterCombat(Unit* /*who*/);
    void KilledUnit(Unit* /*victim*/);
    void JustDied(Unit* /*killer*/);
    void EnterEvadeMode();
    void UpdateAI(const uint32 diff);
    void SpellHit(Unit* /*source*/, const SpellInfo* spell);

private:
    InstanceScript* instance;
    EventMap events;
};

class boss_attumen_mounted : public CreatureScript
{
public:
    boss_attumen_mounted() : CreatureScript("boss_attumen_mounted") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_attumen_mountedAI (creature);
    }
};

#endif // _BOSS_MIDNIGHT_H