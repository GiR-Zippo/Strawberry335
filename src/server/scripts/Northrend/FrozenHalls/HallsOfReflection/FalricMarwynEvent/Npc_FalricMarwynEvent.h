/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.09.2014
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

#ifndef _NPC_FALRICMARWYNEVENT_H
#define _NPC_FALRICMARWYNEVENT_H

#include "../HallsOfReflection.h"


namespace NPC_FALRICMARWYNEVENT
{
    enum Spells
    {
        // Ghostly Priest
        SPELL_SHADOW_WORD_PAIN          = 72318,
        SPELL_CIRCLE_OF_DESTRUCTION     = 72320,
        SPELL_COWER_IN_FEAR             = 72321,
        SPELL_DARK_MENDING              = 72322,

        // Phantom Mage
        SPELL_FIREBALL                  = 72163,
        SPELL_FLAMESTRIKE               = 72169,
        SPELL_FROSTBOLT                 = 72166,
        SPELL_CHAINS_OF_ICE             = 72121,
        SPELL_HALLUCINATION             = 72342,

        // Phantom Hallucination (same as phantom mage + HALLUCINATION_2 when dies)
        SPELL_HALLUCINATION_2           = 72344,

        // Shadowy Mercenary
        SPELL_SHADOW_STEP               = 72326,
        SPELL_DEADLY_POISON             = 72329,
        SPELL_ENVENOMED_DAGGER_THROW    = 72333,
        SPELL_KIDNEY_SHOT               = 72335,

        // Spectral Footman
        SPELL_SPECTRAL_STRIKE           = 72198,
        SPELL_SHIELD_BASH               = 72194,
        SPELL_TORTURED_ENRAGE           = 72203,

        // Tortured Rifleman
        SPELL_SHOOT                     = 72208,
        SPELL_CURSED_ARROW              = 72222,
        SPELL_FROST_TRAP                = 72215,
        SPELL_ICE_SHOT                  = 72268
    };

    enum Events
    {
        // Ghostly Priest
        EVENT_SHADOW_WORD_PAIN          = 1,
        EVENT_CIRCLE_OF_DESTRUCTION     = 2,
        EVENT_COWER_IN_FEAR             = 3,
        EVENT_DARK_MENDING              = 4,

        // Phantom Mage
        EVENT_FIREBALL                  = 5,
        EVENT_FLAMESTRIKE               = 6,
        EVENT_FROSTBOLT                 = 7,
        EVENT_CHAINS_OF_ICE             = 8,
        EVENT_HALLUCINATION             = 9,

        // Shadowy Mercenary
        EVENT_SHADOW_STEP               = 10,
        EVENT_DEADLY_POISON             = 11,
        EVENT_ENVENOMED_DAGGER_THROW    = 12,
        EVENT_KIDNEY_SHOT               = 13,

        // Spectral Footman
        EVENT_SPECTRAL_STRIKE           = 14,
        EVENT_SHIELD_BASH               = 15,
        EVENT_TORTURED_ENRAGE           = 16,

        // Tortured Rifleman
        EVENT_SHOOT                     = 17,
        EVENT_CURSED_ARROW              = 18,
        EVENT_FROST_TRAP                = 19,
        EVENT_ICE_SHOT                  = 20,
    };
} // NPC_FALRICMARWYNEVENT


struct Npc_FalricMarwynEventAI : public ScriptedAI
{
    public:

        Npc_FalricMarwynEventAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            GET_INSTANCESCRIPT_SAFE(pInstance);
        }

        void Reset() override;
        void SpellHit(Unit* /*caster*/, const SpellInfo* /*spellInfo*/) override;

        void UpdateAI(const uint32 diff) override;

    protected:

        virtual void executeEvent(uint32 eventID) {};

        InstanceScript* pInstance;
        EventMap eventMap;
};


class Npc_GhostlyPriest : public CreatureScript
{
    public:

    Npc_GhostlyPriest() : CreatureScript("npc_ghostly_priest") { }

    struct Npc_GhostlyPriestAI : public Npc_FalricMarwynEventAI
    {
        public:

            Npc_GhostlyPriestAI(Creature* pCreature) : Npc_FalricMarwynEventAI(pCreature) { }

            void EnterCombat(Unit* /*who*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_GhostlyPriestAI(pCreature);
    }
};


class Npc_PhantomMage : public CreatureScript
{
    public:

    Npc_PhantomMage() : CreatureScript("npc_phantom_mage") { }

    struct Npc_PhantomMageAI : public Npc_FalricMarwynEventAI
    {
        public:

            Npc_PhantomMageAI(Creature* pCreature) : Npc_FalricMarwynEventAI(pCreature) { }

            void EnterCombat(Unit* /*who*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_PhantomMageAI(pCreature);
    }
};


class Npc_PhantomHallucination : public CreatureScript
{
    public:

    Npc_PhantomHallucination() : CreatureScript("npc_phantom_hallucination") { }

    struct Npc_PhantomHallucinationAI : public Npc_PhantomMage::Npc_PhantomMageAI
    {
        Npc_PhantomHallucinationAI(Creature* pCreature) : Npc_PhantomMage::Npc_PhantomMageAI(pCreature) { }

        void JustDied(Unit* /*killer*/) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_PhantomHallucinationAI(pCreature);
    }
};


class Npc_ShadowyMercenary : public CreatureScript
{
    public:

    Npc_ShadowyMercenary() : CreatureScript("npc_shadowy_mercenary") { }

    struct Npc_ShadowyMercenaryAI : public Npc_FalricMarwynEventAI
    {
        public:

            Npc_ShadowyMercenaryAI(Creature* pCreature) : Npc_FalricMarwynEventAI(pCreature) { }

            void EnterCombat(Unit* /*who*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

        CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_ShadowyMercenaryAI(pCreature);
    }
};


class Npc_SpectralFootman : public CreatureScript
{
    public:

    Npc_SpectralFootman() : CreatureScript("npc_spectral_footman") { }

    struct Npc_SpectralFootmanAI : public Npc_FalricMarwynEventAI
    {
        public:

            Npc_SpectralFootmanAI(Creature* pCreature) : Npc_FalricMarwynEventAI(pCreature) { }

            void EnterCombat(Unit* /*who*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_SpectralFootmanAI(pCreature);
    }
};


class Npc_TorturedRifleman : public CreatureScript
{
    public:

    Npc_TorturedRifleman() : CreatureScript("npc_tortured_rifleman") { }

    struct Npc_TorturedRiflemanAI : public Npc_FalricMarwynEventAI
    {
        public:

            Npc_TorturedRiflemanAI(Creature* pCreature) : Npc_FalricMarwynEventAI(pCreature) { }

            void EnterCombat(Unit* /*who*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_TorturedRiflemanAI(pCreature);
    }
};


#endif // _NPC_FALRICMARWYNEVENT_H
