#include "ScriptMgr.h"

#include "Guild.h"
#include "GuildMgr.h"
#include "Player.h"
#include "World.h"

class StarterGuildJoinScript : public PlayerScript {
  public:
    StarterGuildJoinScript() : PlayerScript("StarterGuildJoinScript") {}

    // Called when a player is created.
    void OnCreate(Player *player) override {
        uint32_t guild_id =
            sWorld->getIntConfig((player->GetTeamId() == TEAM_ALLIANCE)
                                     ? CONFIG_STARTER_GUILD_ALLIANCE
                                     : CONFIG_STARTER_GUILD_HORDE);

        if (auto starter_guild = sGuildMgr->GetGuildById(guild_id))
            starter_guild->AddMember(player);
    };
};

void AddSC_StarterGuild() { new StarterGuildJoinScript(); }
