/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 25.09.2014
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

#ifndef _EVENT_THELICHKINGHOR_H
#define _EVENT_THELICHKINGHOR_H

#include "../HallsOfReflection.h"
#include "Encounter_TheLichKingHor.h"
#include "ScriptedEscortAI.h"


namespace EVENT_THELICHKINGHOR
{
    enum Says
    {
        SAY_LICH_KING_AGGRO_A               = -1594477,
        SAY_LICH_KING_AGGRO_H               = -1594478,
        SAY_JAINA_AGGRO                     = -1594479,
        SAY_SYLVANA_AGGRO                   = -1594480,

        SAY_JAINA_WALL_01                   = -1594487,
        SAY_SYLVANA_WALL_01                 = -1594488,
        SAY_JAINA_WALL_02                   = -1594489,
        SAY_SYLVANA_WALL_02                 = -1594490,
        SAY_LICH_KING_WALL_02               = -1594491,
        SAY_LICH_KING_WALL_03               = -1594492,
        SAY_LICH_KING_WALL_04               = -1594493,
        SAY_JAINA_WALL_03                   = -1594494,
        SAY_JAINA_WALL_04                   = -1594495,
        SAY_SYLVANA_WALL_03                 = -1594496,
        SAY_SYLVANA_WALL_04                 = -1594497,
        SAY_JAINA_ESCAPE_01                 = -1594498,
        SAY_JAINA_ESCAPE_02                 = -1594499,
        SAY_SYLVANA_ESCAPE_01               = -1594500,
        SAY_SYLVANA_ESCAPE_02               = -1594501,
        SAY_JAINA_TRAP                      = -1594502,
        SAY_SYLVANA_TRAP                    = -1594503,
        SAY_MATHEAS_JAINA                   = -1594505,
        SAY_JAINA_FIRE                      = -1594526,
        SAY_SYLVANA_FIRE                    = -1594522,
        SAY_SYLVANA_FINAL_1                 = -1594523,
        SAY_SYLVANA_FINAL_2                 = -1594521,
        SAY_JAINA_FINAL_1                   = -1594527,
        SAY_JAINA_FINAL_2                   = -1594524,
        SAY_JAINA_FINAL_3                   = -1594525,
        SAY_LICH_KING_END_01                = -1594506,
        SAY_LICH_KING_END_02                = -1594507,
        SAY_LICH_KING_END_03                = -1594508,
    };

    enum Spells
    {
        SPELL_DESTROY_ICE_WALL_01           = 69784, // Jaina
        SPELL_DESTROY_ICE_WALL_03           = 70225, // Sylvana
        SPELL_SUMMON_ICE_WALL               = 69767
    };

    enum Quests
    {
        QUEST_DELIVRANCE_FROM_THE_PIT_A2    = 24710,
        QUEST_DELIVRANCE_FROM_THE_PIT_H2    = 24712,
        QUEST_WRATH_OF_THE_LICH_KING_A2     = 24500,
        QUEST_WRATH_OF_THE_LICH_KING_H2     = 24802,
    };

    enum Events
    {
        EVENT_SPELL_SUMMON_ICE_WALL         = 1,
        EVENT_SPELL_DESTROY_ICE_WALL        = 2
    };

    enum Phases
    {
        PHASE_WALL                          = 1
    };

    enum Misc
    {
        MISC_HEALTH_PCT_JAINA_OR_SYLVANAS   = 5,
        MISC_HEALTH_PCT_LICH_KING           = 75
    };

    const float IceWallScale[4] = { 2.0f, 1.75f, 2.4f, 1.5f };

} // EVENT_THELICHKINGHOR


class Npc_JainaOrSylvanasHor_SecondPart : public CreatureScript
{
    public:

    Npc_JainaOrSylvanasHor_SecondPart() : CreatureScript("npc_jaina_and_sylvana_hor_part2") { }

    bool OnGossipHello(Player* /*player*/, Creature* /*creature*/) override;
    bool OnGossipSelect(Player* /*player*/, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override;

    struct Npc_JainaOrSylvanasHorAI_SecondPart : public npc_escortAI
    {
        public:

            Npc_JainaOrSylvanasHorAI_SecondPart(Creature* creature) : npc_escortAI(creature)
            {
                GET_INSTANCESCRIPT_SAFE(pInstance);
            }

            bool CanAIAttack(const Unit* /*victim*/) const override;

            void DoAction(const int32 action) override;
            void Reset() override;

            void WaypointReached(uint32 /*point*/) override;
            void MoveInLineOfSight(Unit* /*who*/) override;

            void MovementInform(uint32 /*type*/, uint32 /*point*/) override;
            void JustSummoned(GameObject* /*summoned*/) override;

            void DamageTaken(Unit* /*pDoneBy*/, uint32& /*damage*/) override;
            void EnterEvadeMode() override;

            void JustDied(Unit* /*killer*/) override;
            void UpdateEscortAI(const uint32 diff) override;

        private:

            void executeEvent(uint32 eventID);
            void attackWall();

            InstanceScript* pInstance;
            EventMap eventMap;

            uint64 iceWallTargetGUID;
            uint64 captainGUID;
            uint32 chestID;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Npc_JainaOrSylvanasHorAI_SecondPart(creature);
    }
};


#endif // _EVENT_THELICHKINGHOR_H
