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

#include "PlayerHelper.h"
#include "Player.h"
#include "InstanceSaveMgr.h"

using namespace PSM;


PlayerStatsModifier::PlayerStatsModifier(Player* pSource) : sourceAvgItemLevel(0.0f), modifier(1.0f)
{
    ASSERT(pSource != NULL);
    pPlayer = pSource;

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::PlayerStatsModifier: Initialized");
}

bool PlayerStatsModifier::IsActive() const
{
    if (targetAvgItemLevel || uint32(modifier) != 1)
    {
        sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::IsActive: return true");
        return true;
    }

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::IsActive: return false");
    return false;
}

float PlayerStatsModifier::GetModifier() const
{
    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::GetModifier: Current factor is %f",
                   modifier);

    return modifier;
}

void PlayerStatsModifier::ApplyModifier(float& value) const
{
    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::ApplyModifier(float): Current value is %f",
                   value);

    if (IsActive())
        value = value * modifier;

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::ApplyModifier(float): Value after modifier is %f with mod %f",
                   value, modifier);
}

void PlayerStatsModifier::ApplyModifier(int32& value) const
{
    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::ApplyModifier(int32): Current value is %d",
                   value);

    if (IsActive())
        value = float(value) * modifier;

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::ApplyModifier(int32): Value after modifier is %d with mod %f",
                   value, modifier);
}

void PlayerStatsModifier::SetModifier(float factor)
{
    if (targetAvgItemLevel)
        targetAvgItemLevel = 0.0f;

    modifier = factor;
    pPlayer->UpdateAllStats();

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::SetModifier: Modifier set to %f", factor);
}

void PlayerStatsModifier::SetModifierDueToItemLevel(float itemLevel)
{
    if (itemLevel > CONFIG_PSM_MAX_ITEMLEVEL || itemLevel < CONFIG_PSM_MIN_ITEMLEVEL)
    {
        ResetModifier();
        return;
    }

    targetAvgItemLevel = itemLevel;
    UpdateModifier();

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::SetModifierDueToItemLevel: targetAvgItemLevel set to %f",
                   itemLevel);
}

void PlayerStatsModifier::UpdateModifier()
{
    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::UpdateModifier()");

    calculateAvgItemLevel();
    calculateModifier();

    pPlayer->UpdateAllStats();
}

void PlayerStatsModifier::ResetModifier()
{
    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::ResetModifier()");

    targetAvgItemLevel = 0.0f;
    sourceAvgItemLevel = 0.0f;

    if (modifier != 1.0f)
    {
        modifier = 1.0f;
        pPlayer->UpdateAllStats();
    }
}

