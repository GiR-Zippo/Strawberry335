#include "BossValkyr.h"

class SpellTwinPower_AuraScript : public AuraScript
{
    public:
        PrepareAuraScript(SpellTwinPower_AuraScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_UNIT;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                if (Creature* Valk = ObjectAccessor::GetCreature(*GetCaster(), instance->GetData64(GetCaster()->GetEntry())))
                    CAST_AI(BossValkyrAI, Valk->AI())->EnableDualWield(true);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                if (Creature* Valk = ObjectAccessor::GetCreature(*GetCaster(), instance->GetData64(GetCaster()->GetEntry())))
                    CAST_AI(BossValkyrAI, Valk->AI())->EnableDualWield(false);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(SpellTwinPower_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(SpellTwinPower_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
};

class SpellTwinPower : public SpellScriptLoader
{
    public:
        SpellTwinPower() : SpellScriptLoader("spell_power_of_the_twins") { }

        AuraScript* GetAuraScript() const
        {
            return new SpellTwinPower_AuraScript();
        }
};

void AddSpellTwinPower()
{
    new SpellTwinPower();
}
