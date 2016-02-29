#include "BossJormungar.h"

enum Data
{
    SPELL_SLIME_POOL_GROWTH                 = 66882,
};

class NpcSlimepool : public CreatureScript
{
    public:
        NpcSlimepool() : CreatureScript("mob_slime_pool") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new NpcSlimepoolAI(creature);
        }

        class NpcSlimepoolAI : public ScriptedAI
        {
            public:
                NpcSlimepoolAI(Creature* creature) : ScriptedAI(creature)
                {
                    m_Instance = me->GetInstanceScript();
                }

                void Reset()
                {
                    me->SetReactState(REACT_PASSIVE);
                    m_Active = false;
                }

                void UpdateAI(const uint32 /*uiDiff*/)
                {
                    if (!m_Active)
                    {
                        m_Active = true;
                        DoCast(me, SPELL_SLIME_POOL_GROWTH);
                    }

                    if (m_Instance)
                        if (m_Instance->GetData(TYPE_ENCOUNTER_STATUS) != IN_PROGRESS)
                            me->DespawnOrUnsummon();
                }

            private:
                InstanceScript* m_Instance;
                bool m_Active;
        };
};

void AddNpcSlimepool()
{
    new NpcSlimepool();
}
