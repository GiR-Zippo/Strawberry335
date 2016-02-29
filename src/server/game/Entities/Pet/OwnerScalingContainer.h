/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 02.03.2015
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
 *
 * @description
 *
 * Amount in percent (0.00f - 1.00f) of owner scaling.
 * Example of container is given after description.
 * (*)  - Not implemented for Guardian
 * (**) - Has to be set
 *
 * ModContainer:
 * UNIT_MOD_STAT_STRENGTH       | UNIT_MOD_STAT_AGILITY             | UNIT_MOD_STAT_STAMINA         | UNIT_MOD_STAT_INTELLECT   | UNIT_MOD_STAT_SPIRIT
 * UNIT_MOD_HEALTH**            | UNIT_MOD_MANA**                   | UNIT_MOD_RAGE*                | UNIT_MOD_FOCUS*           | UNIT_MOD_ENERGY*
 * UNIT_MOD_HAPPINESS*          | UNIT_MOD_RUNE*                    | UNIT_MOD_RUNIC_POWER*         | UNIT_MOD_ARMOR
 * UNIT_MOD_RESISTANCE_HOLY     | UNIT_MOD_RESISTANCE_FIRE          | UNIT_MOD_RESISTANCE_NATURE    | UNIT_MOD_RESISTANCE_FROST | UNIT_MOD_RESISTANCE_SHADOW | UNIT_MOD_RESISTANCE_ARCANE
 * UNIT_MOD_ATTACK_POWER**      | UNIT_MOD_ATTACK_POWER_RANGED**    | UNIT_MOD_DAMAGE_MAINHAND      | UNIT_MOD_DAMAGE_OFFHAND   | UNIT_MOD_DAMAGE_RANGED
    {
        0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
        10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
        0.00f, 0.00f, 0.00f, 0.00f,
        0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
        14.00f, 14.00f, 0.00f, 0.00f, 0.00f
    }
 *
 * Special use for some unit mods:
 * UNIT_MOD_HEALTH (HP / STAT_STAMINA) | UNIT_MOD_MANA (Mana / STAT_INTELLECT)
 * UNIT_MOD_ATTACK_POWER - UNIT_MOD_ATTACK_POWER_RANGED (AP / STAT_STRENGTH)
 * UNIT_MOD_DAMAGE_MAINHAND (Additional bonus of base_value in damage min/max calculation - based on spellpower)
 *
 * CombatRatingContainer:
 * CR_WEAPON_SKILL*             | CR_DEFENSE_SKILL*         | CR_DODGE*             | CR_PARRY*             | CR_BLOCK*
 * CR_HIT_MELEE                 | CR_HIT_RANGED             | CR_HIT_SPELL          | CR_CRIT_MELEE         | CR_CRIT_RANGED        | CR_CRIT_SPELL
 * CR_HIT_TAKEN_MELEE*          | CR_HIT_TAKEN_RANGED*      | CR_HIT_TAKEN_SPELL*   | CR_CRIT_TAKEN_MELEE*  | CR_CRIT_TAKEN_RANGED* | CR_CRIT_TAKEN_SPELL*
 * CR_HASTE_MELEE               | CR_HASTE_RANGED           | CR_HASTE_SPELL
 * CR_WEAPON_SKILL_MAINHAND*    | CR_WEAPON_SKILL_OFFHAND*  | CR_WEAPON_SKILL_RANGED*
 * CR_EXPERTISE*                | CR_ARMOR_PENETRATION*
    {
        0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
        1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
        0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
        1.00f, 1.00f, 1.00f,
        0.00f, 0.00f, 0.00f,
        0.00f, 0.00f
    }
 *
 * ResistanceContainer:
 * SPELL_SCHOOL_MASK_NORMAL     | SPELL_SCHOOL_MASK_HOLY    | SPELL_SCHOOL_MASK_FIRE
 * SPELL_SCHOOL_MASK_NATURE     | SPELL_SCHOOL_MASK_FROST   | SPELL_SCHOOL_MASK_SHADOW
 * SPELL_SCHOOL_MASK_ARCANE
    {
        0.00f, 0.00f, 0.00f,
        0.00f, 0.00f, 0.00f,
        0.00f
    }
 *
 * AttackPowerContainer:
 * BASE_ATTACK                  | OFF_ATTACK                | RANGED_ATTACK
    {
        0.00f, 0.00f, 0.00f
    }
 *
 * SpellPowerContainer:
 * SPELL_SCHOOL_NORMAL          | SPELL_SCHOOL_HOLY         | SPELL_SCHOOL_FIRE
 * SPELL_SCHOOL_NATURE          | SPELL_SCHOOL_FROST        | SPELL_SCHOOL_SHADOW
 * SPELL_SCHOOL_ARCANE
    {
        0.00f, 0.00f, 0.00f,
        0.00f, 0.00f, 0.00f,
        0.00f
    }
 *
 */

