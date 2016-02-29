#include "BossJormungar.h"

enum
{
    SPELL_MOLTEN_SPEW                       = 66821,
    SPELL_BURNING_BITE                      = 66879,
    SPELL_FIRE_SPIT                         = 66796,
    SPELL_BURNING_SPRAY                     = 66902,

    MODEL_DREADSCALE_ROOTED                 = 26935,
    MODEL_DREADSCALE_MOVING                 = 24564,
};

class BossDreadscaleAI : public BossJormungarAI
{
    public:
        BossDreadscaleAI(Creature* creature) : BossJormungarAI(creature),
            m_Move(false)
        {
            modelId[MODEL_ROOTED] = MODEL_DREADSCALE_ROOTED;
            modelId[MODEL_MOVING] = MODEL_DREADSCALE_MOVING;

            spellId[SPELL_SPEW] = SPELL_MOLTEN_SPEW;
            spellId[SPELL_BITE] = SPELL_BURNING_BITE;
            spellId[SPELL_SPIT] = SPELL_FIRE_SPIT;
            spellId[SPELL_SPRAY] = SPELL_BURNING_SPRAY;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == 0)
                m_Move = true;
            else if (type == POINT_MOTION_TYPE && id == 1)
            {
                StartUp();

                m_Jormungar->GetAI()->DoAction(START_UP);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (m_Move)
            {
                m_Move = false;
                me->GetMotionMaster()->MovePoint(1, NorthrendBeastLoc[3]);
            }

            BossJormungarAI::UpdateAI(diff);
        }

    private:
        bool m_Move;
};

class BossDreadscale : public CreatureScript
{
    public:
        BossDreadscale() : CreatureScript("boss_dreadscale") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new BossDreadscaleAI(creature);
        }
};

void AddBossDreadscale()
{
    new BossDreadscale();
}
