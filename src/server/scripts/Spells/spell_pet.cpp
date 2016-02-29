/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 30.09.2014
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

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"


enum HunterPetCalculate
{
    // Scaling Auras
    SPELL_TAMED_PET_PASSIVE_01          = 8875,  // spell_hun_pet_passive_damage_done - Damage Percent Done / Add PCT Modifier (22)
    SPELL_TAMED_PET_PASSIVE_02          = 19580, // unhandled - Resistance PCT 1 (Armor)
    SPELL_TAMED_PET_PASSIVE_03          = 19581, // no handle required - Increase Health Percent
    SPELL_TAMED_PET_PASSIVE_04          = 19582, // spell_tamed_pet_scaling_04 - Increase Speed @todo
    SPELL_TAMED_PET_PASSIVE_05          = 19589, // no handle required - Power Regen Percent
    SPELL_TAMED_PET_PASSIVE_06          = 19591, // no handle required -  Melee / Spell Crit Rating
    SPELL_TAMED_PET_PASSIVE_07          = 20784, // no handle required - Fury Chance
    SPELL_TAMED_PET_PASSIVE_08          = 34666, // replaced by 61017 - Melee / Spell Hit Rating
    SPELL_TAMED_PET_PASSIVE_09          = 34667, // no handle required - Dodge Rating
    SPELL_TAMED_PET_PASSIVE_10          = 34675, // no handle required - Attack Speed
    SPELL_TAMED_PET_PASSIVE_11          = 55566, // unhandled - Resistance 124 (All -Normal -Holy)
    SPELL_HUNTER_PET_SCALING_01         = 34902, // spell_hun_pet_scaling_01 - Stamina / AP / Damage Done
    SPELL_HUNTER_PET_SCALING_02         = 34903, // spell_gen_pet_calculate - Resistance 4 (Fire) / Resistance 16 (Frost) / Resistance 8 (Nature)
    SPELL_HUNTER_PET_SCALING_03         = 34904, // spell_gen_pet_calculate - Resistance 32 (Shadow) / Resistance 64 (Arcane) / Resistance 1 (Armor)
    SPELL_HUNTER_PET_SCALING_04         = 61017, // spell_gen_pet_calculate - Melee / Spell Hit Rating / Expertise

    // Talents
    TALENT_HUNTER_ENDURANCE_TRAINING    = 19583, // Increase Health Percent
    TALENT_HUNTER_FEROCITY              = 19598, // Increase Melee / Spell Crit Rating
    TALENT_HUNTER_BESTIAL_DISCIPLINE    = 19590, // Increase Power Regen Percent
    TALENT_HUNTER_UNLEASHED_FURY        = 19616, // Increase Damage Percent Done
    TALENT_HUNTER_CATLIKE_REFLEXES      = 34462, // Increase Dodge Rating
    TALENT_HUNTER_SERPENTS_SWIFTNESS    = 34466, // Increase Attack Speed
    TALENT_HUNTER_ANIMAL_HANDLER        = 34453, // Increase AP

    // Pet Talents
    TALENT_PET_WILD_HUNT_1              = 62758, // Increase Stamina (Not ranked)
    TALENT_PET_WILD_HUNT_2              = 62762, // Increase Stamina (Not ranked)
    TALENT_PET_COBRA_REFLEXES           = 61682, // Increase Attack Speed - Decrease Damage Percent Done

    // Glyphs
};

enum WarlockPetCalculate
{
    // Scaling auras
    SPELL_WARLOCK_PET_SCALING_01        = 34947, // spell_warl_pet_scaling_01 - Stamina / AP / Damage Done
    SPELL_WARLOCK_PET_SCALING_02        = 34956, // spell_warl_pet_scaling_02 - Intellect / Resistance 1 (Armor) / Resistance 4 (Fire)
    SPELL_WARLOCK_PET_SCALING_03        = 34957, // spell_gen_pet_calculate - Resistance 16 (Frost) / Resistance 64 (Arcane) / Resistance 8 (Nature)
    SPELL_WARLOCK_PET_SCALING_04        = 34958, // spell_warl_pet_scaling_04 - Resistance 32 (Shadow) / Power Regen / Spell Hit
    SPELL_WARLOCK_PET_SCALING_05        = 61013, // spell_gen_pet_calculate - Melee / Spell Hit Rating / Expertise
    SPELL_PET_PASSIVE_CRIT              = 35695, // no handle required - Spell / Melee Crit Rating
    SPELL_PET_PASSIVE_DAMAGE_TAKEN      = 35697, // no handle required - Damage Percent Taken


