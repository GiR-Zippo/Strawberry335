#include "Spell_TheLichKing.h"

bool SpellTheLichKingHarvestSoulAuraScript::Load()
{
    return GetOwner()->GetInstanceScript() != NULL;
}

void SpellTheLichKingHarvestSoulAuraScript::OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    // m_originalCaster to allow stacking from different casters, meh
    if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
        GetTarget()->CastSpell((Unit*)NULL, SPELL_HARVESTED_SOUL, true, NULL, NULL, GetTarget()->GetInstanceScript()->GetData64(DATA_THE_LICH_KING));
}

void SpellTheLichKingHarvestSoulAuraScript::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(SpellTheLichKingHarvestSoulAuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
}


bool SpellTheLichKingHarvestSoulTeleportHeroicSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_HARVEST_SOULS_TELEPORT))
        return false;
    return true;
}

void SpellTheLichKingHarvestSoulTeleportHeroicSpellScript::HandleTeleport(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);

    Player* pPlayer = GetCaster()->ToPlayer();
    if (!pPlayer)
        return;

    InstanceScript* instance = pPlayer->GetInstanceScript();
    if (!instance)
        return;

    Creature* pCreature = sObjectAccessor->GetCreature(*pPlayer, instance->GetData64(DATA_THE_LICH_KING));
    if (!pCreature)
        return;

    float portCounter = instance->GetData(DATA_FROSTMOURNE_TELEPORT);
    float coeff = 0.0f;
    bool lastOne = false;

    if (pCreature->AI()->GetData(DATA_ENCOUNTER_DIFFICULTY) == RAID_DIFFICULTY_10MAN_HEROIC)
    {
        if (portCounter == 10)
            lastOne = true;
        coeff = teleportCoef10;
    }
    else
    {
        if (portCounter == 25)
            lastOne = true;
        coeff = teleportCoef25;
    }

    if (!lastOne)
        instance->SetData(DATA_FROSTMOURNE_TELEPORT, portCounter + 1);
    else
        instance->SetData(DATA_FROSTMOURNE_TELEPORT, 1);

    float angle = portCounter * coeff;
    float destX = portPositionX + (cos(angle) * teleportRadius);
    float destY = portPositionY + (sin(angle) * teleportRadius);

    WorldLocation pWLoc(pPlayer->GetMapId());
    pWLoc.Relocate(destX, destY, portPositionZ);
    pWLoc.m_orientation = pWLoc.GetAngle(portPositionX, portPositionY);

    pPlayer->TeleportTo(pWLoc);
}

void SpellTheLichKingHarvestSoulTeleportHeroicSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellTheLichKingHarvestSoulTeleportHeroicSpellScript::HandleTeleport, EFFECT_1, SPELL_EFFECT_TELEPORT_UNITS);
}