#include "PlayerTransmog.h"

PlayerTransmog::PlayerTransmog(Player *player)
{
    _player = player;
}

void PlayerTransmog::LoadFromDB(PreparedQueryResult unlocked, PreparedQueryResult active, PreparedQueryResult history)
{
    if (unlocked)
    {
        do
        {
            Field *field = unlocked->Fetch();

            uint32 transmogID = field[0].GetUInt32();

            if (TransmogPartEntry const* part = sTransmogMgr->GetTransmogPartEntry(transmogID))
                _unlockedTransmogs[part->Slot].insert(part->ID);
        }
        while (unlocked->NextRow());
    }

    if (active)
    {
        do
        {
            Field *field = active->Fetch();

            uint32 itemGUID = field[0].GetUInt32();
            uint32 transmogID = field[1].GetUInt32();

            _activeTransmog[itemGUID] = transmogID;
        }
        while (active->NextRow());
    }

    if (history)
    {
        do
        {
            Field *field = history->Fetch();

            uint32 transmogID = field[0].GetUInt32();

            if (TransmogPartEntry const* part = sTransmogMgr->GetTransmogPartEntry(transmogID))
            {
                if (TransmogSetEntry const* set = sTransmogMgr->GetTransmogSetEntry(part->Set))
                {
                    if (!set->Season && _unlockedTransmogs[part->Slot].find(part->ID) == _unlockedTransmogs[part->Slot].end())
                        UnlockTransmog(part);
                }

                _historyTransmogs[part->Slot].insert(part->ID);
            }
        }
        while (history->NextRow());
    }
}

void PlayerTransmog::SaveToDB(SQLTransaction trans)
{
    for (std::set<uint32>::iterator itr = _changedActiveTransmogs.begin(); itr != _changedActiveTransmogs.end(); ++itr)
    {
        ItemTransmogMap::iterator itr2 = _activeTransmog.find(*itr);

        if (itr2 == _activeTransmog.end())
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_TRANSMOG_ACTIVE);
            stmt->setUInt32(0, _player->GetGUIDLow());
            stmt->setUInt32(1, *itr);
            trans->Append(stmt);
        }
        else
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CHAR_TRANSMOG_ACTIVE);
            stmt->setUInt32(0, _player->GetGUIDLow());
            stmt->setUInt32(1, *itr);
            stmt->setUInt32(2, _activeTransmog[*itr]);
            trans->Append(stmt);
        }
    }

    for (std::set<uint32>::iterator itr = _newUnlockedTransmogs.begin(); itr != _newUnlockedTransmogs.end(); ++itr)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CHAR_TRANSMOG_UNLOCKED);
        stmt->setUInt32(0, _player->GetGUIDLow());
        stmt->setUInt32(1, *itr);
        trans->Append(stmt);
    }

    for (std::set<uint32>::iterator itr = _newHistoryTransmogs.begin(); itr != _newHistoryTransmogs.end(); ++itr)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CHAR_TRANSMOG_HISTORY);
        stmt->setUInt32(0, _player->GetGUIDLow());
        stmt->setUInt32(1, *itr);
        trans->Append(stmt);
    }
}

uint32 PlayerTransmog::GetEntry(uint8 slot, Item* pItem) const
{
    ASSERT(slot < EQUIPMENT_SLOT_END);

    if (pItem)
    {
        ItemTransmogMap::const_iterator itr = _activeTransmog.find(pItem->GetGUIDLow());

        if (itr != _activeTransmog.end())
        {
            if (TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(itr->second))
            {
                if (sTransmogMgr->Transmogrifiable(pItem->GetEntry(), entry->ItemEntry))
                    return entry->ItemEntry;
            }
        }

        return pItem->GetEntry();
    }

    return 0;
}

void PlayerTransmog::ClearTransmog(Item* pItem)
{
    ItemTransmogMap::iterator itr = _activeTransmog.find(pItem->GetGUIDLow());

    if (itr != _activeTransmog.end())
        _activeTransmog.erase(itr);

    _changedActiveTransmogs.insert(pItem->GetGUIDLow());

    uint8 slot = sTransmogMgr->GetSlot(pItem->GetEntry());
    _player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * 2), GetEntry(slot, pItem));
}

bool PlayerTransmog::ActivateTransmog(Item* pItem, uint32 transmogID)
{
    if (!pItem)
        return false;

    TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(transmogID);

    std::set<uint32>::iterator itr = _unlockedTransmogs[entry->Slot].find(transmogID);

    if (itr != _unlockedTransmogs[entry->Slot].end())
    {
        _activeTransmog[pItem->GetGUIDLow()] = transmogID;
        _changedActiveTransmogs.insert(pItem->GetGUIDLow());
        _player->SetUInt32Value(PLAYER_VISIBLE_ITEM_1_ENTRYID + (entry->Slot * 2), GetEntry(entry->Slot, pItem));
        return true;
    }

    return false;
}

void PlayerTransmog::UnlockTransmog(TransmogPartEntry const* part)
{
    _unlockedTransmogs[part->Slot].insert(part->ID);
    _newUnlockedTransmogs.insert(part->ID);

    if (_historyTransmogs[part->Slot].find(part->ID) == _historyTransmogs[part->Slot].end())
    {
        _historyTransmogs[part->Slot].insert(part->ID);
        _newHistoryTransmogs.insert(part->ID);
    }
}

std::set<uint32> PlayerTransmog::GetUnlockedTransmogForSlot(uint8 slot) const
{
    ASSERT(slot < EQUIPMENT_SLOT_END);
    return _unlockedTransmogs[slot];
}

std::set<uint32> PlayerTransmog::GetAvailableSlots() const
{
    std::set<uint32> slots;

    for (uint32 slot = 0; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (_unlockedTransmogs[slot].size())
            slots.insert(slot);
        else if (_historyTransmogs[slot].size())
            slots.insert(slot);
    }

    return slots;
}

bool PlayerTransmog::IsUnlocked(uint32 transmogID, uint8 slot) const
{
    return _unlockedTransmogs[slot].find(transmogID) != _unlockedTransmogs[slot].end();
}

bool PlayerTransmog::IsHistory(uint32 transmogID, uint8 slot) const
{
    return _historyTransmogs[slot].find(transmogID) != _historyTransmogs[slot].end();
}

uint32 PlayerTransmog::GetActiveTransmog(uint32 itemLowGUID) const
{
    ItemTransmogMap::const_iterator itr = _activeTransmog.find(itemLowGUID);

    if (itr != _activeTransmog.end())
        return itr->second;

    return 0;
}
