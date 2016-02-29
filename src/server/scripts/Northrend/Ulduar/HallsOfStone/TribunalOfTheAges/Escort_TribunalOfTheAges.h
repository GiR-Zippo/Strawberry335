/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef _ESCORT_TRIBUNALOFTHEAGES_H
#define _ESCORT_TRIBUNALOFTHEAGES_H

#include "Data_TribunalOfTheAges.h"
#include "ScriptedEscortAI.h"


class npc_brann_hos : public CreatureScript
{
public:
    npc_brann_hos() : CreatureScript("npc_brann_hos")
    {
        #ifdef DEBUG_TRIBUNAL_OF_AGES
        debugMode = false;
        #endif
    }

    #ifdef DEBUG_TRIBUNAL_OF_AGES
    bool debugMode;
    Position HomePosition;
    Creature* debugController;
    #endif

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action);

    #ifdef DEBUG_TRIBUNAL_OF_AGES
    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code);
    #endif

    bool OnGossipHello(Player* player, Creature* creature);

    #ifdef DEBUG_TRIBUNAL_OF_AGES
    bool getBossStates(uint32& source);
    #endif

    struct npc_brann_hosAI : public npc_escortAI
    {
        npc_brann_hosAI(Creature* creature) : npc_escortAI(creature)
        {
            instance = creature->GetInstanceScript();
        };

        void Reset();
        void WaypointReached(uint32 waypointId);

        void SpawnDwarf(uint32 uiType);
        Creature* GetBossPointer(BossId boss);

        void JustSummoned(Creature* summoned);
        void JumpToNextStep(uint32 uiTimer);

        void StartWP();
        void JustDied(Unit* killer);
        void DamageTaken(Unit* /*done_by*/, uint32 & /*damage*/);

        uint32 GetData(uint32 type);

        void DoAction(const int32 action);
        void UpdateEscortAI(const uint32 uiDiff);

    private:
        bool bIsBattle;
        bool bIsLowHP;
        bool brannSparklinNews;
        bool _addSpawnPos;
        bool _eventStarted;

        uint32 uiStep;
        uint32 uiPhaseTimer;
        InstanceScript* instance;
        Position _homePosition;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_brann_hosAI(creature);
    }
};


#endif // _ESCORT_TRIBUNALOFTHEAGES_H
