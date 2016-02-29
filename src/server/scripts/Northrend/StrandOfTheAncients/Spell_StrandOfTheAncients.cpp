/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 17.08.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#include "Spell_StrandOfTheAncients.h"
#include "BattlegroundSA.h"


void SpellSotACarryingSeaforiumAuraScript::HandleAfterEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    // Only proceed if carrier is dead
    if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
    {
        Player* pPlayer = GetCaster()->ToPlayer();
        if (!pPlayer)
            return;

        // Check if player is in battleground (SotA)
        if (!pPlayer->InBattleground())
            return;

        // Destroy item in inventory
        pPlayer->DestroyItemCount(ITEM_MASSIVE_SEAFORIUM_CHARGE, 1, true);
    }
}

void SpellSotACarryingSeaforiumAuraScript::Register()
{
    AfterEffectRemove += AuraEffectRemoveFn(SpellSotACarryingSeaforiumAuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
}


void SpellSotADriveVehicleAuraScript::HandleAfterEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
{
    if (Unit* pUnit = GetTarget())
    {
        if (aurEff->GetEffIndex() == EFFECT_0)
        {
            switch (pUnit->GetEntry())
            {
                case NPC_DEMOLISHER_SA:
                {
                    // Immunity mask of bg vehicles
                    for (uint8 i = 0; i < (sizeof(BattlegroundVehicleSpellImmunity) / sizeof(uint32)); ++i)
                        pUnit->ApplySpellImmune(0, IMMUNITY_EFFECT, BattlegroundVehicleSpellImmunity[i], true);
                }break;
            }
        }
        else if (aurEff->GetEffIndex() == EFFECT_1)
        {
            // Remove scaling of player
            if (pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->RemoveAurasDueToSpell(SOTA_SPELL_VEHICLE_SCALING);
        }
    }
}

void SpellSotADriveVehicleAuraScript::Register()
{
    AfterEffectApply += AuraEffectApplyFn(SpellSotADriveVehicleAuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    AfterEffectApply += AuraEffectApplyFn(SpellSotADriveVehicleAuraScript::HandleAfterEffectApply, EFFECT_1, SPELL_AURA_LINKED, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
}


void SpellSotAEndOfRoundSpellScript::HandleScriptEffect(SpellEffIndex /*effIndex*/)
{
    if (Unit* pUnit = GetCaster())
    {
        if (Player* pPlayer = pUnit->ToPlayer())
        {
            bool wasAlive = pPlayer->isAlive();
            pPlayer->ResurrectPlayer(1.0f);

            if (!wasAlive)
            {
                pPlayer->SpawnCorpseBones();
                pPlayer->AddAura(GetSpellInfo()->Id, pPlayer);
            }
            else
            {
                pPlayer->RemoveAllAurasOnDeath();
                pPlayer->ResetAllPowers();
                pPlayer->CombatStopWithPets(true);
                pPlayer->ExitVehicle();
            }

            pPlayer->RemoveAllSpellCooldown();
        }
    }
}

void SpellSotAEndOfRoundSpellScript::Register()
{
    OnEffectHit += SpellEffectFn(SpellSotAEndOfRoundSpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
}