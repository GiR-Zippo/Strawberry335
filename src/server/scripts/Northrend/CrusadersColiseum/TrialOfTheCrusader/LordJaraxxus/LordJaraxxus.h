#ifndef _LORD_JARAXXUS_H
#define _LORD_JARAXXUS_H

#include "../TrialCrusader.h"
#include "../Encounter.h"
enum
{
    // Misc
    NPC_MISTRESS_OF_PAIN                    = 34826,
    SPELL_JARAXXUS_CHAINS                   = 67924,

    // Achievements
    THREE_SIXTY_PAIN_SPIKE_10_PLAYER        = 11838,
    THREE_SIXTY_PAIN_SPIKE_10_PLAYER_HEROIC = 11861,
    THREE_SIXTY_PAIN_SPIKE_25_PLAYER        = 11839,
    THREE_SIXTY_PAIN_SPIKE_25_PLAYER_HEROIC = 11862,

    // Events
    JARAXXUS_INTRODUCE_FIZZLEBANG               = 1,    // Entrypoint
    JARAXXUS_START_FIZZLEBANG                   = 2,
    JARAXXUS_FIZZLEBANG_KILLED                  = 3,    // Entrypoint
    JARAXXUS_DONE                               = 5,    // Entrypoint
    JARAXXUS_START                              = 4,
    JARAXXUS_GARROSH_ANGRY                      = 6,
    JARAXXUS_VARIAN_REPLY                       = 7,
    JARAXXUS_TIRION_MEND                        = 8,
};

const Position LordJaraxxusLoc[] =
{
    {563.822f,  223.296f,   394.479f,   4.730f}, // SPAWNPOINT FIZZLEBANG
    {563.822f,  148.821f,   393.990f,   4.730f}, // WAYPOINT FIZZLEBANG
    {563.822f,  130.861f,   393.990f,   1.652f}, // WAYPOINT JARAXXUS
};

class LordJaraxxus : public Encounter
{
    public:
        LordJaraxxus(InstanceScript *Instance) : Encounter(Instance),
              m_MistressOfPainCount(0)
        {
            if (Creature* Jaraxxus = m_Map->GetCreature(m_Instance->GetData64(NPC_JARAXXUS)))
            {
                Jaraxxus->RemoveAurasDueToSpell(SPELL_JARAXXUS_CHAINS);
                Jaraxxus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Jaraxxus->SetReactState(REACT_AGGRESSIVE);
                Jaraxxus->SetInCombatWithZone();
                m_Instance->SetData(TYPE_ENCOUNTER_STATUS, IN_PROGRESS);
            }
            else
                m_EventMap.ScheduleEvent(JARAXXUS_INTRODUCE_FIZZLEBANG, 100);
        }

        void HandleEvent(uint32 EventId);

        void OnDone()
        {
            m_EventMap.ScheduleEvent(JARAXXUS_DONE, 100);
        }

        void OnFail()
        {
            m_EventMap.ScheduleEvent(BARRET_RETURN, 1000);
        }

        void OnCreatureCreate(Creature* creature)
        {
            if (creature->GetEntry() == NPC_MISTRESS_OF_PAIN)
                m_MistressOfPainCount++;
        }

        void OnUnitDeath(Unit *unit)
        {
            switch (unit->GetEntry())
            {
                case NPC_MISTRESS_OF_PAIN:
                    m_MistressOfPainCount--;
                break;
                case NPC_FIZZLEBANG:
                    m_EventMap.ScheduleEvent(JARAXXUS_FIZZLEBANG_KILLED, 5000);
            }
        }
        
        bool CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* source, Unit const* target, uint32 miscvalue1)
        {
            switch (criteria_id)
            {
                case THREE_SIXTY_PAIN_SPIKE_10_PLAYER:
                case THREE_SIXTY_PAIN_SPIKE_10_PLAYER_HEROIC:
                case THREE_SIXTY_PAIN_SPIKE_25_PLAYER:
                case THREE_SIXTY_PAIN_SPIKE_25_PLAYER_HEROIC:
                    return m_MistressOfPainCount >= 2;
            }

            return false;
        }

    private:
        uint32 m_MistressOfPainCount;
};

#endif /* _LORD_JARAXXUS_H */
