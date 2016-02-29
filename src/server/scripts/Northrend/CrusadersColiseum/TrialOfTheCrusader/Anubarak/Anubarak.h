// Known bugs:
// Anubarak - underground phase partially not worked
//          - tele after impale hit a permafrost doesn't work (the entire tele spell should be better)
// Burrow   - visual is vanishing
// Scarab   - Kill credit isn't crediting?

#ifndef _ANUBARAK_H
#define _ANUBARAK_H

#include "../TrialCrusader.h"
#include "../Encounter.h"

#define SPELL_PERMAFROST_HELPER RAID_MODE<uint32>(66193, 67855, 67856, 67857)

enum
{
    // Summons
    NPC_FROST_SPHERE                    = 34606,
    NPC_BURROW                          = 34862,
    NPC_BURROWER                        = 34607,
    NPC_SCARAB                          = 34605,
    NPC_SPIKE                           = 34660,

    ACTION_SHADOW_STRIKE                = 1,
    ACTION_SCARAB_SUBMERGE              = 2,
    ACTION_PERMAFROST_DESPAWN           = 3,

    SPELL_SUBMERGE_EFFECT               = 53421,
    SPELL_EMERGE_EFFECT                 = 66947,
    SPELL_FROST_SPHERE                  = 67539,

    GO_TRIBUTE_CHEST_10H_25             = 195668, // 10man 01-24 attempts
    GO_TRIBUTE_CHEST_10H_45             = 195667, // 10man 25-44 attempts
    GO_TRIBUTE_CHEST_10H_50             = 195666, // 10man 45-49 attempts
    GO_TRIBUTE_CHEST_10H_99             = 195665, // 10man 50 attempts

    GO_TRIBUTE_CHEST_25H_25             = 195672, // 25man 01-24 attempts
    GO_TRIBUTE_CHEST_25H_45             = 195671, // 25man 25-44 attempts
    GO_TRIBUTE_CHEST_25H_50             = 195670, // 25man 45-49 attempts
    GO_TRIBUTE_CHEST_25H_99             = 195669, // 25man 50 attempts

    // Events
    ANUBARAK_INITIATE                   = 1,    // Entrypoint
    ANUBARAK_DONE                       = 2,    // Entrypoint
    ANUBARAK_DONE_ADDITIONAL            = 3,
    ANUBARAK_END_TELEPORT               = 4,
};

const Position AnubarakLoc[]=
{
    {787.932556f, 133.289780f, 142.612152f, M_PI},  // 0 - Anub'arak start location
    {695.240051f, 137.834824f, 142.200000f, 0},     // 1 - Anub'arak move point location
    {694.886353f, 102.484665f, 142.119614f, 0},     // 3 - Nerub Spawn
    {694.500671f, 185.363968f, 142.117905f, 0},     // 5 - Nerub Spawn
    {731.987244f, 83.3824690f, 142.119614f, 0},     // 2 - Nerub Spawn
    {740.184509f, 193.443390f, 142.117584f, 0},     // 4 - Nerub Spawn
};

class Anubarak : public Encounter
{
    public:
        Anubarak(InstanceScript *Instance) : Encounter(Instance)
        {
            m_EventMap.ScheduleEvent(ANUBARAK_INITIATE, 1000);
        }

        void HandleEvent(uint32 EventId);

        void OnDone()
        {
            m_EventMap.ScheduleEvent(ANUBARAK_DONE, 5000);
        }

    private:
        uint32 GetTributeChestId()
        {
            if (m_Map->GetSpawnMode() == RAID_DIFFICULTY_10MAN_HEROIC)
            {
                uint32 Counter = m_Instance->GetData(TYPE_TRIAL_COUNTER);

                if (Counter >= 50)
                    return GO_TRIBUTE_CHEST_10H_99;
                else if (Counter >= 45)
                    return GO_TRIBUTE_CHEST_10H_50;
                else if (Counter >= 25)
                    return GO_TRIBUTE_CHEST_10H_45;
                else
                    return GO_TRIBUTE_CHEST_10H_25;
            }

            if (m_Map->GetSpawnMode() == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                uint32 Counter = m_Instance->GetData(TYPE_TRIAL_COUNTER);

                if (Counter >= 50)
                    return GO_TRIBUTE_CHEST_25H_99;
                else if (Counter >= 45)
                    return GO_TRIBUTE_CHEST_25H_50;
                else if (Counter >= 25)
                    return GO_TRIBUTE_CHEST_25H_45;
                else
                    return GO_TRIBUTE_CHEST_25H_25;
            }

            return 0;
        }
};

#endif /* _ANUBARAK_H */
