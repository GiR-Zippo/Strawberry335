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

enum Spells
{
    SPELL_MARK              = 67574,

    SPELL_SPIKE_SPEED1      = 65920,
    SPELL_SPIKE_TRAIL       = 65921,
    SPELL_SPIKE_SPEED2      = 65922,
    SPELL_SPIKE_SPEED3      = 65923,
    SPELL_SPIKE_FAIL        = 66181,
    SPELL_SPIKE_TELE        = 66170,
};

class AnubarakSpike : public CreatureScript
{
    public:
        AnubarakSpike() : CreatureScript("mob_anubarak_spike") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new AnubarakSpikeAI(creature);
        };

        class AnubarakSpikeAI : public ScriptedAI
        {
            public:
                AnubarakSpikeAI(Creature* creature) : ScriptedAI(creature), targetGUID(NULL),
                    instance(creature->GetInstanceScript())
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    // me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED, true);
                    // me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetInCombatWithZone();
                }

                bool CanAIAttack(Unit const* victim) const
                {
                    return victim->GetTypeId() == TYPEID_PLAYER;
                }

                void ReselectTarget()
                {
                    me->CastSpell(me, SPELL_SPIKE_TELE, true);

                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true);

                    if (!target)
                    {
                        EnterEvadeMode();
                        return;
                    }
                    targetGUID = target->GetGUID();
                    me->TauntApply(target);
                    DoCast(target, SPELL_MARK, true);
                    
                    me->SetSpeed(MOVE_RUN, 0.5f);
                    speed = 0;
                    speedTimer = 1000;
                }

                void DamageTaken(Unit* /*who*/, uint32& damage)
                {
                    damage = 0;
                }

                void UpdateAI(const uint32 diff)
                {
                    if (Aura* aura = me->GetAura(SPELL_PERMAFROST_HELPER))
                    {
                        aura->Remove();
                        me->DespawnOrUnsummon();
                        return;
                    }

                    Unit* target = Unit::GetPlayer(*me, targetGUID);
                    if (!target || target->isDead() || !target->HasAura(SPELL_MARK))
                        ReselectTarget();

                    if (speed == 3)
                        return;

                    if (speedTimer <= diff)
                    {
                        switch (speed)
                        {
                        case 0:
                            DoCast(me, SPELL_SPIKE_SPEED1);
                            DoCast(me, SPELL_SPIKE_TRAIL);
                            break;
                        case 1:
                            DoCast(me, SPELL_SPIKE_SPEED2);
                            break;
                        case 2:
                            DoCast(me, SPELL_SPIKE_SPEED3);
                            break;
                        }
                        speed++;
                        speedTimer = 7*IN_MILLISECONDS;
                    } else speedTimer -= diff;

                }

            private:
                InstanceScript* instance;
                
                uint64 targetGUID;
                uint32 speedTimer;
                uint8  speed;
        };
};

void AddAnubarakSpike()
{
    new AnubarakSpike();
}