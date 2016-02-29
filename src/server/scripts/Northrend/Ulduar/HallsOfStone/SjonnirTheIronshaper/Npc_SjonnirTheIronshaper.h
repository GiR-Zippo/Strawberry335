/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#ifndef _NPC_SJONNIRTHEIRONSHAPER_H
#define _NPC_SJONNIRTHEIRONSHAPER_H
#define _DATA_TRIBUNALOFTHEAGES_H

#include "Data_SjonnirTheIronshaper.h"
#include "../TribunalOfTheAges//Npc_TribunalOfTheAges.h"


class npc_malformed_ooze : public CreatureScript
{
    public:
        npc_malformed_ooze() : CreatureScript("npc_malformed_ooze") {}

        struct npc_malformed_oozeAI : public ScriptedAI
        {
            npc_malformed_oozeAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void EnterCombat(Unit* who);
            void MovementInform(uint32 type, uint32 id);

            void UpdateAI(uint32 const diff);
            void ExecuteEvents();
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_malformed_oozeAI(creature);
        };
};

class npc_iron_sludge : public CreatureScript
{
    public:
        npc_iron_sludge() : CreatureScript("npc_iron_sludge") {}

        struct npc_iron_sludgeAI : public ScriptedAI
        {
            npc_iron_sludgeAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            };

            InstanceScript* instance;

            void JustDied(Unit* /*killer*/);
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_sludgeAI(creature);
        };
};

class npc_brann_hos_final : public CreatureScript
{
    public:
        npc_brann_hos_final() : CreatureScript("npc_brann_hos_final") {}

        struct npc_brann_hos_finalAI : public ScriptedAI
        {
            npc_brann_hos_finalAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void EnterEvadeMode();
            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);

            void MovementInform(uint32 type, uint32 id);
            void UpdateAI(const uint32 diff);
            void ExecuteEvents();
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_brann_hos_finalAI(creature);
        };
};

class npc_trigger_sjonnir : public CreatureScript
{
public:
    npc_trigger_sjonnir() : CreatureScript("npc_trigger_sjonnir") {}

    struct npc_trigger_sjonnirAI : public ScriptedAI
    {
        npc_trigger_sjonnirAI(Creature* creature) : ScriptedAI(creature) {}

        void MoveInLineOfSight(Unit* who);
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_trigger_sjonnirAI(creature);
    };
};


#endif // _NPC_SJONNIRTHEIRONSHAPER_H
