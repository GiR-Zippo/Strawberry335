/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 12.11.2014
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

#ifndef _NPC_CUSTOMIZE_H
#define _NPC_CUSTOMIZE_H


enum GossipActionIds
{
    GOSSIP_ACTION_CHANGE_FACTION    = 0,
    GOSSIP_ACTION_CHANGE_RACE       = 1,
    GOSSIP_ACTION_CHANGE_CHAR       = 2,
    GOSSIP_ACTION_CHANGE_NAME       = 3,
    GOSSIP_ACTION_MAX
};

enum NpcTextIds
{
    NPC_TEXT_ERROR                  = 60000,
    NPC_TEXT_SUCCESS                = 60004,

    NPC_TEXT_UNAVAILABLE            = 60010,
    NPC_TEXT_NOT_ENOUGH_GOLD        = 60011
};


class Npc_Customize : public CreatureScript
{
    public:

        Npc_Customize() : CreatureScript("npc_customize") { }

        bool OnGossipHello(Player* /*player*/, Creature* /*creature*/) override;
        bool OnGossipSelect(Player* /*player*/, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override;

    private:

        /** Check gold amount with specified actionId */
        bool checkAndTakeCurrency(Player* /*pPlayer*/, uint32 /*actionId*/);

        /** Proceed action with specified actionId */
        uint32 proceedAction(Player* /*pPlayer*/, uint32 /*actionId*/);
};


#endif // _NPC_CUSTOMIZE_H