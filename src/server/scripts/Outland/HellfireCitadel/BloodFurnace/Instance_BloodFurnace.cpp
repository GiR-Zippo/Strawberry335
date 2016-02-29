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
SDName: Instance_Blood_Furnace
SD%Complete: 100
SDComment:
SDCategory: Hellfire Citadel/Blood Furnace
EndScriptData */

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "BloodFurnace.h"


class instance_blood_furnace : public InstanceMapScript
{
    public:
        instance_blood_furnace() : InstanceMapScript("instance_blood_furnace", 542){}

        struct instance_blood_furnace_InstanceMapScript : public InstanceScript
        {
            instance_blood_furnace_InstanceMapScript(Map* map) : InstanceScript(map){}

            bool DoorsUpdated;

            uint64 The_MakerGUID;
            uint64 BroggokGUID;
            uint64 Broggok_ControllerGUID;
            uint64 Kelidan_The_BreakerGUID;

            uint64 Door1GUID;
            uint64 Door2GUID;
            uint64 Door3GUID;
            uint64 Door4GUID;
            uint64 Door5GUID;
            uint64 Door6GUID;

            uint64 PrisonCell1GUID;
            uint64 PrisonCell2GUID;
            uint64 PrisonCell3GUID;
            uint64 PrisonCell4GUID;
            uint64 PrisonCell5GUID;
            uint64 PrisonCell6GUID;
            uint64 PrisonCell7GUID;
            uint64 PrisonCell8GUID;

            uint64 BroggokPreEventLeverGUID;

            uint32 m_auiEncounter[MAX_ENCOUNTER];
            uint32 m_auiEvent;
            std::string str_data;

