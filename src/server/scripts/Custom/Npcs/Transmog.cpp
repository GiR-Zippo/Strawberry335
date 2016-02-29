#include "Transmog.h"
#include "ScriptedGossip.h"
#include "PlayerTransmog.h"

UNORDERED_MAP<uint32, uint32> PlayerTransmogMap;

enum ActionTypes
{
    ACTION_SLOT = GOSSIP_ACTION_INFO_DEF,
    ACTION_SLOT_ACTIVATE,
    ACTION_SLOT_SHOW_UNLOCK,
    ACTION_SLOT_UNLOCK,
    ACTION_SLOT_SHOW_REBUY,
    ACTION_SLOT_REBUY,
    ACTION_SLOT_CLEAR,
    ACTION_BACK_TO_SLOT,
    ACTION_BACK_TO_SLOT_OVERVIEW,
    ACTION_MAX
};

std::string GetSlotName(uint8 slot)
{
    switch (slot)
    {
        case EQUIPMENT_SLOT_HEAD:
            return "Kopf";
        case EQUIPMENT_SLOT_SHOULDERS:
            return "Schulter";
        case EQUIPMENT_SLOT_BODY:
        {
            const char value[] = {'K', 0xc3, 0xb6, 'r', 'p', 'e', 'r', '\0'};

            return std::string(value);
        }
        case EQUIPMENT_SLOT_CHEST:
            return "Brust";
        case EQUIPMENT_SLOT_WAIST:
            return "Taille";
        case EQUIPMENT_SLOT_LEGS:
            return "Beine";
        case EQUIPMENT_SLOT_FEET:
        {
            char value[] = {'F', 0xc3, 0xbc, 0xc3, 0x9f, 'e', '\0'};

            return std::string(value);
        }
        case EQUIPMENT_SLOT_WRISTS:
            return "Handgelenke";
        case EQUIPMENT_SLOT_HANDS:
        {
            char value[7] = {'H', 0xc3, 0xa4, 'n', 'd', 'e', '\0'};

            return std::string(value);
        }
        case EQUIPMENT_SLOT_BACK:
        {
            char value[8] = {'R', 0xc3, 0xbc, 'c', 'k', 'e', 'n', '\0'};

            return std::string(value);
        }
        default:
            return "";
    }
}

const char ResetSlotName[] = {'S', 'l', 'o', 't', ' ', 'Z', 'u', 'r', (char) 0xc3, (char) 0xbc, 'c', 'k', 's', 'e', 't', 'z', 'e', 'n', '\0'};
const char BackToMenuName[] = {'Z', 'u', 'r', (char) 0xc3, (char) 0xbc, 'c', 'k', ' ', 'z', 'u', 'r', ' ', (char) 0xc3, (char) 0x9c, 'b', 'e', 'r', 's', 'i', 'c', 'h', 't', '\0'};

void GenerateSlotOverviewMenu(Player* player)
{
    std::set<uint32> transmogSlots = player->GetTransmog()->GetAvailableSlots();
    sTransmogMgr->AddUnlockableSlots(player, transmogSlots);

    for (std::set<uint32>::iterator itr = transmogSlots.begin(); itr != transmogSlots.end(); ++itr)
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GetSlotName(*itr), (*itr), ACTION_SLOT);
}

void GenerateSlotMenu(Player* player, uint32 slot)
{
    std::set<uint32> unlockable = sTransmogMgr->GetDisplayable(player, slot);

    PlayerTransmog *transmog = player->GetTransmog();

    uint32 activeTransmog = 0;
    if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        activeTransmog = transmog->GetActiveTransmog(pItem->GetGUIDLow());

    for (std::set<uint32>::iterator itr = unlockable.begin(); itr != unlockable.end(); ++itr)
    {
        TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(*itr);

        std::stringstream ss;
        ss << entry->Name;

        if (activeTransmog == *itr)
        {
            ss << " [aktiv]";
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, ss.str(), slot, ACTION_SLOT);
        }
        else if (transmog->IsUnlocked(*itr, slot))
        {
            ss << " [gekauft]";
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, ss.str(), (uint32) (*itr), ACTION_SLOT_ACTIVATE);
        }
        else if (transmog->IsHistory(*itr, slot))
        {
            ss << " [gesperrt]";
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, ss.str(), (uint32) (*itr), ACTION_SLOT_SHOW_REBUY);
        }
        else
        {
            ss << " [erwerbbar]";
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, ss.str(), (uint32) (*itr), ACTION_SLOT_SHOW_UNLOCK);
        }
    }

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, std::string(ResetSlotName).c_str(), slot, ACTION_SLOT_CLEAR);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, std::string(BackToMenuName).c_str(), 0, ACTION_BACK_TO_SLOT_OVERVIEW);
}

void ActivateTransmog(Player* player, uint32 transmogID)
{
    TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(transmogID);
    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, sTransmogMgr->GetSlot(entry->ItemEntry));

    player->GetTransmog()->ActivateTransmog(pItem, transmogID);

    GenerateSlotMenu(player, sTransmogMgr->GetSlot(entry->ItemEntry));
}

