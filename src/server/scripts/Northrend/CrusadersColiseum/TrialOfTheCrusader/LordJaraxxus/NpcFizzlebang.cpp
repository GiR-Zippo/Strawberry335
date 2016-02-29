#include "LordJaraxxus.h"

enum
{
    //Spells
    SPELL_WILFRED_PORTAL                    = 68424,

    // Events
    START_CASTING                           = 1,
    SUMMON_PORTAL                           = 2,
    SUMMON_JARAXXUS                         = 3,
    UNSUMMON_PORTALS                        = 4,
    JARAXXUS_MOCK                           = 5,
    FIZZLEBANG_SURPRISED                    = 6,

    // Script Texts
    SAY_JARAXXUS_02                         = -1649081,
    SAY_JARAXXUS_03                         = -1649082,
    SAY_JARAXXUS_04                         = -1649083,
    SAY_JARAXXUS_05                         = -1649030,
    SAY_JARAXXUS_06                         = -1649084,
};

class NpcFizzlebangTrialCrusader : public CreatureScript
{
    public:
        NpcFizzlebangTrialCrusader() : CreatureScript("npc_fizzlebang_toc") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new NpcFizzlebangTrialCrusaderAI(creature);
        }

        class NpcFizzlebangTrialCrusaderAI : public ScriptedAI
        {
            public:
                NpcFizzlebangTrialCrusaderAI(Creature* creature) : ScriptedAI(creature),
                    m_PortalGUID(0),
                    m_CircleGUID(0)
                {
                    m_Instance = me->GetInstanceScript();
                    m_Map = m_Instance->instance;
                }

                void MovementInform(uint32 Type, uint32 Id)
                {
                    if (Type != POINT_MOTION_TYPE)
                        return;

                    if (Id == 0 && m_Instance)
                    {
                        m_Instance->SetData(GO_MAIN_GATE_DOOR, CLOSE);
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                        DoScriptText(SAY_JARAXXUS_02, me);
                        m_EventMap.ScheduleEvent(START_CASTING, 12000);
                    }
                }

                void UpdateAI(uint32 const diff)
                {
                    m_EventMap.Update(diff);

                    while (uint32 EventId = m_EventMap.ExecuteEvent())
                        HandleEvent(EventId);
                }

                void HandleEvent(uint32 EventId)
                {
                    if (!m_Instance || !m_Map)
                        return;
                
                    switch (EventId)
                    {
                        case START_CASTING:
                        {
                            me->HandleEmoteCommand(EMOTE_ONESHOT_SPELL_CAST_OMNI);
                            DoScriptText(SAY_JARAXXUS_03, me);

                            if (Unit* circle = me->SummonCreature(NPC_TRIGGER, TrialCrusaderLoc[0], TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                circle->SetObjectScale(2.0f);
                                circle->SetDisplayId(22862);
                                m_CircleGUID = circle->GetGUID();
                            }

                            m_EventMap.ScheduleEvent(SUMMON_PORTAL, 4000);
                            break;
                        }
                        case SUMMON_PORTAL:
                        {
                            me->HandleEmoteCommand(EMOTE_ONESHOT_SPELL_CAST_OMNI);
                            if (Creature* pPortal = me->SummonCreature(NPC_WILFRED_PORTAL, TrialCrusaderLoc[0], TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                pPortal->SetReactState(REACT_PASSIVE);
                                pPortal->SetObjectScale(2.0f);
                                pPortal->CastSpell(pPortal, SPELL_WILFRED_PORTAL, false);
                                m_PortalGUID = pPortal->GetGUID();
                            }

                            m_EventMap.ScheduleEvent(SUMMON_JARAXXUS, 5000);
                            break;
                        }
                        case SUMMON_JARAXXUS:
                        {
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                            DoScriptText(SAY_JARAXXUS_04, me);
                            if (Creature* jaraxxus = me->SummonCreature(NPC_JARAXXUS, TrialCrusaderLoc[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
                            {
                                jaraxxus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                jaraxxus->SetReactState(REACT_PASSIVE);
                                jaraxxus->GetMotionMaster()->MovePoint(0, LordJaraxxusLoc[2]);
                            }
                            m_EventMap.ScheduleEvent(UNSUMMON_PORTALS, 4000);
                            m_EventMap.ScheduleEvent(JARAXXUS_MOCK, 12000);
                            break;
                        }
                        case UNSUMMON_PORTALS:
                        {
                            if (Creature* Circle = m_Map->GetCreature(m_CircleGUID))
                                Circle->DespawnOrUnsummon();
                            if (Creature* Portal = m_Map->GetCreature(m_PortalGUID))
                                Portal->DespawnOrUnsummon();
                            if (Creature* jaraxxus = m_Map->GetCreature(m_Instance->GetData64(NPC_JARAXXUS)))
                            {
                                jaraxxus->SetTarget(me->GetGUID());
                                jaraxxus->SetFacingToObject(me);
                            }
                            break;
                        }
                        case JARAXXUS_MOCK:
                        {
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            if (Creature* jaraxxus = m_Map->GetCreature(m_Instance->GetData64(NPC_JARAXXUS)))
                            {
                                jaraxxus->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                                DoScriptText(SAY_JARAXXUS_05, jaraxxus);
                            }
                            m_EventMap.ScheduleEvent(FIZZLEBANG_SURPRISED, 6000);
                            break;
                        }
                        case FIZZLEBANG_SURPRISED:
                        {
                            DoScriptText(SAY_JARAXXUS_06, me);
                            if (Creature* jaraxxus = m_Map->GetCreature(m_Instance->GetData64(NPC_JARAXXUS)))
                            {
                                jaraxxus->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                                jaraxxus->CastSpell(me, 67888, false);
                            }
                            break;
                        }
                    }
                }
            private:
                EventMap m_EventMap;
                Map *m_Map;
                InstanceScript *m_Instance;

                uint64 m_PortalGUID;
                uint64 m_CircleGUID;
        };
};

void AddNpcFizzlebangTrialCrusader()
{
    new NpcFizzlebangTrialCrusader();
}
