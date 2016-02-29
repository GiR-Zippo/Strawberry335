/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 19.02.2015
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

#ifndef _UNIT_HELPER_H
#define _UNIT_HELPER_H

#include "SpellAuraEffects.h"


class DelayedCastSpellEvent : public BasicEvent
{
    public:

        explicit DelayedCastSpellEvent(Unit* owner, Unit* victim, uint32 spellId, TriggerCastFlags triggerFlags = TRIGGERED_NONE,
                                       Item* castItem = NULL, const AuraEffect* triggeredByAura = NULL, uint64 originalCaster = 0)
                                       : _owner(owner ? owner->GetGUID() : 0), _victim(victim ? victim->GetGUID() : 0), _spellId(spellId),
                                       _triggerFlags(triggerFlags), _castItem(castItem ? castItem->GetGUID() : 0), _triggeredByAura(false),
                                       _originalCaster(originalCaster)

        {
            if (triggeredByAura)
            {
                _auraId = triggeredByAura->GetId();
                _effIndex = triggeredByAura->GetEffIndex();
                _auraCaster = triggeredByAura->GetCasterGUID();
                _triggeredByAura = true;
            }
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/) override;

    private:

        uint64 _owner;
        uint64 _victim;
        uint32 _spellId;
        TriggerCastFlags _triggerFlags;
        uint64 _castItem;
        bool _triggeredByAura;
        uint32 _auraId;
        int8 _effIndex;
        uint64 _auraCaster;
        uint64 _originalCaster;
};

class DelayedCastCustomSpellEvent : public BasicEvent
{
    public:

        explicit DelayedCastCustomSpellEvent(Unit* owner, Unit* victim, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, TriggerCastFlags triggerFlags,
                                             Item* castItem = NULL, const AuraEffect* triggeredByAura = NULL, uint64 originalCaster = 0)
                                             : _owner(owner ? owner->GetGUID() : 0), _victim(victim ? victim->GetGUID() : 0), _spellId(spellId),
                                             _bp0(bp0), _bp1(bp1), _bp2(bp2), _triggerFlags(triggerFlags), _castItem(castItem ? castItem->GetGUID() : 0),
                                             _triggeredByAura(false), _originalCaster(originalCaster)

        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/) override;

    private:

        uint64 _owner;
        uint64 _victim;
        uint32 _spellId;
        int32 const* _bp0;
        int32 const* _bp1;
        int32 const* _bp2;
        TriggerCastFlags _triggerFlags;
        uint64 _castItem;
        bool _triggeredByAura;
        uint32 _auraId;
        int8 _effIndex;
        uint64 _auraCaster;
        uint64 _originalCaster;
};


#endif // _UNIT_HELPER_H