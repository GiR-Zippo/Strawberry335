#include "Npc_SpireFrostwyrm.h"
#include "Npc_RottingFrostGiant.h"
#include "Npc_Nerubar.h"
#include "Npc_ServantOfTheThrone.h"
#include "Npc_AncientSkeleton.h"

#include "LowerSpireSpells.h"


enum Spells {
    SPELL_DARK_RECKONING_TRIGGER    =   69482
};

class spell_dark_reckoning : public SpellScriptLoader
{
    public:
        spell_dark_reckoning() : SpellScriptLoader("spell_dark_reckoning") { }

        class spell_dark_reckoning_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dark_reckoning_AuraScript);

            void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                Unit* caster = GetCaster();
                if (caster && caster->isAlive())
                    caster->CastSpell(GetTarget(), SPELL_DARK_RECKONING_TRIGGER);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dark_reckoning_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dark_reckoning_AuraScript();
        }
};

void LoadTrashLowerSpire()
{
    new NpcSpireFrostwyrm();
    new AtSpireFrostwyrm();
    new NpcRottingFrostGiant();
    new NpcLowerSpireNerubar();
    new NpcServantOfTheThrone();
    new NpcAncientSkeleton();

    new SpellFrostGiantDeathPlague();
    new spell_dark_reckoning();
}