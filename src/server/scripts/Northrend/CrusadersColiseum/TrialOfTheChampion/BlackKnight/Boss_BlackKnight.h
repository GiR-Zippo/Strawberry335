/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
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

#ifndef _BOSS_BLACK_KNIGHT_H
#define _BOSS_BLACK_KNIGHT_H

#include "../TrialOfTheChampion.h"


enum Boss_BlackKnight_Spells
{
    // Phase 1
    SPELL_PLAGUE_STRIKE     = 67724,
    SPELL_ICY_TOUCH         = 67718,
    SPELL_DEATH_RESPITE     = 66798,
    SPELL_DEATH_RESPITE_H   = 67745,
    SPELL_OBLITERATE        = 67725,
    SPELL_RAISE_ARELAS      = 67705,
    SPELL_RAISE_JAEREN      = 67715,

    // Phase 2 - During this phase, the Black Knight will use the same abilities as in phase 1
    // except for Death's Respite
    SPELL_ARMY_DEAD         = 67761,
    SPELL_DESECRATION       = 67778,
    SPELL_GHOUL_EXPLODE     = 67751,
    SPELL_EXPLODE           = 67729,

    // Phase 3
    SPELL_DEATH_BITE        = 67808,
    SPELL_MARKED_DEATH      = 67882,

    SPELL_BLACK_KNIGHT_RES  = 67693,
};

enum Boss_BlackKnight_Events
{
    // Phase 1
    EVENT_SPELL_PLAGUE_STRIKE     = 1,
    EVENT_SPELL_ICY_TOUCH         = 2,
    EVENT_SPELL_DEATH_RESPITE     = 3,
    EVENT_SPELL_OBLITERATE        = 4,

    // Phase 2 - During this phase, the Black Knight will use the same abilities as in phase 1
    // except for Death's Respite
    EVENT_SPELL_ARMY_DEAD         = 5,
    EVENT_RESTORE_FROM_ARMY_DEAD  = 6,
    EVENT_SPELL_DESECRATION       = 7,
    EVENT_SPELL_GHOUL_EXPLODE     = 8,

    // Phase 3
    EVENT_SPELL_DEATH_BITE        = 9,
    EVENT_SPELL_MARKED_DEATH      = 10,

    // Transition
    EVENT_TRANSITION_RESURRECT    = 11
};

enum Boss_BlackKnight_Models
{
    MODEL_SKELETON  = 29846,
    MODEL_GHOST     = 21300
};

enum Boss_BlackKnight_Phases
{
    PHASE_UNDEAD        = 1,
    PHASE_SKELETON      = 2,
    PHASE_GHOST         = 3,
    PHASE_TRANSITION    = 4,

    PHASE_MASK_UNDEAD       = 1 << (PHASE_UNDEAD - 1),
    PHASE_MASK_SKELETON     = 1 << (PHASE_SKELETON - 1),
    PHASE_MASK_GHOST        = 1 << (PHASE_GHOST - 1),
    PHASE_MASK_TRANSITION   = 1 << (PHASE_TRANSITION - 1),

    PHASE_MASK_FIRST_TRANSITION     = PHASE_MASK_UNDEAD | PHASE_MASK_TRANSITION,
    PHASE_MASK_SECOND_TRANSITION    = PHASE_MASK_SKELETON | PHASE_MASK_TRANSITION
};

enum Boss_BlackKnight_Says
{
    SAY_AGGRO       = 1,
    SAY_KILL        = 2,
    SAY_SKELETON    = 3,
    SAY_GHOST       = 4,
    SAY_DEATH       = 5,
};

class Boss_BlackKnight : public CreatureScript
{
    public:

    Boss_BlackKnight() : CreatureScript("boss_black_knight") { }

    struct Boss_BlackKnightAI : public ScriptedAI
    {
        public:

            Boss_BlackKnightAI(Creature* pCreature) : ScriptedAI(pCreature), summons(pCreature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            void Reset() override;
            void JustReachedHome() override;

            void EnterCombat(Unit* /*attacker*/) override;
            void DamageTaken(Unit* /*pDoneBy*/, uint32& /*damage*/) override;
            void KilledUnit(Unit* /*victim*/) override;

            void JustSummoned(Creature* /*summon*/) override;
            void SummonedCreatureDies(Creature* /*summon*/, Unit* /*killer*/) override;
            void JustDied(Unit* /*killer*/) override;

            void UpdateAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);
            void explodeAliveGhouls();

            InstanceScript* pInstance;
            EventMap eventMap;
            SummonList summons;

            bool resurrectInProgress;
            bool bSummonArmy;
            bool bDeathArmyDone;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Boss_BlackKnightAI(pCreature);
    }
};


#endif // _BOSS_BLACK_KNIGHT_H
