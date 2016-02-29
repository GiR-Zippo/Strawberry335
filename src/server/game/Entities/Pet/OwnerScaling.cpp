/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 28.02.2015
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
 * @copyright Copyright 2015 Laenalith-WoW. All rights reserved.
 */

#include "OwnerScaling.h"
#include "IOwnerStats.h"

PreviouslyUsedContainerMap OwnerScaling::PreviouslyUsedContainerStorage;


#pragma region IOwnerStatsCalc
float IOwnerStatsCalc::GetOwnerStat(Stats stat, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerStat called with invalid source");
        return 0.0f;
    }

    float bonusStat = source->GetStat(stat);

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerStat: Unit %u bonus of stat %u is %f",
                   source->GetGUIDLow(), stat, bonusStat);

    // Return value
    return bonusStat;
}

float IOwnerStatsCalc::GetOwnerCombatRating(CombatRating cr, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerCombatRating called with invalid source");
        return 0.0f;
    }

    float bonusCombatRating = 0.0f;

    if (Player* pPlayer = source->ToPlayer())
        bonusCombatRating = pPlayer->GetRatingBonusValue(cr);

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerCombatRating: Unit %u bonus of CombatRating %u is %f",
                   source->GetGUIDLow(), cr, bonusCombatRating);

    // Return value
    return bonusCombatRating;
}

float IOwnerStatsCalc::GetOwnerResistance(SpellSchoolMask schoolMask, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerResistance called with invalid source");
        return 0.0f;
    }

    float bonusResistance = source->GetResistance(schoolMask);

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerResistance: Unit %u bonus of schoolMask %u is %f",
                   source->GetGUIDLow(), schoolMask, bonusResistance);

    // Return value
    return bonusResistance;
}

float IOwnerStatsCalc::GetOwnerAttackPower(WeaponAttackType type, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerAttackPower called with invalid source");
        return 0.0f;
    }

    float bonusAttackPower = source->GetTotalAttackPowerValue(type);

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerAttackPower: Unit %u bonus is %f",
                   source->GetGUIDLow(), bonusAttackPower);

    // Return value
    return bonusAttackPower;
}

float IOwnerStatsCalc::GetOwnerSpellPower(SpellSchools school, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerSpellPower called with invalid source");
        return 0.0f;
    }

    float bonusSpellPower = 0.0f;

    if (Player* pPlayer = source->ToPlayer())
        bonusSpellPower = float(pPlayer->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school) - pPlayer->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + school));

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerSpellPower: Unit %u bonus of school %u is %f",
                   source->GetGUIDLow(), school, bonusSpellPower);

    // Return value
    return bonusSpellPower;
}


float IOwnerStatsCalc::GetOwnerPermanentStat(Stats stat, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerPermanentStat called with invalid source");
        return 0.0f;
    }

    float bonusStat = 0.0f;

    if (stat < MAX_STATS)
    {
        float factor = 1.0f;

        // Get stat value
        bonusStat += source->GetStat(stat);

        // Remove aura modifiers
        factor = source->GetTotalAuraMultiplierByMiscValue(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, -1);
        factor += (source->GetTotalAuraMultiplierByMiscValue(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, stat) - 1.0f);
        if (factor > 1.0f)
            bonusStat /= factor;

        bonusStat -= source->GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_STAT, stat);

        // Add base aura modifiers
        factor = source->GetBaseAuraMultiplierByMiscValue(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, -1, false);
        factor += (source->GetBaseAuraMultiplierByMiscValue(SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, stat, false) - 1.0f);
        bonusStat += source->GetBaseAuraModifierByMiscValue(SPELL_AURA_MOD_STAT, stat, false);

        if (factor > 1.0f)
            bonusStat *= factor;
    }

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerPermanentStat: Unit %u bonus of stat %u is %f",
                   source->GetGUIDLow(), stat, bonusStat);

    // Return value
    return bonusStat;
}

