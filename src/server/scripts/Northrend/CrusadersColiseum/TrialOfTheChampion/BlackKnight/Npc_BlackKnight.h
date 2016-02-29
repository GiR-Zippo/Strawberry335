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

#ifndef _NPC_BLACK_KNIGHT_H
#define _NPC_BLACK_KNIGHT_H

#include "../TrialOfTheChampion.h"
#include "ScriptedEscortAI.h"


enum Npc_BlackKnight_Spells
{
    SPELL_CLAW              = 67774,
    SPELL_LEAP              = 67749,
    SPELL_EXPLODE_H         = 67886,
};

enum Npc_BlackKnight_Events
{
    EVENT_SPELL_CLAW        = 1,
    EVENT_SPELL_LEAP        = 2
};

class Npc_RisenGhoul : public CreatureScript
{
    public:

    Npc_RisenGhoul() : CreatureScript("npc_risen_ghoul") { }

    struct Npc_RisenGhoulAI : public ScriptedAI
    {
        public:

            Npc_RisenGhoulAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void EnterCombat(Unit* /*who*/) override;

            void SpellHitTarget(Unit* /*victim*/, const SpellInfo* /*spell*/) override;
            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_RisenGhoulAI(pCreature);
    }
};

class Npc_BlackKnight_SkeletalGryphon : public CreatureScript
{
    public:

    Npc_BlackKnight_SkeletalGryphon() : CreatureScript("npc_black_knight_skeletal_gryphon") { }

    struct Npc_BlackKnight_SkeletalGryphonAI : public npc_escortAI
    {
        public:

            Npc_BlackKnight_SkeletalGryphonAI(Creature* pCreature)
            : npc_escortAI(pCreature), pVehicleKit(pCreature->GetVehicleKit())
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
                me->SetSpeed(MOVE_FLIGHT, 2.8f);
            }

            void DoAction(const int32 /*action*/) override;
            void WaypointReached(uint32 /*id*/) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            Vehicle* pVehicleKit;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_BlackKnight_SkeletalGryphonAI(pCreature);
    }
};


#endif // _NPC_BLACK_KNIGHT_H
