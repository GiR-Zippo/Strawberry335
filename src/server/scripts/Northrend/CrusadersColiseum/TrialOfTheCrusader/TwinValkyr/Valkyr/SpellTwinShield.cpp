
class SpellTwinShield_AuraScript : public AuraScript
{
    public:
        PrepareAuraScript(SpellTwinShield_AuraScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_UNIT;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetCaster()->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetCaster()->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(SpellTwinShield_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(SpellTwinShield_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
};

class SpellTwinShield : public SpellScriptLoader
{
    public:
        SpellTwinShield() : SpellScriptLoader("spell_twin_valkyr_shield") { }

        AuraScript* GetAuraScript() const
        {
            return new SpellTwinShield_AuraScript();
        }
};

void AddSpellTwinShield()
{
    new SpellTwinShield();
}