float IOwnerStatsCalc::GetOwnerPermanentCombatRating(CombatRating cr, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerPermanentCombatRating called with invalid source");
        return 0.0f;
    }

    float bonusCombatRating = 0.0f;

    if (Player* pPlayer = source->ToPlayer())
    {
        float factor = 1.0f;
        uint32 maskCombatRating = (1 << cr);

        typedef std::vector<AuraType> AuraTypeVector;
        AuraTypeVector auraModifiers;
        AuraTypeVector auraMultipliers;

        // Select AuraType modifiers
        switch (cr)
        {
            case CR_HIT_MELEE:
            case CR_HIT_RANGED:
                auraModifiers.push_back(SPELL_AURA_MOD_HIT_CHANCE);
                break;

            case CR_HIT_SPELL:
                auraModifiers.push_back(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                break;

            case CR_CRIT_MELEE:
            case CR_CRIT_RANGED:
                bonusCombatRating += pPlayer->GetMeleeCritFromAgility(true);
                auraMultipliers.push_back(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
                auraMultipliers.push_back(SPELL_AURA_MOD_CRIT_PCT);
                break;

            case CR_CRIT_SPELL:
                bonusCombatRating += pPlayer->GetSpellCritFromIntellect(true);
                auraModifiers.push_back(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
                auraMultipliers.push_back(SPELL_AURA_MOD_CRIT_PCT);
                break;

            case CR_HASTE_MELEE:
                auraModifiers.push_back(SPELL_AURA_MOD_MELEE_HASTE);
                break;

            case CR_HASTE_RANGED:
                auraModifiers.push_back(SPELL_AURA_MOD_RANGED_HASTE);
                break;

            case CR_HASTE_SPELL:
                // No Aura
                break;

            case CR_EXPERTISE:
                auraModifiers.push_back(SPELL_AURA_MOD_EXPERTISE);
                break;

            default:
                return bonusCombatRating;
        }

        // Get rating
        bonusCombatRating += pPlayer->GetRatingBonusValue(cr);

        // Remove rating modifiers
        bonusCombatRating -= (pPlayer->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_RATING, maskCombatRating) * pPlayer->GetRatingMultiplier(cr));

        // Add base aura modifiers/multipliers
        bonusCombatRating += (pPlayer->GetBaseAuraModifierByMiscMask(SPELL_AURA_MOD_RATING, maskCombatRating, false) * pPlayer->GetRatingMultiplier(cr));

        if (!auraModifiers.empty())
            for (AuraTypeVector::const_iterator itr = auraModifiers.begin(); itr != auraModifiers.end(); ++itr)
                bonusCombatRating += pPlayer->GetBaseAuraModifier(*itr, false);

        if (!auraMultipliers.empty())
            for (AuraTypeVector::const_iterator itr = auraMultipliers.begin(); itr != auraMultipliers.end(); ++itr)
                factor += CalculatePctF(1.0f, pPlayer->GetBaseAuraMultiplier(*itr, false));

        if (factor > 1.0f)
            bonusCombatRating *= factor;
    }

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerPermanentCombatRating: Unit %u bonus of CombatRating %u is %f",
                   source->GetGUIDLow(), cr, bonusCombatRating);

    // Return value
    return bonusCombatRating;
}

float IOwnerStatsCalc::GetOwnerPermanentResistance(SpellSchoolMask schoolMask, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerPermanentResistance called with invalid source");
        return 0.0f;
    }

    float bonusResistance = 0.0f;
    float factor = 1.0f;

    // Get resistance
    bonusResistance = source->GetResistance(schoolMask);

    // Remove aura modifiers
    factor = source->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_RESISTANCE_PCT, schoolMask);
    factor += (source->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_BASE_RESISTANCE_PCT, schoolMask) - 1.0f);
    if (factor > 1.0f)
        bonusResistance /= factor;

    bonusResistance -= source->GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_RESISTANCE, schoolMask);
    bonusResistance -= source->GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE, schoolMask);

    // Add base aura modifiers
    factor = source->GetBaseAuraMultiplierByMiscMask(SPELL_AURA_MOD_RESISTANCE_PCT, schoolMask, false);
    factor += (source->GetBaseAuraMultiplierByMiscMask(SPELL_AURA_MOD_BASE_RESISTANCE_PCT, schoolMask, false) - 1.0f);
    bonusResistance += source->GetBaseAuraModifierByMiscValue(SPELL_AURA_MOD_RESISTANCE, schoolMask, false);
    bonusResistance += source->GetBaseAuraModifierByMiscValue(SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE, schoolMask, false);

    if (factor > 1.0f)
        bonusResistance *= factor;

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerPermanentResistance: Unit %u bonus of schoolMask %u is %f",
                   source->GetGUIDLow(), schoolMask, bonusResistance);

    // Return value
    return bonusResistance;
}

