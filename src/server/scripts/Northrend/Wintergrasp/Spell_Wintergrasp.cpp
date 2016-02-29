/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 29.07.2014
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

#include "Spell_Wintergrasp.h"
#include "Vehicle.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "BattlegroundSA.h"


Player *CalcPlayerSource(Unit *caster)
{
    if (!caster)
        return NULL;

    Unit *unit = NULL;

    if (Unit *charmer = caster->GetCharmer())
        unit = charmer;
    else if (Vehicle *vehicle = caster->GetVehicleKit())
    {
        if (Unit *passenger = vehicle->GetPassenger(0))
            unit = charmer = passenger;
    }

    return unit ? unit->ToPlayer() : NULL;
}

#define CALC_PLAYER_SOURCE CalcPlayerSource(GetCaster())


/**
 * Calculate damage of vehicle against vehicle/unit/creature/player
 * +Get itemlevel of pPlayer
 * +Calculate factor (if itemlevel > CALC_BASE_ITEMLVL)
 * @param pPlayer : Spell target
 * @param base : Basepoints of the spell
 * @param cFactor : Custom multiply on the end of the calculation
 */
static float CalculateDamage(Player* pPlayer, int32 base, float cFactor = 1.0f)
{
    if (!pPlayer)
        return 1.0f;

    float itemLevel = pPlayer->GetAverageItemLevel();
    float factor = 1.0f;

    if (itemLevel > CALC_BASE_ITEMLVL)
        factor = (((itemLevel - CALC_BASE_ITEMLVL)) + 100) / 100;

    return base * factor * cFactor;
}