void PlayerStatsModifier::calculateAvgItemLevel()
{
    float itemLevel = 0.0f;
    uint32 count = 0;

    for (int i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
    {
        // Don't check tabard, ranged, offhand or shirt
        if (i == EQUIPMENT_SLOT_TABARD || i == EQUIPMENT_SLOT_RANGED || i == EQUIPMENT_SLOT_OFFHAND || i == EQUIPMENT_SLOT_BODY)
            continue;

        if (Item* pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        {
            if (!pItem->GetTemplate()) // Should not happen
                continue;

            if (pItem->GetTemplate()->GetItemLevelIncludingQuality() < CONFIG_PSM_MIN_ITEMLEVEL)
                itemLevel += CONFIG_PSM_MIN_ITEMLEVEL;
            else
                itemLevel += pItem->GetTemplate()->GetItemLevelIncludingQuality();

            ++count;
        }
    }

    sourceAvgItemLevel = float(itemLevel / count);

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::calculateAvgItemLevel: sourceAvgItemLevel set to %f with itemlevel %f and itemcount of %d",
                   sourceAvgItemLevel, itemLevel, count);
}

void PlayerStatsModifier::calculateModifier()
{
    // No calculation if source or target itemlevel not defined
    if (!sourceAvgItemLevel || !targetAvgItemLevel)
        return;

    // Check if player is in range of calculation
    if (uint32(sourceAvgItemLevel) == uint32(targetAvgItemLevel)
        || uint32(sourceAvgItemLevel) > CONFIG_PSM_MAX_ITEMLEVEL
        || uint32(sourceAvgItemLevel) < CONFIG_PSM_MIN_ITEMLEVEL)
        return;

    // Calculate ((itemlevel - baselevel) * factor) + 1
    float base = ((targetAvgItemLevel - CONFIG_PSM_MIN_ITEMLEVEL) * baseModifierFactor) + 1.f;
    float vol = ((sourceAvgItemLevel - CONFIG_PSM_MIN_ITEMLEVEL) * baseModifierFactor) + 1.f;
    float factor = (base / vol);

    sLog->outDebug(LOG_FILTER_UNITS, "PlayerStatsModifier::calculateModifier: modifier set to %f with base %f and vol %f",
                   factor, base, vol);

    modifier = factor;
}


bool DelayedAuraRemove::Execute(uint64 /*time*/, uint32 /*diff*/)
{
    if (_owner && _spellId)
    {
        if (Unit *Owner = ObjectAccessor::FindUnit(_owner))
        {
            if (_source)
                Owner->RemoveAurasDueToItemSpell(_source, _spellId);
            else
                Owner->RemoveAurasDueToSpell(_spellId);
        }
    }
    return true;
}


Item* Player::GetNotPermittedItemInArena()
{
    // Check if player has any item which should be replaced in arena
    for (uint8 i = 0; i < INVENTORY_SLOT_BAG_START; ++i)
        for (uint8 k = 0; k < ITEM_NOT_PERMITTED_COUNT; ++k)
            if (m_items[i] && m_items[i]->GetEntry() == ArenaNotPermittedItems[k][0])
                return m_items[i];
    return NULL;
}

void Player::RemoveItemBonus(Item* pItem)
{
    if (!pItem || pItem->HasRemovedBonus())
        return;

    if (!pItem->IsEquipped())
    {
        pItem->SetRemovedBonus(true);
        return;
    }

    uint8 slot = pItem->GetSlot();

    RemoveEnchantmentDurations(pItem);
    RemoveItemDurations(pItem);
    RemoveTradeableItem(pItem);

    ItemTemplate const* pProto = pItem->GetTemplate();

    // item set bonuses applied only at equip and removed at unequip, and still active for broken items
    if (pProto && pProto->ItemSet)
        RemoveItemsSetItem(this, pProto);

    _ApplyItemMods(pItem, slot, false);

    // remove item dependent auras and casts (only weapon and armor slots)
    if (slot < EQUIPMENT_SLOT_END)
    {
        // remove item dependent auras and casts (only weapon and armor slots)
        RemoveItemDependentAurasAndCasts(pItem);

        // remove held enchantments, update expertise
        if (slot == EQUIPMENT_SLOT_MAINHAND)
        {
            if (pItem->GetItemSuffixFactor())
            {
                pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_3);
                pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_4);
            }
            else
            {
                pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_0);
                pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_1);
            }

            UpdateExpertise(BASE_ATTACK);
        }
        else if (slot == EQUIPMENT_SLOT_OFFHAND)
            UpdateExpertise(OFF_ATTACK);

        // update armor penetration - passive auras may need it
        switch (slot)
        {
            case EQUIPMENT_SLOT_MAINHAND:
            case EQUIPMENT_SLOT_OFFHAND:
            case EQUIPMENT_SLOT_RANGED:
                RecalculateRating(CR_ARMOR_PENETRATION);
            default:
                break;
        }
    }

    pItem->SetRemovedBonus(true);

    sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "RemoveItemBonus: %s with %s", InArena() ? "Arena" : "No Arena", pItem->GetTemplate()->Name1.c_str());
}