            void Initialize()
            {
                memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
                m_auiEvent = NOT_STARTED;

                DoorsUpdated = false;

                The_MakerGUID = 0;
                BroggokGUID = 0;
                Broggok_ControllerGUID = 0;
                Kelidan_The_BreakerGUID = 0;

                Door1GUID = 0;
                Door2GUID = 0;
                Door3GUID = 0;
                Door4GUID = 0;
                Door5GUID = 0;
                Door6GUID = 0;

                PrisonCell1GUID = 0;
                PrisonCell2GUID = 0;
                PrisonCell3GUID = 0;
                PrisonCell4GUID = 0;
                PrisonCell5GUID = 0;
                PrisonCell6GUID = 0;
                PrisonCell7GUID = 0;
                PrisonCell8GUID = 0;

                BroggokPreEventLeverGUID = 0;
            };

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case CREATURE_THE_MAKER:           { The_MakerGUID = creature->GetGUID();           }break;
                    case CREATURE_BROGGOK:             { BroggokGUID = creature->GetGUID();             }break;
                    case CREATURE_KELIDAN_THE_BREAKER: { Kelidan_The_BreakerGUID = creature->GetGUID(); }break;
                    case CREATURE_BROGGOK_CONTROLLER:  { Broggok_ControllerGUID = creature->GetGUID();  }break;
                }
            };

            void OnGameObjectCreate(GameObject* go)
            {
                uint32 gobEntry = go->GetEntry();
                uint64 gobGUID = go->GetGUID();

                switch (gobEntry)
                {
                    case GO_FINAL_EXIT_DOOR:          { Door1GUID = gobGUID;       }break;
                    case GO_THE_MAKER_FRONT_DOOR:     { Door2GUID = gobGUID;       }break;
                    case GO_THE_MAKER_REAR_DOOR:      { Door3GUID = gobGUID;       }break;
                    case GO_BROGGOK_FRONT_DOOR:       { Door4GUID = gobGUID;       }break;
                    case GO_BROGGOK_REAR_DOOR:        { Door5GUID = gobGUID;       }break;
                    case GO_KELIDAN_EXIT_DOOR:        { Door6GUID = gobGUID;       }break;
                    case GO_THE_MAKER_PRISON_CELL_FR: { PrisonCell1GUID = gobGUID; }break;
                    case GO_THE_MAKER_PRISON_CELL_BR: { PrisonCell2GUID = gobGUID; }break;
                    case GO_THE_MAKER_PRISON_CELL_FL: { PrisonCell3GUID = gobGUID; }break;
                    case GO_THE_MAKER_PRISON_CELL_BL: { PrisonCell4GUID = gobGUID; }break;
                    case GO_BROGGOK_PRISON_CELL_FR:   { PrisonCell5GUID = gobGUID; }break;
                    case GO_BROGGOK_PRISON_CELL_BR:   { PrisonCell6GUID = gobGUID; }break;
                    case GO_BROGGOK_PRISON_CELL_FL:   { PrisonCell7GUID = gobGUID; }break;
                    case GO_BROGGOK_PRISON_CELL_BL:   { PrisonCell8GUID = gobGUID; }break;
                    case GO_BROGGOK_PREEVENT_LEVER:   { BroggokPreEventLeverGUID = gobGUID; }break;
                }
            };

            uint64 GetData64(uint32 identifier)
            {
                switch (identifier)
                {
                    case DATA64_THE_MAKER:                return The_MakerGUID;
                    case DATA64_BROGGOK:                  return BroggokGUID;
                    case DATA64_BROGGOK_CONTROLLER:       return Broggok_ControllerGUID;
                    case DATA64_KELIDAN_THE_MAKER:        return Kelidan_The_BreakerGUID;
                    case DATA64_FINAL_EXIT_DOOR:          return Door1GUID;
                    case DATA64_THE_MAKER_FRONT_DOOR:     return Door2GUID;
                    case DATA64_THE_MAKER_REAR_DOOR:      return Door3GUID;
                    case DATA64_BROGGOK_FRONT_DOOR:       return Door4GUID;
                    case DATA64_BROGGOK_REAR_DOOR:        return Door5GUID;
                    case DATA64_KELIDAN_EXIT_DOOR:        return Door6GUID;
                    case DATA64_THE_MAKER_PRISON_CELL_FR: return PrisonCell1GUID;
                    case DATA64_THE_MAKER_PRISON_CELL_BR: return PrisonCell2GUID;
                    case DATA64_THE_MAKER_PRISON_CELL_FL: return PrisonCell3GUID;
                    case DATA64_THE_MAKER_PRISON_CELL_BL: return PrisonCell4GUID;
                    case DATA64_BROGGOK_PRISON_CELL_FR:   return PrisonCell5GUID;
                    case DATA64_BROGGOK_PRISON_CELL_BR:   return PrisonCell6GUID;
                    case DATA64_BROGGOK_PRISON_CELL_FL:   return PrisonCell7GUID;
                    case DATA64_BROGGOK_PRISON_CELL_BL:   return PrisonCell8GUID;
                    case DATA64_BROGGOK_PREEVENT_LEVER:   return BroggokPreEventLeverGUID;
                }

                return 0;
            };

            void SetData(uint32 identifier, uint32 data)
            {
                switch (identifier)
                {
                    case DATA_THE_MAKER:               m_auiEncounter[0] = data;     break;
                    case DATA_BROGGOK:                 m_auiEncounter[1] = data;     break;
                    case DATA_KELIDAN_THE_BREAKER:     m_auiEncounter[2] = data;     break;
                    case DATA_BROGGOK_EVENT:           m_auiEvent        = data;     break;
                }

                if (data == DONE)
                {
                    OUT_SAVE_INST_DATA;

                    std::ostringstream saveStream;
                    saveStream << m_auiEncounter[0] << ' ' << m_auiEncounter[1] << ' ' << m_auiEncounter[2];

                    str_data = saveStream.str();

                    SaveToDB();
                    OUT_SAVE_INST_DATA_COMPLETE;
                }
            };

            uint32 GetData(uint32 identifier)
            {
                switch (identifier)
                {
                    case DATA_THE_MAKER:               return m_auiEncounter[0];
                    case DATA_BROGGOK:                 return m_auiEncounter[1];
                    case DATA_KELIDAN_THE_BREAKER:     return m_auiEncounter[2];
                    case DATA_BROGGOK_EVENT:           return m_auiEvent;
                }

                return 0;
            };

            void OnPlayerEnter(Player* /*player*/)
            {
                if (!DoorsUpdated)
                {
                    OpenGates();
                    DoorsUpdated = true;
                }
            };

            void OpenGates()
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    if (m_auiEncounter[i] == DONE)
                    {
                        switch (i+1)
                        {
                            case DATA_THE_MAKER:
                            {
                                HandleGameObject(Door2GUID, true);
                                HandleGameObject(Door3GUID, true);
                            }break;

                            case DATA_BROGGOK:
                            {
                                HandleGameObject(Door4GUID, true);
                                HandleGameObject(Door5GUID, true);
                            }break;

                            case DATA_KELIDAN_THE_BREAKER:
                            {
                                HandleGameObject(Door1GUID, true);
                                HandleGameObject(Door6GUID, true);
                            }break;
                        }
                    }
                };
            };

            const char* Save()
            {
                return str_data.c_str();
            };

            void Load(const char* in)
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                std::istringstream loadStream(in);
                loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2];

                m_auiEvent = NOT_STARTED;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (m_auiEncounter[i] == IN_PROGRESS || m_auiEncounter[i] == FAIL)
                        m_auiEncounter[i] = NOT_STARTED;

                OUT_LOAD_INST_DATA_COMPLETE;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_blood_furnace_InstanceMapScript(map);
        };
};

void AddSC_instance_blood_furnace()
{
    new instance_blood_furnace();
};
