#include "TwinValkyr.h"

class SpellValkyrEssence_AuraScript : public AuraScript
{
    public:
        PrepareAuraScript(SpellValkyrEssence_AuraScript);

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & /*absorbAmount*/)
        {
            if (urand(0, 99) < 5)
                GetTarget()->CastSpell(GetTarget(), SPELL_SURGE_OF_SPEED, true);
        }

        void Register()
        {
            OnEffectAbsorb += AuraEffectAbsorbFn(SpellValkyrEssence_AuraScript::Absorb, EFFECT_0);
        }
};

class SpellValkyrEssence : public SpellScriptLoader
{
    public:
        SpellValkyrEssence() : SpellScriptLoader("spell_valkyr_essences") { }

        AuraScript* GetAuraScript() const
        {
            return new SpellValkyrEssence_AuraScript();
        }
};

void AddSpellValkyrEssence()
{
    new SpellValkyrEssence();
}
