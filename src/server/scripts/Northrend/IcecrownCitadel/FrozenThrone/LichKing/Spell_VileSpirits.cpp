#include "Spell_TheLichKing.h"

bool SpellTheLichKingVileSpiritsAuraScript::Load()
{
    _is25Man = GetUnitOwner()->GetMap()->Is25ManRaid();
    return true;
}

void SpellTheLichKingVileSpiritsAuraScript::OnPeriodic(AuraEffect const* aurEff)
{
    if (_is25Man || ((aurEff->GetTickNumber() - 1) % 5))
        GetTarget()->CastSpell((Unit*)NULL, GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, true, NULL, aurEff, GetCasterGUID());
}

void SpellTheLichKingVileSpiritsAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellTheLichKingVileSpiritsAuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
}

void SpellTheLichKingVileSpiritsVisualSpellScript::ModDestHeight(SpellEffIndex /*effIndex*/)
{
    Position offset = {0.0f, 0.0f, 15.0f, 0.0f};
    const_cast<WorldLocation*>(GetExplTargetDest())->RelocateOffset(offset);
}

void SpellTheLichKingVileSpiritsVisualSpellScript::Register()
{
    OnEffectLaunch += SpellEffectFn(SpellTheLichKingVileSpiritsVisualSpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_DUMMY);
}

bool SpellTheLichKingVileSpiritMoveTargetSearchSpellScript::Load()
{
    _target = NULL;
    return GetCaster()->GetTypeId() == TYPEID_UNIT;
}

class VileSpiritTargetFilter
{
public:
    bool operator()(WorldObject* target) const
    {
        if (Unit* unit = target->ToUnit())
        {
            if (unit->GetTypeId() != TYPEID_PLAYER)
                return true;

            if (HARVEST_SOUL_CHECK(unit))
                return true;
        }
        return false;
    }
};

void SpellTheLichKingVileSpiritMoveTargetSearchSpellScript::SelectTarget(std::list<WorldObject*>& targets)
{
    if (targets.empty())
        return;

    // Remove targets with debuff harvest souls
    targets.remove_if(VileSpiritTargetFilter());
    if (targets.empty())
        return;

    _target = Trinity::Containers::SelectRandomContainerElement(targets);
}

void SpellTheLichKingVileSpiritMoveTargetSearchSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    // for this spell, all units are in target map, however it should select one to attack
    if (GetHitUnit() != _target)
        return;

    GetCaster()->ToCreature()->AI()->AttackStart(GetHitUnit());
    GetCaster()->AddThreat(GetHitUnit(), 100000.0f);

    GetCaster()->SetSpeed(MOVE_WALK, 0.5f, true);
    GetCaster()->SetSpeed(MOVE_RUN, 0.5f, true);
    GetCaster()->SetSpeed(MOVE_FLIGHT, 0.5f, true);
}

void SpellTheLichKingVileSpiritMoveTargetSearchSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingVileSpiritMoveTargetSearchSpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingVileSpiritMoveTargetSearchSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}

#define TIME_TO_DESPAWN_SPIRIT 3000
bool SpellTheLichKingVileSpiritDamageTargetSearchSpellScript::Load()
{
    return GetCaster()->GetTypeId() == TYPEID_UNIT;
}

void SpellTheLichKingVileSpiritDamageTargetSearchSpellScript::CheckTargetCount(std::list<WorldObject*>& targets)
{
    if (targets.empty())
        return;

    // this spell has SPELL_AURA_BLOCK_SPELL_FAMILY so every next cast of this
    // searcher spell will be blocked
    if (TempSummon* summon = GetCaster()->ToTempSummon())
    {
        if (Unit* summoner = summon->GetSummoner())
            summoner->GetAI()->SetData(DATA_VILE, 1);
    }

    targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
    if (Unit* target = targets.front()->ToUnit())
    {
        if (HARVEST_SOUL_CHECK_N(target))
            GetCaster()->CastSpell((Unit*)NULL, SPELL_SPIRIT_BURST, true);
    }

    GetCaster()->ToCreature()->DespawnOrUnsummon(TIME_TO_DESPAWN_SPIRIT);
    GetCaster()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void SpellTheLichKingVileSpiritDamageTargetSearchSpellScript::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellTheLichKingVileSpiritDamageTargetSearchSpellScript::CheckTargetCount, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
}

void SpellTheLichKingTriggerVileSpiritSpellScript::ActivateSpirit()
{
    Creature* target = GetHitCreature();
    if (!target)
        return;

    VileSpiritActivateEvent(target).Execute(0, 0);
}

void SpellTheLichKingTriggerVileSpiritSpellScript::Register()
{
    OnHit += SpellHitFn(SpellTheLichKingTriggerVileSpiritSpellScript::ActivateSpirit);
}