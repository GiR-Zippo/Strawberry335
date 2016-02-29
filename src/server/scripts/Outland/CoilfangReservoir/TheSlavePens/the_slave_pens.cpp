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

/* ScriptData
SDName: The Slave Pens
SD%Complete: 100%
SDComment:
SDCategory: Coilfang Reservoir, The Slave Pens
EndScriptData */

/* ContentData
npc_naturalist_bite
EndContentData */

#include "ScriptPCH.h"
#include "the_slave_pens.h"

typedef npc_naturalist_bite::npc_naturalist_biteAI                  naturalistAI;

const Position SpawnPosition    = {-163.010f, -741.437f, 37.892f, 4.150250f};
const Position WaypointPosition = {-184.064f, -780.143f, 43.799f, 0.9f};
const Position DespawnPosition  = {-116.816f, -750.135f, 36.988f, 0.364628f};

void naturalistAI::UpdateAI(const uint32 diff)
{
    events.Update(diff);

    if (GetData(EVENT_INTRO)== IN_PROGRESS)
    {
        switch (events.ExecuteEvent())
        {
            // open cage
            case 1:
            {
                if (!pInstance)
                    return;

                GameObject* pObj = GameObject::GetGameObject(*me, pInstance->GetData64(DATA64_GOB_CAGE));
                if (pObj)
                    pObj->UseDoorOrButton();

                events.ScheduleEvent(2, 1*IN_MILLISECONDS);
            }break;

            // leave cage
            case 2:
            {
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(0, WaypointPosition);
                events.ScheduleEvent(3, 7*IN_MILLISECONDS);
            }break;

            // talk
            case 3:
            {
                if (playerGUID != 0)
                    DoScriptText(NPC_NATURALIST_SAY_1, me, me->GetPlayer((*me), playerGUID));
                events.ScheduleEvent(4, 1*IN_MILLISECONDS);
            }break;

            // spawn adds
            case 4:
            {
                adds[0] = me->SummonCreature(NPC_ENTRY_ADD_2, SpawnPosition);

                Position AddSpawnPosition = SpawnPosition;
                AddSpawnPosition.m_positionX += 3.0f;
                AddSpawnPosition.m_positionY += 1.0f;

                for (uint8 i = 0; i < (sizeof(adds)/sizeof(Creature*)); ++i)
                {
                    if (i != 0)
                    {
                        adds[i] = me->SummonCreature(NPC_ENTRY_ADD_1, AddSpawnPosition);
                        AddSpawnPosition.m_positionX += 3.0f;
                        AddSpawnPosition.m_positionY += 1.0f;
                    }

                    if (adds[i])
                        AggroAllPlayers(adds[i]);
                };

                me->SetReactState(REACT_AGGRESSIVE);
                me->SetInCombatWith(adds[0]);
                me->AddThreat(adds[0], 0.0f);
                events.Reset();
                SetData(EVENT_INTRO, DONE);
                SetData(EVENT_ADDS, IN_PROGRESS);
            }break;
        }
    }
    else if (GetData(EVENT_ADDS)== IN_PROGRESS)
    {
        switch (events.ExecuteEvent())
        {
            // check adds
            case 1:
            {
                for (uint8 i = 0; i < (sizeof(adds)/sizeof(Creature*)); ++i)
                {
                    if (!adds[i])
                        return;

                    if (adds[i]->isAlive())
                    {
                        events.ScheduleEvent(1, 1*IN_MILLISECONDS);
                        DoMeleeAttackIfReady();
                        return;
                    }
                }
                events.ScheduleEvent(2, 2*IN_MILLISECONDS);
            }break;

            // buff and complete quest and set next
            case 2:
            {
                BuffAndCompletePlayerQuest();

                events.Reset();
                SetData(EVENT_ADDS, DONE);
                SetData(EVENT_ESCAPE, IN_PROGRESS);
            }break;
        }
    }
    else if (GetData(EVENT_ESCAPE)== IN_PROGRESS)
    {
        switch (events.ExecuteEvent())
        {
            // move to despawn position
            case 1:
            {
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(0, DespawnPosition);
                events.ScheduleEvent(2, 12*IN_MILLISECONDS);
            }break;

            // despawn
            case 2:
            {
                for (uint8 i = 0; i < (sizeof(adds)/sizeof(Creature*)); ++i)
                {
                    if (adds[i])
                        adds[i]->DespawnOrUnsummon();
                };

                events.Reset();
                SetData(EVENT_ESCAPE, DONE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
                me->SetVisible(false);
            }break;
        }
    }
}

void naturalistAI::SetData(uint32 identifier, uint32 data)
{
    eventIds[identifier] = data;

    if (data == IN_PROGRESS)
        events.ScheduleEvent(1, 0);
}

void naturalistAI::SetPlayerGUID(uint64 guid)
{
    playerGUID = guid;
}

void naturalistAI::BuffAndCompletePlayerQuest()
{
    Map::PlayerList const &PlList = me->GetMap()->GetPlayers();

    if (PlList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
    {
        if (Player* pPlayer = i->getSource())
        {
            pPlayer->CastSpell(pPlayer, SPELL_MARK_OF_BITE, true);
            if (pPlayer->IsActiveQuest(QUEST_LOST_IN_ACTION))
                pPlayer->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
        }
    };
}

void naturalistAI::AggroAllPlayers(Creature* creature)
{
    Map::PlayerList const &PlList = creature->GetMap()->GetPlayers();

    if (PlList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
    {
        if (Player* pPlayer = i->getSource())
        {
            if (pPlayer->isAlive())
            {
                creature->SetInCombatWith(pPlayer);
                pPlayer->SetInCombatWith(creature);
                creature->AddThreat(pPlayer, 0.0f);
            }
        }
    }
}

bool npc_naturalist_bite::OnGossipHello(Player* player, Creature* creature)
{
    uint32 gossipTextId = player->GetGossipTextId(creature);
    uint32 gossipMenuId = Player::GetDefaultGossipMenuForSource(creature);

    player->ADD_GOSSIP_ITEM_DB(gossipMenuId, GOSSIP_OPTIONID_RELEASE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    player->SEND_GOSSIP_MENU(gossipTextId, creature->GetGUID());

    return true;
}

bool npc_naturalist_bite::OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
{
    player->PlayerTalkClass->ClearMenus();

    switch (action)
    {
    case GOSSIP_ACTION_INFO_DEF:
        if (creature->AI()->GetData(EVENT_INTRO) != IN_PROGRESS)
        {
            creature->AI()->SetData(EVENT_INTRO, IN_PROGRESS);
            CAST_AI(npc_naturalist_bite::npc_naturalist_biteAI, creature->AI())->SetPlayerGUID(player->GetGUID());
            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            player->CLOSE_GOSSIP_MENU();
        }break;
    }
    return true;
}

void AddSC_the_slave_pens()
{
    new npc_naturalist_bite;
}