void SpellWGCannonSpellScript::RecalculateDamage()
{
    float damage = 0.0f;

    if (const WorldLocation* loc = GetExplTargetDest())
    {
        if (!GetHitUnit())
            return;

        float factor = 1.0f;
        float objectSize = 0.0f;

        // If target is player on vehicle -> set prevent damage
        if (GetHitUnit()->GetVehicle() && GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
        {
            PreventHitDamage();
            return;
        }
        // If target is vehicle -> calc distance
        else if (GetHitUnit()->IsVehicle())
            objectSize = GetHitUnit()->GetObjectSize();
        // Target is nonvehicle -> set damage reduce factor
        else
            factor = CALC_NON_VEHICLE_DAMAGE_FACTOR;

        // Calculate distance factor
        float dist = GetHitUnit()->GetExactDist2d(loc) - objectSize;
        if (dist > 0.0f)
            factor *= CALC_DISTANCE_FACTOR;

        // Manually calculate because basepoints are incorrect
        damage = CalculateDamage(CALC_PLAYER_SOURCE, 4000.0f, factor);
    }

    SetHitDamage(int32(damage));
}

void SpellWGCannonSpellScript::Register()
{
    OnHit += SpellHitFn(SpellWGCannonSpellScript::RecalculateDamage);
}


void SpellWGVehicleRangedSpellScript::RecalculateDamage()
{
    if (const WorldLocation* loc = GetExplTargetDest())
    {
        if (GetHitUnit())
        {
            float damage = 0.0f;
            float factor = 1.0f;
            float objectSize = 0.0f;

            // If target is player on vehicle -> set prevent damage
            if (GetHitUnit()->GetVehicle() && GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
            {
                PreventHitDamage();
                return;
            }
            // If target is vehicle -> calc distance
            else if (GetHitUnit()->IsVehicle())
                objectSize = GetHitUnit()->GetObjectSize();
            // Target is nonvehicle -> set damage reduce factor
            else
                factor = CALC_NON_VEHICLE_DAMAGE_FACTOR;

            // Calculate distance factor
            float dist = GetHitUnit()->GetExactDist2d(loc) - objectSize;
            if (dist > 0.0f)
                factor *= CALC_DISTANCE_FACTOR;

            damage = CalculateDamage(CALC_PLAYER_SOURCE, GetSpellValue()->EffectBasePoints[0], factor);

            SetHitDamage(int32(damage));
        }
    }
}

void SpellWGVehicleRangedSpellScript::Register()
{
    OnHit += SpellHitFn(SpellWGVehicleRangedSpellScript::RecalculateDamage);
}


void SpellWGVehicleMeleeSpellScript::RecalculateDamage()
{
    if (GetHitUnit())
    {
        float damage = 0.0f;
        float factor = 1.0f;

        // If target is player on vehicle -> set prevent damage and effect
        if (GetHitUnit()->GetVehicle() && GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
        {
            PreventHitDamage();
            PreventHitEffect(EFFECT_0);
            return;
        }
        // If target is nonvehicle -> set damage reduce factor
        else if (!GetHitUnit()->IsVehicle())
            factor = CALC_NON_VEHICLE_DAMAGE_FACTOR;

        int32 basePoints = GetSpellInfo()->Id == SPELL_STEAM_RUSH ? GetSpellInfo()->Effects[2].CalcValue() : GetSpellInfo()->Effects[1].CalcValue();
        damage = CalculateDamage(CALC_PLAYER_SOURCE, basePoints, factor);

        SetHitDamage(int32(damage));
    }
}

void SpellWGVehicleMeleeSpellScript::Register()
{
    OnHit += SpellHitFn(SpellWGVehicleMeleeSpellScript::RecalculateDamage);
}


void SpellWGCatapultPlagueSlimeAuraScript::OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (GetTarget())
        pUnit = GetTarget();
}

void SpellWGCatapultPlagueSlimeAuraScript::RecalculateEffectAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
{
    canBeRecalculated = false;

    if (pUnit)
    {
        float factor = 1.0f;

        // If target is player on vehicle -> set prevent action
        if (pUnit->GetVehicle() && pUnit->GetTypeId() == TYPEID_PLAYER)
        {
            PreventDefaultAction();
            return;
        }
        // If target is nonvehicle -> set damage reduce factor
        else if (!pUnit->IsVehicle())
            factor = CALC_NON_VEHICLE_DAMAGE_FACTOR;

        float damage = CalculateDamage(CALC_PLAYER_SOURCE, this->GetSpellInfo()->Effects[2].BasePoints, factor);

        amount = int32(damage);
    }
}

void SpellWGCatapultPlagueSlimeAuraScript::Register()
{
    OnEffectApply += AuraEffectApplyFn(SpellWGCatapultPlagueSlimeAuraScript::OnApply, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    DoEffectCalcAmount += AuraEffectCalcAmountFn(SpellWGCatapultPlagueSlimeAuraScript::RecalculateEffectAmount, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
}


void SpellWGCatapultFlameBreathSpellScript::RecalculateDamage()
{
    if (GetHitUnit())
    {
        float factor = 1.0f;

        // If target is player on vehicle -> set prevent action
        if (GetHitUnit()->GetVehicle() && GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
        {
            PreventHitDamage();
            return;
        }
        // If target is nonvehicle -> set damage reduce factor
        else if (!GetHitUnit()->IsVehicle())
            factor = CALC_NON_VEHICLE_DAMAGE_FACTOR;

        float damage = CalculateDamage(CALC_PLAYER_SOURCE, GetSpellValue()->EffectBasePoints[0], factor);
        damage /= 4.0f;

        SetHitDamage(int32(damage));
    }
}

void SpellWGCatapultFlameBreathSpellScript::Register()
{
    OnHit += SpellHitFn(SpellWGCatapultFlameBreathSpellScript::RecalculateDamage);
}


bool SpellWGForceBuildingSpellScript::Validate(SpellInfo const* /*spell*/)
{
    if (!sSpellMgr->GetSpellInfo(SPELL_WINTERGRASP_BUILD_CATAPULT_FORCE)
        || !sSpellMgr->GetSpellInfo(SPELL_WINTERGRASP_BUILD_DEMOLISHER_FORCE)
        || !sSpellMgr->GetSpellInfo(SPELL_WINTERGRASP_BUILD_SIEGE_VEHICLE_FORCE_HORDE)
        || !sSpellMgr->GetSpellInfo(SPELL_WINTERGRASP_BUILD_SIEGE_VEHICLE_FORCE_ALLIANCE))
        return false;
    return true;
}

void SpellWGForceBuildingSpellScript::HandleScript(SpellEffIndex effIndex)
{
    PreventHitDefaultEffect(effIndex);
    GetHitUnit()->CastSpell(GetHitUnit(), GetEffectValue(), false);
}

void SpellWGForceBuildingSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellWGForceBuildingSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}


void SpellWGGrabPassengerSpellScript::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (Player* pPlayer = GetHitPlayer())
        pPlayer->CastSpell(GetCaster(), SPELL_RIDE_WG_VEHICLE, true);
}

void SpellWGGrabPassengerSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellWGGrabPassengerSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}


SpellCastResult SpellWGDefenderTeleportSpellScript::CheckCast()
{
    if (Player* pPlayer = GetCaster()->ToPlayer())
    {
        if (pPlayer->HasAura(SPELL_WINTERGRASP_DEFENDER_TELEPORT))
            return SPELL_FAILED_CASTER_AURASTATE;

        // Wintergrasp handle
        if (pPlayer->GetZoneId() == 4197)
            if (Battlefield* pBattlefield = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
                if (pPlayer->GetTeamId() != pBattlefield->GetDefenderTeam())
                    return SPELL_FAILED_TARGET_ENEMY;

        // SotA handle
        if (pPlayer->GetZoneId() == 4384)
            if (Battleground* pBattleground = pPlayer->GetBattleground())
                if (pPlayer->GetTeamId() == pBattleground->GetData(BG_SA_DATA_ATTACKER_TEAM))
                    return SPELL_FAILED_TARGET_ENEMY;

        return SPELL_CAST_OK;
    }

    return SPELL_FAILED_BAD_TARGETS;
}

void SpellWGDefenderTeleportSpellScript::HandleDummy(SpellEffIndex /*effindex*/)
{
    if (Unit* pUnit = GetHitUnit())
    {
        WorldLocation loc;
        pUnit->GetPosition(&loc);
        SetExplTargetDest(loc);
    }
}

void SpellWGDefenderTeleportSpellScript::Register()
{
    OnCheckCast += SpellCheckCastFn(SpellWGDefenderTeleportSpellScript::CheckCast);
    OnEffectHitTarget += SpellEffectFn(SpellWGDefenderTeleportSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
}


void SpellWGPortalTeleportSpellScript::HandleScript(SpellEffIndex /*effindex*/)
{
    // This spell can be only used by player
    if (Player* pPlayer = GetHitUnit()->ToPlayer())
    {
        if (Battlefield* Bf = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
        {
            if (Bf->CanPlayerJoinWar(pPlayer))
            {
                pPlayer->AddAura(SPELL_TELEPORT_DALARAN_TO_WG, pPlayer);

                if (pPlayer->GetTeamId() == Bf->GetDefenderTeam())
                {
                    pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_FORTRESS, true);
                    return;
                }
                else if (Bf->IsWarTime())
                {
                    if (pPlayer->GetTeamId() == TEAM_ALLIANCE)
                        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_CAMP_ALLIANCE, true);
                    else
                        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_CAMP_HORDE, true);
                    return;
                }
            }
        }

        pPlayer->GetSession()->SendNotification(LANG_WG_CANNOT_JOIN_BATTLE);
        PreventHitDefaultEffect(EFFECT_0);
    }
}

void SpellWGPortalTeleportSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellWGPortalTeleportSpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}


SpellCastResult SpellWGVehicleTeleportSpellScript::CheckCast()
{
    if (Battlefield* Bf = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
    {
        if (Unit* pUnit = GetExplTargetUnit())
        {
            if (pUnit->GetTypeId() != TYPEID_PLAYER)
                return SPELL_FAILED_BAD_TARGETS;

            Player* pPlayer = pUnit->ToPlayer();
            if (!pPlayer)
                return SPELL_FAILED_BAD_TARGETS;

            if (!pPlayer->GetVehicle())
                return SPELL_FAILED_DONT_REPORT;

            if ((pPlayer->GetZoneId() == 4197 && pPlayer->GetTeamId() != Bf->GetDefenderTeam())
                || pPlayer->HasAura(SPELL_WINTERGRASP_VEHICLE_TELEPORT))
                return SPELL_FAILED_CASTER_AURASTATE;
            else
                return SPELL_CAST_OK;
        }
    }

    return SPELL_FAILED_ERROR;
}

void SpellWGVehicleTeleportSpellScript::HandleTeleport(SpellEffIndex /*effindex*/)
{
    PreventHitDefaultEffect(EFFECT_0);

    Unit* pUnit = GetHitUnit();
    if (!pUnit)
        return;

    std::list<Creature*> pCreatureList;
    pUnit->GetCreatureListWithEntryInGrid(pCreatureList, NPC_WORLD_TRIGGER_LARGE_AOI_NOT_IMMUNE_PC_NPC, 100.0f);
    if (pCreatureList.empty())
        return;

    for (std::list<Creature*>::const_iterator itr = pCreatureList.begin(); itr != pCreatureList.end(); ++itr)
    {
        if ((*itr))
        {
            if ((*itr)->GetDistance2d(pUnit) >= 60.0f)
            {
                ExitVehicleAndTeleport(pUnit->GetVehicle(), *itr);
                break;
            }
        }
    }
}

void SpellWGVehicleTeleportSpellScript::ExitVehicleAndTeleport(Vehicle* pVehicle, Creature* pCreature)
{
    // Get vehicle passengers
    SeatMap passengerMap = pVehicle->Seats;
    if (passengerMap.empty())
        return;

    Position triggerPos;
    triggerPos.Relocate(pCreature);

    // Remove passengers
    pVehicle->RemoveAllPassengers();

    // Get vehicle owner
    Player* pPlayer = sObjectAccessor->FindPlayer(passengerMap.begin()->second.Passenger);
    if (!pPlayer)
        return;

    // Summon new vehicle
    Creature* pNewVehicle = pPlayer->SummonCreature(pVehicle->GetBase()->GetEntry(), triggerPos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10 * IN_MILLISECONDS);
    if (!pNewVehicle)
        return;

    // Teleport players and add to vehicle
    for (SeatMap::const_iterator itr = passengerMap.begin(); itr != passengerMap.end(); ++itr)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(itr->second.Passenger))
            pNewVehicle->CastSpell(pPlayer, SPELL_GRAB_PASSENGER, true);
    }

    // Despawn vehicle
    pVehicle->GetBase()->ToCreature()->DespawnOrUnsummon();
}

