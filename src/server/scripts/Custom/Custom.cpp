#include "ObjectMgr.h"

void LoadEvents();
void LoadNpcs();

void AddSC_Custom_Boss_Thrall();
void AddSC_Custom_Boss_VarianWrynn();

class spell_finkles_mixture_event : public SpellScriptLoader
{
public:
    spell_finkles_mixture_event() : SpellScriptLoader("spell_finkles_mixture_event") { }
     
    class spell_finkles_mixture_eventAuraScript : public AuraScript
    {
        PrepareAuraScript(spell_finkles_mixture_eventAuraScript);
     
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            amount = -1;
        }
     
        void HandleOnEffectAbsorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
            absorbAmount = 0;
     
            if (Unit* target = GetTarget())
            {
                if (target->GetHealth() <= dmgInfo.GetDamage() && target->GetHealth() > 5000)
                    absorbAmount = (dmgInfo.GetDamage() - target->GetHealth()) + 1;
            }

        }
     
        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_finkles_mixture_eventAuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_finkles_mixture_eventAuraScript::HandleOnEffectAbsorb, EFFECT_0);
        }
    };
     
    AuraScript* GetAuraScript() const
    {
        return new spell_finkles_mixture_eventAuraScript();
    }
};

class spell_custom_spirit_link_8y : public SpellScriptLoader
{
public:
    spell_custom_spirit_link_8y() : SpellScriptLoader("spell_custom_spirit_link_8y") { }

    class spell_custom_spirit_link_8y_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_custom_spirit_link_8y_SpellScript);

        bool Load()
        {
            pct = 0;
            players.clear();
            return true;
        }

        void CalculatePercentage()
        {
            std::list<Player*> targets = GetCaster()->GetNearestPlayersList(8, true);
            for (std::list<Player*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                pct = ((pct * players.size()) + ((*itr)->GetHealthPct() / 100.f)) / float(players.size() + 1);
                players.insert(*itr);
            }
        }

        void HandleAfterCast()
        {
            for (std::set<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                int32 damage = 0;
                int32 heal = 0;

                uint32 const bestValue = uint32((*itr)->GetMaxHealth() * pct);

                if (bestValue > (*itr)->GetHealth())
                    heal = bestValue - (*itr)->GetHealth();
                else
                    damage = (*itr)->GetHealth() - bestValue;

                if (damage + heal > 1)
                    (*itr)->CastCustomSpell(*itr, 22806, &heal, &damage, NULL, true);
            }
        }

        void Register()
        {
            BeforeCast += SpellCastFn(spell_custom_spirit_link_8y_SpellScript::CalculatePercentage);
            AfterCast += SpellCastFn(spell_custom_spirit_link_8y_SpellScript::HandleAfterCast);
        }

    private:
        float pct;
        std::set<Player*> players;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_custom_spirit_link_8y_SpellScript();
    }
};

class spell_custom_spirit_link_50y : public SpellScriptLoader
{
public:
    spell_custom_spirit_link_50y() : SpellScriptLoader("spell_custom_spirit_link_50y") { }

    class spell_custom_spirit_link_50y_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_custom_spirit_link_50y_SpellScript);

        bool Load()
        {
            pct = 0;
            players.clear();
            return true;
        }

        void CalculatePercentage()
        {
            std::list<Player*> targets = GetCaster()->GetNearestPlayersList(50, true);
            for (std::list<Player*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                pct = ((pct * players.size()) + ((*itr)->GetHealthPct() / 100.f)) / float(players.size() + 1);
                players.insert(*itr);
            }
        }

        void HandleAfterCast()
        {
            for (std::set<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                int32 damage = 0;
                int32 heal = 0;

                uint32 const bestValue = uint32((*itr)->GetMaxHealth() * pct);

                if (bestValue > (*itr)->GetHealth())
                    heal = bestValue - (*itr)->GetHealth();
                else
                    damage = (*itr)->GetHealth() - bestValue;

                if (damage + heal > 1)
                    (*itr)->CastCustomSpell(*itr, 22806, &heal, &damage, NULL, true);
            }
        }

        void Register()
        {
            BeforeCast += SpellCastFn(spell_custom_spirit_link_50y_SpellScript::CalculatePercentage);
            AfterCast += SpellCastFn(spell_custom_spirit_link_50y_SpellScript::HandleAfterCast);
        }

    private:
        float pct;
        std::set<Player*> players;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_custom_spirit_link_50y_SpellScript();
    }
};

// ID - 70522 Test Spell Has to be changed to hit all players in a range of 50yards
// ID - 58178 CSA Dummy Effect(25 yards)
class spell_custom_rewind : public SpellScriptLoader
{
public:
    spell_custom_rewind() : SpellScriptLoader("spell_custom_rewind") { }

    class spell_custom_rewind_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_custom_rewind_SpellScript);
        
        void HandleHit()
        {
            Unit* hitUnit = GetHitUnit();

            if (Player* player = hitUnit->ToPlayer())
            {
                if (player->isDead())
                    player->ResurrectPlayer(1.0f);

                player->ResetAllPowers();
                player->DurabilityRepairAll(false, 0, false);
                player->RemoveAllSpellCooldown();
                player->RemoveAura(57724); // Heldentum Erschöpfung
                player->RemoveAura(57723); // Kampfrausch Erschöpfung
            }
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_custom_rewind_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_custom_rewind_SpellScript();
    }
};

struct CrossfactionLoot : public PlayerScript {
    CrossfactionLoot() : PlayerScript("custom_crossfaction_loot_player") {}

    void OnPlayerLootItem(Player *player, uint32 &item_entry,
                          ItemPosCountVec & /*dest*/) {
        if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP)) {
            if (auto item = sObjectMgr->GetItemTemplate(item_entry)) {
                auto item_flags2 = item->Flags2;
                auto player_team = player->GetTeam(true);

                auto const &faction_change_items =
                    sObjectMgr->FactionChange_Items;

                if ((item_flags2 & ITEM_FLAGS_EXTRA_HORDE_ONLY) &&
                    player_team != HORDE) {
                    auto result = std::find_if(
                        std::begin(faction_change_items),
                        std::end(faction_change_items),
                        [item_entry](const std::pair<const uint32, uint32> &p) {
                            return p.second == item_entry;
                        });
                    if (result != std::end(faction_change_items)) {
                        item_entry = result->first;
                    }
                } else if ((item_flags2 & ITEM_FLAGS_EXTRA_ALLIANCE_ONLY) &&
                           player_team != ALLIANCE) {
                    auto result = faction_change_items.find(item_entry);
                    if (result != std::end(faction_change_items)) {
                        item_entry = result->second;
                    }
                }
            }
        }
    }
};

void AddSC_custom()
{
	AddSC_Custom_Boss_Thrall();
	AddSC_Custom_Boss_VarianWrynn();

    LoadEvents();
    LoadNpcs();
    new spell_finkles_mixture_event();
    new spell_custom_spirit_link_8y();
    new spell_custom_spirit_link_50y();
    new spell_custom_rewind();

    new CrossfactionLoot();
}