float IOwnerStatsCalc::GetOwnerPermanentAttackPower(WeaponAttackType type, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerPermanentAttackPower called with invalid source");
        return 0.0f;
    }

    float bonusAttackPower = 0.0f;
    float negAttackPower = 0.0f;

    // Select AuraType modifiers
    AuraType auraType[2] = { TOTAL_AURAS, TOTAL_AURAS };

    switch (type)
    {
        case BASE_ATTACK:
        case OFF_ATTACK:
            auraType[0] = SPELL_AURA_MOD_ATTACK_POWER;
            auraType[1] = SPELL_AURA_MOD_ATTACK_POWER_PCT;
            break;

        case RANGED_ATTACK:
            auraType[0] = SPELL_AURA_MOD_RANGED_ATTACK_POWER;
            auraType[1] = SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT;
            break;

        default:
            return bonusAttackPower;
    }

    // Get attack power
    bonusAttackPower = source->GetTotalAttackPowerValue(type);
    float factor = 1.0f;

    // Remove modifier of stats which affect attack power
    switch (source->getClass())
    {
        case CLASS_HUNTER:
            negAttackPower = float(source->getLevel() * 2.0f + source->GetStat(STAT_AGILITY) - 10.0f)
                - float(source->getLevel() * 2.0f + GetOwnerStat(STAT_AGILITY, source) - 10.0f);
            break;

        case CLASS_DEATH_KNIGHT:
            negAttackPower = float(source->getLevel() * 3.0f + (source->GetStat(STAT_STRENGTH) * 2.0f) - 20.0f)
                - float(source->getLevel() * 3.0f + (GetOwnerStat(STAT_STRENGTH, source) * 2.0f) - 20.0f);
            break;
    }

    // Remove aura modifiers
    factor = source->GetTotalAuraMultiplier(auraType[1]);
    if (factor > 1.0f)
        bonusAttackPower /= factor;

    bonusAttackPower -= negAttackPower;
    bonusAttackPower -= source->GetTotalAuraModifier(auraType[0]);

    // Add base aura modifiers
    factor = source->GetBaseAuraMultiplier(auraType[1], false);
    bonusAttackPower += source->GetBaseAuraModifier(auraType[0], false);

    if (factor > 1.0f)
        bonusAttackPower *= factor;

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerPermanentAttackPower: Unit %u bonus is %f",
                   source->GetGUIDLow(), bonusAttackPower);

    // Return value
    return bonusAttackPower;
}

float IOwnerStatsCalc::GetOwnerPermanentSpellPower(SpellSchools school, Unit* source)
{
    if (!source)
    {
        sLog->outError("IOwnerStatsCalc::GetOwnerPermanentSpellPower called with invalid source");
        return 0.0f;
    }

    float bonusSpellPower = 0.0f;

    if (Player* pPlayer = source->ToPlayer())
    {
        float factor = 1.0f;

        uint32 schoolMask = (1 << school);

        // Get spell power
        bonusSpellPower += pPlayer->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school);
        bonusSpellPower -= pPlayer->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + school);

        // Remove aura modifiers
        factor = pPlayer->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, schoolMask);
        if (factor > 1.0f)
            bonusSpellPower /= factor;

        bonusSpellPower -= pPlayer->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_DAMAGE_DONE, schoolMask);
        bonusSpellPower -= pPlayer->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_DAMAGE_OF_STAT_PERCENT, schoolMask);

        // Add base aura modifiers
        factor = pPlayer->GetBaseAuraMultiplierByMiscMask(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, schoolMask, false);
        bonusSpellPower += pPlayer->GetBaseAuraModifierByMiscMask(SPELL_AURA_MOD_DAMAGE_DONE, schoolMask, false);
        bonusSpellPower += pPlayer->GetBaseAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_DAMAGE_OF_STAT_PERCENT, schoolMask, false);

        if (factor > 1.0f)
            bonusSpellPower *= factor;
    }

    sLog->outDebug(LOG_FILTER_PETS, "IOwnerStatsCalc::GetOwnerPermanentSpellPower: Unit %u bonus of school %u is %f",
                   source->GetGUIDLow(), school, bonusSpellPower);

    // Return value
    return bonusSpellPower;
}
#pragma endregion


#pragma region GuardianIOwnerStats
float Guardian::GetOwnerStat(Stats stat) 
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_UNIT_MOD, stat), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerStat(stat, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_UNIT_MOD, stat), bonus);
    }
    return bonus;
}

float Guardian::GetOwnerCombatRating(CombatRating cr)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_COMBAT_RATING, cr), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerCombatRating(cr, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_COMBAT_RATING, cr), bonus);
    }
    return bonus;
}

float Guardian::GetOwnerResistance(SpellSchoolMask schoolMask)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_RESISTANCE, schoolMask), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerResistance(schoolMask, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_RESISTANCE, schoolMask), bonus);
    }
    return bonus;
}