void Player::AddItemBonus(Item* pItem)
{
    if (!pItem || !pItem->HasRemovedBonus())
        return;

    if (!pItem->IsEquipped())
    {
        pItem->SetRemovedBonus(false);
        return;
    }

    uint8 slot = pItem->GetSlot();

    AddEnchantmentDurations(pItem);
    AddItemDurations(pItem);

    VisualizeItem(slot, pItem);

    ItemTemplate const* pProto = pItem->GetTemplate();

    // item set bonuses applied only at equip and removed at unequip, and still active for broken items
    if (pProto && pProto->ItemSet)
        AddItemsSetItem(this, pItem);

    _ApplyItemMods(pItem, slot, true);

    if (pProto && isInCombat() && (pProto->Class == ITEM_CLASS_WEAPON || pProto->InventoryType == INVTYPE_RELIC) && m_weaponChangeTimer == 0)
    {
        uint32 cooldownSpell = getClass() == CLASS_ROGUE ? 6123 : 6119;
        SpellInfo const* spellProto = sSpellMgr->GetSpellInfo(cooldownSpell);

        if (!spellProto)
            sLog->outError("Weapon switch cooldown spell %u couldn't be found in Spell.dbc", cooldownSpell);
        else
        {
            m_weaponChangeTimer = spellProto->StartRecoveryTime;

            GetGlobalCooldownMgr().AddGlobalCooldown(spellProto, m_weaponChangeTimer);

            WorldPacket data(SMSG_SPELL_COOLDOWN, 8 + 1 + 4);
            data << uint64(GetGUID());
            data << uint8(1);
            data << uint32(cooldownSpell);
            data << uint32(0);
            GetSession()->SendPacket(&data);
        }
    }

    if (IsInWorld())
    {
        pItem->AddToWorld();
        pItem->SendUpdateToPlayer(this);
    }

    ApplyEquipCooldown(pItem);

    // update expertise and armor penetration - passive auras may need it
    if (slot == EQUIPMENT_SLOT_MAINHAND)
        UpdateExpertise(BASE_ATTACK);

    else if (slot == EQUIPMENT_SLOT_OFFHAND)
        UpdateExpertise(OFF_ATTACK);

    switch (slot)
    {
        case EQUIPMENT_SLOT_MAINHAND:
        case EQUIPMENT_SLOT_OFFHAND:
        case EQUIPMENT_SLOT_RANGED:
            RecalculateRating(CR_ARMOR_PENETRATION);
        default:
            break;
    }

    pItem->SetRemovedBonus(false);

    sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "AddItemBonus: %s with %s", InArena() ? "Arena" : "No Arena", pItem->GetTemplate()->Name1.c_str());
}

bool Player::HandleWithCrossFactionBG(const Player* pPlayer) const
{
    if (isGameMaster() || isDeveloper()
        || (pPlayer && (pPlayer->isGameMaster() || pPlayer->isDeveloper())))
        return false;

    if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_BATTLEGROUNDS))
    {
        if (InBattleground())
        {
            if (pPlayer)
                return pPlayer->InBattleground();
            return true;
        }
    }
    return false;
}

bool DescSkillValuePredicate(const std::pair<uint32, uint32>& first, const std::pair<uint32, uint32>& second)
{
    return first.second < second.second;
}

void Player::CheckPrimaryProfessions()
{
    // Get professions
    std::vector< std::pair<uint32, uint32> > professions;

    for (uint8 i = 0; i < (sizeof(ProfessionSkills) / sizeof(uint32)); ++i)
        if (HasSkill(ProfessionSkills[i]))
            professions.push_back(std::make_pair(ProfessionSkills[i], GetSkillValue(ProfessionSkills[i])));

    // Check and remove profession with lowest skill value
    if (professions.size() > sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
    {
        std::sort(professions.begin(), professions.end(), DescSkillValuePredicate);

        while (professions.size() > sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
        {
            SetSkill(professions.begin()->first, 0, 0, 0);
            professions.erase(professions.begin());
        }
    }
}

void InstancePlayerBind::SetExtended(uint8 state, uint32 lowGUID, bool onLoad)
{
    if (!onLoad)
    {
        if (state == extended || !save)
            return;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_INSTANCE_EXTENDED);
        stmt->setUInt8(0, uint8(state));
        stmt->setUInt64(1, uint32(lowGUID));
        stmt->setUInt32(2, uint32(save->GetInstanceId()));
        CharacterDatabase.DirectExecute(stmt);
    }

    extended = state;
}
