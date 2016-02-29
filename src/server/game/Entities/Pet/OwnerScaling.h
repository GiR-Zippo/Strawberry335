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

#ifndef _OWNER_SCALING_H
#define _OWNER_SCALING_H

#include "OwnerScalingContainer.h"

typedef std::pair<uint8, uint32> PetIdentifierPair;
typedef std::map<PetIdentifierPair, uint8> PreviouslyUsedContainerMap;


enum StatIdentifier
{
    STAT_IDENTIFIER_UNIT_MOD        = 0,
    STAT_IDENTIFIER_COMBAT_RATING,
    STAT_IDENTIFIER_RESISTANCE,
    STAT_IDENTIFIER_ATTACK_POWER,
    STAT_IDENTIFIER_SPELL_POWER
};


class OwnerScaling
{
    public:

        OwnerScaling(uint8 type, uint32 entry) : petType(type), petEntry(entry)
        {
        }

        bool FindScalingContainer();

        template<class TYPE>
        float GetScaling(StatIdentifier identifier, TYPE stat) const;

        template<class TYPE>
        float CalculateScaling(StatIdentifier identifier, TYPE stat, float value, float add = 0.0f) const;

        static PreviouslyUsedContainerMap PreviouslyUsedContainerStorage;

    private:

        OwnerScalingContainer const* ownerScalingContainer;
        uint8 petType;
        uint32 petEntry;
};


// Templates has to be defined in header
template<class TYPE>
float OwnerScaling::GetScaling(StatIdentifier identifier, TYPE stat) const
{
    uint8 checkId = MAX_COMBAT_RATING;
    checkId = std::min(checkId, static_cast<uint8>(stat));
    if (!ownerScalingContainer)
    {
        sLog->outError("OwnerScaling::GetScaling: ownerScalingContainer is null!");
        return 0.0f;
    }

    sLog->outDebug(LOG_FILTER_PETS, "OwnerScaling::GetScaling called for identifier %u stat %u ", identifier, stat);

    switch (identifier)
    {
        case STAT_IDENTIFIER_UNIT_MOD:
        {
            if (checkId >= UNIT_MOD_END)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ownerScalingContainer->ModContainer[checkId];
        }

        case STAT_IDENTIFIER_COMBAT_RATING:
        {
            if (checkId >= MAX_COMBAT_RATING)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ownerScalingContainer->CombatRatingContainer[checkId];
        }

        case STAT_IDENTIFIER_RESISTANCE:
        {
            switch (stat)
            {
                case SPELL_SCHOOL_MASK_NORMAL:  checkId = SPELL_SCHOOL_NORMAL;  break;
                case SPELL_SCHOOL_MASK_HOLY:    checkId = SPELL_SCHOOL_HOLY;    break;
                case SPELL_SCHOOL_MASK_FIRE:    checkId = SPELL_SCHOOL_FIRE;    break;
                case SPELL_SCHOOL_MASK_NATURE:  checkId = SPELL_SCHOOL_NATURE;  break;
                case SPELL_SCHOOL_MASK_FROST:   checkId = SPELL_SCHOOL_FROST;   break;
                case SPELL_SCHOOL_MASK_SHADOW:  checkId = SPELL_SCHOOL_SHADOW;  break;
                case SPELL_SCHOOL_MASK_ARCANE:  checkId = SPELL_SCHOOL_ARCANE;  break;

                default:
                    sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                    return 0.0f;
            }
            return ownerScalingContainer->ResistanceContainer[checkId];
        }

        case STAT_IDENTIFIER_ATTACK_POWER:
        {
            if (checkId >= MAX_ATTACK)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ownerScalingContainer->AttackPowerContainer[checkId];
        }

        case STAT_IDENTIFIER_SPELL_POWER:
        {
            if (checkId >= MAX_SPELL_SCHOOL)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ownerScalingContainer->SpellPowerContainer[checkId];
        }

        default:
        {
            sLog->outError("OwnerScaling::GetScaling: Identifier %u and checkId %d not found!", identifier, stat);
            return 0.0f;
        }
    }
}

template<class TYPE>
float OwnerScaling::CalculateScaling(StatIdentifier identifier, TYPE stat, float value, float add) const
{
    uint8 checkId = MAX_COMBAT_RATING;
    checkId = std::min(checkId, static_cast<uint8>(stat));
    if (!ownerScalingContainer)
    {
        sLog->outError("OwnerScaling::GetScaling: ownerScalingContainer is null!");
        return 0.0f;
    }

    sLog->outDebug(LOG_FILTER_PETS, "OwnerScaling::CalculateScaling called for identifier %u stat %u with value %f and add %f", identifier, stat, value, add);

    switch (identifier)
    {
        case STAT_IDENTIFIER_UNIT_MOD:
        {
            if (checkId >= UNIT_MOD_END)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ((ownerScalingContainer->ModContainer[checkId]
                + CalculatePctF(ownerScalingContainer->ModContainer[checkId], add))
                * value);
        }

        case STAT_IDENTIFIER_COMBAT_RATING:
        {
            if (checkId >= MAX_COMBAT_RATING)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ((ownerScalingContainer->CombatRatingContainer[checkId]
                + CalculatePctF(ownerScalingContainer->CombatRatingContainer[checkId], add))
                * value);
        }

        case STAT_IDENTIFIER_RESISTANCE:
        {
            switch (stat)
            {
                case SPELL_SCHOOL_MASK_NORMAL:  checkId = SPELL_SCHOOL_NORMAL;  break;
                case SPELL_SCHOOL_MASK_HOLY:    checkId = SPELL_SCHOOL_HOLY;    break;
                case SPELL_SCHOOL_MASK_FIRE:    checkId = SPELL_SCHOOL_FIRE;    break;
                case SPELL_SCHOOL_MASK_NATURE:  checkId = SPELL_SCHOOL_NATURE;  break;
                case SPELL_SCHOOL_MASK_FROST:   checkId = SPELL_SCHOOL_FROST;   break;
                case SPELL_SCHOOL_MASK_SHADOW:  checkId = SPELL_SCHOOL_SHADOW;  break;
                case SPELL_SCHOOL_MASK_ARCANE:  checkId = SPELL_SCHOOL_ARCANE;  break;

                default:
                    sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                    return 0.0f;
            }

            return ((ownerScalingContainer->ResistanceContainer[checkId]
                + CalculatePctF(ownerScalingContainer->ResistanceContainer[checkId], add))
                * value);
        }

        case STAT_IDENTIFIER_ATTACK_POWER:
        {
            if (checkId >= MAX_ATTACK)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ((ownerScalingContainer->AttackPowerContainer[checkId]
                + CalculatePctF(ownerScalingContainer->AttackPowerContainer[checkId], add))
                * value);
        }

        case STAT_IDENTIFIER_SPELL_POWER:
        {
            if (checkId >= MAX_SPELL_SCHOOL)
            {
                sLog->outError("OwnerScaling::GetScaling: Check not passed with identifier %u and checkId %d!", identifier, stat);
                return 0.0f;
            }
            return ((ownerScalingContainer->SpellPowerContainer[checkId]
                + CalculatePctF(ownerScalingContainer->SpellPowerContainer[checkId], add))
                * value);
        }

        default:
        {
            sLog->outError("OwnerScaling::GetScaling: Identifier %u and checkId %d not found!", identifier, stat);
            return 0.0f;
        }
    }
}


#endif // _OWNER_SCALING_H
