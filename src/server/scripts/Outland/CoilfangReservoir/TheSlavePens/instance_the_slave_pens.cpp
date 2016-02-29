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
SDName: Instance - The Slave Pens
SD%Complete: 100
SDComment: Instance Data for The Slave Pens instance
SDCategory: Coilfang Reservoir, The Slave Pens
EndScriptData */

#include "ScriptPCH.h"
#include "the_slave_pens.h"


class instance_the_slave_pens : public InstanceMapScript
{
public:
    instance_the_slave_pens() : InstanceMapScript("instance_the_slave_pens", 547) {};

    struct instance_the_slave_pens_InstanceMapScript : public InstanceScript
    {
        instance_the_slave_pens_InstanceMapScript(Map* map) : InstanceScript(map) {};

        uint64 m_uiNaturalistBiteCageGUID;

        void Initialize()
        {
            m_uiNaturalistBiteCageGUID = 0;
        };

        void OnGameObjectCreate(GameObject* gObj)
        {
            switch (gObj->GetEntry())
            {
                case GOB_ENTRY_CAGE:{ m_uiNaturalistBiteCageGUID = gObj->GetGUID(); }break;
            }
        };

        uint64 GetData64(uint32 identifier)
        {
            switch (identifier)
            {
                case DATA64_GOB_CAGE:{ return m_uiNaturalistBiteCageGUID; }break;
            }

            return 0;
        };
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_the_slave_pens_InstanceMapScript(map);
    };
};

void AddSC_instance_the_slave_pens()
{
    new instance_the_slave_pens();
}
