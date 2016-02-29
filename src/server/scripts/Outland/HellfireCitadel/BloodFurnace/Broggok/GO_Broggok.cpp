/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "GO_Broggok.h"
typedef go_boss_broggok_cell_door_lever GO_Lever_Broggok;


bool GO_Lever_Broggok::OnGossipHello(Player* player, GameObject* pObj)
{
    if (instance = pObj->GetInstanceScript())
    {
        Creature* pCreature = NULL;
        if (pCreature = player->GetCreature((*player), instance->GetData64(DATA64_BROGGOK_CONTROLLER)))
            pCreature->AI()->DoAction(ACTION_START_PREEVENT);
    }

    return false;
};
