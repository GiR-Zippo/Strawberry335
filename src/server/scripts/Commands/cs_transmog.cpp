#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

#include "PlayerTransmog.h"

class transmog_commandscript : public CommandScript
{
public:
    transmog_commandscript() : CommandScript("transmog_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand transmogListCommandTable[] =
        {
            { "active",         SEC_GAMEMASTER,     false, &HandleTransmogListActiveCommand,   "", NULL },
            { "available",      SEC_GAMEMASTER,     false, &HandleTransmogListAvailableCommand, "", NULL },
            { "unlockable",     SEC_GAMEMASTER,     false, &HandleTransmogListUnlockableCommand, "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand transmogCommandTable[] =
        {
            { "list",           SEC_GAMEMASTER,     false, NULL,           "", transmogListCommandTable },
            { "set",            SEC_GAMEMASTER,     false, &HandleTransmogSetCommand,          "", NULL },
            { "clear",          SEC_GAMEMASTER,     false, &HandleTransmogClearCommand,        "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "transmog",       SEC_GAMEMASTER,     false, NULL,               "", transmogCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleTransmogListActiveCommand(ChatHandler* handler, const char* args)
    {
        PlayerTransmog *transmog = NULL;

        if (Player *player = handler->GetSession()->GetPlayer())
            transmog = player->GetTransmog();

        if (!transmog)
            return false;

        ItemTransmogMap activeTransmogs = transmog->GetActiveTransmogs();

        handler->PSendSysMessage("Active transmogs:");

        for (ItemTransmogMap::iterator itr = activeTransmogs.begin(); itr != activeTransmogs.end(); ++itr)
            handler->PSendSysMessage("|- Item %u has transmog %u", itr->first, itr->second);

        return true;
    }

    static bool HandleTransmogListAvailableCommand(ChatHandler* handler, const char* args)
    {
        PlayerTransmog *transmog = NULL;

        if (Player *player = handler->GetSession()->GetPlayer())
            transmog = player->GetTransmog();

        if (!transmog)
            return false;

        std::set<uint32> slots = transmog->GetAvailableSlots();

        for (std::set<uint32>::iterator itr = slots.begin(); itr != slots.end(); ++itr)
        {
            std::set<uint32> transmogs = transmog->GetUnlockedTransmogForSlot(*itr);
            handler->PSendSysMessage("Unlocked transmogs for slot %u:", *itr);

            for (std::set<uint32>::iterator itr2 = transmogs.begin(); itr2 != transmogs.end(); ++itr2)
                handler->PSendSysMessage("|- %u", *itr2);
        }
        return true;
    }

    static bool HandleTransmogListUnlockableCommand(ChatHandler* handler, const char* args)
    {
        Player *player = handler->GetSession()->GetPlayer();

        PlayerTransmog *transmog = player ? player->GetTransmog() : NULL;

        if (!transmog)
            return false;

        for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        {
            std::set<uint32> displayables = sTransmogMgr->GetDisplayable(player, i);

            handler->PSendSysMessage("Unlockable and unlocked transmogs for slot %u:", i);

            for (std::set<uint32>::iterator itr = displayables.begin(); itr != displayables.end(); ++itr)
                handler->PSendSysMessage("|- %u", *itr);
        }
        return true;
    }

    static bool HandleTransmogSetCommand(ChatHandler* handler, const char* args)
    {
        handler->PSendSysMessage("Not yet implemented");
        return true;
    }

    static bool HandleTransmogClearCommand(ChatHandler* handler, const char* args)
    {
        handler->PSendSysMessage("Not yet implemented");
        return true;
    }
};

void AddSC_transmog_commandscript()
{
    new transmog_commandscript();
}
