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

#ifndef _BOSS_GRAND_CHAMPIONS_H
#define _BOSS_GRAND_CHAMPIONS_H

#include "Vehicle_GrandChampions.h"
#include "../TrialOfTheChampion.h"


enum Boss_GrandChampions_Spells
{
    // Marshal Jacob Alerius && Mokra the Skullcrusher || Warrior
    SPELL_MORTAL_STRIKE             = 68783,
    SPELL_BLADESTORM                = 63784,
    SPELL_INTERCEPT                 = 67540,
    SPELL_ROLLING_THROW             = 47115, //not implemented in the AI yet...

    // Ambrose Boltspark && Eressea Dawnsinger || Mage
    SPELL_FIREBALL                  = 66042,
    SPELL_BLAST_WAVE                = 66044,
    SPELL_HASTE                     = 66045,
    SPELL_POLYMORPH                 = 66043,

    // Colosos && Runok Wildmane || Shaman
    SPELL_CHAIN_LIGHTNING           = 67529,
    SPELL_EARTH_SHIELD              = 67530,
    SPELL_HEALING_WAVE              = 67528,
    SPELL_HEX_OF_MENDING            = 67534,
    SPELL_HEX_OF_MENDING_HEAL       = 67535,

    // Jaelyne Evensong && Zul'tore || Hunter
    SPELL_DISENGAGE                 = 68339,
    SPELL_LIGHTNING_ARROWS          = 66085,
    SPELL_LIGHTNING_ARROWS_DAMAGE   = 66095,
    SPELL_LIGHTNING_ARROWS_VISUAL   = 66083,
    SPELL_MULTI_SHOT                = 66081,
    SPELL_SHOOT                     = 65868,

    // Lana Stouthammer Evensong && Deathstalker Visceri || Rouge
    SPELL_DEADLY_POISON             = 67711,
    SPELL_EVISCERATE                = 67709,
    SPELL_FAN_OF_KNIVES             = 67706,
    SPELL_POISON_BOTTLE             = 67701,
    SPELL_DUAL_WIELD                = 42459,
};

enum Boss_GrandChampions_Events
{
    // Marshal Jacob Alerius && Mokra the Skullcrusher || Warrior
    EVENT_SPELL_MORTAL_STRIKE               = 1,
    EVENT_SPELL_BLADESTORM                  = 2,
    EVENT_SPELL_INTERCEPT                   = 3,
    EVENT_SPELL_ROLLING_THROW               = 4, //not implemented in the AI yet...

    // Ambrose Boltspark && Eressea Dawnsinger || Mage
    EVENT_SPELL_FIREBALL                    = 1,
    EVENT_SPELL_BLAST_WAVE                  = 2,
    EVENT_SPELL_HASTE                       = 3,
    EVENT_SPELL_POLYMORPH                   = 4,

    // Colosos && Runok Wildmane || Shaman
    EVENT_SPELL_CHAIN_LIGHTNING             = 1,
    EVENT_SPELL_EARTH_SHIELD                = 2,
    EVENT_SPELL_HEALING_WAVE                = 3,
    EVENT_SPELL_HEX_OF_MENDING              = 4,

    // Jaelyne Evensong && Zul'tore || Hunter
    EVENT_SPELL_DISENGAGE                   = 1,
    EVENT_SPELL_LIGHTNING_ARROWS            = 2,
    EVENT_SPELL_MULTI_SHOT                  = 3,

    // Lana Stouthammer Evensong && Deathstalker Visceri || Rouge
    EVENT_SPELL_EVISCERATE                  = 1,
    EVENT_SPELL_FAN_OF_KNIVES               = 2,
    EVENT_SPELL_POISON_BOTTLE               = 3,
};

// Marshal Jacob Alerius && Mokra the Skullcrusher || Warrior
class Boss_GrandChampionWarrior : public CreatureScript
{
    public:

    Boss_GrandChampionWarrior() : CreatureScript("boss_warrior_toc5") { }

    struct Boss_GrandChampionWarriorAI : public MountedChampionAI
    {
        public:

            Boss_GrandChampionWarriorAI(Creature* pCreature) : MountedChampionAI(pCreature), defeated(false)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void JustReachedHome() override;

            void EnterCombat(Unit* /*who*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;

            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
            bool defeated;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Boss_GrandChampionWarriorAI(pCreature);
    }
};

// Ambrose Boltspark && Eressea Dawnsinger || Mage
class Boss_GrandChampionMage : public CreatureScript
{
public:
    Boss_GrandChampionMage() : CreatureScript("boss_mage_toc5") { }

    struct Boss_GrandChampionMageAI : public MountedChampionAI
    {
        public:

            Boss_GrandChampionMageAI(Creature* pCreature) : MountedChampionAI(pCreature), defeated(false)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void JustReachedHome() override;

            void EnterCombat(Unit* /*who*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);

            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
            bool defeated;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Boss_GrandChampionMageAI(pCreature);
    }
};

// Colosos && Runok Wildmane || Shaman
class Boss_GrandChampionShaman : public CreatureScript
{
public:
    Boss_GrandChampionShaman() : CreatureScript("boss_shaman_toc5") { }

    struct Boss_GrandChampionShamanAI : public MountedChampionAI
    {
        public:

            Boss_GrandChampionShamanAI(Creature* pCreature) : MountedChampionAI(pCreature), defeated(false)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void JustReachedHome() override;

            void EnterCombat(Unit* /*who*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;

            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
            bool defeated;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Boss_GrandChampionShamanAI(pCreature);
    }
};

// Jaelyne Evensong && Zul'tore || Hunter
class Boss_GrandChampionHunter : public CreatureScript
{
public:
    Boss_GrandChampionHunter() : CreatureScript("boss_hunter_toc5") { }

    struct Boss_GrandChampionHunterAI : public MountedChampionAI
    {
        public:

            Boss_GrandChampionHunterAI(Creature* pCreature) : MountedChampionAI(pCreature), defeated(false)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void JustReachedHome() override;

            void EnterCombat(Unit* /*who*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;

            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;

            bool ligthingArrows;
            bool defeated;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Boss_GrandChampionHunterAI(pCreature);
    }
};

class Boss_GrandChampionRouge : public CreatureScript
{
public:
    Boss_GrandChampionRouge() : CreatureScript("boss_rouge_toc5") { }

    // Lana Stouthammer Evensong && Deathstalker Visceri || Rouge
    struct Boss_GrandChampionRougeAI : public MountedChampionAI
    {
        public:

            Boss_GrandChampionRougeAI(Creature* pCreature) : MountedChampionAI(pCreature), defeated(false)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void JustReachedHome() override;

            void EnterCombat(Unit* /*who*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;

            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
            bool defeated;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new Boss_GrandChampionRougeAI(pCreature);
    }
};


#endif // _BOSS_GRAND_CHAMPIONS_H
