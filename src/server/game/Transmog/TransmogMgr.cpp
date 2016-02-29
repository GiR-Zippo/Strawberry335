#include "TransmogMgr.h"

void TransmogMgr::LoadFromDB()
{
    if (QueryResult result = WorldDatabase.PQuery("SELECT `cid`, `id`, `type`, `value` FROM transmog_conditions"))
    {
        uint32 counter = 0;

        do
        {
            Field* field = result->Fetch();

            uint32 cid = field[0].GetUInt32();
            uint32 id = field[1].GetUInt32();
            uint32 type = field[2].GetUInt32();
            uint32 value = field[3].GetUInt32();

            if (_transmogConditionStorage.find(cid) == _transmogConditionStorage.end())
                _transmogConditionStorage[cid] = new TransmogCondition();

            _transmogConditionStorage[cid]->AddSubCondition(id, TransmogSubCondition(type, value));

            counter++;
        }
        while (result->NextRow());

        sLog->outString("Loaded %u transmog conditions", counter);
    }

    if (QueryResult result = WorldDatabase.PQuery("SELECT `cid`, `id`, `type`, `value` FROM transmog_costs"))
    {
        uint32 counter = 0;

        do
        {
            Field* field = result->Fetch();

            uint32 cid = field[0].GetUInt32();
            uint32 id = field[1].GetUInt32();
            uint32 type = field[2].GetUInt32();
            uint32 value = field[3].GetUInt32();

            if (_transmogCostStorage.find(cid) == _transmogCostStorage.end())
                _transmogCostStorage[cid] = new TransmogCost();

            _transmogCostStorage[cid]->AddSubCost(id, TransmogSubCost(type, value));

            counter++;
        }
        while (result->NextRow());

        sLog->outString("Loaded %u transmog costs", counter);
    }

    if (QueryResult result = WorldDatabase.PQuery("SELECT `id`, `condition`, `season` FROM transmog_sets"))
    {
        uint32 counter = 0;

        do
        {
            Field* field = result->Fetch();

            TransmogSetEntry* entry = new TransmogSetEntry();
            entry->ID = field[0].GetUInt32();
            entry->Condition = field[1].GetUInt32();
            entry->Season = field[2].GetUInt32();

            if (entry->Condition && _transmogConditionStorage.find(entry->Condition) == _transmogConditionStorage.end())
                sLog->outError("Condition %u used for set %u does not exist", entry->Condition, entry->ID);

            _transmogSetEntryStorage[entry->ID] = entry;
            counter++;
        }
        while (result->NextRow());

        sLog->outString("Loaded %u transmog sets", counter);
    }

    if (QueryResult result = WorldDatabase.PQuery("SELECT `id`, `slot`, `set`, `item_entry`, `cost`, `cost_rebuy`, `condition`, `name` FROM transmog_parts"))
    {
        uint32 counter = 0;

        do
        {
            Field* field = result->Fetch();

            TransmogPartEntry* entry = new TransmogPartEntry();
            entry->ID = field[0].GetUInt32();
            entry->Slot = field[1].GetUInt8();
            entry->Set = field[2].GetUInt32();
            entry->ItemEntry = field[3].GetUInt32();
            entry->Cost = field[4].GetUInt32();
            entry->RebuyCost = field[5].GetUInt32();
            entry->Condition = field[6].GetUInt32();
            entry->Name = field[7].GetString();

            if (entry->Condition && _transmogConditionStorage.find(entry->Condition) == _transmogConditionStorage.end())
                sLog->outError("Condition %u used for part %u does not exist", entry->Condition, entry->ID);

            if (entry->Condition && _transmogCostStorage.find(entry->Cost) == _transmogCostStorage.end())
                sLog->outError("Cost %u used for part %u does not exist", entry->Condition, entry->ID);

            if (entry->Condition && _transmogCostStorage.find(entry->RebuyCost) == _transmogCostStorage.end())
                sLog->outError("RebuyCost %u used for part %u does not exist", entry->Condition, entry->ID);

            _transmogPartEntryStorage[entry->ID] = entry;
            _transmogPartEntrySlotIndex[entry->Slot].insert(entry);
            counter++;
        }
        while (result->NextRow());

        sLog->outString("Loaded %u transmog parts", counter);
    }
}

TransmogSetEntry const* TransmogMgr::GetTransmogSetEntry(uint32 ID) const
{
    std::map<uint32, TransmogSetEntry*>::const_iterator itr = _transmogSetEntryStorage.find(ID);

    if (itr != _transmogSetEntryStorage.end())
        return itr->second;

    return NULL;
}

