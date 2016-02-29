/*
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

#include "ScriptPCH.h"
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CORRUPTING_BLIGHT                     = 60588,
    SPELL_VOID_STRIKE                           = 60590
};

enum Yells
{
    SAY_AGGRO                                   = -1595045,
    SAY_FAIL                                    = -1595046,
    SAY_DEATH                                   = -1595047
};

enum Events
{
    EVENT_CORRUPTING_BLIGHT                     = 1,
    EVENT_VOID_STRIKE                           = 2,
};

enum Points
{
    POINT_PORTAL                                = 1,
};

class boss_infinite_corruptor : public CreatureScript
{
public:
    boss_infinite_corruptor() : CreatureScript("boss_infinite_corruptor") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_infinite_corruptorAI(creature);
    }

    struct boss_infinite_corruptorAI : public ScriptedAI
    {
        boss_infinite_corruptorAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_INFINITE_ADD_PORTAL:
                    portal = me->SummonCreature(NPC_TIME_RIFT, 2335.35f, 1261.31f, 132.94f, 2.1307f);
                    break;
                case ACTION_INFINITE_EVADE:
                    me->GetMotionMaster()->MovePoint(POINT_PORTAL, portal->GetPositionX(), portal->GetPositionY(), portal->GetPositionZ());
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            switch (id)
            {
                case POINT_PORTAL:
                    instance->SetData(DATA_INFINITE_EVENT, FAIL);
                    portal->DespawnOrUnsummon(5);
                    DoScriptText(SAY_FAIL, me);
                    Unit::GetCreature(*me, instance->GetData64(DATA_INFINITE))->DespawnOrUnsummon(2);
                    break;
            }
        }

        void Reset()
        {
            if (instance)
                instance->SetData(DATA_INFINITE_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
                instance->SetData(DATA_INFINITE_EVENT, IN_PROGRESS);
            DoScriptText(SAY_AGGRO, me);
            events.ScheduleEvent(EVENT_CORRUPTING_BLIGHT, 2000);
            events.ScheduleEvent(EVENT_VOID_STRIKE, 5000);
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            while (uint32 event = events.ExecuteEvent())
            {
                switch (event)
                {
                    case EVENT_CORRUPTING_BLIGHT:
                        target = me->AI()->SelectTarget(SELECT_TARGET_RANDOM, 1, 150.0f, true);
                        if (!target)
                            return;
                        
                        me->CastSpell(target, SPELL_CORRUPTING_BLIGHT);
                        events.ScheduleEvent(EVENT_CORRUPTING_BLIGHT, 10000);
                        break;
                        
                    case EVENT_VOID_STRIKE:
                        me->CastSpell(me->getVictim(), SPELL_VOID_STRIKE);
                        events.ScheduleEvent(EVENT_VOID_STRIKE, 5000);
                        break;
                }
            }
            //Return since we have no target
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (instance)
                instance->SetData(DATA_INFINITE_EVENT, DONE);
            DoScriptText(SAY_DEATH, me);
        }
    private:
        InstanceScript* instance;
        Creature* portal;
        EventMap events;
        Unit* target;
    };

};

void AddSC_boss_infinite_corruptor()
{
    new boss_infinite_corruptor();
}
