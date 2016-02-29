#ifndef _BOSS_ICEHOWL_H
#define _BOSS_ICEHOWL_H

#include "../NorthrendBeasts.h"
enum
{
    EVENT_ARCTIC_BREATH                     = 1,
    EVENT_WHIRL                             = 2,
    EVENT_FEROCIOUS_BUTT                    = 3,
    EVENT_ALLOW_ATTACK                      = 4,
    EVENT_COMBO                             = 5,
    EVENT_MASSIVE_CRASH                     = 6,
    EVENT_SELECT_TARGET                     = 7,
    EVENT_ROAR_STOP                         = 8,
    EVENT_JUMP_BACK                         = 9,
    EVENT_COMBO_CHARGE                      = 10,
    EVENT_TRAMPLE                           = 11,
    EVENT_CRASHED_CHECK                     = 12,
    EVENT_END_COMBO                         = 13,
    EVENT_BERSERK                           = 14,

    TIME_WHIRL_INIT                         = 16000,
    TIME_WHIRL_COMBO                        = 16000,
    TIME_WHIRL                              = 16000,
    TIME_ARCTIC_BREATH_INIT                 = 19000,
    TIME_ARCTIC_BREATH_COMBO                = 5000,
    TIME_ARCTIC_BREATH                      = 19000,
    TIME_FEROCIOUS_BUTT_INIT                = 24000,
    TIME_FEROCIOUS_BUTT_COMBO               = 21000,
    
    TIME_COMBO_INIT                         = 33000,
    TIME_COMBO                              = 60000,

    TIME_MASSIVE_CRASH                      = 2000,
    TIME_SELECT_TARGET                      = 6000,
    TIME_ROAR_STOP                          = 7000,
    TIME_JUMP_BACK                          = 9000,
    TIME_TRAMPLE                            = 200,
    TIME_COMBO_CHARGE                       = 1000,
    TIME_END_COMBO_RAGE                     = 100,
    TIME_END_COMBO                          = 15000,
    TIME_BERSERK                            = 180000,

    MOVE_START                              = 0,
    JUMP_MIDDLE                             = 1,
    JUMP_BACK                               = 2,
    MOVE_CHARGE                             = 3,
};

class BossIcehowlAI : public ScriptedAI
{
    public:
        BossIcehowlAI(Creature* creature): ScriptedAI(creature),
            m_TargetX(0),
            m_TargetY(0),
            m_Combo(false),
            m_Trampled(false)
        {
            m_Instance = creature->GetInstanceScript();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (m_Instance)
                m_Instance->SetData(TYPE_ENCOUNTER_STATUS, DONE);
        }

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
        }

        void MovementInform(uint32 type, uint32 id);

        void SpellHitTarget(Unit* target, const SpellInfo* spell);

        void UpdateAI(uint32 const diff);

    private:
        void HandleEvent(uint32 eventId);

        InstanceScript* m_Instance;
        EventMap m_EventMap;

        bool m_Trampled;
        bool m_Combo;
        float m_TargetX;
        float m_TargetY;
};

class BossIcehowl : public CreatureScript
{
    public:
        BossIcehowl() : CreatureScript("boss_icehowl") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new BossIcehowlAI(creature);
        }
};

#endif /* _BOSS_ICEHOWL_H */