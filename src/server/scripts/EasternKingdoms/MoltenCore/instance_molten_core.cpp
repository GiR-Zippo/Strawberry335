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

/* ScriptData
SDName: Instance_Molten_Core
SD%Complete: 0
SDComment: Place Holder
SDCategory: Molten Core
EndScriptData */

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureAI.h"
#include "instance_molten_core.h"


void instance_molten_core::instance_molten_core_InstanceMapScript::OnPlayerEnter(Player* /*player*/)
{
    if (_executusDone && !_summonedExecutus)
        SummonMajordomoExecutus(true);
};

void instance_molten_core::instance_molten_core_InstanceMapScript::OnCreatureCreate(Creature* creature)
{
    switch (creature->GetEntry())
    {
        case NPC_GOLEMAGG_THE_INCINERATOR:
            _golemaggTheIncineratorGUID = creature->GetGUID();
        break;

        case NPC_MAJORDOMO_EXECUTUS:
            _majordomoExecutusGUID = creature->GetGUID();
        break;

        case NPC_CORE_RAGER:
        {
            if (_npcCoreRagerGUID[0]== 0)
                _npcCoreRagerGUID[0]= creature->GetGUID();
            else
                _npcCoreRagerGUID[1]= creature->GetGUID();
        }break;

        default:
        break;
    }
};

void instance_molten_core::instance_molten_core_InstanceMapScript::OnGameObjectCreate(GameObject* go)
{
    switch (go->GetEntry())
    {
        case GO_CACHE_OF_THE_FIRELORD:
            _cacheOfTheFirelordGUID = go->GetGUID();
        break;

        default:
        break;
    }
};

void instance_molten_core::instance_molten_core_InstanceMapScript::SetData(uint32 type, uint32 data)
{
    if (type == DATA_RAGNAROS_ADDS)
    {
        if (data == 1)
            ++_ragnarosAddDeaths;
        else if (data == 0)
            _ragnarosAddDeaths = 0;
    }
};

uint32 instance_molten_core::instance_molten_core_InstanceMapScript::GetData(uint32 type)
{
    switch (type)
    {
    case DATA_RAGNAROS_ADDS:
        return _ragnarosAddDeaths;
    }

    return 0;
};

uint64 instance_molten_core::instance_molten_core_InstanceMapScript::GetData64(uint32 type)
{
    switch (type)
    {
        case BOSS_GOLEMAGG_THE_INCINERATOR:     return _golemaggTheIncineratorGUID;
        case BOSS_MAJORDOMO_EXECUTUS:           return _majordomoExecutusGUID;
        case DATA_GOLEMAGG_CORE_RAGE_FIRST:     return _npcCoreRagerGUID[0];
        case DATA_GOLEMAGG_CORE_RAGE_SECOND:    return _npcCoreRagerGUID[1];
    }

    return 0;
};

bool instance_molten_core::instance_molten_core_InstanceMapScript::SetBossState(uint32 bossId, EncounterState state)
{
    if (!InstanceScript::SetBossState(bossId, state))
        return false;

    if (state == DONE && bossId < BOSS_MAJORDOMO_EXECUTUS)
        ++_deadBossCount;

    if (_isLoading)
        return true;

    if (_deadBossCount == 8)
        SummonMajordomoExecutus(false);

    if (bossId == BOSS_MAJORDOMO_EXECUTUS && state == DONE)
        DoRespawnGameObject(_cacheOfTheFirelordGUID, 7 * DAY);

    return true;
};

void instance_molten_core::instance_molten_core_InstanceMapScript::SummonMajordomoExecutus(bool done)
{
    if (_summonedExecutus)
        return;

    _summonedExecutus = true;
    if (!done)
    {
        instance->SummonCreature(NPC_MAJORDOMO_EXECUTUS, SummonPositions[0]);
        instance->SummonCreature(NPC_FLAMEWAKER_HEALER, SummonPositions[1]);
        instance->SummonCreature(NPC_FLAMEWAKER_HEALER, SummonPositions[2]);
        instance->SummonCreature(NPC_FLAMEWAKER_HEALER, SummonPositions[3]);
        instance->SummonCreature(NPC_FLAMEWAKER_HEALER, SummonPositions[4]);
        instance->SummonCreature(NPC_FLAMEWAKER_ELITE, SummonPositions[5]);
        instance->SummonCreature(NPC_FLAMEWAKER_ELITE, SummonPositions[6]);
        instance->SummonCreature(NPC_FLAMEWAKER_ELITE, SummonPositions[7]);
        instance->SummonCreature(NPC_FLAMEWAKER_ELITE, SummonPositions[8]);
    }
    else if (TempSummon* summon = instance->SummonCreature(NPC_MAJORDOMO_EXECUTUS, RagnarosTelePos))
        summon->AI()->DoAction(ACTION_START_RAGNAROS_ALT);
};

std::string instance_molten_core::instance_molten_core_InstanceMapScript::GetSaveData()
{
    OUT_SAVE_INST_DATA;

    std::ostringstream saveStream;
    saveStream << "M C " << GetBossSaveData();

    OUT_SAVE_INST_DATA_COMPLETE;
    return saveStream.str();
};

void instance_molten_core::instance_molten_core_InstanceMapScript::Load(char const* data)
{
    if (!data)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    _isLoading = true;
    OUT_LOAD_INST_DATA(data);

    char dataHead1, dataHead2;

    std::istringstream loadStream(data);
    loadStream >> dataHead1 >> dataHead2;

    if (dataHead1 == 'M' && dataHead2 == 'C')
    {
        EncounterState states[MAX_ENCOUNTER];
        uint8 executusCounter = 0;

        // need 2 loops to check spawning executus/ragnaros
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            uint32 tmpState;
            loadStream >> tmpState;
            if (tmpState == IN_PROGRESS || tmpState > TO_BE_DECIDED)
                tmpState = NOT_STARTED;
            states[i] = EncounterState(tmpState);

            if (tmpState == DONE && i < BOSS_MAJORDOMO_EXECUTUS)
                ++executusCounter;
        }

        if (executusCounter >= 9 && states[BOSS_RAGNAROS] != DONE)
            _executusDone = true;

        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            SetBossState(i, states[i]);
    }
    else
        OUT_LOAD_INST_DATA_FAIL;

    OUT_LOAD_INST_DATA_COMPLETE;
    _isLoading = false;
};

void AddSC_instance_molten_core()
{
    new instance_molten_core();
};
