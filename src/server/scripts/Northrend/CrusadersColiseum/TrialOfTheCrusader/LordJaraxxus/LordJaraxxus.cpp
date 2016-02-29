#include "LordJaraxxus.h"

enum
{
    //Talks
    SAY_JARAXXUS_01                         = -1649080,
    SAY_JARAXXUS_07                         = -1649086,
    SAY_JARAXXUS_08                         = -1649087,
    SAY_JARAXXUS_09                         = -1649088,
    SAY_JARAXXUS_10                         = -1649089,
    SAY_JARAXXUS_11                         = -1649090,
};

void LordJaraxxus::HandleEvent(uint32 EventId)
{
    Encounter::HandleEvent(EventId);

    if (!m_Instance || !m_Map)
        return;

    switch (EventId)
    {
        case JARAXXUS_INTRODUCE_FIZZLEBANG:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_JARAXXUS_01, tirion);
                m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 6000);
                m_EventMap.ScheduleEvent(JARAXXUS_START_FIZZLEBANG, 7000);
                m_EventMap.ScheduleEvent(OPEN_DOOR, 7000);
                m_Map->SummonCreatureOnMap(NPC_FIZZLEBANG, LordJaraxxusLoc[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);

                // Despawn icehowl
                if (Creature* icehowl = m_Map->GetCreature(m_Instance->GetData64(NPC_ICEHOWL)))
                    icehowl->DespawnOrUnsummon();
            }
            break;
        }
        case JARAXXUS_START_FIZZLEBANG:
        {
            if (Creature *fizzlebang = m_Map->GetCreature(m_Instance->GetData64(NPC_FIZZLEBANG)))
                fizzlebang->GetMotionMaster()->MovePoint(0, LordJaraxxusLoc[1]);
            break;
        }
        case JARAXXUS_FIZZLEBANG_KILLED:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_JARAXXUS_07, tirion);
                m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 6000);
                m_EventMap.ScheduleEvent(JARAXXUS_START, 7000);
            }
            break;
        }
        case JARAXXUS_START:
        {
            if (Creature* jaraxxus = m_Map->GetCreature(m_Instance->GetData64(NPC_JARAXXUS)))
            {
                jaraxxus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                jaraxxus->SetReactState(REACT_AGGRESSIVE);
                jaraxxus->SetInCombatWithZone();
                m_Instance->SetData(TYPE_ENCOUNTER_STATUS, IN_PROGRESS);
            }
            break;
        }
        case JARAXXUS_DONE:
        {
            if (Creature* tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_JARAXXUS_08, tirion);
                m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 14000);
                m_EventMap.ScheduleEvent(JARAXXUS_GARROSH_ANGRY, 14000);
            }
            break;
        }
        case JARAXXUS_GARROSH_ANGRY:
        {
            if (Creature* garrosh = m_Map->GetCreature(m_Instance->GetData64(NPC_GARROSH)))
            {
                garrosh->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_JARAXXUS_09, garrosh);
                m_EventMap.ScheduleEvent(GARROSH_STOP_TALK, 10000);
                m_EventMap.ScheduleEvent(JARAXXUS_VARIAN_REPLY, 10000);
            }
            break;
        }
        case JARAXXUS_VARIAN_REPLY:
        {
            if (Creature* varian = m_Map->GetCreature(m_Instance->GetData64(NPC_VARIAN)))
            {
                varian->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_JARAXXUS_10, varian);
                m_EventMap.ScheduleEvent(VARIAN_STOP_TALK, 7000);
                m_EventMap.ScheduleEvent(JARAXXUS_TIRION_MEND, 7000);
            }
            break;
        }
        case JARAXXUS_TIRION_MEND:
        {
            if (Creature* tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_JARAXXUS_11, tirion);
                m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 13000);
                m_EventMap.ScheduleEvent(BARRET_RETURN, 13000);
            }
            break;
        }
    }
}

void AddBossLordJaraxus();
void AddNpcFizzlebangTrialCrusader();

void AddEncounterLordJaraxxus()
{
    AddBossLordJaraxus();
    AddNpcFizzlebangTrialCrusader();
}
