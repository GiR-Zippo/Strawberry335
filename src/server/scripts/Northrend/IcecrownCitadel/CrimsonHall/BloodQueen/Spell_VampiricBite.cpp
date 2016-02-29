#include "Spell_BloodQueenLanaThel.h"

bool IsVampire(Unit const* unit)
{
    for (uint8 i = 0; i < 3; ++i)
        if (unit->HasAura(vampireAuras[i][unit->GetMap()->GetSpawnMode()]))
            return true;
    return false;
}

bool SpellBloodQueenVampiricBiteSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_ESSENCE_OF_THE_BLOOD_QUEEN_PLR))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_FRENZIED_BLOODTHIRST))
        return false;
    if (!sSpellMgr->GetSpellInfo(SPELL_PRESENCE_OF_THE_DARKFALLEN))
        return false;
    return true;
}

SpellCastResult SpellBloodQueenVampiricBiteSpellScript::CheckTarget()
{
    if (IsVampire(GetExplTargetUnit()))
    {
        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CANT_TARGET_VAMPIRES);
        return SPELL_FAILED_CUSTOM_ERROR;
    }

    return SPELL_CAST_OK;
}

void SpellBloodQueenVampiricBiteSpellScript::OnCast()
{
    if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
        return;

    uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_FRENZIED_BLOODTHIRST, GetCaster());
    GetCaster()->RemoveAura(spellId, 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);
    GetCaster()->CastSpell(GetCaster(), SPELL_ESSENCE_OF_THE_BLOOD_QUEEN_PLR, true);
    // Presence of the Darkfallen buff on Blood-Queen
    if (GetCaster()->GetMap()->IsHeroic() && GetCaster()->GetInstanceScript())
        if (Creature* LanaThel = ObjectAccessor::GetCreature(*GetCaster(), GetCaster()->GetInstanceScript()->GetData64(DATA_BLOOD_QUEEN_LANA_THEL)))
            if (Aura* pAura = LanaThel->GetAura(SPELL_PRESENCE_OF_THE_DARKFALLEN))
                LanaThel->GetAura(SPELL_PRESENCE_OF_THE_DARKFALLEN)->ModStackAmount(1);
            

    // Shadowmourne questline
    if (GetCaster()->ToPlayer()->GetQuestStatus(QUEST_BLOOD_INFUSION) == QUEST_STATUS_INCOMPLETE)
    {
        if (Aura* aura = GetCaster()->GetAura(SPELL_GUSHING_WOUND))
        {
            if (aura->GetStackAmount() == 3)
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_THIRST_QUENCHED, true);
                GetCaster()->RemoveAura(aura);
            }
            else
                GetCaster()->CastSpell(GetCaster(), SPELL_GUSHING_WOUND, true);
        }
    }
    if (InstanceScript* instance = GetCaster()->GetInstanceScript())
        if (Creature* bloodQueen = ObjectAccessor::GetCreature(*GetCaster(), instance->GetData64(DATA_BLOOD_QUEEN_LANA_THEL)))
            bloodQueen->AI()->SetGUID(GetHitUnit()->GetGUID(), GUID_VAMPIRE);
}

void SpellBloodQueenVampiricBiteSpellScript::Register()
{
    OnCheckCast += SpellCheckCastFn(SpellBloodQueenVampiricBiteSpellScript::CheckTarget);
    BeforeHit += SpellHitFn(SpellBloodQueenVampiricBiteSpellScript::OnCast);
}