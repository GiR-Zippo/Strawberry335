#include "TwinValkyr.h"

class SpellPowerUp_AuraScript : public AuraScript
{
    public:
        PrepareAuraScript(SpellPowerUp_AuraScript);

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
        {
            if (Unit* target = GetTarget())
            {
                if (Aura* aura = target->GetAura(GetId()))
                {
                    if (aura->GetStackAmount() == 100)
                    {
                        if (target->GetDummyAuraEffect(SPELLFAMILY_GENERIC, 2206, EFFECT_1))
                            target->CastSpell(GetUnitOwner(), SPELL_EMPOWERED_DARK);

                        if (target->GetDummyAuraEffect(SPELLFAMILY_GENERIC, 2845, EFFECT_1))
                            target->CastSpell(GetUnitOwner(), SPELL_EMPOWERED_LIGHT);

                        target->RemoveAurasDueToSpell(GetId());
                    }
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(SpellPowerUp_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
};

class SpellPowerUp_SpellScript : public SpellScript
{
    public:
        PrepareSpellScript(SpellPowerUp_SpellScript);

        uint32 spellId;

        bool Load()
        {
            spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_SURGE_OF_SPEED, GetCaster());
            if (!sSpellMgr->GetSpellInfo(spellId))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetExplTargetUnit())
                if (urand(0, 99) < 15)
                    target->CastSpell(target, spellId, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(SpellPowerUp_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
};

class SpellPowerUp : public SpellScriptLoader
{
    public:
        SpellPowerUp() : SpellScriptLoader("spell_powering_up") { }

        AuraScript* GetAuraScript() const
        {
            return new SpellPowerUp_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new SpellPowerUp_SpellScript();
        }
};

void AddSpellPowerUp()
{
    new SpellPowerUp;
}
