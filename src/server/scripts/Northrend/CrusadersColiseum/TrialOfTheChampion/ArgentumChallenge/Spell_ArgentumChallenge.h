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

#ifndef _SPELL_ARGENTUM_CHALLENGE_H
#define _SPELL_ARGENTUM_CHALLENGE_H


class OrientationCheck
{
    public:
    explicit OrientationCheck(Unit* _caster) : caster(_caster) { }
    bool operator()(WorldObject* object)
    {
        return !object->isInFront(caster, 2.5f) || !object->IsWithinDist(caster, 40.0f);
    }

    private:
    Unit* caster;
};

class Spell_Eadric_Radiance : public SpellScriptLoader
{
    public:

    Spell_Eadric_Radiance() : SpellScriptLoader("spell_eadric_radiance") { }

    class Spell_Eadric_Radiance_SpellScript : public SpellScript
    {
        PrepareSpellScript(Spell_Eadric_Radiance_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList);
        void Register() override;
    };

    SpellScript* GetSpellScript() const override
    {
        return new Spell_Eadric_Radiance_SpellScript();
    }
};

class Spell_Eadric_HammerOfRighteous : public SpellScriptLoader
{
    public:

    Spell_Eadric_HammerOfRighteous() : SpellScriptLoader("spell_eadric_hammer_of_righteous") {}

    class Spell_Eadric_HammerOfRighteousSpellScript : public SpellScript
    {
        PrepareSpellScript(Spell_Eadric_HammerOfRighteousSpellScript)

        void HandleBeforeHit();
        void Register() override;
    };

    SpellScript *GetSpellScript() const override
    {
        return new Spell_Eadric_HammerOfRighteousSpellScript();
    }
};

class Spell_Paletress_Shield : public SpellScriptLoader
{
    public:

    Spell_Paletress_Shield() : SpellScriptLoader("spell_paletress_shield") { }

    class Spell_Paletress_ShieldAuraScript : public AuraScript
    {
        PrepareAuraScript(Spell_Paletress_ShieldAuraScript);

        void HandleOnEffectAbsorb(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& /*absorbAmount*/);
        void Register() override;
    };

    AuraScript* GetAuraScript() const override
    {
        return new Spell_Paletress_ShieldAuraScript();
    }
};


#endif // _SPELL_ARGENTUM_CHALLENGE_H
