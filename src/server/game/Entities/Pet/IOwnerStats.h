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
 */

#ifndef _IOWNER_STATS_H
#define _IOWNER_STATS_H

typedef std::pair<StatIdentifier, uint8> IdentifierStatPair;
typedef std::pair<uint32, float> TimeValuePair;
typedef std::map<IdentifierStatPair, TimeValuePair> PreviosValidStatMap;


class IOwnerStats
{
    public:

        IOwnerStats()
        {
        }

        virtual float GetOwnerStat(Stats stat) = 0;
        virtual float GetOwnerCombatRating(CombatRating cr) = 0;
        virtual float GetOwnerResistance(SpellSchoolMask schoolMask) = 0;
        virtual float GetOwnerAttackPower(WeaponAttackType type) = 0;
        virtual float GetOwnerSpellPower(SpellSchools school) = 0;

        static const uint32 ValidStatTime = 500; // New caluclation after 500ms

        virtual void AddValidStat(IdentifierStatPair item, float value) = 0;
        virtual bool GetPreviousValidStat(IdentifierStatPair item, float& value) = 0;
};


// Static functions which can be used as delegates for any interface (IOwnerStats) users
namespace IOwnerStatsCalc
{
    float GetOwnerStat(Stats stat, Unit* source);
    float GetOwnerCombatRating(CombatRating cr, Unit* source);
    float GetOwnerResistance(SpellSchoolMask schoolMask, Unit* source);
    float GetOwnerAttackPower(WeaponAttackType type, Unit* source);
    float GetOwnerSpellPower(SpellSchools school, Unit* source);

    float GetOwnerPermanentStat(Stats stat, Unit* source);
    float GetOwnerPermanentCombatRating(CombatRating cr, Unit* source);
    float GetOwnerPermanentResistance(SpellSchoolMask schoolMask, Unit* source);
    float GetOwnerPermanentAttackPower(WeaponAttackType type, Unit* source);
    float GetOwnerPermanentSpellPower(SpellSchools school, Unit* source);
}

#endif // _IOWNER_STATS_H
