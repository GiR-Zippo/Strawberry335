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

#include "Anubarak.h"
#include "Creature.h"

enum Yells
{
    SAY_INTRO               = -1649055,
    SAY_AGGRO               = -1649056,
    SAY_KILL1               = -1649057,
    SAY_KILL2               = -1649058,
    SAY_DEATH               = -1649059,
    EMOTE_SPIKE             = -1649060,
    SAY_BURROWER            = -1649061,
    EMOTE_LEECHING_SWARM    = -1649062,
    SAY_LEECHING_SWARM      = -1649063,
};

enum Spells
{
    SPELL_FREEZE_SLASH      = 66012,
    SPELL_PENETRATING_COLD  = 66013,
    SPELL_LEECHING_SWARM    = 66118,
    SPELL_LEECHING_HEAL     = 66125,
    SPELL_LEECHING_DAMAGE   = 66240,
    SPELL_SUBMERGE_ANUBARAK = 65981,
    SPELL_CLEAR_ALL_DEBUFFS = 34098,
    SPELL_EMERGE_ANUBARAK   = 65982,
    SPELL_SUMMON_BEATLES    = 66339,
    SPELL_SUMMON_BURROWER   = 66332,
    SPELL_SUMMON_SPIKE      = 66169,

    SPELL_BERSERK           = 26662,

    // Burrow
    SPELL_CHURNING_GROUND   = 66969,
};

const Position SphereSpawn[6] =
{
    { 786.6439f, 108.2498f, 155.6701f, 0 },
    { 806.8429f, 150.5902f, 155.6701f, 0 },
    { 759.1386f, 163.9654f, 155.6701f, 0 },
    { 744.3701f, 119.5211f, 155.6701f, 0 },
    { 710.0211f, 120.8152f, 155.6701f, 0 },
    { 706.6383f, 161.5266f, 155.6701f, 0 },
};

enum Events
{
    EVENT_BERSERK                   = 1,
    EVENT_SUMMON_FROST_SPHERE       = 2,
    EVENT_LEECHING_SWARM            = 3,

    EVENT_FREEZE_SLASH              = 4,
    EVENT_PENETRATING_COLD          = 5,
    EVENT_NERUB_SHADOW_STRIKE       = 6,
    EVENT_SUMMON_NERUB              = 7,
    EVENT_SUBMERGE                  = 8,

    EVENT_PURSUING_SPIKE            = 9,
    EVENT_SUMMON_SCARAB             = 10,
    EVENT_EMERGE                    = 11,
};

enum Time
{
    TIME_FREEZE_SLASH           = 15000,
    TIME_PENETRATING_COLD       = 20000,
    TIME_NERUB_SHADOW_STRIKE    = 30000,
    TIME_SUMMON_NERUB_INIT      = 15000,
    TIME_SUMMON_NERUB           = 40000,
    TIME_SUBMERGE               = 80000,
    TIME_EMERGE                 = 60000,
    TIME_PURSUING_SPIKE         = 2000,
    TIME_SUMMON_SCARAB          = 2000,
    TIME_SUMMON_FROST_SPHERE    = 20000,
    TIME_BERSERK                = 600000,
};

enum Groups
{
    GROUP_EMERGED               = 1,
    GROUP_SUBMERGED             = 2,
};

class BossAnubarakAI : public ScriptedAI
{
public:
    BossAnubarakAI(Creature* creature) : ScriptedAI(creature), intro(false), summons(me), instance(creature->GetInstanceScript()) {}
    
    void KilledUnit(Unit* who)
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, me);
    }

    void MoveInLineOfSight(Unit*)
    {
        if (!intro)
        {
            DoScriptText(SAY_INTRO, me);
            intro = true;
        }
    }

    void SummonedCreatureDespawn(Creature* summoned);
    void Reset();
    void JustReachedHome();
    void JustDied(Unit* /*killer*/);
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);
    void JustSummoned(Creature* summoned);
    void EnterCombat(Unit* /*who*/);
    void HandleEvent(const uint32 Event);
    void UpdateAI(const uint32 diff);
private:
    bool intro;
    bool final;
    bool submerged;

    Creature* burrow[4];
    Creature* spheres[6];

    InstanceScript* instance;
    SummonList summons;
    EventMap events;
};

class BossAnubarak : public CreatureScript
{
public:
    BossAnubarak() : CreatureScript("boss_anubarak_trial") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new BossAnubarakAI(creature);
    };
};

void AddBossAnubarak()
{
    new BossAnubarak();
}