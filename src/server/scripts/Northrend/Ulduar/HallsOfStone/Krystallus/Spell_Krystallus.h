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

#ifndef _SPELL_KRYSTALLUS_H
#define _SPELL_KRYSTALLUS_H

#include "Data_Krystallus.h"


class spell_krystallus_shatter : public SpellScriptLoader
{
    public:
        spell_krystallus_shatter() : SpellScriptLoader("spell_krystallus_shatter") {}

        class spell_krystallus_shatter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_krystallus_shatter_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/);
            void Register();
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_krystallus_shatter_SpellScript();
        }
};

class spell_krystallus_shatter_effect : public SpellScriptLoader
{
    public:
        spell_krystallus_shatter_effect() : SpellScriptLoader("spell_krystallus_shatter_effect") {}

        class spell_krystallus_shatter_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_krystallus_shatter_effect_SpellScript);

            void CalculateDamage();
            void Register();
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_krystallus_shatter_effect_SpellScript();
        }
};

#endif // _DATA_KRYSTALLUS_H