void GenerateUnlockMenu(Player* player, uint32 transmogID)
{
    TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(transmogID);

    PlayerTransmogMap[player->GetGUIDLow()] = transmogID;

    TransmogCost const* cost = sTransmogMgr->GetTransmogCost(entry->Cost);

    std::set<uint32> possibleCosts = TransmogCost::GetCostOptions(cost);

    for (std::set<uint32>::iterator itr = possibleCosts.begin(); itr != possibleCosts.end(); ++itr)
    {
        std::stringstream ss;
        ss << "Kaufen [ " << cost->SubCostToText(*itr) << " ]";

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, ss.str(), *itr, ACTION_SLOT_UNLOCK);
    }

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, std::string(BackToMenuName).c_str(), 0, ACTION_BACK_TO_SLOT_OVERVIEW);
}

void UnlockTransmog(Player* player, uint32 costSubID)
{
    UNORDERED_MAP<uint32, uint32>::iterator itr = PlayerTransmogMap.find(player->GetGUIDLow());

    uint32 transmogID = 0;

    if (itr != PlayerTransmogMap.end())
        transmogID = itr->second;

    if (TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(transmogID))
    {
        TransmogSetEntry const* set = sTransmogMgr->GetTransmogSetEntry(entry->Set);

        TransmogCondition const* entryCondition = sTransmogMgr->GetTransmogCondition(entry->Condition);
        TransmogCondition const* setCondition = set ? sTransmogMgr->GetTransmogCondition(set->Condition) : NULL;

        if (TransmogCondition::IsMatchingCondition(player, entryCondition) && TransmogCondition::IsMatchingCondition(player, setCondition))
        {
            TransmogCost const* cost = sTransmogMgr->GetTransmogCost(entry->Cost);

            if (TransmogCost::Pay(player, cost, costSubID))
                player->GetTransmog()->UnlockTransmog(entry);
        }

        GenerateSlotMenu(player, sTransmogMgr->GetSlot(entry->ItemEntry));
    }
    else
        GenerateSlotOverviewMenu(player);

    PlayerTransmogMap.erase(itr);
}

void GenerateRebuyMenu(Player* player, uint32 transmogID)
{
    TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(transmogID);

    PlayerTransmogMap[player->GetGUIDLow()] = transmogID;

    TransmogCost const* cost = sTransmogMgr->GetTransmogCost(entry->RebuyCost);
    std::set<uint32> possibleCosts = TransmogCost::GetCostOptions(cost);

    for (std::set<uint32>::iterator itr = possibleCosts.begin(); itr != possibleCosts.end(); ++itr)
    {
        std::stringstream ss;
        ss << "Kaufen [ " << cost->SubCostToText(*itr) << " ]";

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, ss.str(), *itr, ACTION_SLOT_REBUY);
    }

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, std::string(BackToMenuName).c_str(), 0, ACTION_BACK_TO_SLOT_OVERVIEW);
}

void RebuyTransmog(Player* player, uint32 costSubID)
{
    UNORDERED_MAP<uint32, uint32>::iterator itr = PlayerTransmogMap.find(player->GetGUIDLow());

    uint32 transmogID = 0;

    if (itr != PlayerTransmogMap.end())
        transmogID = itr->second;

    if (TransmogPartEntry const* entry = sTransmogMgr->GetTransmogPartEntry(transmogID))
    {
        TransmogCost const* cost = sTransmogMgr->GetTransmogCost(entry->RebuyCost);

        if (TransmogCost::Pay(player, cost, costSubID))
            player->GetTransmog()->UnlockTransmog(entry);

        GenerateSlotMenu(player, sTransmogMgr->GetSlot(entry->ItemEntry));
    }
    else
        GenerateSlotOverviewMenu(player);

    PlayerTransmogMap.erase(itr);
}

void ClearTransmog(Player* player, uint32 slot)
{
    PlayerTransmog *transmog = player->GetTransmog();

    if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        transmog->ClearTransmog(pItem);

    GenerateSlotMenu(player, slot);
}

bool NpcTransmog::OnGossipHello(Player* player, Creature* creature)
{
    GenerateSlotOverviewMenu(player);
    player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
    return true;
}

bool NpcTransmog::OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    player->PlayerTalkClass->ClearMenus();

    switch (action)
    {
        case ACTION_SLOT:
        {
            GenerateSlotMenu(player, sender);
            break;
        }
        case ACTION_SLOT_ACTIVATE:
        {
            ActivateTransmog(player, sender);
            break;
        }
        case ACTION_SLOT_SHOW_UNLOCK:
        {
            GenerateUnlockMenu(player, sender);
            break;
        }
        case ACTION_SLOT_UNLOCK:
        {
            UnlockTransmog(player, sender);
            break;
        }
        case ACTION_SLOT_SHOW_REBUY:
        {
            GenerateRebuyMenu(player, sender);
            break;
        }
        case ACTION_SLOT_REBUY:
        {
            RebuyTransmog(player, sender);
            break;
        }
        case ACTION_SLOT_CLEAR:
        {
            ClearTransmog(player, sender);
            break;
        }
        case ACTION_BACK_TO_SLOT:
        {
            GenerateSlotMenu(player, sender);
            break;
        }
        case ACTION_BACK_TO_SLOT_OVERVIEW:
        {
            GenerateSlotOverviewMenu(player);
            break;
        }
        default:
            return false;
    }
    player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
    return true;
}
