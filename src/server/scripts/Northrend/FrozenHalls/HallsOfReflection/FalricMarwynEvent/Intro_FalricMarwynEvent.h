/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 25.09.2014
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

#ifndef _INTRO_FALRIC_MARWYN_EVENT_H
#define _INTRO_FALRIC_MARWYN_EVENT_H

#include "../HallsOfReflection.h"
#include "ScriptedEscortAI.h"


namespace INTRO_FALRIC_MARWYN_EVENT
{
    enum Quests
    {
        QUEST_DELIVRANCE_FROM_THE_PIT_A2    = 24710,
        QUEST_DELIVRANCE_FROM_THE_PIT_H2    = 24712,
        QUEST_WRATH_OF_THE_LICH_KING_A2     = 24500,
        QUEST_WRATH_OF_THE_LICH_KING_H2     = 24802,
    };
} // INTRO_FALRIC_MARWYN_EVENT

class Npc_JainaOrSylvanasHor_FirstPart : public CreatureScript
{
    public:

        Npc_JainaOrSylvanasHor_FirstPart(bool sylvana, const char* name)
            : CreatureScript(name), isSylvana(sylvana) { }

        bool OnGossipHello(Player* /*player*/, Creature* /*creature*/) override;
        bool OnGossipSelect(Player* /*player*/, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override;

        struct Npc_JainaOrSylvanasHorAI_FirstPart : public ScriptedAI
        {
            public:

                Npc_JainaOrSylvanasHorAI_FirstPart(Creature* pCreature) : ScriptedAI(pCreature) { }

                void Reset() override;
        };

        CreatureAI* GetAI(Creature* pCreature) const override
        {
            return new Npc_JainaOrSylvanasHorAI_FirstPart(pCreature);
        }

    private:

        bool isSylvana;
};


#endif // _INTRO_HALLSOFREFLECTION_H
