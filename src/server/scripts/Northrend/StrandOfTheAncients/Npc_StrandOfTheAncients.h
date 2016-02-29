/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 29.07.2014
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

#include "ScriptPCH.h"
#include "BattlegroundSA.h"


enum BG_SA_GOSSIP_OPTIONID
{
    BG_SA_GOSSIP_ENGINEER_BUILD         = 0
};

enum BG_SA_GOSSIP_TEXTID
{
    BG_SA_GOSSIP_ENGINEER_CANNOT_BUILD  = 29262
};

class NpcSotADemolisherEngineer : public CreatureScript
{
    public:
        NpcSotADemolisherEngineer() : CreatureScript("npc_sota_demolisher_engineer") {}

        bool OnGossipHello(Player* player, Creature* creature);
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action);

    private:
        bool CanBuild(Creature* creature);
};