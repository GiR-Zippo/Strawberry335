#ifndef __PLAYER_TRANSMOG_H
#define __PLAYER_TRANSMOG_H

#include <set>

#include "DatabaseEnv.h"
#include "TransmogMgr.h"

typedef std::unordered_map<uint32, uint32> ItemTransmogMap;

class PlayerTransmog
{
public:
    PlayerTransmog(Player *player);

    void LoadFromDB(PreparedQueryResult active, PreparedQueryResult unlocked, PreparedQueryResult history);
    void SaveToDB(SQLTransaction trans);

    uint32 GetEntry(uint8 slot, Item* pItem) const;

    void ClearTransmog(Item* pItem);
    bool ActivateTransmog(Item* pItem, uint32 entry);

    void UnlockTransmog(TransmogPartEntry const* part);

    std::set<uint32> GetUnlockedTransmogForSlot(uint8 slot) const;
    std::set<uint32> GetAvailableSlots() const;

    bool IsUnlocked(uint32 transmogID, uint8 slot) const;
    bool IsHistory(uint32 transmogID, uint8 slot) const;
    uint32 GetActiveTransmog(uint32 itemLowGUID) const;
    ItemTransmogMap GetActiveTransmogs() const { return _activeTransmog; }

private:
    Player *_player;

    std::set<uint32> _changedActiveTransmogs;
    std::set<uint32> _newUnlockedTransmogs;
    std::set<uint32> _newHistoryTransmogs;

    std::set<uint32> _unlockedTransmogs[EQUIPMENT_SLOT_END];
    std::set<uint32> _historyTransmogs[EQUIPMENT_SLOT_END];

    ItemTransmogMap _activeTransmog;
};

#endif /*__PLAYER_TRANSMOG_H */
