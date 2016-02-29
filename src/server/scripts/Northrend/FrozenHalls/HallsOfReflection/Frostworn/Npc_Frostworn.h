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

#ifndef _NPC_FROSTWORN_H
#define _NPC_FROSTWORN_H

#include "../HallsOfReflection.h"


namespace NPC_FROSTWORN
{
    enum Says
    {
        SAY_AGGRO                   = -1594519,
        SAY_DEATH                   = -1594520
    };

    enum Spells
    {
        // General
        SPELL_SHIELD_THROWN         = 69222,
        H_SPELL_SHIELD_THROWN       = 73076,
        SPELL_SPIKE                 = 69184,
        H_SPELL_SPIKE               = 70399,
        SPELL_SUMMON_REFLECTIONS    = 69223,

        // Reflection
        SPELL_BALEFUL_STRIKE        = 69933,
        SPELL_SPIRIT_BURST          = 69900,
        H_SPELL_BALEFUL_STRIKE      = 70400,
        H_SPELL_SPIRIT_BURST        = 73046
    };

    enum Events
    {
        // General
        EVENT_SPELL_SHIELD_THROWN   = 1,
        EVENT_SPELL_SPIKE           = 2,
        EVENT_CLONE                 = 3,

        // Reflection
        EVENT_SPELL_BALEFUL_STRIKE  = 4
    };
} // NPC_FROSTWORN

class Npc_FrostwornGeneral : public CreatureScript
{
    public:

    Npc_FrostwornGeneral() : CreatureScript("npc_frostworn_general") { }

    struct Npc_FrostwornGeneralAI : public ScriptedAI
    {
        public:

            Npc_FrostwornGeneralAI(Creature *pCreature) : ScriptedAI(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void MoveInLineOfSight(Unit* /*who*/) override;

            void EnterCombat(Unit* /*victim*/) override;
            void JustDied(Unit* /*killer*/) override;

            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new Npc_FrostwornGeneralAI(pCreature);
    }
};


class Npc_SpiritualReflection : public CreatureScript
{
    public:

    Npc_SpiritualReflection() : CreatureScript("npc_spiritual_reflection") { }

    struct Npc_SpiritualReflectionAI : public ScriptedAI
    {
        public:

            Npc_SpiritualReflectionAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void EnterCombat(Unit* /*victim*/) override;
            void JustDied(Unit* /*killer*/) override;

            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new Npc_SpiritualReflectionAI(pCreature);
    }
};


#endif // _NPC_FROSTWORN_H
