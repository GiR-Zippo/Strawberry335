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
    SPELL_SPIDER_FRENZY     = 66128,
    SPELL_EXPOSE_WEAKNESS   = 67720, //Passive - Triggered
    SPELL_SHADOW_STRIKE     = 66134,
};

enum Events
{
    EVENT_SUBMERGE              = 1,
    EVENT_EMERGE                = 2,
    EVENT_SHADOW_STRIKE         = 3,
};

enum Times
{
    TIME_SUBMERGE_1             = 30000,
    TIME_SUBMERGE_2             = 20000,
    TIME_SHADOW_STRIKE          = 100,
    TIME_EMERGE                 = 20000,
};

class NerubianBurrower : public CreatureScript
{
public:
    class NerubianBurrowerAI : public ScriptedAI
    {
    public:
        NerubianBurrowerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            emerged = true;
        }

        void Reset()
        {
            emerged = true;
            me->SetCorpseDelay(0);
            events.Reset();
            events.ScheduleEvent(EVENT_SUBMERGE, TIME_SUBMERGE_1);
            DoCast(me, SPELL_EXPOSE_WEAKNESS);
            DoCast(SPELL_SPIDER_FRENZY);
            me->SetInCombatWithZone();
            if (!me->isInCombat())
                me->DisappearAndDie();
        }

        void DoAction(const int32 actionId)
        {
            if (actionId == ACTION_SHADOW_STRIKE)
                events.ScheduleEvent(EVENT_SHADOW_STRIKE, TIME_SHADOW_STRIKE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 Event = events.ExecuteEvent())
            {
                switch (Event)
                {
                case EVENT_SUBMERGE:
                    if (!me->HasAura(SPELL_PERMAFROST_HELPER))
                    {
                        DoCast(me, SPELL_SUBMERGE_EFFECT);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        events.ScheduleEvent(EVENT_EMERGE, TIME_EMERGE);
                        emerged = false;
                    }
                    else
                        events.ScheduleEvent(EVENT_SUBMERGE, TIME_SUBMERGE_2);
                    break;
                case EVENT_EMERGE:
                    me->RemoveAurasDueToSpell(SPELL_SUBMERGE_EFFECT);
                    DoCast(me, SPELL_EMERGE_EFFECT);
                    
                    if (IsHeroic())
                    {
                        // Position pos;
                        // me->GetPosition(&pos);
                        // if (Creature* anubarak = instance->instance->GetCreature(instance->GetData64(NPC_ANUBARAK)))
                        //    anubarak->SummonCreature(NPC_BURROWER, pos);
                        me->SetFullHealth();
                    }
                    emerged = true;
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE| UNIT_FLAG_NOT_SELECTABLE);
                    events.ScheduleEvent(EVENT_SUBMERGE, TIME_SUBMERGE_2);
                    break;
                case EVENT_SHADOW_STRIKE:
                    if (emerged)
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            DoCast(target, SPELL_SHADOW_STRIKE, false);
                    break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }
    private:
        InstanceScript* instance;
        EventMap events;
        bool emerged;
    };

    NerubianBurrower() : CreatureScript("mob_nerubian_burrower") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new NerubianBurrowerAI(creature);
    };
};

void AddNerubianBurrower()
{
    new NerubianBurrower();
}
