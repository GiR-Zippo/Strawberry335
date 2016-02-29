#include "Spell_ProfessorPutricide.h"

////////////////////
// Unbound Plague //
////////////////////

bool SpellScriptPutricideUnboundPlague::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_UNBOUND_PLAGUE))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_UNBOUND_PLAGUE_SEARCHER))
        return false;
    return true;
}

void SpellScriptPutricideUnboundPlague::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (!GetHitUnit())
        return;

    InstanceScript* instance = GetCaster()->GetInstanceScript();
    if (!instance)
        return;

    uint32 plagueId = sSpellMgr->GetSpellIdForDifficulty(SPELL_UNBOUND_PLAGUE, GetCaster());

    if (!GetHitUnit()->HasAura(plagueId))
    {
        if (Creature* professor = ObjectAccessor::GetCreature(*GetCaster(), instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
        {
            if (Aura* oldPlague = GetCaster()->GetAura(plagueId, professor->GetGUID()))
            {
                if (Aura* newPlague = professor->AddAura(plagueId, GetHitUnit()))
                {
                    newPlague->SetMaxDuration(oldPlague->GetMaxDuration());
                    newPlague->SetDuration(oldPlague->GetDuration());
                    oldPlague->Remove();
                    GetCaster()->RemoveAurasDueToSpell(SPELL_UNBOUND_PLAGUE_SEARCHER);
                    GetCaster()->CastSpell(GetCaster(), SPELL_PLAGUE_SICKNESS, true);
                    GetCaster()->CastSpell(GetCaster(), SPELL_UNBOUND_PLAGUE_PROTECTION, true);
                    professor->CastSpell(GetHitUnit(), SPELL_UNBOUND_PLAGUE_SEARCHER, true);
                }
            }
        }
    }
}

void SpellScriptPutricideUnboundPlague::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellScriptPutricideUnboundPlague::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}
