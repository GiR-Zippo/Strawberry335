/*
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

/*
 * Scripts for spells with SPELLFAMILY_PRIEST and SPELLFAMILY_GENERIC spells used by priest players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pri_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum PriestSpells
{
    PRIEST_SPELL_GUARDIAN_SPIRIT_HEAL           = 48153,
    PRIEST_SPELL_PENANCE_R1                     = 47540,
    PRIEST_SPELL_PENANCE_R1_DAMAGE              = 47758,
    PRIEST_SPELL_PENANCE_R1_HEAL                = 47757,
    PRIEST_SPELL_REFLECTIVE_SHIELD_TRIGGERED    = 33619,
    PRIEST_SPELL_REFLECTIVE_SHIELD_R1           = 33201,
    PRIEST_SPELL_VAMPIRIC_TOUCH_DISPEL          = 64085,
    PRIEST_SPELL_EMPOWERED_RENEW                = 63544,
    PRIEST_ICON_ID_EMPOWERED_RENEW_TALENT       = 3021,
    SPELL_PRIEST_GLYPH_OF_CIRCLE_OF_HEALING     = 55675,
    SPELL_PRIEST_GLYPH_OF_LIGHTWELL             = 55673,
};

class PowerCheck
{
public:
    explicit PowerCheck(Powers const power) : _power(power) { }
    bool operator()(WorldObject* obj) const
    {
        if (Unit* target = obj->ToUnit())
            return target->getPowerType() != _power;
        return true;
    }
private:
    Powers const _power;
};

class RaidCheck
{
public:
    explicit RaidCheck(Unit const* caster) : _caster(caster) { }
    bool operator()(WorldObject* obj) const
    {
        if (Unit* target = obj->ToUnit())
            return !_caster->IsInRaidWith(target);
        return true;
    }
private:
    Unit const* _caster;
};

// Guardian Spirit
class spell_pri_guardian_spirit : public SpellScriptLoader
{
    public:
        spell_pri_guardian_spirit() : SpellScriptLoader("spell_pri_guardian_spirit") { }

        class spell_pri_guardian_spirit_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_guardian_spirit_AuraScript);

            uint32 healPct;

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(PRIEST_SPELL_GUARDIAN_SPIRIT_HEAL))
                    return false;
                return true;
            }

            bool Load()
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                Unit* target = GetTarget();
                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                int32 healAmount = int32(target->CountPctFromMaxHealth(healPct));
                // remove the aura now, we don't want 40% healing bonus
                Remove(AURA_REMOVE_BY_ENEMY_SPELL);
                target->CastCustomSpell(target, PRIEST_SPELL_GUARDIAN_SPIRIT_HEAL, &healAmount, NULL, NULL, true);
                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_guardian_spirit_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_guardian_spirit_AuraScript::Absorb, EFFECT_1);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_guardian_spirit_AuraScript();
        }
};

class spell_pri_mana_burn : public SpellScriptLoader
{
    public:
        spell_pri_mana_burn() : SpellScriptLoader("spell_pri_mana_burn") { }

        class spell_pri_mana_burn_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_mana_burn_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* unitTarget = GetHitUnit())
                    unitTarget->RemoveAurasWithMechanic((1 << MECHANIC_FEAR) | (1 << MECHANIC_POLYMORPH));
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_pri_mana_burn_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_mana_burn_SpellScript;
        }
};

class spell_pri_mind_sear : public SpellScriptLoader
{
    public:
        spell_pri_mind_sear() : SpellScriptLoader("spell_pri_mind_sear") { }

        class spell_pri_mind_sear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_mind_sear_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(Trinity::ObjectGUIDCheck(GetCaster()->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_mind_sear_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_mind_sear_SpellScript();
        }
};

class spell_pri_pain_and_suffering_proc : public SpellScriptLoader
{
    public:
        spell_pri_pain_and_suffering_proc() : SpellScriptLoader("spell_pri_pain_and_suffering_proc") { }

        // 47948 Pain and Suffering (proc)
        class spell_pri_pain_and_suffering_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_pain_and_suffering_proc_SpellScript);

            void HandleEffectScriptEffect(SpellEffIndex /*effIndex*/)
            {
                // Refresh Shadow Word: Pain on target
                if (Unit* unitTarget = GetHitUnit())
                    if (AuraEffect* aur = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_PRIEST, 0x8000, 0, 0, GetCaster()->GetGUID()))
                        aur->GetBase()->RefreshDuration();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pri_pain_and_suffering_proc_SpellScript::HandleEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_pain_and_suffering_proc_SpellScript;
        }
};

class spell_pri_penance : public SpellScriptLoader
{
    public:
        spell_pri_penance() : SpellScriptLoader("spell_pri_penance") { }

