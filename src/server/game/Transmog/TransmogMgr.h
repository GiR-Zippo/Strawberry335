#ifndef __TRANSMOG_MGR_H
#define __TRANSMOG_MGR_H

#include "Player.h"
#include "TransmogCondition.h"
#include "TransmogCost.h"

enum EquipmentSlots                                         // 19 slots
{
    EQUIPMENT_SLOT_START        = 0,
    EQUIPMENT_SLOT_HEAD         = 0,
    EQUIPMENT_SLOT_NECK         = 1,
    EQUIPMENT_SLOT_SHOULDERS    = 2,
    EQUIPMENT_SLOT_BODY         = 3,
    EQUIPMENT_SLOT_CHEST        = 4,
    EQUIPMENT_SLOT_WAIST        = 5,
    EQUIPMENT_SLOT_LEGS         = 6,
    EQUIPMENT_SLOT_FEET         = 7,
    EQUIPMENT_SLOT_WRISTS       = 8,
    EQUIPMENT_SLOT_HANDS        = 9,
    EQUIPMENT_SLOT_FINGER1      = 10,
    EQUIPMENT_SLOT_FINGER2      = 11,
    EQUIPMENT_SLOT_TRINKET1     = 12,
    EQUIPMENT_SLOT_TRINKET2     = 13,
    EQUIPMENT_SLOT_BACK         = 14,
    EQUIPMENT_SLOT_MAINHAND     = 15,
    EQUIPMENT_SLOT_OFFHAND      = 16,
    EQUIPMENT_SLOT_RANGED       = 17,
    EQUIPMENT_SLOT_TABARD       = 18,
    EQUIPMENT_SLOT_END          = 19
};

struct TransmogSetEntry
{
    uint32 ID;
    uint32 Condition;
    uint32 Season;
};

struct TransmogPartEntry
{
    uint32 ID;
    std::string Name;
    uint8 Slot;
    uint32 Set;
    uint32 ItemEntry;
    uint32 Cost;
    uint32 RebuyCost;
    uint32 Condition;
};

class TransmogMgr
{
public:
    void LoadFromDB();

    TransmogSetEntry const* GetTransmogSetEntry(uint32 ID) const;
    TransmogPartEntry const* GetTransmogPartEntry(uint32 ID) const;
    TransmogCondition const* GetTransmogCondition(uint32 ID) const;
    TransmogCost const* GetTransmogCost(uint32 ID) const;

    void AddUnlockableSlots(Player* player, std::set<uint32> &slots) const;
    std::set<uint32> GetDisplayable(Player* player, uint32 slot) const;

    bool Transmogrifiable(uint32 itemEntry, uint32 transmogItemEntry) const;
    uint8 GetSlot(uint32 itemEntry) const;

private:
    bool isDisplayable(Player *player, TransmogPartEntry* entry) const;

    std::map<uint32, TransmogSetEntry*> _transmogSetEntryStorage;
    std::map<uint32, TransmogPartEntry*> _transmogPartEntryStorage;

    std::set<TransmogPartEntry*> _transmogPartEntrySlotIndex[EQUIPMENT_SLOT_END];

    std::map<uint32, TransmogCondition*> _transmogConditionStorage;
    std::map<uint32, TransmogCost*> _transmogCostStorage;
};

#define sTransmogMgr ACE_Singleton<TransmogMgr, ACE_Null_Mutex>::instance()
#endif /*__TRANSMOG_HGR_H */