TransmogPartEntry const* TransmogMgr::GetTransmogPartEntry(uint32 ID) const
{
    std::map<uint32, TransmogPartEntry*>::const_iterator itr = _transmogPartEntryStorage.find(ID);

    if (itr != _transmogPartEntryStorage.end())
        return itr->second;

    return NULL;
}

TransmogCondition const* TransmogMgr::GetTransmogCondition(uint32 ID) const
{
    std::map<uint32, TransmogCondition*>::const_iterator itr = _transmogConditionStorage.find(ID);

    if (itr != _transmogConditionStorage.end())
        return itr->second;

    return NULL;
}

TransmogCost const* TransmogMgr::GetTransmogCost(uint32 ID) const
{
    std::map<uint32, TransmogCost*>::const_iterator itr = _transmogCostStorage.find(ID);

    if (itr != _transmogCostStorage.end())
        return itr->second;

    return NULL;
}

void TransmogMgr::AddUnlockableSlots(Player* player, std::set<uint32> &slots) const
{
    for (uint32 slot = 0; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (slots.end() == slots.find(slot) && _transmogPartEntrySlotIndex[slot].size())
        {
            bool insert = false;

            for (std::set<TransmogPartEntry*>::const_iterator itr = _transmogPartEntrySlotIndex[slot].begin(); !insert && itr != _transmogPartEntrySlotIndex[slot].end(); ++itr)
                if (isDisplayable(player, *itr))
                    insert = true;

            if (insert)
                slots.insert(slot);
        }
    }
}

std::set<uint32> TransmogMgr::GetDisplayable(Player* player, uint32 slot) const
{
    std::set<uint32> displayables;

    if (slot < EQUIPMENT_SLOT_END)
    {
        for (std::set<TransmogPartEntry*>::const_iterator itr = _transmogPartEntrySlotIndex[slot].begin(); itr != _transmogPartEntrySlotIndex[slot].end(); ++itr)
        {
            if (isDisplayable(player, *itr))
                displayables.insert((*itr)->ID);
        }
    }

    return displayables;
}

bool TransmogMgr::Transmogrifiable(uint32 itemEntry, uint32 transmogItemEntry) const
{
    uint8 itemSlot = GetSlot(itemEntry);
    uint8 transmogSlot = GetSlot(transmogItemEntry);

    return itemSlot == transmogSlot && itemSlot != NULL_SLOT;
}

uint8 TransmogMgr::GetSlot(uint32 itemEntry) const
{
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemEntry);

    if (!itemProto)
        return NULL_SLOT;

    switch (itemProto->InventoryType)
    {
        case INVTYPE_HEAD:
            return EQUIPMENT_SLOT_HEAD;
        case INVTYPE_SHOULDERS:
            return EQUIPMENT_SLOT_SHOULDERS;
        case INVTYPE_BODY:
            return EQUIPMENT_SLOT_BODY;
        case INVTYPE_CHEST:
            return EQUIPMENT_SLOT_CHEST;
        case INVTYPE_ROBE:
            return EQUIPMENT_SLOT_CHEST;
        case INVTYPE_WAIST:
            return EQUIPMENT_SLOT_WAIST;
        case INVTYPE_LEGS:
            return EQUIPMENT_SLOT_LEGS;
        case INVTYPE_FEET:
            return EQUIPMENT_SLOT_FEET;
        case INVTYPE_WRISTS:
            return EQUIPMENT_SLOT_WRISTS;
        case INVTYPE_HANDS:
            return EQUIPMENT_SLOT_HANDS;
        case INVTYPE_CLOAK:
            return EQUIPMENT_SLOT_BACK;
        case INVTYPE_TABARD:
            return EQUIPMENT_SLOT_TABARD;
        default:
            return NULL_SLOT;
    }
}

bool TransmogMgr::isDisplayable(Player *player, TransmogPartEntry* entry) const
{
    if (!entry || !player)
       return false;

    PlayerTransmog const *transmog = player->GetTransmog();

    if (transmog->IsUnlocked(entry->ID, entry->Slot) || transmog->IsHistory(entry->ID, entry->Slot))
        return true;

    if (TransmogSetEntry const* set = sTransmogMgr->GetTransmogSetEntry(entry->Set))
    {
        if (!set->Season || set->Season == sWorld->getIntConfig(CONFIG_ARENA_SEASON_TRANSMOG_ID))
        {
            TransmogCondition const* entryCondition = sTransmogMgr->GetTransmogCondition(entry->Condition);
            TransmogCondition const* setCondition = sTransmogMgr->GetTransmogCondition(set->Condition);

            return TransmogCondition::IsMatchingCondition(player, entryCondition) && TransmogCondition::IsMatchingCondition(player, setCondition);
        }
    }

    return false;
}
