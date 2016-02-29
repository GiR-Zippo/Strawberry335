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

#include "UnitHelper.h"
#include "Unit.h"


bool DelayedCastSpellEvent::Execute(uint64 /*time*/, uint32 /*diff*/)
{
    if (Unit* owner = sObjectAccessor->FindUnit(_owner))
    {
        Unit* victim = NULL;
        if (_owner != _victim)
            victim = sObjectAccessor->FindUnit(_victim);
        else
            victim = owner;

        Item* castItem = NULL;
        const AuraEffect* triggeredByAura = NULL;

        if (Player* pPlayer = owner->ToPlayer())
            castItem = pPlayer->GetItemByGuid(_castItem);

        if (_triggeredByAura)
        {
            // Search aura at owner
            triggeredByAura = owner->GetAuraEffect(_auraId, _effIndex, _auraCaster);

            // Search aura at victim
            if (!triggeredByAura)
                triggeredByAura = victim->GetAuraEffect(_auraId, _effIndex, _auraCaster);
        }

        owner->CastSpell(victim, _spellId, _triggerFlags, castItem, triggeredByAura, _originalCaster);
    }

    return true;
}

bool DelayedCastCustomSpellEvent::Execute(uint64 /*time*/, uint32 /*diff*/)
{
    if (Unit* owner = sObjectAccessor->FindUnit(_owner))
    {
        Unit* victim = NULL;
        if (_owner != _victim)
            victim = sObjectAccessor->FindUnit(_victim);
        else
            victim = owner;

        Item* castItem = NULL;
        const AuraEffect* triggeredByAura = NULL;

        if (Player* pPlayer = owner->ToPlayer())
            castItem = pPlayer->GetItemByGuid(_castItem);

        if (_triggeredByAura)
        {
            // Search aura at owner
            triggeredByAura = owner->GetAuraEffect(_auraId, _effIndex, _auraCaster);

            // Search aura at victim
            if (!triggeredByAura)
                triggeredByAura = victim->GetAuraEffect(_auraId, _effIndex, _auraCaster);
        }

        owner->CastCustomSpell(victim, _spellId, _bp0, _bp1, _bp2, _triggerFlags, castItem, triggeredByAura, _originalCaster);
    }

    return true;
}

void Unit::DelayedCastSpell(uint32 delay, Unit* victim, uint32 spellId, TriggerCastFlags triggerFlags, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    // Add delayed cast spell event
    m_Events.AddEvent(new DelayedCastSpellEvent(this, victim, spellId, triggerFlags, castItem, triggeredByAura, originalCaster), m_Events.CalculateTime(delay));
}

void Unit::DelayedCastCustomSpell(uint32 delay, Unit* victim, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, TriggerCastFlags triggerFlags, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    // Add delayed cast custom spell event
    m_Events.AddEvent(new DelayedCastCustomSpellEvent(this, victim, spellId, bp0, bp1, bp2, triggerFlags, castItem, triggeredByAura, originalCaster), m_Events.CalculateTime(delay));
}
