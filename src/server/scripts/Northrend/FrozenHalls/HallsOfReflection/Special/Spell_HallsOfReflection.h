/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 13.10.2014
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

#ifndef _SPELL_HALLSOFREFLECTION_H
#define _SPELL_HALLSOFREFLECTION_H

#include "../HallsOfReflection.h"


namespace SPELL_HALLSOFREFLECTION
{
    enum Spells
    {
        // Start Quests
        SPELL_START_HALLS_OF_REFLECTION_QUEST_A     = 71351,
        SPELL_START_HALLS_OF_REFLECTION_QUEST_H     = 71542,

        // Frostworn
        SPELL_CLONE_NAME                            = 57507,
        SPELL_CLONE_MODEL                           = 45204,

        // Gunship
        SPELL_GUNSHIP_CANNON_FIRE_MISSILE_ALLIANCE  = 70021,
        SPELL_GUNSHIP_CANNON_FIRE_MISSILE_HORDE     = 70246
    };
}


// 72900 - Start Halls of Reflection Quest AE
class Spell_HorStartQuest : public SpellScriptLoader
{
    public:

    Spell_HorStartQuest() : SpellScriptLoader("spell_hor_start_quest") { }

    class Spell_HorStartQuest_SpellScript : public SpellScript
    {
        PrepareSpellScript(Spell_HorStartQuest_SpellScript);

        void StartQuests(SpellEffIndex /*effIndex*/);
        void Register() override;
    };

    SpellScript* GetSpellScript() const override
    {
        return new Spell_HorStartQuest_SpellScript();
    }
};


// 70017 - Gunship Cannon Fire
class Spell_HorGunshipCannonFire : public SpellScriptLoader
{
    public:

    Spell_HorGunshipCannonFire() : SpellScriptLoader("spell_hor_gunship_cannon_fire") { }

    class Spell_HorGunshipCannonFire_AuraScript : public AuraScript
    {
        PrepareAuraScript(Spell_HorGunshipCannonFire_AuraScript);

        void HandlePeriodic(AuraEffect const* /*aurEff*/);
        void Register() override;
    };

    AuraScript* GetAuraScript() const override
    {
        return new Spell_HorGunshipCannonFire_AuraScript();
    }
};


// 69223 - Summon Reflections
class Spell_HorSummonReflections : public SpellScriptLoader
{
    public:

    Spell_HorSummonReflections() : SpellScriptLoader("spell_hor_summon_reflections") { }

    class Spell_HorSummonReflections_SpellScript : public SpellScript
    {
        PrepareSpellScript(Spell_HorSummonReflections_SpellScript);

        void HandleEffectHit(SpellEffIndex /*effIndex*/);
        void Register() override;
    };

    SpellScript* GetSpellScript() const override
    {
        return new Spell_HorSummonReflections_SpellScript();
    }
};


#endif // _SPELL_HALLSOFREFLECTION_H