    // Type scaling auras
    SPELL_WARLOCK_TAMED_PET_P_IMP_1     = 18728, // no handle required - Damage Percent Done 1 (Normal) / Damage Percent Done 126 (All -Normal)
    SPELL_WARLOCK_TAMED_PET_P_IMP_2     = 18737, // no handle required - Total Stat Percentage 2 (Stamina)
    SPELL_WARLOCK_TAMED_PET_P_IMP_3     = 18740, // no handle required - Total Stat Percentage 3 (Intellect)
    SPELL_WARLOCK_TAMED_PET_P_VOID_1    = 18727, // no handle required - Damage Percent Done 1 (Normal) / Damage Percent Done 126 (All -Normal)
    SPELL_WARLOCK_TAMED_PET_P_VOID_2    = 18735, // no handle required - Total Stat Percentage 2 (Stamina)
    SPELL_WARLOCK_TAMED_PET_P_VOID_3    = 18742, // no handle required - Total Stat Percentage 3 (Intellect)
    SPELL_WARLOCK_TAMED_PET_P_SUCCU_1   = 18729, // no handle required - Damage Percent Done 1 (Normal) / Damage Percent Done 126 (All -Normal)
    SPELL_WARLOCK_TAMED_PET_P_SUCCU_2   = 18736, // no handle required - Total Stat Percentage 2 (Stamina)
    SPELL_WARLOCK_TAMED_PET_P_SUCCU_3   = 18741, // no handle required - Total Stat Percentage 3 (Intellect)
    SPELL_WARLOCK_TAMED_PET_P_FEL_1     = 18730, // no handle required - Damage Percent Done 1 (Normal) / Damage Percent Done 126 (All -Normal)
    SPELL_WARLOCK_TAMED_PET_P_FEL_2     = 18738, // no handle required - Total Stat Percentage 2 (Stamina)
    SPELL_WARLOCK_TAMED_PET_P_FEL_3     = 18739, // no handle required - Total Stat Percentage 3 (Intellect)
    SPELL_WARLOCK_TAMED_PET_P_FEL_4     = 19007, // no handle required - Resistance 126 (All -Normal)
    SPELL_WARLOCK_TAMED_PET_P_GUARD_1   = 30147, // no handle required - Damage Percent Done 1 (Normal) / Damage Percent Done 126 (All -Normal)
    SPELL_WARLOCK_TAMED_PET_P_GUARD_2   = 30148, // no handle required - Total Stat Percentage 2 (Stamina)
    SPELL_WARLOCK_TAMED_PET_P_GUARD_3   = 30149, // no handle required - Total Stat Percentage 3 (Intellect)

    // Talents
    TALENT_WL_IMPROVED_DEMONIC_TACTICS  = 54347, // Increase Melee / Spell Crit Rating
    TALENT_WL_FEL_VITALITY              = 18731, // Increase Percent Stamina and Intellect
    TALENT_WL_DEMONIC_RESILENCE         = 30319, // Decrease Percent Damage Taken
    TALENT_WL_DEMONIC_TACTICS           = 30242, // Increase Melee / Spell Crit Rating
    TALENT_WL_UNHOLY_POWER              = 18769, // Increase Percent Void, Succu, Felhunter Melee/Damage Done
                                                 // Felguard Melee Damage Done - Imp Damage Done

    // Glyphs
    GLYPH_GLYPH_OF_IMP                  = 56248, // Increase Damage Done PCT (Imp)
    GLYPH_GLYPH_OF_FELGUARD             = 56246, // Increase AP (Felguard)
    GLYPH_GLYPH_OF_VOIDWALKER           = 56247, // Increase Total Stamina (Voidwalker)
};

enum DKPetCalculate
{
    // Scaling auras
    SPELL_DEATH_KNIGHT_PET_SCALING_01   = 54566, // spell_dk_pet_scaling_01 - Stamina / Strength / Damage Done 126
    SPELL_DEATH_KNIGHT_PET_SCALING_02   = 51996, // spell_dk_pet_scaling_02 - Damage Percent Done / Melee Slow / Mechanic Immunity (no handle required)
    SPELL_DEATH_KNIGHT_PET_SCALING_03   = 61697, // spell_gen_pet_calculate - Melee / Spell Hit Rating / Mechanic Immunity 10 (no handle required)
    SPELL_DEATH_KNIGHT_RUNE_WEAPON_02   = 51906, // spell_dk_rune_weapon_scaling_02 - Damage Done / Melee Slow / Damage Percent Done (no handle required)

    // Talents
    TALENT_DK_RAVENOUS_DEAD             = 48965, // Increase Bonus Stat from Owner (STRENGTH, STAMINA)

