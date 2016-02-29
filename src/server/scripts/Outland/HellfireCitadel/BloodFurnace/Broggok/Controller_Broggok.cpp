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

#include "Controller_Broggok.h"
typedef mob_broggok_controller::mob_broggok_controllerAI ControllerAI;


void ControllerAI::Reset()
{
    _currentCell = CELL_FRONT_RIGHT;
    _preEvent = PREEVENT_NOT_STARTED;

    if (instance = me->GetInstanceScript())
        instance->SetData(DATA_BROGGOK_EVENT, NOT_STARTED);
};

void ControllerAI::JustSummoned(Creature* summoned)
{
    summoned->setFaction(16);
    summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
    summoned->SetReactState(REACT_PASSIVE);
};

void ControllerAI::DoAction(const int32 action)
{
    if (action == ACTION_START_PREEVENT && _preEvent == PREEVENT_NOT_STARTED)
        StartPreEvent();
};

void ControllerAI::UpdateAI(const uint32 diff)
{
    if (!events.Empty())
    {
        events.Update(diff);
        ExecutePreEvents();
    }
};

void ControllerAI::ExecutePreEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (_preEvent)
        {
            case PREEVENT_CELLS:
            {
                switch (eventId)
                {
                    // open door
                    case 1:
                    {
                        events.CancelEvent(4);
                        OpenCurrentCellDoor();
                        events.ScheduleEvent(2, 1.5*IN_MILLISECONDS);
                    }break;

                    // mod creatures and start combat - or, if it's the last one, check creature alives
                    case 2:
                    {
                        ModCreaturesAndStartCombat();
                        if (_currentCell != CELL_BACK_LEFT)
                            events.ScheduleEvent(3, TIME_BETWEEN_CELLS);
                        else
                            events.ScheduleEvent(5, 5*IN_MILLISECONDS);

                        events.ScheduleEvent(4, 2*IN_MILLISECONDS);
                    }break;

                    // open door timer
                    case 3:
                    {
                        ++_currentCell;
                        events.ScheduleEvent(1, 1*IN_MILLISECONDS);
                    }break;

                    // check creature combat
                    case 4:
                    {
                        if (CheckCreatureAliveOrCombat(true))
                            events.ScheduleEvent(5, 5*IN_MILLISECONDS);
                        else
                        {
                            events.Reset();
                            ResetPreEvent();
                        }
                    }break;

                    // check creature alive
                    case 5:
                    {
                        if (CheckCreatureAliveOrCombat())
                            events.ScheduleEvent(4, 5*IN_MILLISECONDS);
                        else
                        {
                            events.Reset();
                            _preEvent = PREEVENT_BROGGOK_START;
                            events.ScheduleEvent(1, 1*IN_MILLISECONDS);
                        }
                    }break;
                }
            }break;

            case PREEVENT_BROGGOK_START:
            {
                switch (eventId)
                {
                    // open door
                    case 1:
                    {
                        if (!instance)
                            return;

                        DespawnCreatures();
                        instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_REAR_DOOR), true);
                        events.ScheduleEvent(2, 1*IN_MILLISECONDS);
                    }break;

                    // start movement to combat position
                    case 2:
                    {
                        if (!instance)
                            return;

                        if (Creature* pBroggok = me->GetCreature((*me), instance->GetData64(DATA_BROGGOK)))
                        {
                            pBroggok->GetMotionMaster()->Clear();
                            pBroggok->GetMotionMaster()->MovePoint(0, BroggokCombatPosition);
                            events.ScheduleEvent(3, 8*IN_MILLISECONDS);
                        }
                    }break;

                    // set new home position, close door, remove flags and set preevent done
                    case 3:
                    {
                        if (Creature* pBroggok = me->GetCreature((*me), instance->GetData64(DATA_BROGGOK)))
                        {
                            pBroggok->SetHomePosition(BroggokCombatPosition);

                            if (!instance)
                                return;

                            instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_REAR_DOOR), false);
                            instance->SetData(DATA_BROGGOK_EVENT, DONE);

                            pBroggok->GetMotionMaster()->Clear();
                            AggroAllPlayers(pBroggok);
                            me->DespawnOrUnsummon();
                        }
                    }break;
                }
            }break;
        }
    }
};

void ControllerAI::SummonCreaturesForCell(uint8 cellId)
{
    uint32 crtCount = 0;

    switch (cellId)
    {
        // 3 creatures posID 0
        case CELL_FRONT_RIGHT:{ crtCount = COUNT_PRISON_0; }break;
        // 4 creatures posID 1
        case CELL_BACK_RIGHT: { crtCount = COUNT_PRISON_1; }break;
        // 5 creatures posID 2
        case CELL_FRONT_LEFT: { crtCount = COUNT_PRISON_2; }break;
        // 6 creatures posID 3
        case CELL_BACK_LEFT:  { crtCount = COUNT_PRISON_3; }break;
        default: return;
    }

    Position SpawnPositon = SpawnPositonCells[cellId];

    for (uint8 i = 0; i < crtCount; ++i)
    {
        SpawnPositon.m_positionX = SpawnPositonCells[cellId].m_positionX;

        if (i%2 == 0)
        {
            if (cellId <= CELL_BACK_RIGHT)
                SpawnPositon.m_positionX -= 5.0f;
            else
                SpawnPositon.m_positionX += 5.0f;
        }
        else
            SpawnPositon.m_positionY += 2.66f;

        Creature* pCreature= NULL;
        if (pCreature = DoSummon(CREATURE_BROGGOK_EVENT, SpawnPositon, 0, TEMPSUMMON_MANUAL_DESPAWN))
        {
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            _cellCreatures[cellId].push_back(pCreature->GetGUID());
        }
    };
};