        class spell_pri_penance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_penance_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(PRIEST_SPELL_PENANCE_R1))
                    return false;
                // can't use other spell than this penance due to spell_ranks dependency
                if (sSpellMgr->GetFirstSpellInChain(PRIEST_SPELL_PENANCE_R1) != sSpellMgr->GetFirstSpellInChain(spellEntry->Id))
                    return false;

                uint8 rank = sSpellMgr->GetSpellRank(spellEntry->Id);
                if (!sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_R1_DAMAGE, rank, true))
                    return false;
                if (!sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_R1_HEAL, rank, true))
                    return false;

                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                {
                    if (!unitTarget->isAlive())
                        return;

                    uint8 rank = sSpellMgr->GetSpellRank(GetSpellInfo()->Id);

                    if (caster->IsFriendlyTo(unitTarget))
                        caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_R1_HEAL, rank), false, 0);
                    else
                        caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(PRIEST_SPELL_PENANCE_R1_DAMAGE, rank), false, 0);
                }
            }

            SpellCastResult CheckCast()
            {
                Player* caster = GetCaster()->ToPlayer();
                if (Unit* target = GetExplTargetUnit())
                    if (!caster->IsFriendlyTo(target) && !caster->IsValidAttackTarget(target))
                        return SPELL_FAILED_BAD_TARGETS;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                // add dummy effect spell handler to Penance
                OnEffectHitTarget += SpellEffectFn(spell_pri_penance_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_pri_penance_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pri_penance_SpellScript;
        }
};

// Reflective Shield
class spell_pri_reflective_shield_trigger : public SpellScriptLoader
{
    public:
        spell_pri_reflective_shield_trigger() : SpellScriptLoader("spell_pri_reflective_shield_trigger") { }

        class spell_pri_reflective_shield_trigger_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_reflective_shield_trigger_AuraScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(PRIEST_SPELL_REFLECTIVE_SHIELD_TRIGGERED) || !sSpellMgr->GetSpellInfo(PRIEST_SPELL_REFLECTIVE_SHIELD_R1))
                    return false;
                return true;
            }

            void Trigger(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                Unit* target = GetTarget();
                if (dmgInfo.GetAttacker() == target)
                    return;

                if (GetCaster())
                    if (AuraEffect* talentAurEff = target->GetAuraEffectOfRankedSpell(PRIEST_SPELL_REFLECTIVE_SHIELD_R1, EFFECT_0))
                    {
                        int32 bp = CalculatePctN(absorbAmount, talentAurEff->GetAmount());
                        target->CastCustomSpell(dmgInfo.GetAttacker(), PRIEST_SPELL_REFLECTIVE_SHIELD_TRIGGERED, &bp, NULL, NULL, true, NULL, aurEff);
                    }
            }

            void Register()
            {
                 AfterEffectAbsorb += AuraEffectAbsorbFn(spell_pri_reflective_shield_trigger_AuraScript::Trigger, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_reflective_shield_trigger_AuraScript();
        }
};

enum PrayerOfMending
{
    SPELL_T9_HEALING_2_PIECE = 67201,
    SPELL_SPIRITUAL_HEAL = 14898,
    SPELL_DIVINE_PROVIDENCE = 47562,
    SPELL_TWIN_DISC = 47586,
};
// Prayer of Mending Heal
class spell_pri_prayer_of_mending_heal : public SpellScriptLoader
{
public:
    spell_pri_prayer_of_mending_heal() : SpellScriptLoader("spell_pri_prayer_of_mending_heal") { }

    class spell_pri_prayer_of_mending_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_prayer_of_mending_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetOriginalCaster())
            {
                int32 heal = GetHitHeal();
                uint32 multi = 0;

                if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_T9_HEALING_2_PIECE, EFFECT_0))
                    multi += aurEff->GetAmount();
                if (AuraEffect* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_SPIRITUAL_HEAL, EFFECT_0))
                    multi += aurEff->GetAmount();
                if (AuraEffect* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_DIVINE_PROVIDENCE, EFFECT_0))
                    multi += aurEff->GetAmount();
                if (AuraEffect* aurEff = caster->GetAuraEffectOfRankedSpell(SPELL_TWIN_DISC, EFFECT_0))
                    multi += aurEff->GetAmount();

                if (multi)
                    AddPctN(heal, multi);
                SetHitHeal(heal);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_prayer_of_mending_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pri_prayer_of_mending_heal_SpellScript();
    }
};

class spell_pri_vampiric_touch : public SpellScriptLoader
{
    public:
        spell_pri_vampiric_touch() : SpellScriptLoader("spell_pri_vampiric_touch") { }

