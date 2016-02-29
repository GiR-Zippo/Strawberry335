#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

class node_commandscript : public CommandScript
{
public:
    node_commandscript() : CommandScript("node_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand nodeCommandTable[] =
        {
            { "list",           SEC_ADMINISTRATOR,  false, NULL,                        "",              NULL },
            { "change",         SEC_ADMINISTRATOR,  false, &HandleNodeChangeCommand,    "",              NULL },
            { "",               SEC_ADMINISTRATOR,  false, &HandleNodeCommand,          "",              NULL },
            { NULL,             SEC_PLAYER,         false, NULL,                        "",              NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "node",           SEC_ADMINISTRATOR,  false, NULL,                        "",              nodeCommandTable },
            { NULL,             SEC_PLAYER,         false, NULL,                        "",              NULL }
        };

        return commandTable;
    }

    static bool HandleNodeCommand(ChatHandler* handler, const char* args)
    {
        if (*args)
            return false;

        std::stringstream str;
        str << "Current Node: " << realmID;
        handler->SendSysMessage(str.str().c_str());
        return true;
    }

    static bool HandleNodeChangeCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Unit* unit = handler->getSelectedUnit();
        Player* player = NULL;
        if (!unit || (unit->GetTypeId() != TYPEID_PLAYER))
            player = handler->GetSession()->GetPlayer();
        else
            player = (Player*)unit;

        if (!unit)
            unit = player;

        uint32 nodeId = atoi(args);
        player->GetSession()->SendChangeNode(nodeId);

        return true;
    }

    static bool HandleNodeListCommand(ChatHandler* handler, const char* args)
    {
        handler->SendSysMessage("Not implemented");
        return true;
    }
};

void AddSC_node_commandscript()
{
    new node_commandscript();
}