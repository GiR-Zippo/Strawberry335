#include "NorthrendBeasts.h"

enum
{
    // Script Texts
    SAY_BEASTS_01                           = -1649070,
    SAY_BEASTS_02                           = -1649071,
    SAY_BEASTS_03a                          = -1649072,
    SAY_BEASTS_03h                          = -1649073,
    SAY_BEASTS_04                           = -1649074,
    SAY_BEASTS_05                           = -1649075,
    SAY_BEASTS_06                           = -1649076,
    SAY_BEASTS_WIPE                         = -1649077,
};

void NorthrendBeasts::HandleEvent(uint32 EventId)
{
    Encounter::HandleEvent(EventId);

    if (!m_Instance || !m_Map)
        return;

    switch (EventId)
    {
        case BEASTS_INTRODUCE_INSTANCE:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_BEASTS_01, tirion);
            }

            m_EventMap.ScheduleEvent(BEASTS_INTRODUCE_GORMOK, 23000);
            break;
        }
        case BEASTS_INTRODUCE_GORMOK:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_BEASTS_02, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 7000);
            if (m_Instance->GetData(TYPE_TEAM) == ALLIANCE)
                m_EventMap.ScheduleEvent(BEASTS_VARIAN_MOCK, 9000);
            else
                m_EventMap.ScheduleEvent(BEASTS_GARROSH_MOCK, 9000);
            break;
        }
        case BEASTS_GARROSH_MOCK:
        {
            if (Creature *garrosh = m_Map->GetCreature(m_Instance->GetData64(NPC_GARROSH)))
            {
                garrosh->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_BEASTS_03h, garrosh);
            }

            m_EventMap.ScheduleEvent(GARROSH_STOP_TALK, 3000);
            m_EventMap.ScheduleEvent(OPEN_DOOR, 3000);
            m_EventMap.ScheduleEvent(BEASTS_SPAWN_GORMOK, 6000);
            break;
        }
        case BEASTS_VARIAN_MOCK:
        {
            if (Creature *varian = m_Map->GetCreature(m_Instance->GetData64(NPC_VARIAN)))
            {
                varian->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_BEASTS_03a, varian);
            }

            m_EventMap.ScheduleEvent(VARIAN_STOP_TALK, 3000);
            m_EventMap.ScheduleEvent(OPEN_DOOR, 3000);
            m_EventMap.ScheduleEvent(BEASTS_SPAWN_GORMOK, 6000);
            break;
        }
        case BEASTS_SPAWN_GORMOK:
        {
            if (Creature* gormok = m_Map->SummonCreatureOnMap(NPC_GORMOK, NorthrendBeastLoc[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
            {
                gormok->GetMotionMaster()->MovePoint(0, NorthrendBeastLoc[1]);
                gormok->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                gormok->SetReactState(REACT_PASSIVE);
            }

            if (m_Map->IsHeroic())
                m_EventMap.ScheduleEvent(BEASTS_INTRODUCE_JORMUNGAR, FORCED_SPAWN_TIME);

            if (m_Instance)
                m_Instance->SetData(TYPE_ENCOUNTER_STATUS, IN_PROGRESS);
            break;
        }
        case BEASTS_INTRODUCE_JORMUNGAR:
        {
            if (!m_JormungarStarted)
            {
                m_JormungarStarted = true;

                if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
                {
                    tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    DoScriptText(SAY_BEASTS_04, tirion);
                }

                m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 8000);
                m_EventMap.ScheduleEvent(OPEN_DOOR, 8000);
                m_EventMap.ScheduleEvent(BEASTS_SPAWN_JORMUNGAR, 11000);
            }
            break;
        }
        case BEASTS_SPAWN_JORMUNGAR:
        {
            Creature* dreadscale = m_Map->SummonCreatureOnMap(NPC_DREADSCALE, NorthrendBeastLoc[0], TEMPSUMMON_MANUAL_DESPAWN, DESPAWN_TIME);
            Creature* acidmaw = m_Map->SummonCreatureOnMap(NPC_ACIDMAW, NorthrendBeastLoc[4], TEMPSUMMON_MANUAL_DESPAWN, DESPAWN_TIME);
            if (!dreadscale || !acidmaw)
                break;

            dreadscale->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            dreadscale->SetReactState(REACT_PASSIVE);
            acidmaw->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            acidmaw->SetReactState(REACT_PASSIVE);

            acidmaw->GetAI()->SetGUID(dreadscale->GetGUID());
            dreadscale->GetAI()->SetGUID(acidmaw->GetGUID());

            acidmaw->SetDisplayId(MODEL_UNDERGROUND);
            dreadscale->GetMotionMaster()->MovePoint(0, NorthrendBeastLoc[2]);

            if (m_Map->IsHeroic())
                m_EventMap.ScheduleEvent(BEASTS_INTRODUCE_ICEHOWL, FORCED_SPAWN_TIME);

            // Despawn Gormok
            if (Creature* gormok = m_Map->GetCreature(m_Instance->GetData64(NPC_GORMOK)))
                gormok->DespawnOrUnsummon();

            break;
        }
        case BEASTS_INTRODUCE_ICEHOWL:
        {
            if (!m_IcehowlStarted)
            {
                m_IcehowlStarted = true;

                if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
                {
                    tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    DoScriptText(SAY_BEASTS_05, tirion);
                }

                m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 8000);
                m_EventMap.ScheduleEvent(OPEN_DOOR, 8000);
                m_EventMap.ScheduleEvent(BEASTS_SPAWN_ICEHOWL, 11000);
            }
            break;
        }
        case BEASTS_SPAWN_ICEHOWL:
        {
            if (Creature* icehowl = m_Map->SummonCreatureOnMap(NPC_ICEHOWL, NorthrendBeastLoc[0], TEMPSUMMON_DEAD_DESPAWN, DESPAWN_TIME))
            {
                icehowl->GetMotionMaster()->MovePoint(0, NorthrendBeastLoc[1]);
                icehowl->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                icehowl->SetReactState(REACT_PASSIVE);

                // Despawn Jormungar
                if (Creature* dreadscale = m_Map->GetCreature(m_Instance->GetData64(NPC_DREADSCALE)))
                    dreadscale->DespawnOrUnsummon();
                if (Creature* acidmaw = m_Map->GetCreature(m_Instance->GetData64(NPC_ACIDMAW)))
                    acidmaw->DespawnOrUnsummon();
            }
            break;
        }
        case BEASTS_DONE:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_BEASTS_06, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 5000);
            m_EventMap.ScheduleEvent(BARRET_RETURN, 5000);
            break;
        }
        case BEASTS_FAIL:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_BEASTS_WIPE, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 5000);
            m_EventMap.ScheduleEvent(BARRET_RETURN, 5000);
            break;
        }
    }
}

void AddEncounterGormok();
void AddEncounterJormungar();
void AddEncounterIcehowl();

void AddEncounterNorthrendBeasts()
{
    AddEncounterGormok();
    AddEncounterJormungar();
    AddEncounterIcehowl();
}
