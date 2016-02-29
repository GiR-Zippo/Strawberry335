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

#ifndef _SPELL_SJONNIRTHEIRONSHAPER_H
#define _SPELL_SJONNIRTHEIRONSHAPER_H

#include "Data_SjonnirTheIronshaper.h"


class LightningAndStaticTargetFilter
{
    public:
        bool operator()(WorldObject* target) const
        {
            if (Unit* unit = target->ToUnit())
            {
                if (unit->GetTypeId() == TYPEID_PLAYER)
                    return false;

                switch (unit->GetEntry())
                {
                    case CREATURE_BRANN_FINAL:
                    case CREATURE_SJONNIR:
                        return true;
                    break;
                }
            }
            return false;
        }
};

class spell_hos_sjonnir_lightning_ring_trigger : public SpellScriptLoader
{
    public:
        spell_hos_sjonnir_lightning_ring_trigger() : SpellScriptLoader("spell_hos_sjonnir_lightning_ring_trigger") {}

        class spell_hos_sjonnir_lightning_ring_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hos_sjonnir_lightning_ring_trigger_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets);
            void Register();
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hos_sjonnir_lightning_ring_trigger_SpellScript();
        };
};

class spell_hos_sjonnir_static_charge_trigger : public SpellScriptLoader
{
    public:
        spell_hos_sjonnir_static_charge_trigger() : SpellScriptLoader("spell_hos_sjonnir_static_charge_trigger") {}

        class spell_hos_sjonnir_static_charge_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hos_sjonnir_static_charge_trigger_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets);
            void Register();
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hos_sjonnir_static_charge_trigger_SpellScript();
        };
};


#endif // _SPELL_SJONNIRTHEIRONSHAPER_H
