/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#ifndef _NPC_ARGENTUM_CHALLENGE_H
#define _NPC_ARGENTUM_CHALLENGE_H

#include "../TrialOfTheChampion.h"


enum Npc_ArgentumChallenge_Spells
{
    //Memory
    SPELL_SHADOW_FORM           = 34429, // THIS IS NOT CORRECT!
    SPELL_OLD_WOUNDS            = 66620,
    SPELL_SHADOWS_PAST          = 66619,
    SPELL_WAKING_NIGHTMARE      = 66552,

    // Monk
    SPELL_DIVINE_SHIELD         = 67251,
    SPELL_FINAL_MEDITATION      = 67255,
    SPELL_FLURRY_OF_BLOWS       = 67233,
    SPELL_PUMMEL                = 67235,

    // Lightwielder
    SPELL_BLAZING_LIGHT         = 67247,
    SPELL_CLEAVE                = 15284,
    SPELL_UNBALANCING_STRIKE    = 67237,

    // Priest
    SPELL_HOLY_SMITE            = 36176,
    SPELL_HOLY_SMITE_H          = 67289,
    SPELL_SHADOW_WORD_PAIN      = 34941,
    SPELL_SHADOW_WORD_PAIN_H    = 34942,
    SPELL_MIND_CONTROL          = 67229,
    SPELL_FOUNTAIN_OF_LIGHT     = 67194,
};

enum Npc_ArgentumChallenge_Events
{
    //Memory
    EVENT_SPELL_OLD_WOUNDS          = 1,
    EVENT_SPELL_SHADOWS_PAST        = 2,
    EVENT_SPELL_WAKING_NIGHTMARE    = 3,

    // Monk
    EVENT_SPELL_FLURRY_OF_BLOWS     = 1,
    EVENT_SPELL_PUMMEL              = 2,

    // Lightwielder
    EVENT_SPELL_BLAZING_LIGHT       = 1,
    EVENT_SPELL_CLEAVE              = 2,
    EVENT_SPELL_UNBALANCING_STRIKE  = 3,

    // Priest
    EVENT_SPELL_SMITE               = 1,
    EVENT_SPELL_SHADOW_WORD_PAIN    = 2,
    EVENT_SPELL_MIND_CONTROL        = 3,
    EVENT_SPELL_FOUNTAIN_OF_LIGHT   = 4,
};

class Npc_Memory : public CreatureScript
{
    public:

    Npc_Memory() : CreatureScript("npc_memory") { }

    struct Npc_MemoryAI : public ScriptedAI
    {
        public:

            Npc_MemoryAI(Creature* creature) : ScriptedAI(creature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;

            void EnterCombat(Unit* /*attacker*/) override;
            void JustDied(Unit* /*killer*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Npc_MemoryAI(creature);
    }
};

class Npc_ArgentMonk : public CreatureScript
{
    public:

    Npc_ArgentMonk() : CreatureScript("npc_argent_monk") { }

    struct Npc_ArgentMonkAI : public ScriptedAI
    {
        public:

            Npc_ArgentMonkAI(Creature* creature) : ScriptedAI(creature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            //void JustReachedHome() override;

            void EnterCombat(Unit* /*attacker*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;
            void JustDied(Unit* /*killer*/) override;

            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
            bool shieldCasted;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Npc_ArgentMonkAI(creature);
    }
};

class Npc_ArgentLightwielder : public CreatureScript
{
    public:

    Npc_ArgentLightwielder() : CreatureScript("npc_argent_lightwielder") { }

    struct Npc_ArgentLightwielderAI : public ScriptedAI
    {
        public:

            Npc_ArgentLightwielderAI(Creature* creature) : ScriptedAI(creature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            //void JustReachedHome() override;

            void EnterCombat(Unit* /*attacker*/) override;
            void JustDied(Unit* /*killer*/) override;

            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Npc_ArgentLightwielderAI(creature);
    }
};

class Npc_ArgentPriest : public CreatureScript
{
    public:

    Npc_ArgentPriest() : CreatureScript("npc_argent_priest") { }

    struct Npc_ArgentPriestAI : public ScriptedAI
    {
        public:

            Npc_ArgentPriestAI(Creature* creature) : ScriptedAI(creature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            //void JustReachedHome() override;

            void EnterCombat(Unit* /*attacker*/) override;
            void JustDied(Unit* /*killer*/) override;

            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Npc_ArgentPriestAI(creature);
    }
};


#endif // _NPC_ARGENTUM_CHALLENGE_H
