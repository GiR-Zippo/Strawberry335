#include "ScriptMgr.h"
#include "Chat.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "AccountMgr.h"
#include "ObjectMgr.h"

class arenateam_commandscript : public CommandScript
{
public:
    arenateam_commandscript() : CommandScript("arenateam_commandscript") { }

    ChatCommand* GetCommands() const
    {

        static ChatCommand arenaTeamCommandTable[] =
        {
            { "viewid",         SEC_GAMEMASTER,     false, &HandleArenaTeamViewIdCommand,     "", NULL},
            { "viewname",       SEC_GAMEMASTER,     false, &HandleArenaTeamViewNameCommand,   "", NULL},
            { "create",         SEC_GAMEMASTER,     false, &HandleArenaTeamCreateCommand,     "", NULL},
            { "rename",         SEC_GAMEMASTER,     false, &HandleArenaTeamReNameCommand,     "", NULL},
            { "delete",         SEC_GAMEMASTER,     false, &HandleArenaTeamDeleteCommand,     "", NULL},
            { NULL,             0,                  false, NULL,                                                       "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "arenateam",      SEC_GAMEMASTER,     false, NULL,                                           "", arenaTeamCommandTable},
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleArenaTeamViewIdCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 id = 0;
        char* szId = strtok((char*)args, " ");
        if (szId)
            id = strtol(szId, NULL, 10);

        ArenaTeam* arenaTeam = sArenaTeamMgr->GetArenaTeamById(id);
        if (!arenaTeam)
        {
            handler->SendSysMessage(LANG_ARENA_TEAM_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(LANG_ARENA_TEAM_VIEW, arenaTeam->GetName().c_str(), arenaTeam->GetId(), arenaTeam->GetType(), arenaTeam->GetType(), arenaTeam->GetRating());
        for (ArenaTeam::MemberList::iterator itr = arenaTeam->m_membersBegin(); itr != arenaTeam->m_membersEnd(); ++itr)
        {
            std::string accName("");
            uint32 accId = sObjectMgr->GetPlayerAccountIdByGUID((*itr).Guid);
            AccountMgr::GetName(accId, accName);

            if (arenaTeam->GetCaptain() == (*itr).Guid)
                handler->PSendSysMessage(LANG_ARENA_TEAM_CAPTAIN, (*itr).Name.c_str(), accName.c_str());
            else
                handler->PSendSysMessage(LANG_ARENA_TEAM_MEMBER, (*itr).Name.c_str(), accName.c_str());
        }

        return true;
    }

    static bool HandleArenaTeamViewNameCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        char* tailStr = *args != '"' ? strtok((char*)args, " ") : (char*)args;
        if (!tailStr)
            return false;

        char* name = handler->extractQuotedArg(tailStr);
        if (!name)
            return false;

        ArenaTeam* arenaTeam = sArenaTeamMgr->GetArenaTeamByName(name);
        if (!arenaTeam)
        {
            handler->SendSysMessage(LANG_ARENA_TEAM_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(LANG_ARENA_TEAM_VIEW, arenaTeam->GetName().c_str(), arenaTeam->GetId(), arenaTeam->GetType(), arenaTeam->GetType(), arenaTeam->GetRating());
        for (ArenaTeam::MemberList::iterator itr = arenaTeam->m_membersBegin(); itr != arenaTeam->m_membersEnd(); ++itr)
        {
            std::string accName("");
            uint32 accId = sObjectMgr->GetPlayerAccountIdByGUID((*itr).Guid);
            AccountMgr::GetName(accId, accName);

            if (arenaTeam->GetCaptain() == (*itr).Guid)
                handler->PSendSysMessage(LANG_ARENA_TEAM_CAPTAIN, (*itr).Name.c_str(), accName.c_str());
            else
                handler->PSendSysMessage(LANG_ARENA_TEAM_MEMBER, (*itr).Name.c_str(), accName.c_str());
        }

        return true;
    }

    static bool HandleArenaTeamReNameCommand(ChatHandler* handler, const char* args)
    {
        /* command should be:
        .arenateam rename "oldteam";"newteam"
        */
            if (!*args)
            return false;

        char* oldname = NULL;
        char* newname = NULL;
        if (*args == '"')
        {
            oldname = strtok((char*)args, ";");
            newname = strtok(NULL, ";");
        }

        if (!oldname || !newname)
            return false;

        oldname = handler->extractQuotedArg(oldname);
        newname = handler->extractQuotedArg(newname);

        ArenaTeam* arenaTeam = sArenaTeamMgr->GetArenaTeamByName(oldname);
        if (!arenaTeam)
        {
            handler->SendSysMessage(LANG_ARENA_TEAM_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

            CharacterDatabase.PExecute("UPDATE arena_team SET name = '%s' WHERE name = '%s'", newname, oldname);
            handler->PSendSysMessage(LANG_ARENA_TEAM_RENAMED, oldname, newname);
            return true;
    }

    static bool HandleArenaTeamDeleteCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 id = 0;
        char* name;
        if (isNumeric(args))
        {
            char* szId = strtok((char*)args, " ");
            if (szId)
                id = strtol(szId, NULL, 10);
        }
        else
        {
            char* tailStr = *args != '"' ? strtok((char*)args, " ") : (char*)args;
            if (!tailStr)
                return false;

            name = handler->extractQuotedArg(tailStr);
            if (!name)
                return false;
        }

        ArenaTeam* arenaTeam;
        if (id)
            arenaTeam = sArenaTeamMgr->GetArenaTeamById(id);
        else
            arenaTeam = sArenaTeamMgr->GetArenaTeamByName(name);

        if (!arenaTeam)
        {
            handler->SendSysMessage(LANG_ARENA_TEAM_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(LANG_ARENA_TEAM_DELETED, arenaTeam->GetName().c_str(), arenaTeam->GetId());
        arenaTeam->Disband(handler->GetSession());
        delete arenaTeam;
        return true;
    }

    static bool HandleArenaTeamCreateCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget(*args != '"' ? (char*)args : NULL, &target))
            return false;

        char* tailStr = *args != '"' ? strtok(NULL, "") : (char*)args;
        if (!tailStr)
            return false;

        char* name = handler->extractQuotedArg(tailStr);
        if (!name)
            return false;

        char* typeStr = strtok(NULL, "");
        if (!typeStr)
            return false;

        int8 type = atoi(typeStr);
        if (sArenaTeamMgr->GetArenaTeamByName(name))
        {
            handler->PSendSysMessage(LANG_ARENA_TEAM_ERROR_NAME_EXISTS, name);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (type == 2 || type == 3 || type == 5 )
        {
            if (Player::GetArenaTeamIdFromDB(target->GetGUID(), type) != 0)
            {
                handler->PSendSysMessage(LANG_ARENA_TEAM_ERROR_SIZE, target->GetName(), type, type);
                handler->SetSentErrorMessage(true);
                return false;
            }

            ArenaTeam* arena = new ArenaTeam();

            if (!arena->Create(target->GetGUID(), type, name, 4293102085, 101, 4293253939, 4, 4284049911))
            {
                delete arena;
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            sArenaTeamMgr->AddArenaTeam(arena);
            handler->PSendSysMessage(LANG_ARENA_TEAM_CREATE, arena->GetName().c_str(), arena->GetId(), arena->GetType(), arena->GetCaptain());
        }
        else
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        return true;
    }
};

void AddSC_arenateam_commandscript()
{
    new arenateam_commandscript();
}