    // Glyphs
    GLYPH_DK_GLYPH_OF_GHOUL             = 58686, // Increase Bonus Stat from Owner (STRENGTH, STAMINA)

    // Spells
    SPELL_NIGHT_OF_THE_DEAD             = 55620
};

enum ShamanPetCalculate
{
    SPELL_FERAL_SPIRIT_PET_UNK_01       = 35674,
    SPELL_FERAL_SPIRIT_PET_UNK_02       = 35675,
    SPELL_FERAL_SPIRIT_PET_UNK_03       = 35676,
    SPELL_FERAL_SPIRIT_PET_SCALING_04   = 61783, // spell_gen_pet_calculate - Melee / Spell Hit Rating / Expertise
};

enum MiscPetCalculate
{
    SPELL_PET_HEALTH_SCALING            = 61679, // unhandled - Increase Health
    SPELL_PET_UNK_01                    = 67561,
    SPELL_PET_UNK_02                    = 67557,

    SPELL_MAGE_PET_PASSIVE_ELEMENTAL    = 44559,
};


class spell_gen_pet_calculate : public SpellScriptLoader
{
    public:

    spell_gen_pet_calculate() : SpellScriptLoader("spell_gen_pet_calculate") { }

    class spell_gen_pet_calculate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_pet_calculate_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
                if (pPet->GetOwnerScaling())
                    amount += int32(pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_COMBAT_RATING, CR_HIT_MELEE, pPet->GetOwnerCombatRating(CR_HIT_MELEE)));
        }

        void CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
                if (pPet->GetOwnerScaling())
                    amount += int32(pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_COMBAT_RATING, CR_HIT_SPELL, pPet->GetOwnerCombatRating(CR_HIT_SPELL)));
        }

        void CalculateAmountExpertise(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
            {
                if (pPet->GetOwnerScaling())
                {
                    float bonus = 0.0f;
                    float hitchance = std::max(pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_COMBAT_RATING, CR_HIT_MELEE, pPet->GetOwnerCombatRating(CR_HIT_MELEE)),
                                               pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_COMBAT_RATING, CR_HIT_SPELL, pPet->GetOwnerCombatRating(CR_HIT_SPELL)));

                    // Expertise should be at max if player has hit cap
                    if (hitchance >= PetSpellHitCapPct)
                        bonus = pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_COMBAT_RATING, CR_EXPERTISE, 60.0f);
                    else
                        bonus = pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_COMBAT_RATING, CR_EXPERTISE, pPet->GetOwnerCombatRating(CR_EXPERTISE));

                    amount += int32(bonus);
                }
            }
        }

        void CalculateAmountResistance(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
            {
                if (pPet->GetOwnerScaling())
                {
                    SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetMiscValue());
                    amount += int32(pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_RESISTANCE, schoolMask, pPet->GetOwnerResistance(schoolMask)));
                }
            }
        }

        void Register() override
        {
            switch (m_scriptSpellId)
            {
                case SPELL_HUNTER_PET_SCALING_04:
                case SPELL_FERAL_SPIRIT_PET_SCALING_04:
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountExpertise, EFFECT_2, SPELL_AURA_MOD_EXPERTISE);
                case SPELL_DEATH_KNIGHT_PET_SCALING_03:
                case SPELL_WARLOCK_PET_SCALING_05:
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
                    break;

                case SPELL_HUNTER_PET_SCALING_02:
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
                    break;

                case SPELL_HUNTER_PET_SCALING_03:
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
                    break;

                case SPELL_WARLOCK_PET_SCALING_03:
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
                    DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_pet_calculate_AuraScript::CalculateAmountResistance, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
                    break;

                default:
                    break;
            }
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_pet_calculate_AuraScript();
    }
};


#pragma region HunterPet
class spell_tamed_pet_scaling_04 : public SpellScriptLoader
{
    public:

    spell_tamed_pet_scaling_04() : SpellScriptLoader("spell_tamed_pet_scaling_04") { }

    class spell_tamed_pet_scaling_04_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tamed_pet_scaling_04_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void Register() override
        {
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_tamed_pet_scaling_04_AuraScript();
    }
};

class spell_hun_pet_scaling_01 : public SpellScriptLoader
{
    public:

    spell_hun_pet_scaling_01() : SpellScriptLoader("spell_hun_pet_scaling_01") { }

