#ifndef _TWIN_VALKYR_H
#define _TWIN_VALKYR_H

#include "../TrialCrusader.h"
#include "../Encounter.h"

enum
{
    // Events
    VALKYR_INTRODUCE            = 1,    // Entrypoint
    VALKYR_INTRODUCE_2          = 2,
    VALKYR_SPAWN                = 3,
    VALKYR_START                = 4,
    VALKYR_END_VARIAN           = 5,    // Entrypoint
    VALKYR_END_GARROSH          = 6,    // Entrypoint
    LICHKING_START              = 7,
    LICHKING_ANNOUNCE           = 8,
    LICHKING_SUMMON_PORTAL      = 9,
    LICHKING_APPEAR             = 10,
    LICHKING_MOVE               = 11,
    LICHKING_DESPAWN_PORTAL     = 12,
    LICHKING_TIRION_ANSWER      = 13,
    LICHKING_MOCK               = 14,
    LICHKING_TALK               = 15,
    LICHKING_SHATTER_INIT       = 16,
    LICHKING_SHATTER            = 17,
    LICHKING_SHATTER_FINISH     = 18,

    //Spells
    SPELL_SURGE_OF_SPEED        = 65828,
    SPELL_EMPOWERED_DARK        = 65724,
    SPELL_EMPOWERED_LIGHT       = 65748,
    AURA_DARK_ESSENCE           = 65684,
    AURA_LIGHT_ESSENCE          = 65686
};

const Position TwinValkyrsLoc[]=
{
    {586.060f,  117.515f,   394.314f,   0.0f},      // 0 - Dark essence 1
    {541.602f,  161.880f,   394.588f,   0.0f},      // 1 - Dark essence 2
    {541.021f,  117.263f,   395.315f,   0.0f},      // 2 - Light essence 1
    {586.201f,  162.146f,   394.626f,   0.0f},      // 3 - Light essence 2
    {570.451f,  223.296f,   394.479f,   4.654f},    // 4 - Left spawn
    {554.951f,  223.296f,   394.479f,   4.748f},    // 5 - Right spawn
    {573.5f,    180.5f,     395.14f,    0},         // 6 - Move 0 Right
    {553.5f,    180.5f,     395.14f,    0},         // 7 - Move 0 Left
    {585.5f,    170.0f,     395.14f,    0},         // 8 - Move 1 Right
    {545.5f,    170.0f,     395.14f,    0},         // 9 - Move 1 Left
};

#define SPELL_DARK_ESSENCE_HELPER RAID_MODE<uint32>(65684, 67176, 67177, 67178)
#define SPELL_LIGHT_ESSENCE_HELPER RAID_MODE<uint32>(65686, 67222, 67223, 67224)

#define SPELL_EMPOWERED_DARK_HELPER RAID_MODE<uint32>(65724, 67213, 67214, 67215)
#define SPELL_EMPOWERED_LIGHT_HELPER RAID_MODE<uint32>(65748, 67216, 67217, 67218)

#define SPELL_POWERING_UP_HELPER RAID_MODE<uint32>(67590, 67602, 67603, 67604)

class TwinValkyr : public Encounter
{
    public:
        TwinValkyr(InstanceScript *Instance) : Encounter(Instance)
        {
            m_EventMap.ScheduleEvent(VALKYR_INTRODUCE, 100);
        }

        void HandleEvent(uint32 EventId);

        void OnDone()
        {
            DespawnOrbs();
            RemoveAuras();
            m_Instance->SetData(GO_EAST_PORTCULLIS, OPEN);

            if (m_Instance->GetData(TYPE_TEAM) == ALLIANCE)
                m_EventMap.ScheduleEvent(VALKYR_END_VARIAN, 4000);
            else
                m_EventMap.ScheduleEvent(VALKYR_END_GARROSH, 4000);
        }

        void OnFail()
        {
            DespawnOrbs();
            RemoveAuras();
            m_EventMap.ScheduleEvent(BARRET_RETURN, 1000);
        }

    private:
        uint64 m_EssenceGUID[4];
        uint64 m_PortalGUID;

        void DespawnOrbs()
        {
            for (uint8 i = 0; i < 4; ++i)
                if (Creature *Essence = m_Map->GetCreature(m_EssenceGUID[i]))
                    Essence->DespawnOrUnsummon();
        }

        void RemoveAuras()
        {
            if (!m_Instance)
            {
                if (!m_Instance->instance)
                    return;
            }

            Map::PlayerList const& players = m_Instance->instance->GetPlayers();

            if (players.isEmpty())
                return;

            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* player = itr->getSource())
                {
                    player->RemoveAurasDueToSpell(AURA_DARK_ESSENCE);
                    player->RemoveAurasDueToSpell(AURA_LIGHT_ESSENCE);
                }
            }
        }
};

#endif /* _TWIN_VALKYR_H */
