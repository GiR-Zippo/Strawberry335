#include "Spell_PlagueWorks.h"

//////////////
// Decimate //
//////////////

void SpellScriptPlagueWorksDecimate::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (GetHitUnit()->GetHealthPct() > float(GetEffectValue()))
    {
        uint32 newHealth = GetHitUnit()->GetMaxHealth() * uint32(GetEffectValue()) / 100;
        GetHitUnit()->SetHealth(newHealth);
    }
}

void SpellScriptPlagueWorksDecimate::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPlagueWorksDecimate::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}
