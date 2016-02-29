#ifndef _ENCOUNTER_H
#define _ENCOUNTER_H

#include "TrialCrusader.h"

enum
{
    TIRION_STOP_TALK                        = 100,
    GARROSH_STOP_TALK                       = 101,
    VARIAN_STOP_TALK                        = 102,
    OPEN_DOOR                               = 103,
    BARRET_RETURN                           = 104,
};

class Encounter
{
    public:
        virtual void OnStart() { }
        virtual void OnFail() { }
        virtual void OnDone() { }
        virtual uint32 OnSpecial() { return SPECIAL; }

        virtual void Update(uint32 diff)
        {
            m_EventMap.Update(diff);

            while (uint32 EventId = m_EventMap.ExecuteEvent())
                HandleEvent(EventId);
        }

        virtual void OnCreatureCreate(Creature* creature) { }
        virtual void OnUnitDeath(Unit *unit) { }

        virtual bool CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* source, Unit const* target, uint32 miscvalue1) { return false; }

    protected:
        EventMap m_EventMap;
        InstanceScript *m_Instance;
        Map *m_Map;

        Encounter (InstanceScript *Instance) : m_Instance(Instance)
        {
            m_Map = m_Instance->instance;
        }

        virtual void HandleEvent(uint32 EventId)
        {
            if (!m_Map || !m_Instance)
                return;

            switch (EventId)
            {
                case TIRION_STOP_TALK:
                {
                    if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
                        tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    break;
                }
                case GARROSH_STOP_TALK:
                {
                    if (Creature *garrosh = m_Map->GetCreature(m_Instance->GetData64(NPC_GARROSH)))
                        garrosh->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    break;
                }
                case VARIAN_STOP_TALK:
                {
                    if (Creature *varian = m_Map->GetCreature(m_Instance->GetData64(NPC_VARIAN)))
                        varian->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    break;
                }
                case OPEN_DOOR:
                {
                    m_Instance->SetData(GO_MAIN_GATE_DOOR, OPEN);
                    break;
                }
                case BARRET_RETURN:
                {
                    if (Creature *barrett = m_Map->GetCreature(m_Instance->GetData64(NPC_BARRETT)))
                        barrett->GetAI()->DoAction(1);
                    break;
                }
            }
        }
};

#endif /* _ENCOUNTER_H */