    class spell_hun_pet_scaling_01_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_pet_scaling_01_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void HandleAfterEffectApply(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pUnit = GetUnitOwner())
            {
                if (Pet* pPet = pUnit->ToPet())
                {
                    pPet->UpdateMaxHealth();
                    pPet->SetFullHealth();
                }
            }
        }

        void CalculateStaminaAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pUnit = GetUnitOwner())
            {
                Pet* pPet = pUnit->ToPet();
                if (!pPet)
                    return;

                float add = 0.0f;

                if (Player* pPlayerOwner = pPet->GetOwner()->ToPlayer())
                {
                    PetSpellMap::const_iterator itr = pPet->m_spells.find(TALENT_PET_WILD_HUNT_1);
                    if (itr == pPet->m_spells.end())
                        itr = pPet->m_spells.find(TALENT_PET_WILD_HUNT_2);

                    if (itr != pPet->m_spells.end()) // If pet has Wild Hunt
                    {
                        const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(itr->first); // Then get the SpellProto and add the dummy effect value
                        if (spellInfo)
                            add += spellInfo->Effects[EFFECT_0].CalcValue();
                    }
                }

                if (pPet->GetOwnerScaling())
                    amount += int32(pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_UNIT_MOD, UNIT_MOD_STAT_STAMINA,
                                    pPet->GetOwnerStat(STAT_STAMINA), add));
            }
        }

        void CalculateAttackPowerAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pUnit = GetUnitOwner())
            {
                Pet* pPet = pUnit->ToPet();
                if (!pPet)
                    return;

                if (Player* pPlayerOwner = pPet->GetOwner()->ToPlayer())
                {
                    float add = 0.0f;

                    PetSpellMap::const_iterator itr = pPet->m_spells.find(TALENT_PET_WILD_HUNT_1);
                    if (itr == pPet->m_spells.end())
                        itr = pPet->m_spells.find(TALENT_PET_WILD_HUNT_2);

                    if (itr != pPet->m_spells.end()) // If pet has Wild Hunt
                    {
                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first); // Then get the SpellProto and add the dummy effect value
                        if (spellInfo)
                            add += spellInfo->Effects[EFFECT_1].CalcValue();
                    }

                    if (pPet->GetOwnerScaling())
                        amount += int32(pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_ATTACK_POWER, RANGED_ATTACK,
                                        pPet->GetOwnerAttackPower(RANGED_ATTACK), add));
                }
            }
        }

        void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pUnit = GetUnitOwner())
            {
                Pet* pPet = pUnit->ToPet();
                if (!pPet)
                    return;

                if (Player* pPlayerOwner = pPet->GetOwner()->ToPlayer())
                {
                    float add = 0.0f;
                    float bonus = 0.0f;

                    PetSpellMap::const_iterator itr = (pPet->m_spells.find(TALENT_PET_WILD_HUNT_1));
                    if (itr == pPet->m_spells.end())
                        itr = pPet->m_spells.find(TALENT_PET_WILD_HUNT_2);

                    if (itr != pPet->m_spells.end()) // If pet has Wild Hunt
                    {
                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first); // Then get the SpellProto and add the dummy effect value
                        if (spellInfo)
                            add += spellInfo->Effects[EFFECT_1].CalcValue();
                    }

                    if (pPet->GetOwnerScaling())
                        bonus += int32(pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_SPELL_POWER, SPELL_SCHOOL_NORMAL,
                                        pPet->GetOwnerAttackPower(RANGED_ATTACK), add));

                    amount += int32(bonus);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_pet_scaling_01_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_01_AuraScript::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_01_AuraScript::CalculateAttackPowerAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_scaling_01_AuraScript::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_pet_scaling_01_AuraScript();
    }
};

class spell_hun_pet_passive_damage_done : public SpellScriptLoader
{
    public:

    spell_hun_pet_passive_damage_done() : SpellScriptLoader("spell_hun_pet_passive_damage_done") { }

    class spell_hun_pet_passive_damage_done_AuraScript : public AuraScript
    {
        public:

            PrepareAuraScript(spell_hun_pet_passive_damage_done_AuraScript);

            bool Load() override
            {
                if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                    return false;
                return true;
            }

