/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 22.09.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#ifndef _BOSS_MARWYN_H
#define _BOSS_MARWYN_H

#include "../../HallsOfReflection.h"


namespace BOSS_MARWYN
{
    enum Yells
    {
        SAY_AGGRO                   = -1668060,
        SAY_SLAY_1                  = -1668061,
        SAY_SLAY_2                  = -1668062,
        SAY_DEATH                   = -1668063,
        SAY_CORRUPTED_FLESH_1       = -1668064,
        SAY_CORRUPTED_FLESH_2       = -1668065
    };

    enum Spells
    {
        SPELL_OBLITERATE            = 72360,
        SPELL_WELL_OF_CORRUPTION    = 72362,
        SPELL_CORRUPTED_FLESH       = 72363,
        SPELL_SHARED_SUFFERING      = 72368,

        // Start Quests
        SPELL_START_HALLS_OF_REFLECTION_QUEST_AE = 72900,
    };

    enum Events
    {
        EVENT_NONE                  = 1,
        EVENT_OBLITERATE            = 2,
        EVENT_WELL_OF_CORRUPTION    = 3,
        EVENT_CORRUPTED_FLESH       = 4,
        EVENT_SHARED_SUFFERING      = 5
    };
} // BOSS_MARWYN

class Boss_Marwyn : public CreatureScript
{
    public:

    Boss_Marwyn() : CreatureScript("boss_marwyn") { }

    struct Boss_MarwynAI : public ScriptedAI
    {
        public:

            Boss_MarwynAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;

            void EnterCombat(Unit* /*who*/) override;
            void KilledUnit(Unit* /*victim*/) override;

            void JustDied(Unit* /*killer*/) override;
            void EnterEvadeMode() override;

            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);

            InstanceScript* pInstance;
            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Boss_MarwynAI(pCreature);
    }
};

#endif // _BOSS_MARWYN_H