void SpellWGVehicleTeleportSpellScript::Register()
{
    OnCheckCast += SpellCheckCastFn(SpellWGVehicleTeleportSpellScript::CheckCast);
    OnEffectHitTarget += SpellEffectFn(SpellWGVehicleTeleportSpellScript::HandleTeleport, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
}


void SpellWGDriveVehicleAuraScript::HandleAfterEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
{
    if (Unit* pUnit = GetTarget())
    {
        if (aurEff->GetEffIndex() == EFFECT_0)
        {
            switch (pUnit->GetEntry())
            {
                case BATTLEFIELD_WG_TOWER_CANNON:
                    // If its a turret -> root it and disable jump
                    pUnit->AddAura(42716, pUnit);
                    pUnit->AddUnitMovementFlag(MOVEMENTFLAG_HOVER);
                    // No break -> cannons should also be immune against spell effects beneath
                case BATTLEFIELD_WG_CATAPULT:
                case BATTLEFIELD_WG_DEMOLISHER:
                case BATTLEFIELD_WG_SIEGE_ENGINE_HORDE:
                case BATTLEFIELD_WG_SIEGE_ENGINE_ALLIANCE:
                case BATTLEFIELD_WG_SIEGE_ENGINE_CANNON_HORDE:
                case BATTLEFIELD_WG_SIEGE_ENGINE_CANNON_ALLIANCE:
                {
                    // Immunity mask of bg vehicles
                    for (uint8 i = 0; i < (sizeof(BattlegroundVehicleSpellImmunity) / sizeof(uint32)); ++i)
                        pUnit->ApplySpellImmune(0, IMMUNITY_EFFECT, BattlegroundVehicleSpellImmunity[i], true);
                }break;
            }

            // Remove unit flag not selectable (Set in OnCreatureCreate in WG script)
            pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            // If its a player on catapult -> set not attackable
            if (pUnit->GetEntry() == BATTLEFIELD_WG_CATAPULT && pUnit->GetVehicleKit()->GetPassenger(0))
                pUnit->GetVehicleKit()->GetPassenger(0)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
        else if (aurEff->GetEffIndex() == EFFECT_1)
        {
            // Remove scaling of player
            if (pUnit->GetTypeId() == TYPEID_PLAYER)
                pUnit->RemoveAurasDueToSpell(WG_SPELL_VEHICLE_SCALING);
        }
    }
}

void SpellWGDriveVehicleAuraScript::HandleAfterEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
{
    if (Unit* pUnit = GetTarget())
    {
        Player* pPlayerOwner = sObjectAccessor->GetPlayer(*pUnit, pUnit->GetCreatorGUID());
        if (!pPlayerOwner)
            pUnit->Kill(pUnit);
        else
        {
            switch (pUnit->GetEntry())
            {
                case BATTLEFIELD_WG_CATAPULT:
                {
                    // Remove not attackable flag of player
                    pPlayerOwner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    // No break -> catapults should also get killed after player gets out
                }
                case BATTLEFIELD_WG_DEMOLISHER:
                case BATTLEFIELD_WG_SIEGE_ENGINE_HORDE:
                case BATTLEFIELD_WG_SIEGE_ENGINE_ALLIANCE:
                {
                    // If owner gets out -> kill it
                    if (Vehicle* pVehicle = pUnit->GetVehicleKit())
                        if (!pVehicle->IsPassenger(pPlayerOwner) && !pUnit->isDead())
                            pUnit->Kill(pUnit);
                }break;
            }
        }
    }
}

void SpellWGDriveVehicleAuraScript::Register()
{
    AfterEffectApply += AuraEffectApplyFn(SpellWGDriveVehicleAuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    AfterEffectApply += AuraEffectApplyFn(SpellWGDriveVehicleAuraScript::HandleAfterEffectApply, EFFECT_1, SPELL_AURA_LINKED, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    AfterEffectRemove += AuraEffectRemoveFn(SpellWGDriveVehicleAuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
}


void SpellWGRocketLauncherSpellScript::HandleDummy(SpellEffIndex /*effindex*/)
{
    if (Unit* pUnit = GetHitUnit())
        GetCaster()->CastSpell(pUnit, WG_SPELL_ROCKET_LAUNCHER_TRIGGER, false);
}

void SpellWGRocketLauncherSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SpellWGRocketLauncherSpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
}