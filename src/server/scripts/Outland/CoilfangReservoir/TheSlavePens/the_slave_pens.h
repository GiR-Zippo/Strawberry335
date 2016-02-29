/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
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

#ifndef _THE_SLAVE_PENS_H
#define _THE_SLAVE_PENS_H

enum Say
{
    NPC_NATURALIST_SAY_1       = -1900000
};

enum Entry
{
    NPC_ENTRY_ADD_1            = 17957,
    NPC_ENTRY_ADD_2            = 17960,
    GOB_ENTRY_CAGE             = 182094
};

enum SpellId
{
    SPELL_MARK_OF_BITE         = 34906
};

enum QuestEntry
{
    QUEST_LOST_IN_ACTION       = 9738
};

enum GossipOptionId
{
    GOSSIP_OPTIONID_RELEASE    = 0,
};

enum Data
{
    EVENT_INTRO                = 0,
    EVENT_ADDS,
    EVENT_ESCAPE,
    EVENTS_MAX,
};

enum Data64
{
    DATA64_GOB_CAGE            = 0
};

class npc_naturalist_bite : public CreatureScript
{
public:
    npc_naturalist_bite() : CreatureScript("npc_naturalist_bite") { }

    struct npc_naturalist_biteAI : public ScriptedAI
    {
        npc_naturalist_biteAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = me->GetInstanceScript();
        }

        EventMap events;
        uint32 eventIds[EVENTS_MAX];
        Creature* adds[3];
        uint64 playerGUID;
        InstanceScript* pInstance;

        void UpdateAI(const uint32 diff);
        void SetData(uint32 type, uint32 data);
        void SetPlayerGUID(uint64 guid);
        uint32 GetData(uint32 type) { return eventIds[type]; };

        void SpawnAdds();
        void BuffAndCompletePlayerQuest();
        void AggroAllPlayers(Creature* creature);
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_naturalist_biteAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action);
};


#endif // _THE_SLAVE_PENS_H
