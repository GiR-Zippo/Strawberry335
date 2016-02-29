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

#include "Npc_TribunalOfTheAges.h"
typedef npc_spell_shadow_orb::npc_spell_shadow_orbAI ShadowOrbAI;
typedef npc_brann_adds::npc_brann_addsAI             BrannAddsAI;


void ShadowOrbAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == 1)
        SendNewOrbToPlayer();

    if (id == 2)
    {
        me->CastSpell(me, DUNGEON_MODE(SPELL_DARK_MATTER, H_SPELL_DARK_MATTER), true);
        me->DespawnOrUnsummon();
    }
};

void ShadowOrbAI::SendNewOrbToPlayer()
{
    if (Creature* summon = me->SummonCreature(CREATURE_SHADOW_ORB, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f))
    {
        std::list<Player*> PlList = me->GetNearestPlayersList(100.0f);

        if (PlList.empty())
        {
            summon->DespawnOrUnsummon();
            return;
        }

        std::list<Player*>::const_iterator itr = PlList.begin();

        if (PlList.size() > 1)
            advance(itr, rand()%PlList.size());

        if (Player* target = (*itr))
        {
            Position TargetPosition;
            TargetPosition.Relocate(target);
            summon->GetMotionMaster()->MovePoint(2, TargetPosition);
        }
        else
            summon->DespawnOrUnsummon();
    }

    me->DespawnOrUnsummon();
};

void BrannAddsAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == 1)
    {
        if (me->isInCombat())
            return;

        if (instance = me->GetInstanceScript())
        {
            Creature* pCreature = NULL;
            if (pCreature = Creature::GetCreature((*me), instance->GetData64(DATA_ESCORT_BRANN)))
            {
                me->AddThreat(pCreature, 0.0f);
                me->AI()->AttackStart(pCreature);
            }
        }
    }
    if (id == 2)
        DoAfterMovement();
};