#ifndef _OWNER_SCALING_CONTAINER_H
#define _OWNER_SCALING_CONTAINER_H


enum OwnerScalingContainerMisc
{
    SCALING_SUMMON_PET          = 0,
    SCALING_HUNTER_PET          = 1,
    SCALING_MAX_PET_TYPE        = 4,
    PET_AND_GUARDIAN_SCALING    = 17
};

enum PetEntries
{
    ENTRY_IMP                       = 416,
    ENTRY_VOIDWALKER                = 1860,
    ENTRY_SUCCUBUS                  = 1863,
    ENTRY_FELHUNTER                 = 417,
    ENTRY_FELGUARD                  = 17252,
    ENTRY_WATER_ELEMENTAL           = 510,
    ENTRY_TREANT                    = 1964,
    ENTRY_FIRE_ELEMENTAL            = 15438,
    ENTRY_EARTH_ELEMENTAL           = 15352,
    ENTRY_GHOUL                     = 26125,
    ENTRY_BLOODWORM                 = 28017,
    ENTRY_FERAL_SPIRIT              = 29264,
    ENTRY_SHADOWFIEND               = 19668,
    ENTRY_MIRROR_IMAGE              = 31216,
    ENTRY_EBON_GARGOYLE             = 27829,
    ENTRY_ARMY_OF_THE_DEAD_GHOUL    = 24207
};


struct OwnerScalingContainer
{
    uint8 petType;
    uint32 petEntry;

    float ModContainer[UNIT_MOD_END];
    float CombatRatingContainer[MAX_COMBAT_RATING];
    float ResistanceContainer[MAX_SPELL_SCHOOL];
    float AttackPowerContainer[MAX_ATTACK];
    float SpellPowerContainer[MAX_SPELL_SCHOOL];
};


