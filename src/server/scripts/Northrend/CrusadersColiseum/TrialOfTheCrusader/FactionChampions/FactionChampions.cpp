#include "FactionChampions.h"

enum
{
    // Script Texts
    SAY_CHAMPS_01                       = -1649091,
    SAY_CHAMPS_02a                      = -1649092,
    SAY_CHAMPS_02h                      = -1649093,
    SAY_CHAMPS_03                       = -1649094,
    SAY_CHAMPS_04a                      = -1649095,
    SAY_CHAMPS_04h                      = -1649096,
    SAY_CHAMPS_05a                      = -1649097,
    SAY_CHAMPS_05h                      = -1649098,
    SAY_CHAMPS_06                       = -1649099,
};

void FactionChampions::HandleEvent(uint32 EventId)
{
    Encounter::HandleEvent(EventId);

    if (!m_Instance || !m_Map)
        return;

    switch (EventId)
    {
        case CHAMPIONS_INTRO:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_01, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 8000);

            if (m_Instance->GetData(TYPE_TEAM) == ALLIANCE)
                m_EventMap.ScheduleEvent(CHAMPIONS_GARROSH_INTERRUPT, 8000);
            else
                m_EventMap.ScheduleEvent(CHAMPIONS_VARIAN_INTERRUPT, 8000);

            // Despawn Fizzle and Jaraxxus
            if (Creature *fizzlebang = m_Map->GetCreature(m_Instance->GetData64(NPC_FIZZLEBANG)))
                fizzlebang->DespawnOrUnsummon();
            if (Creature* jaraxxus = m_Map->GetCreature(m_Instance->GetData64(NPC_JARAXXUS)))
                jaraxxus->DespawnOrUnsummon();

            break;
        }
        case CHAMPIONS_GARROSH_INTERRUPT:
        {
            if (Creature *garrosh = m_Map->GetCreature(m_Instance->GetData64(NPC_GARROSH)))
            {
                garrosh->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_02h, garrosh);
            }

            m_EventMap.ScheduleEvent(GARROSH_STOP_TALK, 15000);
            m_EventMap.ScheduleEvent(CHAMPIONS_TIRION_GIVE_IN, 15000);
            break;
        }
        case CHAMPIONS_VARIAN_INTERRUPT:
        {
            if (Creature *varian = m_Map->GetCreature(m_Instance->GetData64(NPC_VARIAN)))
            {
                varian->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_02a, varian);
            }

            m_EventMap.ScheduleEvent(VARIAN_STOP_TALK, 15000);
            m_EventMap.ScheduleEvent(CHAMPIONS_TIRION_GIVE_IN, 15000);
            break;
        }
        case CHAMPIONS_TIRION_GIVE_IN:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_03, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 5000);

            if (m_Instance->GetData(TYPE_TEAM) == ALLIANCE)
                m_EventMap.ScheduleEvent(CHAMPIONS_GARROSH_CONTINUE, 5000);
            else
                m_EventMap.ScheduleEvent(CHAMPIONS_VARIAN_CONTINUE, 5000);

            break;
        }
        case CHAMPIONS_GARROSH_CONTINUE:
        {
            if (Creature *garrosh = m_Map->GetCreature(m_Instance->GetData64(NPC_GARROSH)))
            {
                garrosh->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_04h, garrosh);
            }

            m_EventMap.ScheduleEvent(GARROSH_STOP_TALK, 6000);
            m_EventMap.ScheduleEvent(CHAMPIONS_VARIAN_ANSWER, 6000);
            break;
        }
        case CHAMPIONS_VARIAN_CONTINUE:
        {
            if (Creature *varian = m_Map->GetCreature(m_Instance->GetData64(NPC_VARIAN)))
            {
                varian->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_04a, varian);
            }

            m_EventMap.ScheduleEvent(VARIAN_STOP_TALK, 5000);
            m_EventMap.ScheduleEvent(CHAMPIONS_GARROSH_ANSWER, 5000);
            break;
        }
        case CHAMPIONS_GARROSH_ANSWER:
        {
            if (Creature *garrosh = m_Map->GetCreature(m_Instance->GetData64(NPC_GARROSH)))
            {
                garrosh->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_05h, garrosh);
            }

            m_EventMap.ScheduleEvent(GARROSH_STOP_TALK, 3000);
            m_EventMap.ScheduleEvent(CHAMPIONS_SUMMON, 3000);
            break;
        }
        case CHAMPIONS_VARIAN_ANSWER:
        {
            if (Creature *varian = m_Map->GetCreature(m_Instance->GetData64(NPC_VARIAN)))
            {
                varian->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_05a, varian);
            }

            m_EventMap.ScheduleEvent(VARIAN_STOP_TALK, 3000);
            m_EventMap.ScheduleEvent(CHAMPIONS_SUMMON, 3000);
            break;
        }
        case CHAMPIONS_SUMMON:
        {
            if (Creature* pChampionController = m_Map->SummonCreature(NPC_CHAMPIONS_CONTROLLER, TrialCrusaderLoc[0]))
                pChampionController->AI()->SetData(0, m_Instance->GetData(TYPE_TEAM));
            m_EventMap.ScheduleEvent(CHAMPIONS_START, 3000);
            break;
        }
        case CHAMPIONS_START:
        {
            if (Creature* pChampionController = m_Map->GetCreature(m_Instance->GetData64(NPC_CHAMPIONS_CONTROLLER)))
                pChampionController->AI()->SetData(1, NOT_STARTED);
            break;
        }
        case CHAMPIONS_END:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_CHAMPS_06, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 5000);
            m_EventMap.ScheduleEvent(BARRET_RETURN, 5000);
            break;
        }
    }
}
