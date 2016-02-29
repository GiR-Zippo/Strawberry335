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

#ifndef _BOSS_FALRIC_H
#define _BOSS_FALRIC_H

#include "../../HallsOfReflection.h"


namespace BOSS_FALRIC
{
    enum Yells
    {
        SAY_AGGRO                       = -1668050,
        SAY_SLAY_1                      = -1668051,
        SAY_SLAY_2                      = -1668052,
        SAY_DEATH                       = -1668053,
        SAY_IMPENDING_DESPAIR           = -1668054,
        SAY_DEFILING_HORROR             = -1668055
    };

    enum Spells
    {
        SPELL_QUIVERING_STRIKE          = 72422,
        SPELL_IMPENDING_DESPAIR         = 72426,
        SPELL_DEFILING_HORROR           = 72435,
        SPELL_HOPELESSNESS              = 72395,
        H_SPELL_HOPELESSNESS            = 72390 // TODO: not in dbc. Add in DB.
    };

    enum Events
    {
        EVENT_NONE                      = 1,
        EVENT_QUIVERING_STRIKE          = 2,
        EVENT_IMPENDING_DESPAIR         = 3,
        EVENT_DEFILING_HORROR           = 4
    };

    enum Phases
    {
        PHASE_HOPELESSNESS_FIRST        = 2,
        PHASE_HOPELESSNESS_SECOND       = 3,
        PHASE_HOPELESSNESS_THIRD        = 4,

        PHASE_MASK_HOPELESSNESS_FIRST   = 1 << (PHASE_HOPELESSNESS_FIRST - 1),
        PHASE_MASK_HOPELESSNESS_SECOND  = 1 << (PHASE_HOPELESSNESS_SECOND - 1),
        PHASE_MASK_HOPELESSNESS_THIRD   = 1 << (PHASE_HOPELESSNESS_THIRD - 1)
    };
} // BOSS_FALRIC

class Boss_Falric : public CreatureScript
{
    public:

    Boss_Falric() : CreatureScript("boss_falric") { }

    struct Boss_FalricAI : public ScriptedAI
    {
        public:

            Boss_FalricAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void MovementInform(uint32 /*type*/, uint32 /*id*/) override;

            void EnterCombat(Unit* /*who*/) override;
            void DamageTaken(Unit* /*pDoneBy*/, uint32& /*damage*/) override;

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
        return new Boss_FalricAI(pCreature);
    }
};


#endif // _BOSS_FALRIC_H