void ControllerAI::StartPreEvent()
{
    for (uint8 i = 0; i < CELL_COUNT; ++i)
    {
        _cellCreatures[i].clear();
        SummonCreaturesForCell(i);
    };

    instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_FRONT_DOOR), false);
    _preEvent = PREEVENT_CELLS;

    events.ScheduleEvent(1, 5*IN_MILLISECONDS);
    DoZoneInCombat();
};

void ControllerAI::OpenCurrentCellDoor()
{
    if (!instance)
        return;

    uint64 gObjGUID = 0;

    switch (_currentCell)
    {
        case CELL_FRONT_RIGHT: { gObjGUID = instance->GetData64(DATA64_BROGGOK_PRISON_CELL_FR); }break;
        case CELL_BACK_RIGHT:  { gObjGUID = instance->GetData64(DATA64_BROGGOK_PRISON_CELL_BR); }break;
        case CELL_FRONT_LEFT:  { gObjGUID = instance->GetData64(DATA64_BROGGOK_PRISON_CELL_FL); }break;
        case CELL_BACK_LEFT:   { gObjGUID = instance->GetData64(DATA64_BROGGOK_PRISON_CELL_BL); }break;
        default: return;
    }

    instance->HandleGameObject(gObjGUID, true);
};

void ControllerAI::ModCreaturesAndStartCombat()
{
    GUIDStore::const_iterator itr;

    for (itr = _cellCreatures[_currentCell].begin(); itr != _cellCreatures[_currentCell].end(); ++itr)
    {
        if (Creature* pCreature = Creature::GetCreature((*me), *itr))
            AggroAllPlayers(pCreature);
    }
};

bool ControllerAI::CheckCreatureAliveOrCombat(bool onlyCheckCombat)
{
    GUIDStore::const_iterator itr;

    if (onlyCheckCombat)
    {
        for (itr = _cellCreatures[_currentCell].begin(); itr != _cellCreatures[_currentCell].end(); ++itr)
        {
            if (Creature* pCreature = Creature::GetCreature((*me), *itr))
            {
                if (!pCreature->isInCombat() && pCreature->isAlive())
                    return false;
            }
        };

        return true;
    }
    else
    {
        for (uint8 cellId = 0; cellId < CELL_COUNT; ++cellId)
        {
            for (itr = _cellCreatures[cellId].begin(); itr != _cellCreatures[cellId].end(); ++itr)
            {
                if (Creature* pCreature = Creature::GetCreature((*me), *itr))
                {
                    if (pCreature->isAlive())
                        return true;
                }
            }
        }

        return false;
    }
};

void ControllerAI::DespawnCreatures()
{
    GUIDStore::const_iterator itr;

    for (uint8 cellId = 0; cellId < CELL_COUNT; ++cellId)
    {
        for (itr = _cellCreatures[cellId].begin(); itr != _cellCreatures[cellId].end(); ++itr)
        {
            if (Creature* pCreature = Creature::GetCreature((*me), *itr))
                pCreature->DespawnOrUnsummon();
        }
    }
};

void ControllerAI::ResetPreEvent()
{
    DespawnCreatures();

    if (!instance)
        return;

    instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_PRISON_CELL_FR), false);
    instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_PRISON_CELL_BR), false);
    instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_PRISON_CELL_FL), false);
    instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_PRISON_CELL_BL), false);
    instance->HandleGameObject(instance->GetData64(DATA64_BROGGOK_FRONT_DOOR), true);

    GameObject* gObj = NULL;
    if (gObj = GameObject::GetGameObject((*me), instance->GetData64(DATA64_BROGGOK_PREEVENT_LEVER)))
        gObj->Respawn();

    instance->SetData(DATA_BROGGOK_EVENT, FAIL);
    _preEvent = PREEVENT_NOT_STARTED;
    _currentCell = CELL_FRONT_RIGHT;
};

void ControllerAI::AggroAllPlayers(Creature* creature)
{
    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
    creature->SetReactState(REACT_AGGRESSIVE);

    Map::PlayerList const &PlList = creature->GetMap()->GetPlayers();

    if (PlList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
    {
        if (Player* player = i->getSource())
        {
            if (player->isGameMaster())
                continue;

            if (player->isAlive())
            {
                creature->SetInCombatWith(player);
                player->SetInCombatWith(creature);
                creature->AddThreat(player, 0.0f);
            }
        }
    }
};
