/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.09.2014
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

#ifndef _MISC_HALLSOFREFLECTION_H
#define _MISC_HALLSOFREFLECTION_H

#include "../HallsOfReflection.h"


namespace MISC_HALLSOFREFLECTION
{
    enum Says
    {
        SAY_UTHER_1             = 1,
        SAY_UTHER_2             = 2,
        SAY_UTHER_3             = 3,

        EMOTE_QUELDELAR         = -2000005,
        EMOTE_QUELDELAR_2       = -2000008
    };

    enum Spells
    {
        // Quel'Delar Event
        SPELL_QUELDELAR_AURA    = 70013,
        SPELL_UNKNOW_1          = 70700,
        SPELL_UNKNOW_2          = 69966
    };

    enum Events
    {
        EVENT_QUELDELAR         = 1
    };

    enum Npcs
    {
        NPC_UTHER_LIGHTBRINGER  = 26528
    };

    const Position UtherSpawnPos = { 5297.657715f, 1982.822388f, 707.695068f, 3.7363f };
}

class At_WavesRestarterHor : public AreaTriggerScript
{
    public:

    At_WavesRestarterHor() : AreaTriggerScript("at_hor_waves_restarter") {}

    bool OnTrigger(Player* pPlayer, AreaTriggerEntry const* /*trigger*/) override;
};


class Npc_Queldelar : public CreatureScript
{
    public:

    Npc_Queldelar() : CreatureScript("npc_queldelar") { }

    struct Npc_QueldelarAI  : public ScriptedAI
    {
        Npc_QueldelarAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void MoveInLineOfSight(Unit* /*who*/) override;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new Npc_QueldelarAI(pCreature);
    }
};


class NpcFrostmourneHor : public CreatureScript
{
    public:

    NpcFrostmourneHor() : CreatureScript("npc_frostmourne_hall_of_reflection") { }

    struct NpcFrostmourneHorAI : public Scripted_NoMovementAI
    {
        public:

            NpcFrostmourneHorAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) {}

            void Reset() override;
            void MoveInLineOfSight(Unit* /*who*/) override;

            void UpdateAI(const uint32 diff) override;

        private:

          EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new NpcFrostmourneHorAI(pCreature);
    }
};


#endif // _MISC_HALLSOFREFLECTION_H
