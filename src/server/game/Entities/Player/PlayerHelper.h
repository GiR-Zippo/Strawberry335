/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
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

#ifndef _PLAYER_HELPER_H
#define _PLAYER_HELPER_H


namespace PSM
{
    enum ConfigPlayerStatsModifier
    {
        CONFIG_PSM_MIN_ITEMLEVEL        = 200,
        CONFIG_PSM_MAX_ITEMLEVEL        = 277,

        CONFIG_PSM_SCALING_AURA_ENTRY   = 7742
    };

    const float baseModifierFactor  = 0.015f;
};

class Player;

class PlayerStatsModifier
{
    public:

        PlayerStatsModifier(Player* pSource);

        bool IsActive() const;

        float GetModifier() const;
        float GetAvgItemLevelOfPlayer() const { return sourceAvgItemLevel; }
        float GetAvgItemLevelOfModifier() const { return targetAvgItemLevel; }

        void ApplyModifier(float& value) const;
        void ApplyModifier(int32& value) const;

        /** Static scaling */
        void SetModifier(float factor);

        /** Dynamic scaling
         * - Maximun und Minimum can be found in ConfigPlayerStatsModifier
         * - Modifier will reset if out of this range
         */
        void SetModifierDueToItemLevel(float avgItemLevel);

        void UpdateModifier();
        void ResetModifier();

    private:

        void calculateAvgItemLevel();
        void calculateModifier();

        Player* pPlayer;

        float targetAvgItemLevel;
        float sourceAvgItemLevel;

        float modifier;
};


class DelayedAuraRemove : public BasicEvent
{
    public:

        explicit DelayedAuraRemove(Unit* owner, uint32 spellId, Item* source = NULL)
            : _owner(owner ? owner->GetGUID() : 0), _spellId(spellId), _source(source ? source->GetGUID() : 0)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/) override;

    private:

        uint64 _owner;
        uint32 _spellId;
        uint64 _source;
};


#endif // _PLAYER_HELPER_H