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

#ifndef _VEHICLE_GRAND_CHAMPIONS_H
#define _VEHICLE_GRAND_CHAMPIONS_H

#include "../TrialOfTheChampion.h"


enum Vehicle_GrandChampions_Spells
{
    SPELL_CHARGE                    = 68282,
    SPELL_SHIELD_BREAKER            = 62575,
    SPELL_SHIELD_BREAKER_VISUAL     = 45815,
    SPELL_SHIELD                    = 66482,
    SPELL_THRUST                    = 62544,
    SPELL_KNEE                      = 68442,
    SPELL_DEFEND                    = 62719,
    SPELL_VISUAL_SHIELD_1           = 63130,
    SPELL_VISUAL_SHIELD_2           = 63131,
    SPELL_VISUAL_SHIELD_3           = 63132,
    // 67870
};

enum Vehicle_GrandChampions_Events
{
    EVENT_SPELL_CHARGE              = 1,
    EVENT_SPELL_SHIELD_BREAKER      = 2,
    EVENT_SPELL_SHIELD              = 3,
};

class Npc_MountedChampion : public CreatureScript
{
    public:

    Npc_MountedChampion() : CreatureScript("generic_vehicleAI_toc5") { }

    struct Npc_MountedChampionAI : ScriptedAI
    {
        public:

            Npc_MountedChampionAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;

            void EnterCombat(Unit* /*who*/) override;
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override;
            void JustDied(Unit* /*killer*/) override;

            void UpdateAI(const uint32 diff) override;

        protected:

            void executeEvent(uint32 eventID);

            uint32 getMountId();

            bool isGrandChampion();
            bool isInMountedGauntlet();

            InstanceScript* pInstance;
            EventMap eventMap;

            bool defeated;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new Npc_MountedChampionAI(pCreature);
    }
};

typedef Npc_MountedChampion::Npc_MountedChampionAI MountedChampionAI;


#endif // _VEHICLE_GRAND_CHAMPIONS_H