static const OwnerScalingContainer PetAndGuardianScaling[PET_AND_GUARDIAN_SCALING] = 
{
    // Warlock Imp
    { SCALING_SUMMON_PET, ENTRY_IMP,

        { // ModContainer
            0.00f, 0.00f, 0.75f, 0.30f, 0.00f,
            8.40f, 4.90f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.35f, 0.40f, 0.40f,
            0.40f, 0.40f, 0.40f,
            0.40f
        },

        { // AttackPowerContainer
            0.57f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.15f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Warlock Voidwalker
    { SCALING_SUMMON_PET, ENTRY_VOIDWALKER,

        { // ModContainer
            0.00f, 0.00f, 0.75f, 0.30f, 0.00f,
            11.0f, 11.5f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            13.57f, 13.57f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.35f, 0.40f, 0.40f,
            0.40f, 0.40f, 0.40f,
            0.40f
        },

        { // AttackPowerContainer
            0.57f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.15f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Warlock Succubus
    { SCALING_SUMMON_PET, ENTRY_SUCCUBUS,

        { // ModContainer
            0.00f, 0.00f, 0.75f, 0.30f, 0.00f,
            9.10f, 11.50f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            11.11f, 11.11f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.35f, 0.40f, 0.40f,
            0.40f, 0.40f, 0.40f,
            0.40f
        },

        { // AttackPowerContainer
            0.57f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.15f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Warlock Felhunter
    { SCALING_SUMMON_PET, ENTRY_FELHUNTER,

        { // ModContainer
            0.00f, 0.00f, 0.75f, 0.30f, 0.00f,
            9.50f, 11.50f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.58f, 14.58f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.35f, 0.40f, 0.40f,
            0.40f, 0.40f, 0.40f,
            0.40f
        },

        { // AttackPowerContainer
            0.57f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.15f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Warlock Felguard
    { SCALING_SUMMON_PET, ENTRY_FELGUARD,

        { // ModContainer
            0.00f, 0.00f, 0.75f, 0.30f, 0.00f,
            11.00f, 11.50f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            11.11f, 11.11f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.35f, 0.40f, 0.40f,
            0.40f, 0.40f, 0.40f,
            0.40f
        },

        { // AttackPowerContainer
            0.57f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.15f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Hunter
    { SCALING_HUNTER_PET, 0,

        { // ModContainer
            0.00f, 0.0, 0.40f, 0.00f, 0.00f,
            10.50f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.35f, 0.40f, 0.40f,
            0.40f, 0.40f, 0.40f,
            0.40f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.22f
        },

        { // SpellPowerContainer
            0.1287f, 0.1287f, 0.1287f,
            0.1287f, 0.1287f, 0.1287f,
            0.1287f
        }
    },

    // Deathknight Perm/Temp Ghoul
    { SCALING_MAX_PET_TYPE, ENTRY_GHOUL,

        { // ModContainer
            0.70f, 0.00f, 0.30f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.15f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Deathknight Bloodworm
    { SCALING_MAX_PET_TYPE, ENTRY_EBON_GARGOYLE,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.35f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Deathknight Army of the Dead Ghoul
    { SCALING_MAX_PET_TYPE, ENTRY_ARMY_OF_THE_DEAD_GHOUL,

        { // ModContainer
            0.70f, 0.00f, 0.00f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.0f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Deathknight Bloodworm
    { SCALING_MAX_PET_TYPE, ENTRY_BLOODWORM,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.006f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Mage Water Elemental
    { SCALING_MAX_PET_TYPE, ENTRY_WATER_ELEMENTAL,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.30f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.33f, 0.00f,
            0.00f
        }
    },

    // Mage Mirror Image
    { SCALING_MAX_PET_TYPE, ENTRY_MIRROR_IMAGE,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.30f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.33f, 0.00f,
            0.00f
        }
    },

    // Druid Treant
    { SCALING_MAX_PET_TYPE, ENTRY_TREANT,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.09f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.15f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Shaman Fire Elemental
    { SCALING_MAX_PET_TYPE, ENTRY_FIRE_ELEMENTAL,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.40f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.50f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Shaman Earth Elemental
    { SCALING_MAX_PET_TYPE, ENTRY_EARTH_ELEMENTAL,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Shaman Feral Spirit
    { SCALING_MAX_PET_TYPE, ENTRY_FERAL_SPIRIT,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.30f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.35f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.30f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        }
    },

    // Priest Shadowfiend
    { SCALING_MAX_PET_TYPE, ENTRY_SHADOWFIEND,

        { // ModContainer
            0.00f, 0.00f, 0.45f, 0.00f, 0.00f,
            10.00f, 15.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            14.00f, 14.00f, 0.00f, 0.00f, 0.00f
        },

        { // CombatRatingContainer
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f,
            1.00f, 1.00f, 1.00f,
            0.00f, 0.00f, 0.00f,
            1.00f, 0.00f
        },

        { // ResistanceContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f,
            0.00f
        },

        { // AttackPowerContainer
            0.00f, 0.00f, 0.00f
        },

        { // SpellPowerContainer
            0.00f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.30f,
            0.00f
        }
    },
};


#endif // _OWNER_SCALING_CONTAINER_H