            void CalculateAmountDamageDone(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (GetUnitOwner()->isPet())
                {
                    if (Pet* pPet = GetUnitOwner()->ToPet())
                    {
                        if (Unit* pUnitOwner = pPet->GetOwner())
                        {
                            //  Pet's base damage changes depending on happiness
                            if (pPet->isHunterPet())
                                amount += int32(getHappinessPCT(pPet));

                            if (AuraEffect* pAuraCobraReflexes = pPet->GetAuraEffectOfRankedSpell(TALENT_PET_COBRA_REFLEXES, EFFECT_0))
                                // Gives half of the aspd increase as done dmg malus
                                // and because blizzard (nearly) always chooses the
                                // outcome that is worse for the player in regards
                                // to rounding, we want 15 -> 8 and 30 -> 15,
                                // hence we are using the ( + 1) / 2 construction
                                amount -= (pAuraCobraReflexes->GetAmount() + 1) / 2;
                        }
                    }
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_pet_passive_damage_done_AuraScript::CalculateAmountDamageDone, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }

        private:

            float getHappinessPCT(Pet* pPet)
            {
                if (pPet)
                {
                    switch (pPet->GetHappinessState())
                    {
                        case HAPPY:
                            // 125% of normal
                            return 125.0f;
                        case CONTENT:
                            // 100% of normal
                            return 100.0f;
                        case UNHAPPY:
                            // 75% of normal
                            return 75.0f;
                    }
                }

                return 100.0f;
            }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_pet_passive_damage_done_AuraScript();
    }
};
#pragma endregion


#pragma region WarlockPet
class spell_warl_pet_scaling_01 : public SpellScriptLoader
{
    public:

    spell_warl_pet_scaling_01() : SpellScriptLoader("spell_warl_pet_scaling_01") { }

    class spell_warl_pet_scaling_01_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_01_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void HandleAfterEffectApply(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pUnit = GetUnitOwner())
            {
                if (pUnit->isPet())
                {
                    if (Pet* pPet = pUnit->ToPet())
                    {
                        pPet->UpdateMaxHealth();
                        pPet->SetFullHealth();
                    }
                }
            }
        }

        void HandleOnEffectRemove(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pUnit = GetUnitOwner())
            {
                if (pUnit->isPet())
                {
                    const PetLevelInfo* pInfo = sObjectMgr->GetPetLevelInfo(pUnit->GetEntry(), pUnit->getLevel());
                    if (!pInfo)
                        return;

                    pUnit->ToPet()->SetCreateHealth(pInfo->health);
                }
            }
        }

        void CalculateStaminaAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            float bonusSta = 0.0f;

            if (Pet* pPet = GetUnitOwner()->ToPet())
                if (pPet->GetOwnerScaling())
                    bonusSta += pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_UNIT_MOD, UNIT_MOD_STAT_STAMINA,
                                                                            pPet->GetOwnerStat(STAT_STAMINA));

            amount += int32(bonusSta);
        }

        void CalculateAttackPowerAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
            {
                if (Player* pPlayerOwner = pPet->GetOwner()->ToPlayer())
                {
                    float bonus = 0.0f;
                    if (pPet->GetOwnerScaling())
                        bonus += pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_ATTACK_POWER, BASE_ATTACK,
                                                                            pPet->GetOwnerSpellPower(SPELL_SCHOOL_FIRE));

                    const CreatureTemplate*  pCreatureTemplate = sObjectMgr->GetCreatureTemplate(pPet->GetEntry());
                    if (pCreatureTemplate)
                    {
                        if (pCreatureTemplate->family == CREATURE_FAMILY_FELGUARD)
                        {
                            if (AuraEffect* pAuraEffect = pPlayerOwner->GetAuraEffect(GLYPH_GLYPH_OF_FELGUARD, EFFECT_0))
                            {
                                float baseAttPower =
                                    pPet->GetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE)
                                    * pPet->GetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_PCT);

                                bonus += CalculatePctN(bonus + baseAttPower, pAuraEffect->GetAmount());
                            }
                        }
                    }

                    amount += int32(bonus);
                }
            }
        }

        void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
            {
                float bonus = 0.0f;
                if (pPet->GetOwnerScaling())
                    bonus += pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_SPELL_POWER, SPELL_SCHOOL_FIRE,
                                                                        pPet->GetOwnerSpellPower(SPELL_SCHOOL_FIRE));

                amount += int32(bonus);
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warl_pet_scaling_01_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_pet_scaling_01_AuraScript::HandleOnEffectRemove, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_01_AuraScript::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_01_AuraScript::CalculateAttackPowerAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_01_AuraScript::CalculateDamageDoneAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_pet_scaling_01_AuraScript();
    }
};

class spell_warl_pet_scaling_02 : public SpellScriptLoader
{
    public:

    spell_warl_pet_scaling_02() : SpellScriptLoader("spell_warl_pet_scaling_02") { }

    class spell_warl_pet_scaling_02_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_02_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void HandleAfterEffectApply(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
            {
                pPet->UpdateMaxPower(pPet->getPowerType());
                pPet->SetPower(pPet->getPowerType(), pPet->GetMaxPower(pPet->getPowerType()));
            }
        }

