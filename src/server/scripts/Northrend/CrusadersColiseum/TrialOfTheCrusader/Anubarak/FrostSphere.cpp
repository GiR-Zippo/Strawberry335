/*
 * Copyright (C) 2012 Zirkon
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

#include "Anubarak.h"

enum
{
    SPELL_PERMAFROST        = 66193,
    SPELL_PERMAFROST_VISUAL = 65882,
};

enum MovementPoints
{
    POINT_FALL_GROUND           = 1
};

class FrostSphere : public CreatureScript
{
public:
    class FrostSphereAI : public ScriptedAI
    {
    public:
        FrostSphereAI(Creature* creature) : ScriptedAI(creature){}

        void Reset()
        {
            falling = false;
            me->SetReactState(REACT_PASSIVE);
            me->SetDisableGravity(true);
            me->SetHover(true);
            me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
            me->SetSpeed(MOVE_RUN, 0.5f, false);
            me->GetMotionMaster()->MoveRandom(5.0f);
            DoCast(SPELL_FROST_SPHERE);
        }

        void DamageTaken(Unit* /*who*/, uint32& damage)
        {
            if (me->GetHealth() > damage)
                return;

            damage = 0;
            if (falling)
                return;

            falling = true;
            me->GetMotionMaster()->MoveIdle();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->HandleEmoteCommand(EMOTE_ONESHOT_FLYDEATH);
            me->GetMotionMaster()->MoveFall(POINT_FALL_GROUND);
            if (Creature* anubarak = Unit::GetCreature((*me), me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(NPC_ANUBARAK) : 0))
                if (anubarak->GetAI())
                    ((ScriptedAI *)anubarak->GetAI())->SummonedCreatureDespawn(me);
        }

        void MovementInform(uint32 type, uint32 pointId)
        {
            if (type != EFFECT_MOTION_TYPE || pointId != POINT_FALL_GROUND)
                return;

            me->RemoveAurasDueToSpell(SPELL_FROST_SPHERE);
            me->SetDisplayId(me->GetCreatureTemplate()->Modelid1);
            DoCast(SPELL_PERMAFROST_VISUAL);
            DoCast(SPELL_PERMAFROST);
            me->SetObjectScale(2.0f);
        }

        private:
            bool falling;
    };    
    
    FrostSphere() : CreatureScript("mob_frost_sphere") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new FrostSphereAI(creature);
    };
};

void AddFrostSphere()
{
    new FrostSphere();
}