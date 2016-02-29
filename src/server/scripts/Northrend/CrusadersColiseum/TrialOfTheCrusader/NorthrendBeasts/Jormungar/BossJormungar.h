#ifndef _BOSS_JORMUNGAR_H
#define _BOSS_JORMUNGAR_H

#include "../NorthrendBeasts.h"

enum
{
    //Events
    EVENT_SPEW                              = 1,    // Moving
    EVENT_BITE                              = 2,    // Moving
    EVENT_SLIME_POOL                        = 3,    // Moving
    EVENT_SPRAY                             = 4,    // Rooted
    EVENT_SWEEP                             = 5,    // Rooted
    EVENT_BURROW                            = 6,    // PhaseChange
    EVENT_UNBURROW                          = 7,    // PhaseChange

    // Spell indices
    SPELL_SPEW                              = 0,
    SPELL_BITE                              = 1,
    SPELL_SPIT                              = 2,
    SPELL_SPRAY                             = 3,
    MAX_SPELL                               = 4,

    // Spells
    SPELL_SLIME_POOL                        = 66883,
    SPELL_SWEEP                             = 66794,
    SPELL_ENRAGE                            = 68335,
    SPELL_UNBURROW                          = 66947,
    SPELL_BURROW                            = 66948,

    // Models
    MODEL_ROOTED                            = 0,
    MODEL_MOVING                            = 1,
    MAX_MODEL                               = 2,

    // Emotes
    EMOTE_BURROW                            = -1649010,
    EMOTE_UNBURROW                          = -1649011,
    EMOTE_ENRAGE                            = -1649012,

    START_UP                                = 1,
};

class BossJormungarAI : public ScriptedAI
{
    public:
        BossJormungarAI(Creature* creature) : ScriptedAI(creature)
        {
            m_Instance = me->GetInstanceScript();
            m_Map = m_Instance->instance;
        }

        void SetGUID(uint64 guid, int32 /*id*/)
        {
            if (!guid)
                return;

            m_Jormungar = m_Map->GetCreature(guid);
        }

        void DoAction(int32 const action)
        {
            if (action == START_UP)
                StartUp();
        }

        void Reset() { m_EventMap.Reset(); }

        void EnterEvadeMode()
        {
            if (m_Instance)
                m_Instance->SetData(GO_MAIN_GATE_DOOR, OPEN);

            ScriptedAI::EnterEvadeMode();
        }

        void JustReachedHome()
        {
            if (m_Instance)
                m_Instance->SetData(GO_MAIN_GATE_DOOR, CLOSE);

            me->DespawnOrUnsummon();
            if (m_Jormungar)
                m_Jormungar->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (m_Jormungar->isDead() && !me->HasAura(SPELL_ENRAGE))
                Enrage();

            m_EventMap.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 EventID = m_EventMap.ExecuteEvent())
            {
                HandleEvent(EventID);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            if (!m_Transit && m_Rooted)
                DoSpellAttackIfReady(spellId[SPELL_SPIT]);
            else if (!m_Transit && m_Moving)
                DoMeleeAttackIfReady();
        }

    protected:
        uint32 spellId[MAX_SPELL];
        uint32 modelId[MAX_MODEL];
        Creature* m_Jormungar;
        void StartUp();

    private:
        bool m_Transit;
        bool m_Rooted;
        bool m_Moving;

        EventMap m_EventMap;
        InstanceScript *m_Instance;
        Map *m_Map;
        
        void SetRooted();
        void SetMoving();
        void Enrage();

        void HandleEvent(uint32 eventID);
};

#endif /* _BOSS_JORMUNGAR_H */
