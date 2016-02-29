//Angelolaf

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "PoolMgr.h"
#include "Group.h"
#include "IcecrownCitadel.h"


enum eAngelolaf
{
    //Angelolaf
    EVENT_WATER_SPIT = 1,
    EVENT_WATER_WAL = 2,
    EVENT_WATER_TOMB = 3,
    EVENT_WATER_BUBBLE = 4,
    EVENT_AQUA_JET = 5,
    EVENT_NASSES_GRAB = 6,

    //Murloc
    EVENT_VER_MURLOC = 7,
    EVENT_NETZ = 8,
    EVENT_GEYSIR = 9,
    EVENT_WASSER_BLASE = 10,
    EVENT_STATISCH = 11,

    //Angelolaf

    SPELL_WATER_SPIT = 35008,
    SPELL_WATER_WAL = 57652,
    SPELL_WATER_TOMB = 59261,
    SPELL_WATER_BUBBLE = 54399,
    SPELL_AQUA_JET = 21790,
    SPELL_NASSES_GRAB = 38023,

    //Murloc
    SPELL_VER_MURLOC = 49935,
    SPELL_NETZ = 59260,
    SPELL_GEYSIR = 37478,
    SPELL_STATISCH = 31715,
};

static const Position MurlocR = { -578.7066f, 2154.167f, 51.01529f, 1.692969f };
static const Position MurlocL = { -578.6996f, 2269.856f, 51.01529f, 4.590216f };

class boss_angelolaf : public CreatureScript
{
    public:
        boss_angelolaf() : CreatureScript("boss_angelolaf") { }

        struct boss_angelolafAI : public ScriptedAI
        {
            boss_angelolafAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                events.Reset();
                enraged = false;

                events.ScheduleEvent(EVENT_WATER_SPIT, urand(4000, 4500));
                events.ScheduleEvent(EVENT_WATER_WAL, 3000);
                events.ScheduleEvent(EVENT_WATER_TOMB, urand(5000, 8000));
                events.ScheduleEvent(EVENT_WATER_BUBBLE, urand(1800, 2500));
                events.ScheduleEvent(EVENT_AQUA_JET, urand(1000, 1100));
                events.ScheduleEvent(EVENT_NASSES_GRAB, urand(10000, 15000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (!enraged && HealthBelowPct(30))
                {
                    enraged = true; //Spawnpunkte in ICC vor Lady aussehen:27649
                    DoSummon(43306, MurlocR, 0, TEMPSUMMON_DEAD_DESPAWN); //MurlocR
                    DoSummon(43301, MurlocL, 0, TEMPSUMMON_DEAD_DESPAWN); //MurlocL
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WATER_SPIT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_WATER_SPIT);
                            events.ScheduleEvent(EVENT_WATER_SPIT, urand(4000, 4500));
                            break;
                        case EVENT_WATER_WAL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f, true))
                                DoCast(target, SPELL_WATER_WAL);
                            events.ScheduleEvent(EVENT_WATER_WAL, 3000);
                            break;
                        case EVENT_WATER_TOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_WATER_TOMB);
                            events.ScheduleEvent(EVENT_WATER_TOMB, urand(5000, 8000));
                            break;
                        case EVENT_WATER_BUBBLE:
                            events.ScheduleEvent(EVENT_WATER_BUBBLE, urand(1800, 2500));
                            break;
                        case EVENT_AQUA_JET:
                            DoCastVictim(SPELL_AQUA_JET);
                            events.ScheduleEvent(EVENT_AQUA_JET, urand(1000, 1100));
                            break;
                        case EVENT_NASSES_GRAB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_NASSES_GRAB);
                            events.ScheduleEvent(EVENT_NASSES_GRAB, urand(10000, 15000));
                    }
                }

                DoMeleeAttackIfReady();
            }

            EventMap events;
            uint32 enraged;
        };


        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_angelolafAI(creature);
        }
};

class npc_MurlocR : public CreatureScript
{
    public:
        npc_MurlocR() : CreatureScript("npc_MurlocR") { }

        struct npc_MurlocRAI : public ScriptedAI
        {
            npc_MurlocRAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(SPELL_VER_MURLOC, urand(1100, 1000));
                events.ScheduleEvent(EVENT_NETZ, urand(950, 1100));
                events.ScheduleEvent(EVENT_GEYSIR, urand(2000, 2700));
                events.ScheduleEvent(EVENT_STATISCH, urand(10000, 10000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_VER_MURLOC:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_VER_MURLOC);
                            events.ScheduleEvent(SPELL_VER_MURLOC, urand(1100, 1000));
                            break;
                        case EVENT_NETZ:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_NETZ);
                            events.ScheduleEvent(EVENT_NETZ, urand(950, 1100));
                            break;
                        case EVENT_GEYSIR:
                            DoCast(me, SPELL_GEYSIR);
                            events.ScheduleEvent(EVENT_GEYSIR, urand(2000, 2700));
                            break;
                        case EVENT_STATISCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_STATISCH);
                            events.ScheduleEvent(EVENT_STATISCH, urand(10000, 10000));
                    }
                }

                DoMeleeAttackIfReady();
            }

        protected:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_MurlocRAI(creature);
        }
};

class npc_MurlocL : public CreatureScript
{
    public:
        npc_MurlocL() : CreatureScript("npc_MurlocL") { }

        struct npc_MurlocLAI : public ScriptedAI
        {
            npc_MurlocLAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(SPELL_VER_MURLOC, urand(1100, 1000));
                events.ScheduleEvent(EVENT_NETZ, urand(950, 1100));
                events.ScheduleEvent(EVENT_GEYSIR, urand(2000, 2700));
                events.ScheduleEvent(EVENT_STATISCH, urand(10000, 10000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_VER_MURLOC:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_VER_MURLOC);
                            events.ScheduleEvent(SPELL_VER_MURLOC, urand(1100, 1000));
                            break;
                        case EVENT_NETZ:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_NETZ);
                            events.ScheduleEvent(EVENT_NETZ, urand(950, 1100));
                            break;
                        case EVENT_GEYSIR:
                            DoCast(me, SPELL_GEYSIR);
                            events.ScheduleEvent(EVENT_GEYSIR, urand(20000, 27000));
                            break;
                        case EVENT_STATISCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_STATISCH);
                            events.ScheduleEvent(EVENT_STATISCH, urand(10000, 10000));
                    }
                }

                DoMeleeAttackIfReady();
            }

        protected:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_MurlocLAI(creature);
        }
};

void AddSC_boss_angelolaf()
{
    new boss_angelolaf();
    new npc_MurlocR();
    new npc_MurlocL();
}
