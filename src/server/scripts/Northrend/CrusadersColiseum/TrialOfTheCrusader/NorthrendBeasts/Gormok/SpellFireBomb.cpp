#include "../NorthrendBeasts.h"

enum Data
{
    TIME_DESPAWN                            = 30000,
    NPC_FIRE_BOMB                           = 34854,
};

class SpellFireBomb : public SpellScriptLoader
{
    public:
        SpellFireBomb() : SpellScriptLoader("spell_snobold_fire_bomb") {}

        SpellScript* GetSpellScript() const
        {
            return new SpellFireBomb_SpellScript();    
        }

        class SpellFireBomb_SpellScript : public SpellScript
        {
            PrepareSpellScript(SpellFireBomb_SpellScript);

            void TriggerFireBomb(SpellEffIndex effIndex)
            {
                if (const WorldLocation* pos = GetExplTargetDest())
                    if (Unit* caster = GetCaster())
                        caster->SummonCreature(NPC_FIRE_BOMB, pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, TIME_DESPAWN);
            }
    
            void Register()
    		{
                OnEffectHitTarget += SpellEffectFn(SpellFireBomb_SpellScript::TriggerFireBomb, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };
};

void AddSpellFireBomb()
{
    new SpellFireBomb();
}
