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

#ifndef _BOSS_TRIBUNALOFTHEAGES_H
#define _BOSS_TRIBUNALOFTHEAGES_H

#include "Data_TribunalOfTheAges.h"


class mob_tribuna_controller : public CreatureScript
{
public:
    mob_tribuna_controller() : CreatureScript("mob_tribuna_controller") {}

    struct mob_tribuna_controllerAI : public ScriptedAI
    {
        mob_tribuna_controllerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            SetCombatMovement(false);
        };

        InstanceScript* instance;
        EventMap events;

        #ifdef DEBUG_TRIBUNAL_OF_AGES
        bool debugSpells;
        uint32 debugSpellTimer[3];
        #endif

        bool KaddrakEye;
        bool MarnakEye;

        void Reset();
        void ResetEvent(bool finished = false);
        void JustDied(Unit* killer);

        void UpdateAI(const uint32 diff);
        void ExecuteEvents();
        void DoAction(const int32 action);

        void StartEvent();
        void ActivateBoss(Events bossEvent);
        void KaddrakSpell();
        void MarnakSpell();
        void AbedneumSpell();

        void ActivateOrSwitchBossGO(BossId bossId, BossGOState state);
        bool CheckPlayers();

        #ifdef DEBUG_TRIBUNAL_OF_AGES
        void DeactivateBoss(Events bossEvent);
        void SetNewSpellTimer(Events bossEvent, uint32 timer);
        #endif
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_tribuna_controllerAI(creature);
    }
};


#endif // _BOSS_TRIBUNALOFTHEAGES_H
