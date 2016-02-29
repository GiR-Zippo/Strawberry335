/*
 * Copyright (C) 2014 Zirkon <mor1bondev@gmail.com>
 *
 * This part is not for distribution.
 */

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

class restore_commandscript : public CommandScript
{
public:
    restore_commandscript() : CommandScript("restore_commandscript") {}

    ChatCommand* GetCommands() const
    {
        static ChatCommand restoreCommandTable[] =
        {
            { "entry",          SEC_GAMEMASTER,     false, &HandleLostItemsRestoreEntry,      "", NULL },
            { "guid",           SEC_GAMEMASTER,     false, &HandleLostItemsRestoreGUID,       "", NULL },
            { "all",            SEC_GAMEMASTER,     false, &HandleLostItemsRestoreAll,        "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };

        static ChatCommand lostitemsCommandTable[] =
        {
            { "list",           SEC_GAMEMASTER,     false, &HandleLostItemsList,              "", NULL },
            { "restore",        SEC_GAMEMASTER,     false, NULL,                              "", restoreCommandTable },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "lostitems",      SEC_GAMEMASTER,     false, NULL,                              "", lostitemsCommandTable },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };

        return commandTable;
    }

    static bool HandleLostItemsList(ChatHandler* handler, char const* args)
    {
        int32 itemEntry = 0;

        if (*args)
        {
            itemEntry = atoi((char*) args);
            if (itemEntry <= 0)
                return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::list<LostItemInfo> lostItems = getLostItems(target->GetGUID(), itemEntry, 0);

        handler->PSendSysMessage("Found %u lost items:", lostItems.size());

        for (std::list<LostItemInfo>::iterator itr = lostItems.begin(); itr != lostItems.end(); ++itr)
        {
            if (const ItemTemplate* pItemTemplate = sObjectMgr->GetItemTemplate((*itr).entry))
            {
                std::string name = pItemTemplate->Name1;

                int localeIndex = handler->GetSessionDbLocaleIndex();
                if (localeIndex >= 0)
                {
                    uint8 ulocaleIndex = uint8(localeIndex);
                    if (const ItemLocale* pItemLocale = sObjectMgr->GetItemLocale((*itr).entry))
                        if (pItemLocale->Name.size() > ulocaleIndex && !pItemLocale->Name[ulocaleIndex].empty())
                            name = pItemLocale->Name[ulocaleIndex];
                }

                handler->PSendSysMessage(" - %u x |cffffffff|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r ", (*itr).count, (*itr).entry, name.c_str());
            }
            else
                handler->PSendSysMessage(" - %u x %d - Item couldn't be found in ItemTemplateStore!", (*itr).count, (*itr).entry);
        }

        return true;
    }

    static bool HandleLostItemsRestoreEntry(ChatHandler* handler, char const* args)
    {
        int32 itemEntry = 0;

        if (!*args)
            return false;

        itemEntry = atoi((char*) args);
        if (itemEntry <= 0)
            return false;

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::list<LostItemInfo> lostItems = getLostItems(target->GetGUID(), itemEntry, 0);

        handler->PSendSysMessage("Found %u lost items, restoring now", lostItems.size());

        uint32 count = restoreLostItems(handler, target->GetGUID(), lostItems);
        handler->PSendSysMessage("Restored %u items, check your mailbox", count);
        return true;
    }

    static bool HandleLostItemsRestoreGUID(ChatHandler* handler, char const* args)
    {
        int32 itemGUID = 0;

        if (!*args)
            return false;

        itemGUID = atoi((char*) args);
        if (itemGUID <= 0)
            return false;

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::list<LostItemInfo> lostItems = getLostItems(target->GetGUID(), 0, itemGUID);

        handler->PSendSysMessage("Found %u lost items, restoring now", lostItems.size());

        uint32 count = restoreLostItems(handler, target->GetGUID(), lostItems);
        handler->PSendSysMessage("Restored %u items, check your mailbox", count);
        return true;
    }

    static bool HandleLostItemsRestoreAll(ChatHandler* handler, char const* args)
    {
        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::list<LostItemInfo> lostItems = getLostItems(target->GetGUID(), 0, 0);

        handler->PSendSysMessage("Found %u lost items, restoring now", lostItems.size());

        uint32 count = restoreLostItems(handler, target->GetGUID(), lostItems);
        handler->PSendSysMessage("Restored %u items, check your mailbox", count);
        return true;
    }

private:
    struct LostItemInfo
    {
        LostItemInfo(uint32 guid_, uint32 entry_, uint32 count_) : guid(guid_), entry(entry_), count(count_) { }

        uint32 guid;
        uint32 entry;
        uint32 count;
    };

    static std::list<LostItemInfo> getLostItems(uint64 playerGUID, uint32 itemEntry, uint32 itemGUID, bool showStackable = false)
    {
        std::list<LostItemInfo> list;

        QueryResult result;

        if (itemEntry && itemGUID)
        {
            result = CharacterDatabase.PQuery("SELECT ii.guid, ii.count, ii.itemEntry FROM item_instance ii WHERE ii.owner_guid = %u AND ii.itemEntry = %u AND ii.guid = %u AND "
                "ii.guid NOT IN (SELECT ci.item FROM character_inventory ci WHERE ci.guid = %u) AND "
                "ii.guid NOT IN (SELECT mi.item_guid FROM mail_items mi WHERE mi.receiver = %u) AND "
                "ii.guid NOT IN (SELECT ah.itemguid FROM auctionhouse ah)", playerGUID, itemEntry, itemGUID, playerGUID, playerGUID);
        }
        else if (itemEntry && !itemGUID)
        {
            result = CharacterDatabase.PQuery("SELECT ii.guid, ii.count, ii.itemEntry FROM item_instance ii WHERE ii.owner_guid = %u AND ii.itemEntry = %u AND "
                "ii.guid NOT IN (SELECT ci.item FROM character_inventory ci WHERE ci.guid = %u) AND "
                "ii.guid NOT IN (SELECT mi.item_guid FROM mail_items mi WHERE mi.receiver = %u) AND "
                "ii.guid NOT IN (SELECT ah.itemguid FROM auctionhouse ah)", playerGUID, itemEntry, playerGUID, playerGUID);
        }
        else if (!itemEntry && itemGUID)
        {
            result = CharacterDatabase.PQuery("SELECT ii.guid, ii.count, ii.itemEntry FROM item_instance ii WHERE ii.owner_guid = %u AND ii.guid = %u AND "
                "ii.guid NOT IN (SELECT ci.item FROM character_inventory ci WHERE ci.guid = %u) AND "
                "ii.guid NOT IN (SELECT mi.item_guid FROM mail_items mi WHERE mi.receiver = %u) AND "
                "ii.guid NOT IN (SELECT ah.itemguid FROM auctionhouse ah)", playerGUID, itemGUID, playerGUID, playerGUID);
        }
        else /* (!itemEntry && !itemGUID) */
        {
            result = CharacterDatabase.PQuery("SELECT ii.guid, ii.count, ii.itemEntry FROM item_instance ii WHERE ii.owner_guid = %u AND "
                "ii.guid NOT IN (SELECT ci.item FROM character_inventory ci WHERE ci.guid = %u) AND "
                "ii.guid NOT IN (SELECT mi.item_guid FROM mail_items mi WHERE mi.receiver = %u) AND "
                "ii.guid NOT IN (SELECT ah.itemguid FROM auctionhouse ah)", playerGUID, playerGUID, playerGUID);
        }

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 guid = fields[0].GetUInt32();
                uint32 count = fields[1].GetUInt32();
                uint32 entry = fields[2].GetUInt32();

                if (const ItemTemplate* pItemTemplate = sObjectMgr->GetItemTemplate(entry))
                    if (pItemTemplate->GetMaxStackSize() <= 1 || showStackable)
                        list.push_back(LostItemInfo(guid, entry, count));
            }
            while (result->NextRow());
        }

        return list;
    }

