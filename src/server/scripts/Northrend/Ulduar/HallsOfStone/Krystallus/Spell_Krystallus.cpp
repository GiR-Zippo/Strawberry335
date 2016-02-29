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

#include "Spell_Krystallus.h"
typedef spell_krystallus_shatter::spell_krystallus_shatter_SpellScript SpellShatter;
typedef spell_krystallus_shatter_effect::spell_krystallus_shatter_effect_SpellScript SpellShatterEffect;


void SpellShatter::HandleScript(SpellEffIndex /*effIndex*/)
{
    if (Unit* target = GetHitUnit())
    {
        target->RemoveAurasDueToSpell(SPELL_STONED);
        target->CastSpell((Unit*)NULL, SPELL_SHATTER_EFFECT, true);
    }
}

void SpellShatter::Register()
{
    OnEffectHitTarget += SpellEffectFn(spell_krystallus_shatter_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}


void SpellShatterEffect::CalculateDamage()
{
    if (!GetHitUnit())
        return;

    float radius = GetSpellInfo()->Effects[EFFECT_0].CalcRadius(GetCaster());
    if (!radius)
        return;

    float distance = GetCaster()->GetDistance2d(GetHitUnit());
    if (distance > 1.0f)
        SetHitDamage(int32(GetHitDamage() * ((radius - distance) / radius)));
}

void SpellShatterEffect::Register()
{
    OnHit += SpellHitFn(spell_krystallus_shatter_effect_SpellScript::CalculateDamage);
}
