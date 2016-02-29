#include "Spell_TheLichKing.h"

bool SpellTheLichKingNecroticPlagueAuraScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_NECROTIC_PLAGUE_JUMP))
        return false;
    return true;
}

void SpellTheLichKingNecroticPlagueAuraScript::OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (Creature* pCreature = GetCaster()->ToCreature())
        if (Unit* pUnit = GetTarget())
            pCreature->AI()->BossWhisper(EMOTE_NECROTIC_PLAGUE_WARNING, pUnit->GetGUID());
}

void SpellTheLichKingNecroticPlagueAuraScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    switch (GetTargetApplication()->GetRemoveMode())
    {
        case AURA_REMOVE_BY_ENEMY_SPELL:
        case AURA_REMOVE_BY_EXPIRE:
        case AURA_REMOVE_BY_DEATH:
            break;
        default:
            return;
    }

    CustomSpellValues values;
    //values.AddSpellMod(SPELLVALUE_AURA_STACK, 2);
    values.AddSpellMod(SPELLVALUE_MAX_TARGETS, 1);
    GetTarget()->CastCustomSpell(SPELL_NECROTIC_PLAGUE_JUMP, values, NULL, TRIGGERED_FULL_MASK, NULL, NULL, GetCasterGUID());
    if (Unit* caster = GetCaster())
        caster->CastSpell(caster, SPELL_PLAGUE_SIPHON, true);
}

void SpellTheLichKingNecroticPlagueAuraScript::Register()
{
    AfterEffectApply += AuraEffectApplyFn(SpellTheLichKingNecroticPlagueAuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    AfterEffectRemove += AuraEffectRemoveFn(SpellTheLichKingNecroticPlagueAuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
}

bool SpellTheLichKingNecroticPlagueJumpSpellScript::Load()
{
    _hadAura = false;
    return true;
}

void SpellTheLichKingNecroticPlagueJumpSpellScript::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.remove_if(NecrotigPlagueJumpTargetFilter());
    if (targets.size() < 1)
    {
        targets.clear();
        return;
    }

    targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
    targets.resize(1);
}

void SpellTheLichKingNecroticPlagueJumpSpellScript::CheckAura()
{
    if (GetHitUnit()->HasAura(GetSpellInfo()->Id))
        _hadAura = true;
}

void SpellTheLichKingNecroticPlagueJumpSpellScript::AddMissingStack()
{
    if (GetHitAura() && !_hadAura && GetSpellValue()->EffectBasePoints[EFFECT_1] != AURA_REMOVE_BY_ENEMY_SPELL)
        GetHitAura()->ModStackAmount(1);
}

void SpellTheLichKingNecroticPlagueJumpSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingNecroticPlagueJumpSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    BeforeHit += SpellHitFn(SpellTheLichKingNecroticPlagueJumpSpellScript::CheckAura);
    OnHit += SpellHitFn(SpellTheLichKingNecroticPlagueJumpSpellScript::AddMissingStack);
}

bool SpellTheLichKingNecroticPlagueJumpAuraScript::Load()
{
    _lastAmount = 0;
    return true;
}

void SpellTheLichKingNecroticPlagueJumpAuraScript::OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (Unit* caster = GetCaster())
        caster->GetAI()->SetData(DATA_PLAGUE_STACK, GetStackAmount());
}

void SpellTheLichKingNecroticPlagueJumpAuraScript::OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
{
    _lastAmount = aurEff->GetAmount();
    switch (GetTargetApplication()->GetRemoveMode())
    {
        case AURA_REMOVE_BY_EXPIRE:
        case AURA_REMOVE_BY_DEATH:
            break;
        default:
            return;
    }

    CustomSpellValues values;
    values.AddSpellMod(SPELLVALUE_AURA_STACK, GetStackAmount());
    GetTarget()->CastCustomSpell(SPELL_NECROTIC_PLAGUE_JUMP, values, NULL, TRIGGERED_FULL_MASK, NULL, NULL, GetCasterGUID());
    if (Unit* caster = GetCaster())
        caster->CastSpell(caster, SPELL_PLAGUE_SIPHON, true);
}

void SpellTheLichKingNecroticPlagueJumpAuraScript::OnDispel(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
{
    _lastAmount = aurEff->GetAmount();
}

void SpellTheLichKingNecroticPlagueJumpAuraScript::AfterDispel(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
{
    // this means the stack increased so don't process as if dispelled
    if (aurEff->GetAmount() > _lastAmount)
        return;

    CustomSpellValues values;
    values.AddSpellMod(SPELLVALUE_AURA_STACK, GetStackAmount());
    values.AddSpellMod(SPELLVALUE_BASE_POINT1, AURA_REMOVE_BY_ENEMY_SPELL); // add as marker (spell has no effect 1)
    GetTarget()->CastCustomSpell(SPELL_NECROTIC_PLAGUE_JUMP, values, NULL, TRIGGERED_FULL_MASK, NULL, NULL, GetCasterGUID());
    if (Unit* caster = GetCaster())
        caster->CastSpell(caster, SPELL_PLAGUE_SIPHON, true);

    Remove(AURA_REMOVE_BY_ENEMY_SPELL);
}

void SpellTheLichKingNecroticPlagueJumpAuraScript::Register()
{
    OnEffectApply += AuraEffectApplyFn(SpellTheLichKingNecroticPlagueJumpAuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    AfterEffectRemove += AuraEffectRemoveFn(SpellTheLichKingNecroticPlagueJumpAuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    AfterEffectRemove += AuraEffectRemoveFn(SpellTheLichKingNecroticPlagueJumpAuraScript::OnDispel, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
    AfterEffectApply += AuraEffectRemoveFn(SpellTheLichKingNecroticPlagueJumpAuraScript::AfterDispel, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
}