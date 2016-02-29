#include "Spell_Sindragosa.h"

bool SpellFrostwardenHandlerOrderWhelpSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_FOCUS_FIRE))
        return false;
    return true;
}

void SpellFrostwardenHandlerOrderWhelpSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.remove_if(Trinity::ObjectTypeIdCheck(TYPEID_PLAYER, false));
    if (targets.empty())
        return;

    WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
    targets.clear();
    targets.push_back(target);
}

void SpellFrostwardenHandlerOrderWhelpSpellScript::HandleForcedCast(SpellEffIndex effIndex)
{
    // caster is Frostwarden Handler, target is player, caster of triggered is whelp
    PreventHitDefaultEffect(effIndex);
    std::list<Creature*> unitList;
    GetCreatureListWithEntryInGrid(unitList, GetCaster(), NPC_FROSTWING_WHELP, 150.0f);
    if (Creature* creature = GetCaster()->ToCreature())
        unitList.remove_if(OrderWhelpTargetSelector(creature));

    if (unitList.empty())
        return;

    Trinity::Containers::SelectRandomContainerElement(unitList)->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
}

void SpellFrostwardenHandlerOrderWhelpSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellFrostwardenHandlerOrderWhelpSpellScript::HandleForcedCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellFrostwardenHandlerOrderWhelpSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
}

void SpellFrostwardenHandlerFocusFireSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetCaster()->AddThreat(GetHitUnit(), float(GetEffectValue()));
    GetCaster()->GetAI()->SetData(DATA_WHELP_MARKER, 1);
}

void SpellFrostwardenHandlerFocusFireSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellFrostwardenHandlerFocusFireSpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
}

void SpellFrostwardenHandlerFocusFireAuraScript::PeriodicTick(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    if (Unit* caster = GetCaster())
    {
        caster->AddThreat(GetTarget(), -float(GetSpellInfo()->Effects[EFFECT_1].CalcValue()));
        caster->GetAI()->SetData(DATA_WHELP_MARKER, 0);
    }
}

void SpellFrostwardenHandlerFocusFireAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellFrostwardenHandlerFocusFireAuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
}