        void HandleOnEffectRemove(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
        {
            if (Pet* pPet = GetUnitOwner()->ToPet())
            {
                const PetLevelInfo* pInfo = sObjectMgr->GetPetLevelInfo(pPet->GetEntry(), pPet->getLevel());
                if (!pInfo)
                    return;

                pPet->SetCreateMana(pInfo->mana);
            }
        }

        void CalculateIntellectAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            float bonusInt = 0.0f;

            if (Pet* pPet = GetUnitOwner()->ToPet())
                if (pPet->GetOwnerScaling())
                    bonusInt += pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_UNIT_MOD, UNIT_MOD_STAT_INTELLECT, pPet->GetOwnerStat(STAT_INTELLECT));

            amount += int32(bonusInt);
        }

        void CalculateArmorAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            float bonus = 0.0f;

            if (Pet* pPet = GetUnitOwner()->ToPet())
                if (pPet->GetOwnerScaling())
                    bonus += pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_RESISTANCE, SPELL_SCHOOL_MASK_NORMAL, pPet->GetOwnerResistance(SPELL_SCHOOL_MASK_NORMAL));

            amount += int32(bonus);
        }

        void CalculateFireResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            float bonus = 0.0f;

            if (Pet* pPet = GetUnitOwner()->ToPet())
                if (pPet->GetOwnerScaling())
                    bonus += pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_RESISTANCE, SPELL_SCHOOL_MASK_FIRE, pPet->GetOwnerResistance(SPELL_SCHOOL_MASK_FIRE));

            amount += int32(bonus);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warl_pet_scaling_02_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_pet_scaling_02_AuraScript::HandleOnEffectRemove, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_02_AuraScript::CalculateIntellectAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_02_AuraScript::CalculateArmorAmount, EFFECT_1, SPELL_AURA_MOD_RESISTANCE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_02_AuraScript::CalculateFireResistanceAmount, EFFECT_2, SPELL_AURA_MOD_RESISTANCE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_pet_scaling_02_AuraScript();
    }
};

class spell_warl_pet_scaling_04 : public SpellScriptLoader
{
    public:

    spell_warl_pet_scaling_04() : SpellScriptLoader("spell_warl_pet_scaling_04") { }

    class spell_warl_pet_scaling_04_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_pet_scaling_04_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void CalculateShadowResistanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            float bonus = 0.0f;

            if (Pet* pPet = GetUnitOwner()->ToPet())
                if (pPet->GetOwnerScaling())
                    bonus += pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_RESISTANCE, SPELL_SCHOOL_MASK_SHADOW, pPet->GetOwnerResistance(SPELL_SCHOOL_MASK_SHADOW));

            amount += int32(bonus);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_pet_scaling_04_AuraScript::CalculateShadowResistanceAmount, EFFECT_0, SPELL_AURA_MOD_RESISTANCE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_pet_scaling_04_AuraScript();
    }
};
#pragma endregion


#pragma region DKPet
class spell_dk_avoidance_passive : public SpellScriptLoader
{
    public:

    spell_dk_avoidance_passive() : SpellScriptLoader("spell_dk_avoidance_passive") { }

    class spell_dk_avoidance_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_avoidance_passive_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void CalculateAvoidanceAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pUnit = GetUnitOwner())
            {
                if (Unit* pUnitOwner = pUnit->GetOwner())
                {
                    if (pUnit->GetEntry() == ENTRY_ARMY_OF_THE_DEAD_GHOUL) // Army of the dead ghoul
                        amount -= 90;
                    else if (Aura* pAura = pUnitOwner->GetAuraOfRankedSpell(SPELL_NIGHT_OF_THE_DEAD)) // Night of the dead
                        amount -= pAura->GetSpellInfo()->Effects[EFFECT_2].CalcValue();
                }
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_avoidance_passive_AuraScript::CalculateAvoidanceAmount, EFFECT_0, SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_avoidance_passive_AuraScript();
    }
};

class spell_dk_pet_scaling_01 : public SpellScriptLoader
{
    public:

    spell_dk_pet_scaling_01() : SpellScriptLoader("spell_dk_pet_scaling_01") { }

    class spell_dk_pet_scaling_01_AuraScript : public AuraScript
    {
        public:

            PrepareAuraScript(spell_dk_pet_scaling_01_AuraScript);

            bool Load() override
            {
                if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                    return false;

                tempBonus = 0;
                return true;
            }

            void HandleAfterEffectApply(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* pUnit = GetUnitOwner())
                {
                    if (Guardian* pPet = GetUnitOwner()->ToGuardian())
                    {
                        pPet->UpdateMaxHealth();
                        pPet->SetFullHealth();
                    }
                }
            }

