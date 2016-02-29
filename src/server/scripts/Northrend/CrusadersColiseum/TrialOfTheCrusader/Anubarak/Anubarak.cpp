#include "Anubarak.h"

enum
{
    SAY_ANUBARAK_END                = -1649109,
    SAY_ANUBARAK_ADDITIONAL         = -1649110,

    SPELL_CORPSE_TELEPORT           = 69016,
    DALARAN_PORTAL                  = 53142,
};

void Anubarak::HandleEvent(uint32 EventId)
{
    if (!m_Instance || !m_Map)
        return;

    switch (EventId)
    {
        case ANUBARAK_INITIATE:
        {
            if (GameObject* floor = m_Map->GetGameObject(m_Instance->GetData64(GO_ARGENT_COLISEUM_FLOOR)))
                floor->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);

            Creature* Anubarak = m_Map->GetCreature(m_Instance->GetData64(NPC_ANUBARAK));

            if (!Anubarak && (m_Instance->GetData(TYPE_TRIAL_COUNTER) || !m_Map->IsHeroic()))
                Anubarak = m_Map->SummonCreatureOnMap(NPC_ANUBARAK, AnubarakLoc[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
            break;
        }
        case ANUBARAK_DONE:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->NearTeleportTo(AnubarakLoc[0].GetPositionX(), AnubarakLoc[0].GetPositionY(), AnubarakLoc[0].GetPositionZ(), 4.0f);
                DoScriptText(SAY_ANUBARAK_END, tirion);
            }

            if (m_Map->IsHeroic())
                m_EventMap.ScheduleEvent(ANUBARAK_DONE_ADDITIONAL, 20000);
            else
                m_EventMap.ScheduleEvent(ANUBARAK_END_TELEPORT, 20000);
            break;
        }
        case ANUBARAK_DONE_ADDITIONAL:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                DoScriptText(SAY_ANUBARAK_ADDITIONAL, tirion);
            
                if (uint32 tributeChest = GetTributeChestId())
                {
                    GameObject* chest = m_Map->SummonGameObjectOnMap(tributeChest, 805.62f, 134.87f, 142.16f, 3.27f, 0, 0, 0, 0, 90000000);
                    chest->SetRespawnTime(7 * DAY);
                }
            }
            m_EventMap.ScheduleEvent(ANUBARAK_END_TELEPORT, 20000);
            break;
        }
        case ANUBARAK_END_TELEPORT:
        {
            if (Creature *tirion = m_Map->GetCreature(m_Instance->GetData64(NPC_TIRION)))
            {
                tirion->CastSpell(tirion->GetPositionX(), tirion->GetPositionY(), tirion->GetPositionZ(), DALARAN_PORTAL, true);
                tirion->DespawnOrUnsummon();
            }
            break;
        }
    }
}

void AddBossAnubarak();
void AddSwarmScarab();
void AddNerubianBurrower();
void AddAnubarakSpike();
void AddFrostSphere();

void AddEncounterAnubarakTrialCrusader()
{
    AddBossAnubarak();
    AddSwarmScarab();
    AddNerubianBurrower();
    AddAnubarakSpike();
    AddFrostSphere();
}
