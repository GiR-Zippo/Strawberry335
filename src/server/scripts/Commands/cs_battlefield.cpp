#include "ScriptMgr.h"
#include "Chat.h"
#include "BattlefieldMgr.h"
#include "Battlefield.h"


class battlefield_commandscript : public CommandScript
{
public:
    battlefield_commandscript() : CommandScript("battlefield_commandscript") { }
    
    ChatCommand* GetCommands() const
    {
        static ChatCommand battlefieldcommandTable[] =
        {
            { "start",  SEC_ADMINISTRATOR, false, &HandleBattlefieldStart,  "", NULL },
            { "stop",   SEC_ADMINISTRATOR, false, &HandleBattlefieldEnd,    "", NULL },
            { "switch", SEC_ADMINISTRATOR, false, &HandleBattlefieldSwitch, "", NULL },
            { "timer",  SEC_ADMINISTRATOR, false, &HandleBattlefieldTimer,  "", NULL },
            { "enable", SEC_GAMEMASTER,    false, &HandleBattlefieldEnable, "", NULL },
            { NULL, 0, false, NULL, "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "bf", SEC_GAMEMASTER, false, NULL, "", battlefieldcommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        return commandTable;
    }

    static bool HandleBattlefieldStart(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->StartBattle();

        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp has been started.");

        return true;
    }

    static bool HandleBattlefieldEnd(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->EndBattle(true);

        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp has been ended.");

        return true;
    }

    static bool HandleBattlefieldEnable(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        if (bf->IsEnabled())
        {
            if (bf->IsWarTime())
                bf->EndBattle(true);

            bf->ToggleBattlefield(false);
            if (battleid == 1)
                handler->SendGlobalGMSysMessage("Wintergrasp has been disabled.");
        }
        else
        {
            bf->ToggleBattlefield(true);
            if (battleid == 1)
                handler->SendGlobalGMSysMessage("Wintergrasp has been enabled.");
        }

        return true;
    }
    
    static bool HandleBattlefieldSwitch(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->EndBattle(false);

        if (battleid == 1)
        {
            std::string message = "Wintergrasp switched to ";
            message += bf->GetDefenderTeam() == TEAM_ALLIANCE ? "Alliance." : "Horde.";
            handler->SendGlobalGMSysMessage(message.c_str());
        }

        return true;
    }

    static bool HandleBattlefieldTimer(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        uint32 time = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;
        char* time_str = strtok(NULL, " ");
        if (!time_str)
            return false;

        battleid = atoi(battleid_str);
        time = atoi(time_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->SetTimer(time * IN_MILLISECONDS);
        bf->SendInitWorldStatesToAll(true);

        if (battleid == 1)
        {
            std::ostringstream oString;
            oString << "Wintergrasp timer set to " << (time / 60) << " minute(s) " << (time % 60) << " second(s).";
            handler->SendGlobalGMSysMessage(oString.str().c_str());
        }

        return true;
    }
};

void AddSC_battlefield_commandscript()
{
    new battlefield_commandscript();
}