    static uint32 restoreLostItems(ChatHandler* handler, uint64 playerGUID, std::list<LostItemInfo> &lostItems, bool restoreStackable = false)
    {
        uint32 count = 0;

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        while (!lostItems.empty())
        {
            std::string subject = sObjectMgr->GetTrinityString(LANG_NOT_EQUIPPED_ITEM, sWorld->GetDefaultDbcLocale());

            MailDraft draft(subject, "There were problems with equipping item(s).");
            for (uint8 i = 0; !lostItems.empty() && i < MAX_MAIL_ITEMS; ++i)
            {
                LostItemInfo info = (*lostItems.begin());

                if (QueryResult result = CharacterDatabase.PQuery("SELECT creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, durability, playedTime, text FROM item_instance WHERE guid = '%u'", info.guid))
                {
                    if (ItemTemplate const* proto = sObjectMgr->GetItemTemplate(info.entry))
                    {
                        if (proto->GetMaxStackSize() <= 1 || restoreStackable)
                        {
                            if (WorldSession *session = handler->GetSession())
                            {
                                sLog->outCommand(session->GetAccountId(), "RESTORED ITEM GUID %u ENTRY %u COUNT %u FOR PLAYER %u",
                                    info.guid, info.entry, info.count, playerGUID);
                            }

                            Item *item = NewItemOrBag(proto);
                            if (item->LoadFromDB(info.guid, playerGUID, result->Fetch(), info.entry))
                            {
                                draft.AddItem(item);
                                lostItems.pop_front();
                                count++;
                            }
                        }
                    }
                }
            }
            draft.SendMailTo(trans, MailReceiver(NULL, playerGUID), MailSender(MAIL_NORMAL, playerGUID, MAIL_STATIONERY_GM), MAIL_CHECK_MASK_COPIED);
        }

        CharacterDatabase.CommitTransaction(trans);

        return count;
    }
};

void AddSC_restore_commandscript()
{
    new restore_commandscript();
}
