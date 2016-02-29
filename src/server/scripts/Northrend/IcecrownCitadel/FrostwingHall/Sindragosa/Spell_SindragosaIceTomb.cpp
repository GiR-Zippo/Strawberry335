#include "Spell_Sindragosa.h"

bool SpellSindragosaIceTombAuraScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sObjectMgr->GetCreatureTemplate(NPC_ICE_TOMB))
        return false;
    if (!sObjectMgr->GetGameObjectTemplate(GO_ICE_BLOCK))
        return false;
    return true;
}

bool SpellSindragosaIceTombAuraScript::Load()
{
    _apply = true;
    return true;
}

void SpellSindragosaIceTombAuraScript::PeriodicTick(AuraEffect const* /*aurEff*/)
{
    PreventDefaultAction();
    Unit *caster = GetCaster();

    if (_apply && caster)
    {
        Position pos;
        GetTarget()->GetPosition(&pos);
        if (TempSummon* summon = caster->SummonCreature(NPC_ICE_TOMB, pos, TEMPSUMMON_DEAD_DESPAWN))
        {
            summon->SetIgnoreLoS(true);
            summon->AI()->SetGUID(GetTarget()->GetGUID(), DATA_TRAPPED_PLAYER);
            GetTarget()->CastSpell(GetTarget(), SPELL_ICE_TOMB_UNTARGETABLE);
            GetTarget()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            if (GameObject* go = summon->ToCreature()->SummonGameObject(GO_ICE_BLOCK, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 0))
            {
                go->SetSpellId(SPELL_ICE_TOMB_DAMAGE);
                summon->AddGameObject(go);
                go->SetGoState(GO_STATE_READY);
                go->EnableCollision(true);
                go->setActive(true);
            }
        }
        _apply = false;
    }
}

void SpellSindragosaIceTombAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(SpellSindragosaIceTombAuraScript::PeriodicTick, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}