            void HandleOnEffectRemove(AuraEffect const* /* aurEff */, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* pUnit = GetUnitOwner())
                {
                    const PetLevelInfo* pInfo = sObjectMgr->GetPetLevelInfo(pUnit->GetEntry(), pUnit->getLevel());
                    if (!pInfo)
                        return;

                    pUnit->SetCreateHealth(pInfo->health);
                }
            }

            void CalculateStaminaAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Guardian* pPet = GetUnitOwner()->ToGuardian())
                {
                    if (Player* pPlayerOwner = pPet->GetOwner()->ToPlayer())
                    {
                        float add = 0.0f;
                        float bonusSta = 0.0f;

                        if (Aura* pAura = pPlayerOwner->GetAuraOfRankedSpell(TALENT_DK_RAVENOUS_DEAD))
                            add += pAura->GetSpellInfo()->Effects[EFFECT_1].BasePoints;

                        if (const AuraEffect* pAuraEffect = pPlayerOwner->GetAuraEffect(GLYPH_DK_GLYPH_OF_GHOUL, EFFECT_0))
                            add += pAuraEffect->GetAmount();

                        // DK Ghoul only increases stamina if player bonus stat is higher than (basehealth / multiplicator) of Ghoul
                        float baseStaFromHealth = pPet->GetCreateHealth() / 10.0f;

                        if (pPet->GetOwnerScaling())
                            bonusSta = pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_UNIT_MOD, UNIT_MOD_STAT_STAMINA,
                                                                                    pPet->GetOwnerStat(STAT_STAMINA), add);
                        bonusSta -= baseStaFromHealth;

                        if (bonusSta < 0.0f)
                            bonusSta = 0.0f;

                        tempBonus = bonusSta;
                        amount += int32(bonusSta);
                    }
                }
            }

            void CalculateStrengthAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Guardian* pPet = GetUnitOwner()->ToGuardian())
                {
                    if (Player* pPlayerOwner = pPet->GetOwner()->ToPlayer())
                    {
                        float add = 0.0f;
                        float bonusStr = 0.0f;

                        if (Aura* pAura = pPlayerOwner->GetAuraOfRankedSpell(TALENT_DK_RAVENOUS_DEAD))
                            add += pAura->GetSpellInfo()->Effects[EFFECT_1].BasePoints;

                        if (const AuraEffect* pAuraEffect = pPlayerOwner->GetAuraEffect(GLYPH_DK_GLYPH_OF_GHOUL, EFFECT_0))
                            add += pAuraEffect->GetAmount();

                        if (pPet->GetOwnerScaling())
                            bonusStr = pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_UNIT_MOD, UNIT_MOD_STAT_STRENGTH,
                                                                                    pPet->GetOwnerStat(STAT_STRENGTH), add);

                        amount += int32(bonusStr);
                    }
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dk_pet_scaling_01_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_pet_scaling_01_AuraScript::HandleOnEffectRemove, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_01_AuraScript::CalculateStaminaAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_01_AuraScript::CalculateStrengthAmount, EFFECT_1, SPELL_AURA_MOD_STAT);
            }

        private:

            uint32 tempBonus;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_pet_scaling_01_AuraScript();
    }
};

class spell_dk_pet_scaling_02 : public SpellScriptLoader
{
    public:

    spell_dk_pet_scaling_02() : SpellScriptLoader("spell_dk_pet_scaling_02") { }

    class spell_dk_pet_scaling_02_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_pet_scaling_02_AuraScript);

        bool Load() override
        {
            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

        void CalculateMeleeHasteAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
        {
            float bonusHaste = 0.0f;

            if (Guardian* pPet = GetUnitOwner()->ToGuardian())
                if (pPet->GetOwnerScaling())
                    bonusHaste = pPet->GetOwnerScaling()->CalculateScaling(STAT_IDENTIFIER_COMBAT_RATING, CR_HASTE_MELEE, pPet->GetOwnerCombatRating(CR_HASTE_MELEE));

            amount += int32(bonusHaste);
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_pet_scaling_02_AuraScript::CalculateMeleeHasteAmount, EFFECT_1, SPELL_AURA_MELEE_SLOW);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_pet_scaling_02_AuraScript();
    }
};

