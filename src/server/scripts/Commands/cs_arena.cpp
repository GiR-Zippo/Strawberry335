#include "ScriptMgr.h"
#include "Chat.h"
#include "GossipDef.h"
#include "BattlegroundMgr.h"

class arena_commandscript : public CommandScript
{
public:
    arena_commandscript() : CommandScript("arena_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand commandTable[] =
        {
            { "arena",          SEC_PLAYER,         false,  &HandleArenaCommand,          "", NULL },
            { NULL,             0,                  false, NULL,                          "", NULL }
        };
        return commandTable;
    }

    static bool HandleArenaCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* p = handler->GetSession()->GetPlayer();
        p->PlayerTalkClass->ClearMenus();
        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundListPacket(&data, 0, p, BATTLEGROUND_AA, 0);
        p->GetSession()->SendPacket(&data);
        return true;
    }
};

void AddSC_arena_commandscript()
{
    new arena_commandscript();
}