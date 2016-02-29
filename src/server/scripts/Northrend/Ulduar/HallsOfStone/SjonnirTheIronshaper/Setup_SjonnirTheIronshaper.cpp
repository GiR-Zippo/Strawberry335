/*
 * Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "Boss_SjonnirTheIronshaper.h"
#include "Npc_SjonnirTheIronshaper.h"
#include "Misc_SjonnirTheIronshaper.h"
#include "Spell_SjonnirTheIronshaper.h"


void AddSC_boss_sjonnir()
{
    new boss_sjonnir_the_ironshaper();
    new npc_malformed_ooze();
    new npc_iron_sludge();
    new npc_brann_hos_final();
    new npc_trigger_sjonnir();
    new achievement_abuse_the_ooze();
    new spell_hos_sjonnir_lightning_ring_trigger();
    new spell_hos_sjonnir_static_charge_trigger();
};
