/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 01.09.2014
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

#ifndef _NPC_TRIAL_OF_THE_CHAMPION_H
#define _NPC_TRIAL_OF_THE_CHAMPION_H

#include "TrialOfTheChampion.h"


#define ORIENTATION_TO_DOOR 1.5f

enum GossipTexts
{
    GOSSIP_TEXT_NOT_MOUNTED_HORDE          = 15043,
    GOSSIP_TEXT_NOT_MOUNTED_ALLIANCE       = 14757,
    GOSSIP_TEXT_FACTION_CHAMPIONS_DEFEATED = 14737,
    GOSSIP_TEXT_ARGENT_CHAMPIONS_DEFEATED  = 14738,
};

enum GossipOptionId
{
    GOSSIP_OPTIONID_EVENT1                 = 0,
    GOSSIP_OPTIONID_EVENT1_FAST            = 1,
    GOSSIP_OPTIONID_EVENT2                 = 2,
    GOSSIP_OPTIONID_EVENT3                 = 3
};

class Npc_TrialOfTheChampion_Announcer : public CreatureScript
{
    public:

        Npc_TrialOfTheChampion_Announcer() : CreatureScript("npc_announcer_toc5") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature) override;
        bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 action) override;

        struct Npc_TrialOfTheChampion_AnnouncerAI : public ScriptedAI
        {
            public:

                Npc_TrialOfTheChampion_AnnouncerAI(Creature* pCreature) : ScriptedAI(pCreature)
                {
                    GET_INSTANCESCRIPT_SAFE(pInstance);
                    me->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                }

                void DoAction(const int32 /*action*/) override;
                void MovementInform(uint32 /*type*/, uint32 /*id*/) override;

            private:

                InstanceScript* pInstance;
        };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new Npc_TrialOfTheChampion_AnnouncerAI(pCreature);
    }
};


#endif // _NPC_TRIAL_OF_THE_CHAMPION_H