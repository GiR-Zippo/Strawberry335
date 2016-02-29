/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Spell_SjonnirTheIronshaper.h"
typedef spell_hos_sjonnir_lightning_ring_trigger::spell_hos_sjonnir_lightning_ring_trigger_SpellScript SpellLightningRing;
typedef spell_hos_sjonnir_static_charge_trigger::spell_hos_sjonnir_static_charge_trigger_SpellScript SpellStaticCharge;


void SpellLightningRing::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.remove_if(LightningAndStaticTargetFilter());
};

void SpellLightningRing::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hos_sjonnir_lightning_ring_trigger_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
};


void SpellStaticCharge::FilterTargets(std::list<WorldObject*>& targets)
{
    targets.remove_if(LightningAndStaticTargetFilter());
};

void SpellStaticCharge::Register()
{
    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hos_sjonnir_static_charge_trigger_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
};
