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

#ifndef _BOSS_ARGENTUMCHALLENGE_H
#define _BOSS_ARGENTUMCHALLENGE_H

#include "../TrialOfTheChampion.h"


enum Boss_ArgentumChallenge_Spells
{
    //Eadric
    SPELL_EADRIC_ACHIEVEMENT    = 68197, // Faceroller achievement
    SPELL_HAMMER_JUSTICE        = 66863,
    SPELL_HAMMER_RIGHTEOUS      = 66867,
    SPELL_RADIANCE              = 66935,
    SPELL_VENGEANCE             = 66865,
    SPELL_VENGANCE_AURA         = 66889,

    // Necessary for the achievement
    SPELL_HAMMER_RIGHTEOUS_P    = 66904, // Aura on player, changes to a stance with the spell
    SPELL_HAMMER_RIGHTEOUS_RET  = 66905, // Casted by player to Eadric
    SPELL_HAMMER_JUSTICE_STUN   = 66940,

    // Paletress
    SPELL_SMITE                 = 66536,
    SPELL_SMITE_H               = 67674,
    SPELL_HOLY_FIRE             = 66538,
    SPELL_HOLY_FIRE_H           = 67676,
    SPELL_RENEW                 = 66537,
    SPELL_RENEW_H               = 67675,
    SPELL_HOLY_NOVA             = 66546,
    SPELL_HOLY_SHIELD           = 66515,
    SPELL_CONFESS               = 66680,
    SPELL_SUMMON_MEMORY         = 66545
};

enum Boss_ArgentumChallenge_Events
{
    // Eadric
    EVENT_SPELL_VENGANCE            = 1,
    EVENT_SPELL_RADIANCE            = 2,
    EVENT_SPELL_HAMMER_OF_JUSTICE   = 3,

    // Paletress
    EVENT_SPELL_HOLY_FIRE   = 1,
    EVENT_SPELL_HOLY_SMITE  = 2,
    EVENT_SPELL_RENEW       = 3
};

class Boss_Eadric : public CreatureScript
{
    public:

    Boss_Eadric() : CreatureScript("boss_eadric") { }

    struct Boss_EadricAI : public ScriptedAI
    {
        public:

            Boss_EadricAI(Creature* creature) : ScriptedAI(creature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void JustReachedHome() override;

            void SpellHit(Unit* /*caster*/, const SpellInfo* /*spell*/) override;
            void EnterCombat(Unit* /*attacker*/) override;
            void KilledUnit(Unit* /*target*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;

            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
            bool defeated;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new Boss_EadricAI(creature);
    }
};

class Boss_Paletress : public CreatureScript
{
    public:

    Boss_Paletress() : CreatureScript("boss_paletress") { }

    struct Boss_PaletressAI : public ScriptedAI
    {
        public:

            Boss_PaletressAI(Creature* creature) : ScriptedAI(creature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void DoAction(const int32 /*action*/) override;

            void Reset() override;
            void JustSummoned(Creature* /*summon*/) override;
            void JustReachedHome() override;

            void EnterCombat(Unit* /*attacker*/) override;
            void KilledUnit(Unit* /*target*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;

            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
            bool shielded;
            bool defeated;

            uint64 memoryGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new Boss_PaletressAI(creature);
    }
};


#endif // _BOSS_ARGENTUMCHALLENGE_H