float Guardian::GetOwnerAttackPower(WeaponAttackType type)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_ATTACK_POWER, type), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerAttackPower(type, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_ATTACK_POWER, type), bonus);
    }
    return bonus;
}

float Guardian::GetOwnerSpellPower(SpellSchools school)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_SPELL_POWER, school), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerSpellPower(school, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_SPELL_POWER, school), bonus);
    }
    return bonus;
}

void Guardian::AddValidStat(IdentifierStatPair item, float value)
{
    previosValidStatStorage[item] = TimeValuePair(getMSTime(), value);
}

bool Guardian::GetPreviousValidStat(IdentifierStatPair item, float& value)
{
    if (previosValidStatStorage.count(item))
    {
        if (GetMSTimeDiffToNow(previosValidStatStorage[item].first) < IOwnerStats::ValidStatTime)
        {
            value = previosValidStatStorage[item].second;
            return true;
        }
    }
    return false;
}
#pragma endregion


#pragma region PetIOwnerStats
float Pet::GetOwnerStat(Stats stat)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_UNIT_MOD, stat), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerPermanentStat(stat, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_UNIT_MOD, stat), bonus);
    }
    return bonus;
}

float Pet::GetOwnerCombatRating(CombatRating cr)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_COMBAT_RATING, cr), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerPermanentCombatRating(cr, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_COMBAT_RATING, cr), bonus);
    }
    return bonus;
}

float Pet::GetOwnerResistance(SpellSchoolMask schoolMask)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_RESISTANCE, schoolMask), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerPermanentResistance(schoolMask, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_RESISTANCE, schoolMask), bonus);
    }
    return bonus;
}

float Pet::GetOwnerAttackPower(WeaponAttackType type)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_ATTACK_POWER, type), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerPermanentAttackPower(type, GetOwner());
        AddValidStat(std::make_pair(STAT_IDENTIFIER_ATTACK_POWER, type), bonus);
    }
    return bonus;
}

float Pet::GetOwnerSpellPower(SpellSchools school)
{
    float bonus = 0.0f;
    if (!GetPreviousValidStat(std::make_pair(STAT_IDENTIFIER_SPELL_POWER, school), bonus))
    {
        bonus = IOwnerStatsCalc::GetOwnerPermanentSpellPower(school, GetOwner());
        AddValidStat(IdentifierStatPair(STAT_IDENTIFIER_SPELL_POWER, school), bonus);
    }
    return bonus;
}

void Pet::AddValidStat(IdentifierStatPair item, float value)
{
    Guardian::AddValidStat(item, value);
}

bool Pet::GetPreviousValidStat(IdentifierStatPair item, float& value)
{
    return Guardian::GetPreviousValidStat(item, value);
}
#pragma endregion


#pragma region OwnerScaling
bool OwnerScaling::FindScalingContainer()
{
    sLog->outDebug(LOG_FILTER_PETS, "OwnerScaling::FindScalingContainer called for petType %u and petEntry %u", petType, petEntry);

    PetIdentifierPair petIdentifier(petType, petEntry);
    if (PreviouslyUsedContainerStorage.count(petIdentifier))
    {
        ownerScalingContainer = &PetAndGuardianScaling[PreviouslyUsedContainerStorage[petIdentifier]];
        return true;
    }

    // Search for pet type and entry
    for (uint8 i = 0; i < PET_AND_GUARDIAN_SCALING; ++i)
    {
        if (PetAndGuardianScaling[i].petType == petType
            && PetAndGuardianScaling[i].petEntry == petEntry)
        {
            ownerScalingContainer = &PetAndGuardianScaling[i];
            PreviouslyUsedContainerStorage[petIdentifier] = i;
            return true;
        }
    }

    // Search for entry
    for (uint8 i = 0; i < PET_AND_GUARDIAN_SCALING; ++i)
    {
        if (PetAndGuardianScaling[i].petEntry == petEntry)
        {
            ownerScalingContainer = &PetAndGuardianScaling[i];
            PreviouslyUsedContainerStorage[petIdentifier] = i;
            return true;
        }
    }

    // Search for pet type
    for (uint8 i = 0; i < PET_AND_GUARDIAN_SCALING; ++i)
    {
        if (PetAndGuardianScaling[i].petType == petType)
        {
            ownerScalingContainer = &PetAndGuardianScaling[i];
            PreviouslyUsedContainerStorage[petIdentifier] = i;
            return true;
        }
    }

    sLog->outError("OwnerScaling::FindScalingContainer: Found no scaling container for petType %u petEntry %u. Fix needed!", petType, petEntry);
    return false;
}
#pragma endregion
