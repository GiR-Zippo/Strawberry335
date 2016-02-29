/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef _NPC_TRIBUNALOFTHEAGES_H
#define _NPC_TRIBUNALOFTHEAGES_H

#include "Data_TribunalOfTheAges.h"


class npc_spell_shadow_orb : public CreatureScript
{
    public:

        npc_spell_shadow_orb() : CreatureScript("npc_spell_shadow_orb") {}

        struct npc_spell_shadow_orbAI : public ScriptedAI
        {
            npc_spell_shadow_orbAI(Creature* creature) : ScriptedAI(creature) {}

            void MovementInform(uint32 type, uint32 id);
            void SendNewOrbToPlayer();
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_spell_shadow_orbAI(creature);
        }
};

class npc_brann_adds : public CreatureScript
{
    public:

        npc_brann_adds() : CreatureScript("npc_brann_adds") {}

        struct npc_brann_addsAI : public ScriptedAI
        {
            npc_brann_addsAI(Creature* creature) : ScriptedAI(creature) {}

            InstanceScript* instance;

            void MovementInform(uint32 type, uint32 id);
            void DoAfterMovement();
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_brann_addsAI(creature);
        }
};


#endif // _NPC_TRIBUNALOFTHEAGES_H
