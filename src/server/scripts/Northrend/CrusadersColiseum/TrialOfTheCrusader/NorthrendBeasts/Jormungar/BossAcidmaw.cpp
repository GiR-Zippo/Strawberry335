#include "BossJormungar.h"

enum Spells
{
    SPELL_ACID_SPEW                         = 66818,
    SPELL_PARALYTIC_BITE                    = 66824,
    SPELL_ACID_SPIT                         = 66880,
    SPELL_PARALYTIC_SPRAY                   = 66901,

    MODEL_ACIDMAW_ROOTED                    = 29815,
    MODEL_ACIDMAW_MOVING                    = 29816,
};

class BossAcidmawAI : public BossJormungarAI
{
    public:
        BossAcidmawAI(Creature* creature): BossJormungarAI(creature) 
        {
            modelId[MODEL_ROOTED] = MODEL_ACIDMAW_ROOTED;
            modelId[MODEL_MOVING] = MODEL_ACIDMAW_MOVING;

            spellId[SPELL_SPEW] = SPELL_ACID_SPEW;
            spellId[SPELL_BITE] = SPELL_PARALYTIC_BITE;
            spellId[SPELL_SPIT] = SPELL_ACID_SPIT;
            spellId[SPELL_SPRAY] = SPELL_PARALYTIC_SPRAY;
        }
};

class BossAcidmaw : public CreatureScript
{
public:
    BossAcidmaw() : CreatureScript("boss_acidmaw") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new BossAcidmawAI(creature);
    }
};

void AddBossAcidmaw()
{
    new BossAcidmaw();
}
