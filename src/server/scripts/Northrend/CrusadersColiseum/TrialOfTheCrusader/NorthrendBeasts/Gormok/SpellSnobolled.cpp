#include "../NorthrendBeasts.h"

class SpellSnobolled : public SpellScriptLoader
{
    public:
        SpellSnobolled() : SpellScriptLoader("spell_snobolled") {}

        class SpellSnobolled_AuraScript : public AuraScript
        {
            public:
                PrepareAuraScript(SpellSnobolled_AuraScript);

                void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
                {
                    if (Unit *caster = GetCaster())
                        if (caster->isAlive())
                            return;

                    if (GetTarget())
                        Remove(AURA_REMOVE_BY_DEFAULT);
                }

                void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
                {
                    Unit *target = GetTarget();
                    Unit *caster = GetCaster();

                    if (target && caster)
                    {
                        target->CreateVehicleKit(VEHICLE_PLAYER, 0);
                        caster->EnterVehicle(target, SEAT_PLAYER);
                        caster->GetAI()->DoAction(SNOBOLD_PLAYER_STATE);
                    }
                }

                void HandleEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
                {
                    Unit *target = GetTarget();
                    Unit *caster = GetCaster();

                    if (caster && caster->isAlive())
                    {
                        caster->ExitVehicle();
                        caster->GetAI()->DoAction(SNOBOLD_PLAYER_DIED);
                    }

                    if (target)
                        target->RemoveVehicleKit();
                }

                void Register()
    	    	{
                    AfterEffectApply += AuraEffectApplyFn(SpellSnobolled_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY,AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(SpellSnobolled_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY,AURA_EFFECT_HANDLE_REAL);
                    OnEffectPeriodic += AuraEffectPeriodicFn(SpellSnobolled_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                }
        };

        AuraScript* GetAuraScript() const
        {
            return new SpellSnobolled_AuraScript();    
        }
};

void AddSpellSnobolled()
{
    new SpellSnobolled();
}