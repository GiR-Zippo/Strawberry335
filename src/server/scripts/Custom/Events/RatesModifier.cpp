#include "RatesModifier.h"
#include "ScriptedGossip.h"
#include "Config.h"
#include "GameEventMgr.h"

enum ExpModGossips
{
    EXP_MOD_BLIZZARD = 1,
    EXP_MOD_RESET = 0,
    EXP_MOD_BONUS = 8,

    EXP_MOD_NONE = 100,
};

bool RatesModifier::OnGossipHello(Player* player, Creature* creature)
{
    uint32 xpRate = sWorld->getRate(RATE_XP_KILL);
    std::stringstream actualXpRateStr;
    actualXpRateStr << "Deine aktuelle Rate ist auf ";
    if (uint32 rate = player->GetOverrideRate(RATE_OVERRIDE_XP_KILL))
        actualXpRateStr << rate;
    else
        actualXpRateStr << xpRate;
    actualXpRateStr << "x";
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, actualXpRateStr.str().c_str(), creature->GetEntry(), EXP_MOD_NONE);
    
    std::stringstream xpRateStr;
    xpRateStr << "Laenalith Standard (";
    xpRateStr << xpRate;
    xpRateStr << "x)";
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Blizzard Standard (1x)", creature->GetEntry(), EXP_MOD_BLIZZARD);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, xpRateStr.str().c_str(), creature->GetEntry(), EXP_MOD_RESET);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Laenalith Bonus (8x)", creature->GetEntry(), EXP_MOD_BONUS);
    player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
    return true;
}

bool RatesModifier::OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    if (creature->GetEntry() != sender)
        return false;
    switch (action)
    {
        case EXP_MOD_BLIZZARD:
        case EXP_MOD_RESET:
        case EXP_MOD_BONUS:
            player->SetOverrideRate(RATE_OVERRIDE_XP_EXPLORE, action);
            player->SetOverrideRate(RATE_OVERRIDE_XP_KILL, action);
            player->SetOverrideRate(RATE_OVERRIDE_XP_QUEST, action);
            player->GetSession()->SendNotification("Deine XP Raten wurden auf %ux gesetzt.", action);
            break;
        case EXP_MOD_NONE:
        default:
            break;
    }
    player->PlayerTalkClass->ClearMenus();
    player->PlayerTalkClass->SendCloseGossip();
    return true;
}

uint32 RatesModifierPlayer::EventIds[MAX_MOD_EVENT] = 
{
    110
};

void RatesModifierPlayer::OnLogin(Player* player)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_RATE_OVERRIDES);
    stmt->setUInt32(0, player->GetGUIDLow());
    PreparedQueryResult res = CharacterDatabase.Query(stmt);
    if (res)
    {
        do
        {
            Field* row = res->Fetch();
            uint32 rate = row[0].GetUInt8();
            if (uint32 value = row[1].GetUInt32())
                player->SetOverrideRate(PlayerRatesOverride(rate), value);
        }
        while (res->NextRow());
    }
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_RATE_OVERRIDES);
    stmt->setUInt32(0, player->GetGUIDLow());
    CharacterDatabase.Execute(stmt);
    for (uint8 i = 0; i < MAX_MOD_EVENT; ++i)
    {
        bool active = sGameEventMgr->IsActiveEvent(EventIds[i]);
        switch (i)
        {
            case MOD_EVENT_XP:
            {
                if (!active)
                {
                    if (player->GetOverrideRate(RATE_OVERRIDE_XP_KILL) != 0)
                    {
                        uint32 xpRate = sWorld->getRate(RATE_XP_KILL);
                        std::stringstream xpRateStr;
                        xpRateStr << "Laenalith Standard (";
                        xpRateStr << xpRate;
                        xpRateStr << "x)";
                        player->GetSession()->SendNotification("Deine XP Raten wurden zurück gesetzt. %s", xpRateStr.str().c_str());
                    }
                    player->SetOverrideRate(RATE_OVERRIDE_XP_EXPLORE, 0);
                    player->SetOverrideRate(RATE_OVERRIDE_XP_KILL, 0);
                    player->SetOverrideRate(RATE_OVERRIDE_XP_QUEST, 0);
                }
                else
                {
                    player->GetSession()->SendNotification("Deine XP Raten wurden auf %ux gesetzt.", player->GetOverrideRate(RATE_OVERRIDE_XP_KILL));
                }
                break;
            }
            default:
                break;
        }
    }
}

void RatesModifierPlayer::OnLogout(Player* player)
{
    PreparedStatement* stmt = NULL;
    for (uint8 i = 0; i < MAX_OVERRIDE_RATES; ++i)
    {
        if (uint32 rate = player->GetOverrideRate(PlayerRatesOverride(i)))
        {
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_RATE_OVERRIDES);
            stmt->setUInt32(0, player->GetGUIDLow());
            stmt->setUInt8(1, i);
            stmt->setUInt32(2, rate);
            CharacterDatabase.DirectExecute(stmt);
        }
    }
}
