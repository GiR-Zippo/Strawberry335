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

#ifndef DEF_MOLTEN_CORE_H
#define DEF_MOLTEN_CORE_H

enum Encounters
{
    BOSS_LUCIFRON                   = 0,
    BOSS_MAGMADAR                   = 1,
    BOSS_GEHENNAS                   = 2,
    BOSS_GARR                       = 3,
    BOSS_SHAZZRAH                   = 4,
    BOSS_BARON_GEDDON               = 5,
    BOSS_SULFURON_HARBINGER         = 6,
    BOSS_GOLEMAGG_THE_INCINERATOR   = 7,
    BOSS_MAJORDOMO_EXECUTUS         = 8,
    BOSS_RAGNAROS                   = 9,
    MAX_ENCOUNTER,
};

enum Actions
{
    ACTION_START_RAGNAROS       = 0,
    ACTION_START_RAGNAROS_ALT   = 1,
};

Position const RagnarosTelePos   = {829.159f, -815.773f, -228.972f, 5.30500f};
Position const RagnarosSummonPos = {838.510f, -829.840f, -232.000f, 2.00000f};

enum Creatures
{
    NPC_LUCIFRON                    = 12118,
    NPC_MAGMADAR                    = 11982,
    NPC_GEHENNAS                    = 12259,
    NPC_GARR                        = 12057,
    NPC_SHAZZRAH                    = 12264,
    NPC_BARON_GEDDON                = 12056,
    NPC_SULFURON_HARBINGER          = 12098,
    NPC_GOLEMAGG_THE_INCINERATOR    = 11988,
    NPC_MAJORDOMO_EXECUTUS          = 12018,
    NPC_RAGNAROS                    = 11502,
    NPC_FLAMEWAKER_HEALER           = 11663,
    NPC_FLAMEWAKER_ELITE            = 11664,
    NPC_CORE_RAGER                  = 11672,
};

enum GameObjects
{
    GO_CACHE_OF_THE_FIRELORD        = 179703,
};

enum Data
{
    DATA_RAGNAROS_ADDS              = 0,
};

enum Data64
{
    DATA_GOLEMAGG_CORE_RAGE_FIRST   = 0,
    DATA_GOLEMAGG_CORE_RAGE_SECOND  = 1
};

Position const SummonPositions[10] =
{
    {737.850f, -1145.35f, -120.288f, 4.71368f},
    {744.162f, -1151.63f, -119.726f, 4.58204f},
    {751.247f, -1152.82f, -119.744f, 4.49673f},
    {759.206f, -1155.09f, -120.051f, 4.30104f},
    {755.973f, -1152.33f, -120.029f, 4.25588f},
    {731.712f, -1147.56f, -120.195f, 4.95955f},
    {726.499f, -1149.80f, -120.156f, 5.24055f},
    {722.408f, -1152.41f, -120.029f, 5.33087f},
    {718.994f, -1156.36f, -119.805f, 5.75738f},
    {838.510f, -829.840f, -232.000f, 2.00000f},
};

class instance_molten_core : public InstanceMapScript
{
    public:
        instance_molten_core() : InstanceMapScript("instance_molten_core", 409) {}

        struct instance_molten_core_InstanceMapScript : public InstanceScript
        {
            instance_molten_core_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
                _golemaggTheIncineratorGUID = 0;
                _npcCoreRagerGUID[0]= 0;
                _npcCoreRagerGUID[1]= 0;
                _majordomoExecutusGUID = 0;
                _cacheOfTheFirelordGUID = 0;
                _executusDone = false;
                _deadBossCount = 0;
                _ragnarosAddDeaths = 0;
                _isLoading = false;
                _summonedExecutus = false;
            }

            void OnPlayerEnter(Player* /*player*/);
            void OnCreatureCreate(Creature* creature);
            void OnGameObjectCreate(GameObject* go);

            void SetData(uint32 type, uint32 data);
            uint32 GetData(uint32 type);
            uint64 GetData64(uint32 type);

            bool SetBossState(uint32 bossId, EncounterState state);
            void SummonMajordomoExecutus(bool done);

            std::string GetSaveData();
            void Load(char const* data);

        private:
            uint64 _golemaggTheIncineratorGUID;
            uint64 _npcCoreRagerGUID[2];
            uint64 _majordomoExecutusGUID;
            uint64 _cacheOfTheFirelordGUID;
            bool _executusDone;
            uint8 _deadBossCount;
            uint8 _ragnarosAddDeaths;
            bool _isLoading;
            bool _summonedExecutus;
        };

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_molten_core_InstanceMapScript(map);
    }
};

#endif
