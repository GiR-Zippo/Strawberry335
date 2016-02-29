#include "TwinValkyr.h"
#include "../InstanceTrialCrusader.h"

enum
{
    // Script Texts
    SAY_VALKYR_01                   = -1649100,
    SAY_VALKYR_02                   = -1649101,
    SAY_VALKYR_03a                  = -1649102,
    SAY_VALKYR_03h                  = -1649103,

    SAY_LICHKING_01                 = -1649104,
    SAY_LICHKING_02                 = -1649105,
    SAY_LICHKING_03                 = -1649106,
    SAY_LICHKING_04                 = -1649107,
    SAY_LICHKING_05                 = -1649108,

    // Lichking misc
    SPELL_DESTROY_FLOOR_KNOCKUP     = 68193,
    DISPLAYID_DESTROYED_FLOOR       = 9060,
};

const Position LichKingLoc[]=
{
    {563.822f,  170.659f,   394.479f,   4.730f},          // 0 - Lich king start
    {563.547f,  141.613f,   393.908f,   4.748f},          // 1 - Lich king end
};

void TwinValkyr::HandleEvent(uint32 EventId)
{
    Encounter::HandleEvent(EventId);

    if (!m_Instance || !m_Map)
        return;

    switch (EventId)
    {
        case VALKYR_INTRODUCE:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_VALKYR_01, tirion);
            }

            m_EventMap.ScheduleEvent(VALKYR_INTRODUCE_2, 13000);
            break;
        }
        case VALKYR_INTRODUCE_2:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_VALKYR_02, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 4000);
            m_EventMap.ScheduleEvent(OPEN_DOOR, 5000);
            m_EventMap.ScheduleEvent(VALKYR_SPAWN, 7000);
            m_EventMap.ScheduleEvent(VALKYR_START, 9000);
            break;
        }
        case VALKYR_SPAWN:
        {
            if (Creature* lightbane = m_Map->SummonCreatureOnMap(NPC_LIGHTBANE, TwinValkyrsLoc[4], TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
            {
                lightbane->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                lightbane->SetReactState(REACT_PASSIVE);
            }

            if (Creature* darkbane = m_Map->SummonCreatureOnMap(NPC_DARKBANE, TwinValkyrsLoc[5], TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
            {
                darkbane->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                darkbane->SetReactState(REACT_PASSIVE);
            }
            
            for (uint32 i = 0; i < 2; ++i)
            {
                if (Creature *Essence = m_Map->SummonCreature(NPC_LIGHT_ESSENCE, TwinValkyrsLoc[i]))
                    m_EssenceGUID[i] = Essence->GetGUID();

                if (Creature *Essence = m_Map->SummonCreature(NPC_DARK_ESSENCE, TwinValkyrsLoc[2+i]))
                    m_EssenceGUID[2+i] = Essence->GetGUID();
            }
            break;
        }
        case VALKYR_START:
        {
            if (Creature* lightbane = m_Map->GetCreature(m_Instance->GetData64(NPC_LIGHTBANE)))
                lightbane->GetMotionMaster()->MovePoint(1, TwinValkyrsLoc[6]);

            if (Creature* darkbane = m_Map->GetCreature(m_Instance->GetData64(NPC_DARKBANE)))
                darkbane->GetMotionMaster()->MovePoint(1, TwinValkyrsLoc[7]);

            break;
        }
        case VALKYR_END_VARIAN:
        {
            if (Creature *varian = m_Map->GetCreature(m_Instance->GetData64(NPC_VARIAN)))
            {
                varian->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                DoScriptText(SAY_VALKYR_03a, varian);
            }

            m_EventMap.ScheduleEvent(VARIAN_STOP_TALK, 5000);
            m_EventMap.ScheduleEvent(LICHKING_START, 65000);
            break;
        }
        case VALKYR_END_GARROSH:
        {
            if (Creature *garrosh = m_Map->GetCreature(m_Instance->GetData64(NPC_GARROSH)))
            {
                garrosh->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                DoScriptText(SAY_VALKYR_03h, garrosh);
            }

            m_EventMap.ScheduleEvent(GARROSH_STOP_TALK, 5000);
            m_EventMap.ScheduleEvent(LICHKING_START, 65000);
            break;
        }
        // Here we start Lichking ...
        case LICHKING_START:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_LICHKING_01, tirion);
            }

            m_EventMap.ScheduleEvent(LICHKING_ANNOUNCE, 17000);
            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 17000);

            // Despawn valkyr
            if (Creature* lightbane = m_Map->GetCreature(m_Instance->GetData64(NPC_LIGHTBANE)))
                lightbane->DespawnOrUnsummon();
            if (Creature* darkbane = m_Map->GetCreature(m_Instance->GetData64(NPC_DARKBANE)))
                darkbane->DespawnOrUnsummon();

            break;
        }
        case LICHKING_ANNOUNCE:
        {
            if (Creature *lichking = m_Map->SummonCreature(NPC_LICH_KING_TOCR, LichKingLoc[0]))
            {
                DoScriptText(SAY_LICHKING_02, lichking);
                lichking->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                lichking->SetVisible(false);
            }
            m_EventMap.ScheduleEvent(LICHKING_SUMMON_PORTAL, 4000);
            break;
        }
        case LICHKING_SUMMON_PORTAL:
        {
            if (Creature* portal = m_Map->SummonCreature(NPC_TRIGGER, LichKingLoc[0]))
            {
                portal->CastSpell(portal, 51807, true);
                portal->SetDisplayId(11686);
                m_PortalGUID = portal->GetGUID();
            }

            m_EventMap.ScheduleEvent(LICHKING_APPEAR, 2500);
            break;
        }
        case LICHKING_APPEAR:
        {
            if (Creature *lichking = m_Map->GetCreature(m_Instance->GetData64(NPC_LICH_KING_TOCR)))
                lichking->SetVisible(true);

            m_EventMap.ScheduleEvent(LICHKING_TIRION_ANSWER, 2000);
            m_EventMap.ScheduleEvent(LICHKING_MOVE, 1000);
            m_EventMap.ScheduleEvent(LICHKING_DESPAWN_PORTAL, 10000);
            break;
        }
        case LICHKING_MOVE:
        {
            if (Creature *lichking = m_Map->GetCreature(m_Instance->GetData64(NPC_LICH_KING_TOCR)))
            {
                lichking->SetWalk(true);
                lichking->GetMotionMaster()->MovePoint(0, LichKingLoc[1]);
            }
            break;
        }
        case LICHKING_DESPAWN_PORTAL:
        {
            if (Creature* portal = m_Map->GetCreature(m_PortalGUID))
                portal->DespawnOrUnsummon();
            break;
        }
        case LICHKING_TIRION_ANSWER:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                DoScriptText(SAY_LICHKING_03, tirion);
            }

            m_EventMap.ScheduleEvent(TIRION_STOP_TALK, 11000);
            m_EventMap.ScheduleEvent(LICHKING_MOCK, 11000);
            break;
        }
        case LICHKING_MOCK:
        {
            if (Creature *lichking = m_Map->GetCreature(m_Instance->GetData64(NPC_LICH_KING_TOCR)))
            {
                lichking->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_LAUGH);
                DoScriptText(SAY_LICHKING_04, lichking);
            }
            
            m_EventMap.ScheduleEvent(LICHKING_SHATTER_INIT, 19000);
            m_EventMap.ScheduleEvent(LICHKING_TALK, 3000);
            break;
        }
        case LICHKING_TALK:
        {
            if (Creature *lichking = m_Map->GetCreature(m_Instance->GetData64(NPC_LICH_KING_TOCR)))
                lichking->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
            break;
        }
        case LICHKING_SHATTER_INIT:
        {
            if (Creature *lichking = m_Map->GetCreature(m_Instance->GetData64(NPC_LICH_KING_TOCR)))
            {
                lichking->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_KNEEL);
                lichking->CastSpell(lichking, SPELL_DESTROY_FLOOR_KNOCKUP, false);
            }

            m_EventMap.ScheduleEvent(LICHKING_SHATTER, 1000);
            m_EventMap.ScheduleEvent(LICHKING_SHATTER_FINISH, 3000);
            break;
        }
        case LICHKING_SHATTER:
        {
            if (GameObject* floor = m_Map->GetGameObject(m_Instance->GetData64(GO_ARGENT_COLISEUM_FLOOR)))
            {
                floor->SetDisplayId(DISPLAYID_DESTROYED_FLOOR);
                floor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_NODESPAWN);
                floor->SetGoState(GO_STATE_ACTIVE);
            }
            break;
        }
        case LICHKING_SHATTER_FINISH:
        {
            if (Creature *lichking = m_Map->GetCreature(m_Instance->GetData64(NPC_LICH_KING_TOCR)))
            {       
                DoScriptText(SAY_LICHKING_05, lichking);
                lichking->DespawnOrUnsummon();
            }

            m_Instance->SetData(TYPE_ENCOUNTER_CURRENT, ENCOUNTER_ANUBARAK);
            break;
        }
    }
}

void AddBossTwinValkyr();
void AddBullets();
void AddNpcValkyrEssence();
void AddSpellValkyrEssence();
void AddSpellPowerUp();

void AddEncounterTwinValkyr()
{
    AddBossTwinValkyr();
    AddBullets();
    AddNpcValkyrEssence();
    AddSpellValkyrEssence();
    AddSpellPowerUp();
}