        class spell_pri_vampiric_touch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_vampiric_touch_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(PRIEST_SPELL_VAMPIRIC_TOUCH_DISPEL))
                    return false;
                return true;
            }

            void HandleDispel(DispelInfo* /*dispelInfo*/)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetUnitOwner())
                        if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                        {
                            int32 damage = aurEff->GetAmount() * 8;
                            // backfire damage
                            caster->CastCustomSpell(target, PRIEST_SPELL_VAMPIRIC_TOUCH_DISPEL, &damage, NULL, NULL, true, NULL, aurEff);
                        }
            }

            void Register()
            {
                AfterDispel += AuraDispelFn(spell_pri_vampiric_touch_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pri_vampiric_touch_AuraScript();
        }
};

class spell_priest_renew : public SpellScriptLoader
{
    public:
        spell_priest_renew() : SpellScriptLoader("spell_priest_renew") { }

        class spell_priest_renew_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_priest_renew_AuraScript);

            bool Load()
            {
                return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleApplyEffect(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    // Empowered Renew
                    if (AuraEffect const* empoweredRenewAurEff = caster->GetDummyAuraEffect(SPELLFAMILY_PRIEST, PRIEST_ICON_ID_EMPOWERED_RENEW_TALENT, EFFECT_1))
                    {
                        uint32 heal = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), GetEffect(EFFECT_0)->GetAmount(), DOT);
                        heal = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, DOT);

                        int32 basepoints0 = empoweredRenewAurEff->GetAmount() * GetEffect(EFFECT_0)->GetTotalTicks() * int32(heal) / 100;
                        caster->CastCustomSpell(GetTarget(), PRIEST_SPELL_EMPOWERED_RENEW, &basepoints0, NULL, NULL, true, NULL, aurEff);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_priest_renew_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_priest_renew_AuraScript();
        }
};

// 64844 - Divine Hymn
class spell_pri_divine_hymn : public SpellScriptLoader
{
public:
    spell_pri_divine_hymn() : SpellScriptLoader("spell_pri_divine_hymn") { }
    class spell_pri_divine_hymn_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_divine_hymn_SpellScript);
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(RaidCheck(GetCaster()));
            uint32 const maxTargets = 3;
            if (targets.size() > maxTargets)
            {
                targets.sort(Trinity::HealthPctOrderPred());
                targets.resize(maxTargets);
            }
        }
        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_divine_hymn_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ALLY);
        }
    };
    SpellScript* GetSpellScript() const override
    {
        return new spell_pri_divine_hymn_SpellScript();
    }
};

// 64904 - Hymn of Hope
class spell_pri_hymn_of_hope : public SpellScriptLoader
{
public:
    spell_pri_hymn_of_hope() : SpellScriptLoader("spell_pri_hymn_of_hope") { }
    class spell_pri_hymn_of_hope_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_hymn_of_hope_SpellScript);
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(PowerCheck(POWER_MANA));
            targets.remove_if(RaidCheck(GetCaster()));
            uint32 const maxTargets = 3;
            if (targets.size() > maxTargets)
            {
                targets.sort(Trinity::PowerPctOrderPred(POWER_MANA));
                targets.resize(maxTargets);
            }
        }
        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_hymn_of_hope_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ALLY);
        }
    };
    SpellScript* GetSpellScript() const override
    {
        return new spell_pri_hymn_of_hope_SpellScript();
    }
};

// -34861 - Circle of Healing
class spell_pri_circle_of_healing : public SpellScriptLoader
{
public:
    spell_pri_circle_of_healing() : SpellScriptLoader("spell_pri_circle_of_healing") { }
    class spell_pri_circle_of_healing_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_circle_of_healing_SpellScript);
        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_GLYPH_OF_CIRCLE_OF_HEALING))
                return false;
            return true;
        }
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(RaidCheck(GetCaster()));
            uint32 const maxTargets = GetCaster()->HasAura(SPELL_PRIEST_GLYPH_OF_CIRCLE_OF_HEALING) ? 6 : 5; // Glyph of Circle of Healing
            if (targets.size() > maxTargets)
            {
                targets.sort(Trinity::HealthPctOrderPred());
                targets.resize(maxTargets);
            }
        }
        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_circle_of_healing_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };
    SpellScript* GetSpellScript() const override
    {
        return new spell_pri_circle_of_healing_SpellScript();
    }
};

void AddSC_priest_spell_scripts()
{
    new spell_pri_guardian_spirit();
    new spell_pri_mana_burn();
    new spell_pri_pain_and_suffering_proc();
    new spell_pri_penance();
    new spell_pri_reflective_shield_trigger();
    new spell_pri_mind_sear();
    new spell_pri_prayer_of_mending_heal();
    new spell_pri_vampiric_touch();
    new spell_priest_renew();
    new spell_pri_divine_hymn();
    new spell_pri_hymn_of_hope();
    new spell_pri_circle_of_healing();
}