//class spell_dk_rune_weapon_scaling_02 : public SpellScriptLoader
//{
//    public:
//
//    spell_dk_rune_weapon_scaling_02() : SpellScriptLoader("spell_dk_rune_weapon_scaling_02") { }
//
//    class spell_dk_rune_weapon_scaling_02_AuraScript : public AuraScript
//    {
//        PrepareAuraScript(spell_dk_rune_weapon_scaling_02_AuraScript);
//
//        bool Load() override
//        {
//            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
//                return false;
//            return true;
//        }
//
//        void CalculateDamageDoneAmount(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
//        {
//            if (Unit* pUnit = GetUnitOwner())
//            {
//                if (pUnit->isGuardian())
//                {
//                    if (Unit* pUnitOwner = pUnit->GetOwner())
//                    {
//                        ((Guardian*)pUnit)->SetBonusDamage(pUnitOwner->GetTotalAttackPowerValue(BASE_ATTACK));
//
//                        amount += pUnitOwner->CalculateBaseWeaponDamage(BASE_ATTACK, true, true);
//                    }
//                }
//            }
//        }
//
//        void CalculateAmountMeleeHaste(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
//        {
//            if (Player* pPlayerOwner = GetUnitOwner()->GetOwner()->ToPlayer())
//            {
//                // For others recalculate it from:
//                float hasteMelee = 0.0f;
//                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
//                hasteMelee += (1 / pPlayerOwner->GetHaste(CTYPE_BASE) - 1) * 100;
//
//                amount += int32(hasteMelee);
//            }
//        }
//
//        void Register() override
//        {
//            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_rune_weapon_scaling_02_AuraScript::CalculateDamageDoneAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
//            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_rune_weapon_scaling_02_AuraScript::CalculateAmountMeleeHaste, EFFECT_1, SPELL_AURA_MELEE_SLOW);
//        }
//    };
//
//    AuraScript* GetAuraScript() const override
//    {
//        return new spell_dk_rune_weapon_scaling_02_AuraScript();
//    }
//};
#pragma endregion


#pragma region ShamanPet
//class spell_sha_pet_scaling_04 : public SpellScriptLoader
//{
//    public:
//
//    spell_sha_pet_scaling_04() : SpellScriptLoader("spell_sha_pet_scaling_04") { }
//
//    class spell_sha_pet_scaling_04_AuraScript : public AuraScript
//    {
//        PrepareAuraScript(spell_sha_pet_scaling_04_AuraScript);
//
//        bool Load()
//        {
//            if (!GetUnitOwner() || !GetUnitOwner()->isPet() || !GetUnitOwner()->GetOwner() || GetUnitOwner()->GetOwner()->GetTypeId() != TYPEID_PLAYER)
//                return false;
//            return true;
//        }
//
//        void CalculateAmountMeleeHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
//        {
//            if (Player* owner = GetUnitOwner()->GetOwner()->ToPlayer())
//            {
//                // For others recalculate it from:
//                float HitMelee = 0.0f;
//                // Increase hit from SPELL_AURA_MOD_HIT_CHANCE
//                HitMelee += owner->GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE, true);
//                // Increase hit melee from meele hit ratings
//                HitMelee += owner->GetRatingBonusValue(CR_HIT_MELEE);
//
//                amount += int32(HitMelee);
//            }
//        }
//
//        void CalculateAmountSpellHit(AuraEffect const* /* aurEff */, int32& amount, bool& /*canBeRecalculated*/)
//        {
//            if (Player* owner = GetUnitOwner()->GetOwner()->ToPlayer())
//            {
//                // For others recalculate it from:
//                float HitSpell = 0.0f;
//                // Increase hit from SPELL_AURA_MOD_SPELL_HIT_CHANCE
//                HitSpell += owner->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE, true);
//                // Increase hit spell from spell hit ratings
//                HitSpell += owner->GetRatingBonusValue(CR_HIT_SPELL);
//
//                amount += int32(HitSpell);
//            }
//        }
//
//        void Register()
//        {
//            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_pet_scaling_04_AuraScript::CalculateAmountMeleeHit, EFFECT_0, SPELL_AURA_MOD_HIT_CHANCE);
//            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_pet_scaling_04_AuraScript::CalculateAmountSpellHit, EFFECT_1, SPELL_AURA_MOD_SPELL_HIT_CHANCE);
//        }
//    };
//
//    AuraScript* GetAuraScript() const
//    {
//        return new spell_sha_pet_scaling_04_AuraScript();
//    }
//};
#pragma endregion


void AddSC_pet_spell_scripts()
{
    // Generic
    new spell_gen_pet_calculate();

    // Hunter
    new spell_tamed_pet_scaling_04();
    new spell_hun_pet_scaling_01();
    new spell_hun_pet_passive_damage_done();

    // Warlock
    new spell_warl_pet_scaling_01();
    new spell_warl_pet_scaling_02();
    new spell_warl_pet_scaling_04();

    // DK
    new spell_dk_avoidance_passive();
    new spell_dk_pet_scaling_01();
    new spell_dk_pet_scaling_02();
    //new spell_dk_rune_weapon_scaling_02();

    // Shaman
    //new spell_sha_pet_scaling_04();
}
