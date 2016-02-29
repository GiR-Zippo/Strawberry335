/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 10.08.2014
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
 *
 * @description
 *
 * Horrible script for the summoned npc of the lich king, shadowguard valkyr
 * Calculate every step to the trigger position and move the npc
 * This has to be done like this because of the movement generator
 *
 * @todo : Find a better way to do this
 */

#ifndef __NPC_VALKYRSHADOWGUARD_H_
#define __NPC_VALKYRSHADOWGUARD_H_

#include "TheLichKing.h"
#include "ScriptedEscortAI.h"


enum ValkyrSpecialData
{
    VALKYR_REACHED_HOME_DISPLAYID   = 31312,
    VALKYR_HOME_POSITION_CALC       = 10
};

class NpcValkyrShadowguardAI : public ScriptedAI
{
    public:

        NpcValkyrShadowguardAI(Creature* creature) : ScriptedAI(creature),
            dropPointTriggerGUID(0), grabbedPlayer(0), pathSteps(0),
            instance(creature->GetInstanceScript())
        {
        }

        void InitializeAI() override;
        void IsSummonedBy(Unit* /*summoner*/);
        void DamageTaken(Unit* /*attacker*/, uint32& damage);
        void JustReachedHome();
        void AttackStart(Unit* /*target*/);
        void MovementInform(uint32 type, uint32 id);
        void SpellHit(Unit* /*caster*/, SpellInfo const* /*spell*/);
        void SetGUID(uint64 guid, int32 /* = 0*/);
        void UpdateAI(const uint32 diff);

    private:

        uint64 dropPointTriggerGUID;
        uint64 grabbedPlayer;
        uint8 pathSteps;
        std::vector<Position> pathVector;

        EventMap events;
        InstanceScript* instance;

        void calcPathToDropPoint();
        void calcAndSetNewHomePosition();
        void dropPlayer(bool despawn);
};

class NpcValkyrShadowguard : public CreatureScript
{
    public:

        NpcValkyrShadowguard() : CreatureScript("npc_valkyr_shadowguard") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<NpcValkyrShadowguardAI>(creature);
        }
};

#endif