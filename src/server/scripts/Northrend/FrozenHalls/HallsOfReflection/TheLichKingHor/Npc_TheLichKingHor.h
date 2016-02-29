/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 23.09.2014
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

#ifndef _NPC_THELICHKINGHOR_H
#define _NPC_THELICHKINGHOR_H

#include "../HallsOfReflection.h"


namespace NPC_THELICHKINGHOR
{
    enum Npcs
    {
        NPC_RAGING_GHOUL                = 36940
    };

    enum Spells
    {
        // Generic
        SPELL_EMERGE_VISUAL             = 50142,

        //Raging Gnoul
        SPELL_GNOUL_JUMP                = 70150,

        //Witch Doctor
        SPELL_COURSE_OF_DOOM            = 70144,
        H_SPELL_COURSE_OF_DOOM          = 70183,
        SPELL_SHADOW_BOLT_VOLLEY        = 70145,
        H_SPELL_SHADOW_BOLT_VOLLEY      = 70184,
        SPELL_SHADOW_BOLT               = 70080,
        H_SPELL_SHADOW_BOLT             = 70182,

        //Lumbering Abomination
        SPELL_ABON_STRIKE               = 40505,
        SPELL_VOMIT_SPRAY               = 70176,
        H_SPELL_VOMIT_SPRAY             = 70181
    };

    enum Events
    {
        // Generic
        EVENT_EMERGE                    = 1,

        // Raging Gnoul
        EVENT_SPELL_GNOUL_JUMP          = 2,

        // Witch Doctor
        EVENT_COURSE_OF_DOOM            = 3,
        EVENT_SPELL_SHADOW_BOLT_VOLLEY  = 4,
        EVENT_SPELL_SHADOW_BOLT         = 5,

        // Lumbering Abomination
        EVENT_SPELL_ABON_STRIKE         = 6,
        EVENT_SPELL_VOMIT_SPRAY         = 7
    };

    enum Actions
    {
        ACTION_MOVEMENT_THIRD_WALL      = 1,
        ACTION_MOVEMENT_FOURTH_WALL     = 2
    };

    const Position ThirdWaveMovePosition    = { 5451.630371f, 1905.720215f, 747.691650f, 4.171420f };
    const Position FourthWaveMovePosition   = { 5394.234375f, 1814.840088f, 760.270142f, 4.053098f };
}

struct Npc_TheLichKingHorAI : public ScriptedAI
{
    public:

        Npc_TheLichKingHorAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            GET_INSTANCESCRIPT_SAFE(pInstance);
        }

        void DoAction(const int32 action) override;
        void Reset() override;

        void MovementInform(uint32 /*type*/, uint32 /*point*/) override;
        void JustDied(Unit* /*pKiller*/) override;

        void UpdateAI(const uint32 diff) override;

    protected:

        virtual void executeEvent(uint32 eventID) { }

        InstanceScript* pInstance;
        EventMap eventMap;
};

class Npc_RagingGhoul : public CreatureScript
{
    public:

    Npc_RagingGhoul() : CreatureScript("npc_raging_gnoul") { }

    struct Npc_RagingGhoulAI : public Npc_TheLichKingHorAI
    {
        public:

            Npc_RagingGhoulAI(Creature* pCreature) : Npc_TheLichKingHorAI(pCreature) { }

            void EnterCombat(Unit* /*attacker*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_RagingGhoulAI(pCreature);
    }
};


class Npc_RisenWitchDoctor : public CreatureScript
{
    public:

    Npc_RisenWitchDoctor() : CreatureScript("npc_risen_witch_doctor") { }

    struct Npc_RisenWitchDoctorAI : public Npc_TheLichKingHorAI
    {
        public:

            Npc_RisenWitchDoctorAI(Creature* pCreature) : Npc_TheLichKingHorAI(pCreature) { }

            void EnterCombat(Unit* /*attacker*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_RisenWitchDoctorAI(pCreature);
    }
};


class Npc_Abon : public CreatureScript
{
    public:

    Npc_Abon() : CreatureScript("npc_abon") { }

    struct Npc_AbonAI : public Npc_TheLichKingHorAI
    {
        public:

            Npc_AbonAI(Creature* pCreature) : Npc_TheLichKingHorAI(pCreature) { }

            void EnterCombat(Unit* /*attacker*/) override;

        private:

            void executeEvent(uint32 eventID) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_AbonAI(pCreature);
    }
};


#endif // _NPC_THELICHKINGHOR_H
