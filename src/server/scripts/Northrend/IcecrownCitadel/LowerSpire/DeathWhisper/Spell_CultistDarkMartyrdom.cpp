#include "Spell_LadyDeathwhisper.h"

////////////////////
// Dark Martyrdom //
////////////////////

void SpellScriptCultistDarkMartyrdom::HandleEffect(SpellEffIndex /*effIndex*/)
{
    if (GetCaster()->isSummon())
        if (Unit* owner = GetCaster()->ToTempSummon()->GetSummoner())
            owner->GetAI()->SetGUID(GetCaster()->GetGUID(), GUID_CULTIST);

    GetCaster()->Kill(GetCaster());
    GetCaster()->SetDisplayId(uint32(GetCaster()->GetEntry() == NPC_CULT_FANATIC ? 38009 : 38010));
}

void SpellScriptCultistDarkMartyrdom::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptCultistDarkMartyrdom::HandleEffect, EFFECT_2, SPELL_EFFECT_FORCE_DESELECT);
}
