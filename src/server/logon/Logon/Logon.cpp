#include "Common.h"
#include "DatabaseEnv.h"
#include "Config.h"
#include "SystemConfig.h"
#include "Log.h"
#include "ClientSessionMgr.h"
#include "ClientSession.h"
#include "RoutingHelper.h"
#include "Chat.h"
#include "AchievementMgr.h"
#include "DBCStores.h"
#include "AccountMgr.h"

#include "Logon.h"
#include "AddonMgr.h"
#include "ObjectMgr.h"
#include "LocalesMgr.h"
#include "MapHolder.h"

#include "WorldPacket.h"
#include "Opcodes.h"
#include "ControlSessionMgr.h"
#include "ControlSession.h"
#include "DatabaseRoutines.h"
#include "WardenHandler.h"

//Caching
#include "GameObjects.h"
#include "Creatures.h"
#include "Items.h"
#include "Quest.h"

//Bg-System
#include "BattlegroundMgr.h"

//Tickets
#include "TicketMgr.h"
#include "GMQualityManager.h"

#include "AutoBroadcast.h"

#include <algorithm>
#include <functional>

std::atomic<bool> Logon::m_stopEvent(false);
std::atomic<bool> Logon::m_haltEvent(false);
uint8 Logon::m_ExitCode = SHUTDOWN_EXIT_CODE;
std::atomic<uint32> Logon::m_logonLoopCounter(0);

Logon::Logon()
{
    m_ShutdownMask = 0;
    m_ShutdownTimer = 0;
    m_HaltTimer     = 0;

    m_defaultDbcLocale = LOCALE_enUS;

    m_gameTime = time(NULL);
    m_startTime = m_gameTime;
    m_isClosed = false;
    sClientSessionMgr->SetPlayerAmountLimit(0);
}

Logon::~Logon()
{
    sClientSessionMgr->ServerClose();
}

/// Initialize config values
void Logon::LoadConfigSettings(bool reload)
{
    if (reload)
    {
        if (!ConfigMgr::Load())
        {
            sLog->outError("Logon settings reload fail: can't read settings from %s.",ConfigMgr::GetFilename().c_str());
            return;
        }
    }

    ///- Read the "Data" directory from the config file
    std::string dataPath = ConfigMgr::GetStringDefault("DataDir", "./");
    if (dataPath.at(dataPath.length()-1) != '/' && dataPath.at(dataPath.length()-1) != '\\')
        dataPath.append("/");

    if (reload)
    {
        if (dataPath != m_dataPath)
            sLog->outError("DataDir option can't be changed at worldserver.conf reload, using current value (%s).", m_dataPath.c_str());
    }
    else
    {
        m_dataPath = dataPath;
        sLog->outString("Using DataDir %s", m_dataPath.c_str());
    }

    SetFakeMsg("Ich bin Keazy und stehe auf Blumenkohl.");

    sClientSessionMgr->SetPlayerAmountLimit(ConfigMgr::GetIntDefault("PlayerLimit", 100));

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("LogonServerPort", 8085);
        if (val != m_int_configs[CONFIG_PORT_LOGON])
            sLog->outError("LogonServerPort option can't be changed at logonserver.conf reload, using current value (%u).",m_int_configs[CONFIG_PORT_LOGON]);
    }
    else
        m_int_configs[CONFIG_PORT_LOGON] = ConfigMgr::GetIntDefault("LogonServerPort", 8085);

    m_int_configs[CONFIG_SOCKET_TIMEOUTTIME] = ConfigMgr::GetIntDefault("SocketTimeOutTime", 900000);
    m_int_configs[CONFIG_INTERVAL_DISCONNECT_TOLERANCE] = ConfigMgr::GetIntDefault("DisconnectToleranceInterval", 0);
    m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] = ConfigMgr::GetIntDefault("MaxOverspeedPings",2);
    if (m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] != 0 && m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] < 2)
    {
        sLog->outError("MaxOverspeedPings (%i) must be in range 2..infinity (or 0 to disable check). Set to 2.",m_int_configs[CONFIG_MAX_OVERSPEED_PINGS]);
        m_int_configs[CONFIG_MAX_OVERSPEED_PINGS] = 2;
    }

    m_int_configs[CONFIG_SESSION_ADD_DELAY] = ConfigMgr::GetIntDefault("SessionAddDelay", 10000);

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("GameType", 0);
        if (val != m_int_configs[CONFIG_GAME_TYPE])
            sLog->outError("GameType option can't be changed at logonserver.conf reload, using current value (%u).", m_int_configs[CONFIG_GAME_TYPE]);
    }
    else
        m_int_configs[CONFIG_GAME_TYPE] = ConfigMgr::GetIntDefault("GameType", 0);

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("RealmZone", REALM_ZONE_DEVELOPMENT);
        if (val != m_int_configs[CONFIG_REALM_ZONE])
            sLog->outError("RealmZone option can't be changed at logonserver.conf reload, using current value (%u).", m_int_configs[CONFIG_REALM_ZONE]);
    }
    else
        m_int_configs[CONFIG_REALM_ZONE] = ConfigMgr::GetIntDefault("RealmZone", REALM_ZONE_DEVELOPMENT);

    if (reload)
    {
        uint32 val = ConfigMgr::GetIntDefault("Expansion",1);
        if (val != m_int_configs[CONFIG_EXPANSION])
            sLog->outError("Expansion option can't be changed at logonserver.conf reload, using current value (%u).",m_int_configs[CONFIG_EXPANSION]);
    }
    else
        m_int_configs[CONFIG_EXPANSION] = ConfigMgr::GetIntDefault("Expansion",3);

    m_int_configs[CONFIG_REBASE_INTERVAL] = ConfigMgr::GetIntDefault("OptimizeInterval", 7);

    if (int32 clientCacheId = ConfigMgr::GetIntDefault("ClientCacheVersion", 0))
    {
        // overwrite DB/old value
        if (clientCacheId > 0)
        {
            m_int_configs[CONFIG_CLIENTCACHE_VERSION] = clientCacheId;
            sLog->outString("Client cache version set to: %u", clientCacheId);
        }
        else
            sLog->outError("ClientCacheVersion can't be negative %d, ignored.", clientCacheId);
    }

    m_int_configs[CONFIG_UPTIME_UPDATE] = ConfigMgr::GetIntDefault("UpdateUptimeInterval", 10);
    if (int32(m_int_configs[CONFIG_UPTIME_UPDATE]) <= 0)
    {
        sLog->outError("UpdateUptimeInterval (%i) must be > 0, set to default 10.", m_int_configs[CONFIG_UPTIME_UPDATE]);
        m_int_configs[CONFIG_UPTIME_UPDATE] = 10;
    }
    if (reload)
    {
        m_timers[LUPDATE_UPTIME].SetInterval(m_int_configs[CONFIG_UPTIME_UPDATE]*MINUTE*IN_MILLISECONDS);
        m_timers[LUPDATE_UPTIME].Reset();
    }

    // MySQL ping time interval
    m_int_configs[CONFIG_DB_PING_INTERVAL]              = ConfigMgr::GetIntDefault("MaxPingTime", 30);
    if (!reload)
    {
        m_int_configs[CONFIG_SESSION_MT_THREADS]        = ConfigMgr::GetIntDefault("SessionThreadNumbers", 0);
        m_int_configs[CONFIG_SESSION_DYN_TIMER]         = ConfigMgr::GetIntDefault("DynamicThreading.Interval", 90000);
        m_int_configs[CONFIG_SESSION_DYN_MAX]           = ConfigMgr::GetIntDefault("DynamicThreading.Max", 30);
        m_bool_configs[CONFIG_SESSION_DYN_ENABLED]      = ConfigMgr::GetBoolDefault("DynamicThreading.Enabled", false);
        m_bool_configs[CONFIG_INTERNAL_CACHE]           = ConfigMgr::GetBoolDefault("InternalCache", true);
    }

    // Logging
    m_bool_configs[CONFIG_CHATLOG_CHANNEL]              = ConfigMgr::GetBoolDefault("ChatLogs.Channel", false);
    m_bool_configs[CONFIG_CHATLOG_WHISPER]              = ConfigMgr::GetBoolDefault("ChatLogs.Whisper", false);
    m_bool_configs[CONFIG_CHATLOG_SYSCHAN]              = ConfigMgr::GetBoolDefault("ChatLogs.SysChan", false);
    m_bool_configs[CONFIG_CHATLOG_PARTY]                = ConfigMgr::GetBoolDefault("ChatLogs.Party", false);
    m_bool_configs[CONFIG_CHATLOG_RAID]                 = ConfigMgr::GetBoolDefault("ChatLogs.Raid", false);
    m_bool_configs[CONFIG_CHATLOG_GUILD]                = ConfigMgr::GetBoolDefault("ChatLogs.Guild", false);
    m_bool_configs[CONFIG_CHATLOG_PUBLIC]               = ConfigMgr::GetBoolDefault("ChatLogs.Public", false);
    m_bool_configs[CONFIG_CHATLOG_ADDON]                = ConfigMgr::GetBoolDefault("ChatLogs.Addon", false);
    m_bool_configs[CONFIG_CHATLOG_BGROUND]              = ConfigMgr::GetBoolDefault("ChatLogs.Battleground", false);
    m_bool_configs[CONFIG_CHATLOG_GM_ACTIVITY]          = ConfigMgr::GetBoolDefault("ChatLogs.GM.Activity", false);

    // PLAYER INTERACTION
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT]    = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Chat", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL] = ConfigMgr::GetBoolDefault("AllowTwoSide.Interaction.Channel", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_WHO_LIST]      = ConfigMgr::GetBoolDefault("AllowTwoSide.WhoList", false);
    m_bool_configs[CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND]    = ConfigMgr::GetBoolDefault("AllowTwoSide.AddFriend", false);

    // CHAT SETTINGS
    m_int_configs[CONFIG_CHAT_WHISPER_LEVEL_REQ]        = ConfigMgr::GetIntDefault("ChatLevelReq.Whisper", 1);

    // GAME MASTER SETTINGS
    m_int_configs[CONFIG_GM_LOGIN_STATE]                = ConfigMgr::GetIntDefault("GM.LoginState", 2);
    m_int_configs[CONFIG_GM_VISIBLE_STATE]              = ConfigMgr::GetIntDefault("GM.Visible", 2);
    m_int_configs[CONFIG_GM_CHAT]                       = ConfigMgr::GetIntDefault("GM.Chat", 2);
    m_int_configs[CONFIG_GM_WHISPERING_TO]              = ConfigMgr::GetIntDefault("GM.WhisperingTo", 2);
    m_int_configs[CONFIG_GM_LEVEL_IN_GM_LIST]           = ConfigMgr::GetIntDefault("GM.InGMList.Level", SEC_ADMINISTRATOR);
    m_int_configs[CONFIG_GM_LEVEL_IN_WHO_LIST]          = ConfigMgr::GetIntDefault("GM.InWhoList.Level", SEC_ADMINISTRATOR);
    m_bool_configs[CONFIG_ALLOW_GM_GROUP]               = ConfigMgr::GetBoolDefault("GM.AllowInvite", false);
    m_bool_configs[CONFIG_ALLOW_GM_FRIEND]              = ConfigMgr::GetBoolDefault("GM.AllowFriend", false);
    m_bool_configs[CONFIG_GM_LOWER_SECURITY]            = ConfigMgr::GetBoolDefault("GM.LowerSecurity", false);
    m_int_configs[CONFIG_GM_LEVEL_ALLOW_ACHIEVEMENTS]   = ConfigMgr::GetIntDefault("GM.AllowAchievementGain.Level", SEC_ADMINISTRATOR);

    // Chat & Channels


    // Warden
    m_int_configs[CONFIG_WARDEN_NUM_MEM_CHECKS]         = ConfigMgr::GetIntDefault("Warden.NumMemChecks", 3);
    m_int_configs[CONFIG_WARDEN_NUM_OTHER_CHECKS]       = ConfigMgr::GetIntDefault("Warden.NumOtherChecks", 7);
    m_int_configs[CONFIG_WARDEN_CLIENT_BAN_DURATION]    = ConfigMgr::GetIntDefault("Warden.BanDuration", 86400);
    m_int_configs[CONFIG_WARDEN_CLIENT_CHECK_HOLDOFF]   = ConfigMgr::GetIntDefault("Warden.ClientCheckHoldOff", 30);
    m_int_configs[CONFIG_WARDEN_CLIENT_RESPONSE_DELAY]  = ConfigMgr::GetIntDefault("Warden.ClientResponseDelay", 600);

    // Tickets
    m_bool_configs[CONFIG_GMTICKET_ALLOW_TICKETS] = ConfigMgr::GetBoolDefault("GMTicket.AllowTickets", true);
    m_int_configs[CONFIG_GMTICKET_LEVEL_REQ] = ConfigMgr::GetIntDefault("GMTicket.LevelReq", 1);
    m_float_configs[CONFIG_GMTICKET_CHANCE_OF_GM_SURVEY] = ConfigMgr::GetFloatDefault("GMTicket.ChanceOfGMSurvey", 50.0f);

    // Misc
    m_bool_configs[CONFIG_DIE_COMMAND_MODE] = ConfigMgr::GetBoolDefault("Die.Command.Mode", true);

    // AutoBroadcast
    m_bool_configs[CONFIG_AUTOBROADCAST] = ConfigMgr::GetBoolDefault("AutoBroadcast.On", false);
    m_int_configs[CONFIG_AUTOBROADCAST_CENTER] = ConfigMgr::GetIntDefault("AutoBroadcast.Center", 0);
    m_int_configs[CONFIG_AUTOBROADCAST_INTERVAL] = ConfigMgr::GetIntDefault("AutoBroadcast.Timer", 60000);

    // Rates
    {
        rate_values[RATE_HEALTH]                        = ConfigMgr::GetFloatDefault("Rate.Health", 1);
        if (rate_values[RATE_HEALTH] < 0)
        {
            sLog->outError("Rate.Health (%f) must be > 0. Using 1 instead.", rate_values[RATE_HEALTH]);
            rate_values[RATE_HEALTH]                    = 1;
        }
        rate_values[RATE_POWER_MANA]                    = ConfigMgr::GetFloatDefault("Rate.Mana", 1);
        if (rate_values[RATE_POWER_MANA] < 0)
        {
            sLog->outError("Rate.Mana (%f) must be > 0. Using 1 instead.", rate_values[RATE_POWER_MANA]);
            rate_values[RATE_POWER_MANA]                = 1;
        }
        rate_values[RATE_POWER_RAGE_INCOME]             = ConfigMgr::GetFloatDefault("Rate.Rage.Income", 1);
        rate_values[RATE_POWER_RAGE_LOSS]               = ConfigMgr::GetFloatDefault("Rate.Rage.Loss", 1);
        if (rate_values[RATE_POWER_RAGE_LOSS] < 0)
        {
            sLog->outError("Rate.Rage.Loss (%f) must be > 0. Using 1 instead.", rate_values[RATE_POWER_RAGE_LOSS]);
            rate_values[RATE_POWER_RAGE_LOSS]           = 1;
        }
        rate_values[RATE_POWER_RUNICPOWER_INCOME]       = ConfigMgr::GetFloatDefault("Rate.RunicPower.Income", 1);
        rate_values[RATE_POWER_RUNICPOWER_LOSS]         = ConfigMgr::GetFloatDefault("Rate.RunicPower.Loss", 1);
        if (rate_values[RATE_POWER_RUNICPOWER_LOSS] < 0)
        {
            sLog->outError("Rate.RunicPower.Loss (%f) must be > 0. Using 1 instead.", rate_values[RATE_POWER_RUNICPOWER_LOSS]);
            rate_values[RATE_POWER_RUNICPOWER_LOSS]     = 1;
        }
        rate_values[RATE_POWER_FOCUS]                   = ConfigMgr::GetFloatDefault("Rate.Focus", 1.0f);
        rate_values[RATE_POWER_ENERGY]                  = ConfigMgr::GetFloatDefault("Rate.Energy", 1.0f);
        rate_values[RATE_SKILL_DISCOVERY]               = ConfigMgr::GetFloatDefault("Rate.Skill.Discovery", 1.0f);
        rate_values[RATE_DROP_ITEM_POOR]                = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Poor", 1.0f);
        rate_values[RATE_DROP_ITEM_NORMAL]              = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Normal", 1.0f);
        rate_values[RATE_DROP_ITEM_UNCOMMON]            = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Uncommon", 1.0f);
        rate_values[RATE_DROP_ITEM_RARE]                = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Rare", 1.0f);
        rate_values[RATE_DROP_ITEM_EPIC]                = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Epic", 1.0f);
        rate_values[RATE_DROP_ITEM_LEGENDARY]           = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Legendary", 1.0f);
        rate_values[RATE_DROP_ITEM_ARTIFACT]            = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Artifact", 1.0f);
        rate_values[RATE_DROP_ITEM_REFERENCED]          = ConfigMgr::GetFloatDefault("Rate.Drop.Item.Referenced", 1.0f);
        rate_values[RATE_DROP_MONEY]                    = ConfigMgr::GetFloatDefault("Rate.Drop.Money", 1.0f);
        rate_values[RATE_DROP_ITEM_REFERENCED_AMOUNT]   = ConfigMgr::GetFloatDefault("Rate.Drop.Item.ReferencedAmount", 1.0f);
        rate_values[RATE_XP_KILL]                       = ConfigMgr::GetFloatDefault("Rate.XP.Kill", 1.0f);
        rate_values[RATE_XP_QUEST]                      = ConfigMgr::GetFloatDefault("Rate.XP.Quest", 1.0f);
        rate_values[RATE_XP_EXPLORE]                    = ConfigMgr::GetFloatDefault("Rate.XP.Explore", 1.0f);
        rate_values[RATE_REPAIRCOST]                    = ConfigMgr::GetFloatDefault("Rate.RepairCost", 1.0f);
        if (rate_values[RATE_REPAIRCOST] < 0.0f)
        {
            sLog->outError("Rate.RepairCost (%f) must be >=0. Using 0.0 instead.", rate_values[RATE_REPAIRCOST]);
            rate_values[RATE_REPAIRCOST] = 0.0f;
        }
        rate_values[RATE_REST_INGAME]                   = ConfigMgr::GetFloatDefault("Rate.Rest.InGame", 1.0f);
        rate_values[RATE_REST_OFFLINE_IN_TAVERN_OR_CITY] = ConfigMgr::GetFloatDefault("Rate.Rest.Offline.InTavernOrCity", 1.0f);
        rate_values[RATE_REST_OFFLINE_IN_WILDERNESS]    = ConfigMgr::GetFloatDefault("Rate.Rest.Offline.InWilderness", 1.0f);
        rate_values[RATE_DAMAGE_FALL]                   = ConfigMgr::GetFloatDefault("Rate.Damage.Fall", 1.0f);
        rate_values[RATE_AUCTION_TIME]                  = ConfigMgr::GetFloatDefault("Rate.Auction.Time", 1.0f);
        rate_values[RATE_AUCTION_DEPOSIT]               = ConfigMgr::GetFloatDefault("Rate.Auction.Deposit", 1.0f);
        rate_values[RATE_AUCTION_CUT]                   = ConfigMgr::GetFloatDefault("Rate.Auction.Cut", 1.0f);
        rate_values[RATE_HONOR]                         = ConfigMgr::GetFloatDefault("Rate.Honor", 1.0f);
        rate_values[RATE_MINING_AMOUNT]                 = ConfigMgr::GetFloatDefault("Rate.Mining.Amount", 1.0f);
        rate_values[RATE_MINING_NEXT]                   = ConfigMgr::GetFloatDefault("Rate.Mining.Next", 1.0f);
        rate_values[RATE_TALENT]                        = ConfigMgr::GetFloatDefault("Rate.Talent", 1.0f);
        if (rate_values[RATE_TALENT] < 0.0f)
        {
            sLog->outError("Rate.Talent (%f) must be > 0. Using 1 instead.", rate_values[RATE_TALENT]);
            rate_values[RATE_TALENT] = 1.0f;
        }
        rate_values[RATE_REPUTATION_GAIN]               = ConfigMgr::GetFloatDefault("Rate.Reputation.Gain", 1.0f);
        rate_values[RATE_REPUTATION_LOWLEVEL_KILL]      = ConfigMgr::GetFloatDefault("Rate.Reputation.LowLevel.Kill", 1.0f);
        rate_values[RATE_REPUTATION_LOWLEVEL_QUEST]     = ConfigMgr::GetFloatDefault("Rate.Reputation.LowLevel.Quest", 1.0f);
        rate_values[RATE_REPUTATION_RECRUIT_A_FRIEND_BONUS] = ConfigMgr::GetFloatDefault("Rate.Reputation.RecruitAFriendBonus", 0.1f);

        m_int_configs[CONFIG_SKILL_GAIN_CRAFTING]       = ConfigMgr::GetIntDefault("SkillGain.Crafting", 1);
        m_int_configs[CONFIG_SKILL_GAIN_DEFENSE]        = ConfigMgr::GetIntDefault("SkillGain.Defense", 1);
        m_int_configs[CONFIG_SKILL_GAIN_GATHERING]      = ConfigMgr::GetIntDefault("SkillGain.Gathering", 1);
        m_int_configs[CONFIG_SKILL_GAIN_WEAPON]         = ConfigMgr::GetIntDefault("SkillGain.Weapon", 1);
        m_int_configs[CONFIG_SKILL_CHANCE_ORANGE]       = ConfigMgr::GetIntDefault("SkillChance.Orange", 100);
        m_int_configs[CONFIG_SKILL_CHANCE_YELLOW]       = ConfigMgr::GetIntDefault("SkillChance.Yellow", 75);
        m_int_configs[CONFIG_SKILL_CHANCE_GREEN]        = ConfigMgr::GetIntDefault("SkillChance.Green", 25);
        m_int_configs[CONFIG_SKILL_CHANCE_GREY]         = ConfigMgr::GetIntDefault("SkillChance.Grey", 0);
        m_int_configs[CONFIG_SKILL_CHANCE_MINING_STEPS]  = ConfigMgr::GetIntDefault("SkillChance.MiningSteps", 75);
        m_int_configs[CONFIG_SKILL_CHANCE_SKINNING_STEPS]   = ConfigMgr::GetIntDefault("SkillChance.SkinningSteps", 75);
    }
}

void Logon::LoadDBVersion()
{
    QueryResult result = WorldDatabase.Query("SELECT db_version, cache_id FROM version LIMIT 1");
    if (result)
    {
        Field* fields = result->Fetch();
        m_DBVersion = fields[0].GetString();

        // will be overwrite by config values if different and non-0
        m_int_configs[CONFIG_CLIENTCACHE_VERSION] = fields[1].GetUInt32();
    }

    if (m_DBVersion.empty())
        m_DBVersion = "Unknown world database.";
}

/// Initialize logonserver
void Logon::SetInitialLogonSettings()
{
    ///- Server startup begin
    uint32 startupBegin = getMSTime();

    ///- Initialize the random number generator
    srand((unsigned int)time(NULL));

    ///- Loading strings. Getting no records means core load has to be canceled because no error message can be output.
    sLog->outString();
    sLog->outString("Loading Trinity strings...");
    if (!sObjectMgr->LoadTrinityStrings())
        exit(1);                                            // Error message displayed in function already

    //Update Realmlist
    uint32 server_type;
    if (IsFFAPvPRealm())
        server_type = REALM_TYPE_PVP;
    else
        server_type = getIntConfig(CONFIG_GAME_TYPE);
    uint32 realm_zone = getIntConfig(CONFIG_REALM_ZONE);
    LoginDatabase.PExecute("UPDATE realmlist SET icon = %u, timezone = %u WHERE id = '%d'", server_type, realm_zone, realmID);

    ///- Init highest guids before any table loading to prevent using not initialized guids in some code.
    sLog->outString("Calculating highest guids...");
    sObjectMgr->SetHighestGuids();

    //sLog->outString("Loading Character-Item GUIDs...");
    //sGuidHolder->Initialize();

    ///- Load the DBC files
    sLog->outString("Initialize data stores...");
    LoadDBCStores(m_dataPath);
    DetectDBCLang();
    sLog->outString("Loading Locales...");
    sLocalesMgr->LoadLocales();

    sLog->outString("Loading client addons...");
    sAddonMgr->LoadFromDB();

    sLog->outString("Loading Cache...");
    sGameObjects->WarmingCache();
    sCreatures->WarmingCache();
    sQuest->WarmingCache();
    sLog->outString();

    sItems->WarmingCache();
    sLog->outString();

    sLog->outString("Loading Achievements...");
    sAchievementMgr->LoadAchievementReferenceList();
    sLog->outString("Loading Achievement Criteria Lists...");
    sAchievementMgr->LoadAchievementCriteriaList();
    sLog->outString("Loading Achievement Criteria Data...");
    sAchievementMgr->LoadAchievementCriteriaData();
    sLog->outString("Loading Achievement Rewards...");
    sAchievementMgr->LoadRewards();
    sLog->outString("Loading Achievement Reward Locales...");
    sAchievementMgr->LoadRewardLocales();
    sLog->outString("Loading Completed Achievements...");
    sAchievementMgr->LoadCompletedAchievements();
    sLog->outString("Loading Warden Modules..." );
    WardenModuleStorage.Init();
    sLog->outString("Loading LogonServer States...");              // must be loaded before battleground, outdoor PvP and conditions
    LoadLogonStates();
    sLog->outString("Loading Battleground Templates...");
    sBattlegroundMgr->Initialize();

    sLog->outString("Loading NodeList...");
    sRoutingHelper->LoadNodeList();
    sLog->outString("Loading NodeMap...");
    sRoutingHelper->LoadNodeMap();
    sLog->outString("Loading AntiSpam...");
    InitAntiSpam();
    sClientSessionMgr->Initialize();

    sLog->outString("Loading GM tickets...");
    sTicketMgr->LoadTickets();
    sTicketMgr->Initialize();

    sLog->outString("Loading GM surveys...");
    sTicketMgr->LoadSurveys();

    sLog->outString("Loading GM Quality Manager...");
    sGMQualityManager->Load();

    sLog->outString("Loading Autobroadcasts...");
    sAutoBroadcast.load();

    ///- Initialize game time and timers
    sLog->outString("Initialize game time and timers");
    m_gameTime = time(NULL);
    m_startTime=m_gameTime;

    tm local;
    time_t curr;
    time(&curr);
    local=*(localtime(&curr));                              // dereference and assign
    char isoDate[128];
    sprintf(isoDate, "%04d-%02d-%02d %02d:%02d:%02d",
        local.tm_year+1900, local.tm_mon+1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);

    LoginDatabase.PExecute("INSERT INTO uptime (realmid, starttime, uptime, revision) VALUES('%u', " UI64FMTD ", 0, '%s%s%s')",
        realmID, uint64(m_startTime), _FULLVERSION_A, git::getProduktVersionStr(), _FULLVERSION_B);

    m_timers[LUPDATE_UPTIME].SetInterval(m_int_configs[CONFIG_UPTIME_UPDATE]*MINUTE*IN_MILLISECONDS);
    m_timers[LUPDATE_PINGDB].SetInterval(getIntConfig(CONFIG_DB_PING_INTERVAL)*MINUTE*IN_MILLISECONDS);    // Mysql ping time in minutes
    m_timers[LUPDATE_RECONNECT].SetInterval((MINUTE*IN_MILLISECONDS)/2);
    m_timers[LUPDATE_BAN].SetInterval(5*MINUTE*IN_MILLISECONDS); // Update expired bans
    m_timers[LUPDATE_AUTOBROADCAST].SetInterval(getIntConfig(CONFIG_AUTOBROADCAST_INTERVAL));

    //m_timers[LUPDATE_RECONNECT].SetInterval((IN_MILLISECONDS));

    //RebaseTimer
    sRebase->InitAutomaticRebase();

    uint32 startupDuration = GetMSTimeDiffToNow(startupBegin);
    sLog->outString();
    sLog->outString("LOGON: Logon initialized in %u minutes %u seconds", (startupDuration / 60000), ((startupDuration % 60000) / 1000) );

    time_t Timestamp; tm *now; Timestamp = time(0); now = localtime(&Timestamp);
    sLog->outString("LOGON: %u:%u:%u", now->tm_hour, now->tm_min, now->tm_sec);
    sLog->outString();
}

void Logon::Update(uint32 diff)
{
    m_updateTime = diff;

    ///- Update the different timers
    for (int i = 0; i < LUPDATE_COUNT; ++i)
    {
        if (m_timers[i].GetCurrent() >= 0)
            m_timers[i].Update(diff);
        else
            m_timers[i].SetCurrent(0);
    }

    ///- Update the game time and check for shutdown time
    _UpdateGameTime();

    ///- Refreshing onlinelist
    if (m_timers[LUPDATE_RECONNECT].Passed())
    {
        m_timers[LUPDATE_RECONNECT].Reset();
        sRoutingHelper->RefreshOnlineStat();

        //30 Sek should be good enough for rebasecheck
        sRebase->UpdateTimers(diff);
    }

    ///- Ping to keep MySQL connections alive
    if (m_timers[LUPDATE_PINGDB].Passed())
    {
        m_timers[LUPDATE_PINGDB].Reset();
        sLog->outDetail("Ping MySQL to keep connection alive");
        CharacterDatabase.KeepAlive();
        LogonDatabase.KeepAlive();
        LoginDatabase.KeepAlive();
        WorldDatabase.KeepAlive();
        LogDatabase.KeepAlive();
    }

    /// <li> Update uptime table
    if (m_timers[LUPDATE_UPTIME].Passed())
    {
        uint32 tmpDiff = uint32(m_gameTime - m_startTime);
        uint32 maxOnlinePlayers = sClientSessionMgr->GetMaxPlayerCount();
        uint32 onlinePlayers = sClientSessionMgr->GetPlayerCount();
        uint32 allyPlayers = sClientSessionMgr->GetAllyCount();
        uint32 hordePlayers = sClientSessionMgr->GetHordeCount();

        m_timers[LUPDATE_UPTIME].Reset();

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_UPTIME_PLAYERS);

        stmt->setUInt64(0, uint64(tmpDiff));
        stmt->setUInt16(1, uint16(maxOnlinePlayers));
        stmt->setUInt32(2, realmID);
        stmt->setUInt64(3, uint64(m_startTime));

        LoginDatabase.Execute(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_STATS);

        stmt->setUInt64(0, uint64(tmpDiff));
        stmt->setUInt16(1, uint16(maxOnlinePlayers));
        stmt->setUInt16(2, uint16(onlinePlayers));
        stmt->setUInt16(3, uint16(allyPlayers));
        stmt->setUInt16(4, uint16(hordePlayers));
        stmt->setInt32(5, realmID+1);

        LoginDatabase.Execute(stmt);
    }

    /// Update expired bans
    if (m_timers[LUPDATE_BAN].Passed())
    {
        // Delete expired ip bans
        LoginDatabase.Execute(LoginDatabase.GetPreparedStatement(LOGIN_DEL_EXPIRED_IP_BANS));
        // Update expired account bans
        LoginDatabase.Execute(LoginDatabase.GetPreparedStatement(LOGIN_UPD_EXPIRED_ACCOUNT_BANS));

        m_timers[LUPDATE_BAN].Reset();
    }

    if (getBoolConfig(CONFIG_AUTOBROADCAST))
    {
        if (m_timers[LUPDATE_AUTOBROADCAST].Passed())
        {
            m_timers[LUPDATE_AUTOBROADCAST].Reset();
            sAutoBroadcast.send();
            //RecordTimeDiff("Autobroadcast");
        }
    }

    sObjectMgr->Update();
    sControlSessionMgr->Update(diff);
    sClientSessionMgr->Update(diff);

    ProcessQueryCallbacks();
    ProcessCliCommands();
}

void Logon::ProcessQueryCallbacks()
{
    PreparedQueryResult result;

    while (!m_realmCharCallbacks.is_empty())
    {
        ACE_Future<PreparedQueryResult> lResult;
        ACE_Time_Value timeout = ACE_Time_Value::zero;
        if (m_realmCharCallbacks.next_readable(lResult, &timeout) != 1)
            break;

        if (lResult.ready())
        {
            lResult.get(result);
            _UpdateRealmCharCount(result);
            lResult.cancel();
        }
    }
}

void Logon::UpdateRealmCharCount(uint32 accountId)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_COUNT);
    stmt->setUInt32(0, accountId);
    PreparedQueryResultFuture result = CharacterDatabase.AsyncQuery(stmt);
    m_realmCharCallbacks.insert(result);
}

void Logon::_UpdateRealmCharCount(PreparedQueryResult resultCharCount)
{
    if (resultCharCount)
    {
        Field* fields = resultCharCount->Fetch();
        uint32 accountId = fields[0].GetUInt32();
        uint32 charCount = fields[1].GetUInt32();

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_REALM_CHARACTERS);
        stmt->setUInt32(0, accountId);
        stmt->setUInt32(1, realmID);
        LoginDatabase.Execute(stmt);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_REALM_CHARACTERS);
        stmt->setUInt32(0, charCount);
        stmt->setUInt32(1, accountId);
        stmt->setUInt32(2, realmID);
        LoginDatabase.Execute(stmt);
    }
}

/// Update the game time
void Logon::_UpdateGameTime()
{
    ///- update the time
    time_t thisTime = time(NULL);
    uint32 elapsed = uint32(thisTime - m_gameTime);
    m_gameTime = thisTime;

    if (!m_haltEvent && m_HaltTimer > 0 && elapsed > 0)
    {
        ///- ... and it is overdue, stop the world (set m_stopEvent)
        if (m_HaltTimer <= elapsed)
        {
            LoginDatabase.DirectPExecute("UPDATE realmlist SET color = 2 WHERE id = '%d'", 2, 1, realmID);
            m_HaltTimer = 0;
            m_haltEvent = true;
        }
        else
        {
            m_HaltTimer -= elapsed;

            sClientSessionMgr->HaltMsg(m_HaltTimer, m_ShutdownMask);
        }
    }

    if (!m_stopEvent && m_ShutdownTimer > 0 && elapsed > 0)
    {
        ///- ... and it is overdue, stop the world (set m_stopEvent)
        if (m_ShutdownTimer <= elapsed)
        {
            if (!(m_ShutdownMask & SHUTDOWN_MASK_IDLE) || sClientSessionMgr->GetActiveAndQueuedSessionCount() == 0)
                m_stopEvent = true;                         // exist code already set
            else
                m_ShutdownTimer = 1;                        // minimum timer value to wait idle state
        }
        ///- ... else decrease it and if necessary display a shutdown countdown to the users
        else
        {
            m_ShutdownTimer -= elapsed;

            sClientSessionMgr->ShutdownMsg(m_ShutdownTimer, m_ShutdownMask);
        }
    }
}

/// Halt the server
void Logon::HaltServ(uint32 time, uint32 options)
{
    m_ShutdownMask = options;
    if (time == 0)  ///- If the halt time is 0, set m_haltEvent
        m_haltEvent = true;
    else            ///- Else set the halt timer and warn users
    {
        m_HaltTimer = time;
        sClientSessionMgr->ShutdownMsg(m_ShutdownTimer, m_ShutdownMask, true);
    }
}

/// Shutdown the server
void Logon::ShutdownServ(uint32 time, uint32 options, uint8 exitcode)
{
    // ignore if server shutdown at next tick
    if (m_stopEvent)
        return;

    m_ShutdownMask = options;
    m_ExitCode = exitcode;

    ///- If the shutdown time is 0, set m_stopEvent (except if shutdown is 'idle' with remaining sessions)
    if (time == 0)
    {
        if (!(options & SHUTDOWN_MASK_IDLE) || sClientSessionMgr->GetActiveAndQueuedSessionCount() == 0)
            m_stopEvent = true;                             // exist code already set
        else
            m_ShutdownTimer = 1;                            //So that the session count is re-evaluated at next world tick
    }
    ///- Else set the shutdown timer and warn users
    else
    {
        m_ShutdownTimer = time;
        sClientSessionMgr->ShutdownMsg(m_ShutdownTimer, m_ShutdownMask, true);
    }
}

/// Cancel a planned server shutdown
void Logon::ShutdownCancel()
{
    // nothing cancel or too later
    if (!m_ShutdownTimer || m_stopEvent)
        return;

    ServerMessageType msgid = (m_ShutdownMask & SHUTDOWN_MASK_RESTART) ? SERVER_MSG_RESTART_CANCELLED : SERVER_MSG_SHUTDOWN_CANCELLED;

    m_ShutdownMask = 0;
    m_ShutdownTimer = 0;
    m_ExitCode = SHUTDOWN_EXIT_CODE;                       // to default value
    sClientSessionMgr->SendServerMessage(msgid);

    sLog->outStaticDebug("Server %s cancelled.",(m_ShutdownMask & SHUTDOWN_MASK_RESTART ? "restart" : "shuttingdown"));
}

void Logon::ProcessCliCommands()
{
    CliCommandHolder::Print* zprint = NULL;
    void* callbackArg = NULL;
    CliCommandHolder* command;
    while (cliCmdQueue.next(command))
    {
        sLog->outDetail("CLI command under processing...");
        zprint = command->m_print;
        callbackArg = command->m_callbackArg;
        CliHandler handler(callbackArg, zprint);
        handler.ParseCommands(command->m_command);
        if(command->m_commandFinished)
            command->m_commandFinished(callbackArg, !handler.HasSentErrorMessage());
        delete command;
    }
}

void Logon::DetectDBCLang()
{
    uint8 m_lang_confid = ConfigMgr::GetIntDefault("DBC.Locale", 255);

    if (m_lang_confid != 255 && m_lang_confid >= TOTAL_LOCALES)
    {
        sLog->outError("Incorrect DBC.Locale! Must be >= 0 and < %d (set to 0)", TOTAL_LOCALES);
        m_lang_confid = LOCALE_enUS;
    }

    ChrRacesEntry const* race = sChrRacesStore.LookupEntry(1);

    std::string availableLocalsStr;

    uint8 default_locale = TOTAL_LOCALES;
    for (uint8 i = default_locale-1; i < TOTAL_LOCALES; --i)  // -1 will be 255 due to uint8
    {
        if (race->name[i][0] != '\0')                     // check by race names
        {
            default_locale = i;
            m_availableDbcLocaleMask |= (1 << i);
            availableLocalsStr += localeNames[i];
            availableLocalsStr += " ";
        }
    }

    if (default_locale != m_lang_confid && m_lang_confid < TOTAL_LOCALES &&
        (m_availableDbcLocaleMask & (1 << m_lang_confid)))
    {
        default_locale = m_lang_confid;
    }

    if (default_locale >= TOTAL_LOCALES)
    {
        sLog->outError("Unable to determine your DBC Locale! (corrupt DBC?)");
        exit(1);
    }

    m_defaultDbcLocale = LocaleConstant(default_locale);

    sLog->outString("Using %s DBC Locale as default. All available DBC locales: %s", localeNames[m_defaultDbcLocale], availableLocalsStr.empty() ? "<none>" : availableLocalsStr.c_str());
    sLog->outString();
}

//LogonState Routines
void Logon::LoadLogonStates()
{
    uint32 oldMSTime = getMSTime();

	QueryResult result = LogonDatabase.PQuery("SELECT entry, value FROM logonstates WHERE realmid = '%u'", realmID);

    if (!result)
    {
        sLog->outString(">> Loaded 0 logon states. DB table `logonstates` is empty!");
        sLog->outString();
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        m_logonstates[fields[0].GetUInt32()] = fields[1].GetUInt32();
        ++count;
    }
    while (result->NextRow());

    sLog->outString(">> Loaded %u logon states in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void Logon::setLogonState(uint32 index, uint64 value)
{
    LogonStatesMap::const_iterator it = m_logonstates.find(index);
    if (it != m_logonstates.end())
    {
        PreparedStatement* stmt = LogonDatabase.GetPreparedStatement(LOGON_UPD_LOGON_STATE);

        stmt->setUInt32(0, uint32(value));
        stmt->setUInt32(1, index);
		stmt->setUInt32(2, realmID);

        LogonDatabase.Execute(stmt);
    }
    else
    {
        PreparedStatement* stmt = LogonDatabase.GetPreparedStatement(LOGON_INS_LOGON_STATE);

		stmt->setUInt32(0, realmID);
        stmt->setUInt32(1, index);
        stmt->setUInt32(2, uint32(value));

        LogonDatabase.Execute(stmt);
    }
    m_logonstates[index] = value;
}

uint64 Logon::getLogonState(uint32 index) const
{
    LogonStatesMap::const_iterator it = m_logonstates.find(index);
    return it != m_logonstates.end() ? it->second : 0;
}

//Bans
/// Ban an account or ban an IP address, duration will be parsed using TimeStringToSecs if it is positive, otherwise permban
BanReturn Logon::BanAccount(BanMode mode, std::string nameOrIP, std::string duration, std::string reason, std::string author)
{
    uint32 duration_secs = TimeStringToSecs(duration);
    PreparedQueryResult resultAccounts = PreparedQueryResult(NULL); //used for kicking
    PreparedStatement* stmt = NULL;

    ///- Update the database with ban information
    switch (mode)
    {
        case BAN_IP:
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BY_IP);
            stmt->setString(0, nameOrIP);
            resultAccounts = LoginDatabase.Query(stmt);
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_IP_BANNED);
            stmt->setString(0, nameOrIP);
            stmt->setUInt32(1, duration_secs);
            stmt->setString(2, author);
            stmt->setString(3, reason);
            LoginDatabase.Execute(stmt);
            break;
        case BAN_ACCOUNT:
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ID_BY_NAME);
            stmt->setString(0, nameOrIP);
            resultAccounts = LoginDatabase.Query(stmt);
            break;
        case BAN_CHARACTER:
            // No SQL injection with prepared statements
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_NAME);
            stmt->setString(0, nameOrIP);
            resultAccounts = CharacterDatabase.Query(stmt);
            break;
        default:
            return BAN_SYNTAX_ERROR;
    }

    if (!resultAccounts)
    {
        if (mode == BAN_IP)
            return BAN_SUCCESS;                             // ip correctly banned but nobody affected (yet)
        else
            return BAN_NOTFOUND;                            // Nobody to ban
    }

    ///- Disconnect all affected players (for IP it can be several)
    SQLTransaction trans = LoginDatabase.BeginTransaction();
    do
    {
        Field* fieldsAccount = resultAccounts->Fetch();
        uint32 account = fieldsAccount[0].GetUInt32();

        if (mode != BAN_IP)
        {
            // make sure there is only one active ban
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_NOT_BANNED);
            stmt->setUInt32(0, account);
            trans->Append(stmt);
            // No SQL injection with prepared statements
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_BANNED);
            stmt->setUInt32(0, account);
            stmt->setUInt32(1, duration_secs);
            stmt->setString(2, author);
            stmt->setString(3, reason);
            trans->Append(stmt);
        }

        if (ClientSession* sess = sClientSessionMgr->FindSession(account))
            if (std::string(sess->GetPlayer()->GetPlayerName()) != author)
                sess->KickPlayer();
    } while (resultAccounts->NextRow());

    LoginDatabase.CommitTransaction(trans);

    return BAN_SUCCESS;
}

/// Ban an account or ban an IP address, duration will be parsed using TimeStringToSecs if it is positive, otherwise permban
BanReturn Logon::BanAccountbyId(uint32 accountId, int32 duration, std::string reason, std::string author)
{
    PreparedStatement* stmt = NULL;

    ///- Disconnect all affected players (for IP it can be several)
    SQLTransaction trans = LoginDatabase.BeginTransaction();
    // make sure there is only one active ban
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_NOT_BANNED);
    stmt->setUInt32(0, accountId);
    trans->Append(stmt);
    // No SQL injection with prepared statements
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_BANNED);
    stmt->setUInt32(0, accountId);
    stmt->setUInt32(1, duration);
    stmt->setString(2, author);
    stmt->setString(3, reason);
    trans->Append(stmt);

    if (ClientSession* sess = sClientSessionMgr->FindSession(accountId))
        if (Player *plr = sess->GetPlayer())
            if (std::string(plr->GetName()) != author)
                sess->KickPlayer();

    LoginDatabase.CommitTransaction(trans);

    return BAN_SUCCESS;
}

/// Remove a ban from an account or IP address
bool Logon::RemoveBanAccount(BanMode mode, std::string nameOrIP)
{
    PreparedStatement* stmt = NULL;
    if (mode == BAN_IP)
    {
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_IP_NOT_BANNED);
        stmt->setString(0, nameOrIP);
        LoginDatabase.Execute(stmt);
    }
    else
    {
        uint32 account = 0;
        if (mode == BAN_ACCOUNT)
            account = AccountMgr::GetId(nameOrIP);
        else if (mode == BAN_CHARACTER)
            account = sObjectMgr->FindPlayerByName(nameOrIP.c_str())->GetSession()->GetAccountId();

        if (!account)
            return false;

        //NO SQL injection as account is uint32
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_NOT_BANNED);
        stmt->setUInt32(0, account);
        LoginDatabase.Execute(stmt);
    }
    return true;
}

/// Ban an account or ban an IP address, duration will be parsed using TimeStringToSecs if it is positive, otherwise permban
BanReturn Logon::BanCharacter(std::string name, std::string duration, std::string reason, std::string author)
{
    Player* pBanned = sObjectMgr->FindPlayerByName(name.c_str());
    uint32 guid = 0;

    uint32 duration_secs = TimeStringToSecs(duration);

    /// Pick a player to ban if not online
    if (!pBanned)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);
        stmt->setString(0, name);
        PreparedQueryResult resultCharacter = CharacterDatabase.Query(stmt);

        if (!resultCharacter)
            return BAN_NOTFOUND;                                    // Nobody to ban

        guid = (*resultCharacter)[0].GetUInt32();
    }
    else
        guid = pBanned->GetGUIDLow();

    // make sure there is only one active ban
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    stmt->setUInt32(1, duration_secs);
    stmt->setString(2, author);
    stmt->setString(3, reason);
    CharacterDatabase.Execute(stmt);

    if (pBanned)
        pBanned->GetSession()->KickPlayer();

    return BAN_SUCCESS;
}

/// Remove a ban from a character
bool Logon::RemoveBanCharacter(std::string name)
{
    Player* pBanned = sObjectMgr->FindPlayerByName(name.c_str());
    uint32 guid = 0;

    /// Pick a player to ban if not online
    if (!pBanned)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);
        stmt->setString(0, name);
        PreparedQueryResult resultCharacter = CharacterDatabase.Query(stmt);

        if (!resultCharacter)
            return false;

        guid = (*resultCharacter)[0].GetUInt32();
    }
    else
        guid = pBanned->GetGUIDLow();

    if (!guid)
        return false;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_BAN);
    stmt->setUInt32(0, guid);
    CharacterDatabase.Execute(stmt);
    return true;
}

void Logon::InitAntiSpam()
{
    TRINITY_GUARD(ACE_Thread_Mutex, _Mutex_);
    _antiSpam.clear();
    QueryResult result = LoginDatabase.Query("SELECT input FROM antispam");

    int count = 0;
    if (!result)
    {
        sLog->outString();
        sLog->outString(">> Loaded %u descriptions ", count);
        return;
    }

    do
    {
        Field *fields = result->Fetch();

        InsertSpamMsg(fields[0].GetString());
        ++count;

    }
    while (result->NextRow());

    _antiSpamReplace[0xc2a1] = 'i';
    _antiSpamReplace[0xc2a2] = 'c';
    _antiSpamReplace[0xc2a5] = 'y';
    _antiSpamReplace[0xc2a6] = 'i';
    _antiSpamReplace[0xc380] = 'a';
    _antiSpamReplace[0xc381] = 'a';
    _antiSpamReplace[0xc382] = 'a';
    _antiSpamReplace[0xc383] = 'a';
    _antiSpamReplace[0xc384] = 'a';
    _antiSpamReplace[0xc385] = 'a';
    _antiSpamReplace[0xc386] = 'a';
    _antiSpamReplace[0xc387] = 'c';
    _antiSpamReplace[0xc388] = 'e';
    _antiSpamReplace[0xc389] = 'e';
    _antiSpamReplace[0xc38a] = 'e';
    _antiSpamReplace[0xc38b] = 'e';
    _antiSpamReplace[0xc38c] = 'i';
    _antiSpamReplace[0xc38d] = 'i';
    _antiSpamReplace[0xc38e] = 'i';
    _antiSpamReplace[0xc38f] = 'i';
    _antiSpamReplace[0xc390] = 'd';
    _antiSpamReplace[0xc391] = 'n';
    _antiSpamReplace[0xc392] = 'o';
    _antiSpamReplace[0xc393] = 'o';
    _antiSpamReplace[0xc394] = 'o';
    _antiSpamReplace[0xc395] = 'o';
    _antiSpamReplace[0xc396] = 'o';
    _antiSpamReplace[0xc397] = 'x';
    _antiSpamReplace[0xc398] = 'o';
    _antiSpamReplace[0xc399] = 'u';
    _antiSpamReplace[0xc39a] = 'u';
    _antiSpamReplace[0xc39b] = 'u';
    _antiSpamReplace[0xc39c] = 'u';
    _antiSpamReplace[0xc39d] = 'y';
    _antiSpamReplace[0xc39e] = 'p';
    _antiSpamReplace[0xc39f] = 's';
    _antiSpamReplace[0xc3a0] = 'a';
    _antiSpamReplace[0xc3a1] = 'a';
    _antiSpamReplace[0xc3a2] = 'a';
    _antiSpamReplace[0xc3a3] = 'a';
    _antiSpamReplace[0xc3a4] = 'a';
    _antiSpamReplace[0xc3a5] = 'a';
    _antiSpamReplace[0xc3a6] = 'a';
    _antiSpamReplace[0xc3a7] = 'c';
    _antiSpamReplace[0xc3a8] = 'e';
    _antiSpamReplace[0xc3a9] = 'e';
    _antiSpamReplace[0xc3aa] = 'e';
    _antiSpamReplace[0xc3ab] = 'e';
    _antiSpamReplace[0xc3ac] = 'i';
    _antiSpamReplace[0xc3ad] = 'i';
    _antiSpamReplace[0xc3ae] = 'i';
    _antiSpamReplace[0xc3af] = 'i';
    _antiSpamReplace[0xc3b0] = 'd';
    _antiSpamReplace[0xc3b1] = 'n';
    _antiSpamReplace[0xc3b2] = 'o';
    _antiSpamReplace[0xc3b3] = 'o';
    _antiSpamReplace[0xc3b4] = 'o';
    _antiSpamReplace[0xc3b5] = 'o';
    _antiSpamReplace[0xc3b6] = 'o';
    _antiSpamReplace[0xc3b8] = 'o';
    _antiSpamReplace[0xc3b9] = 'u';
    _antiSpamReplace[0xc3ba] = 'u';
    _antiSpamReplace[0xc3bb] = 'u';
    _antiSpamReplace[0xc3bc] = 'u';
    _antiSpamReplace[0xc3bd] = 'y';
    _antiSpamReplace[0xc3be] = 'p';
    _antiSpamReplace[0xc3bf] = 'y';
}

char Logon::GetReplaceChar(uint16 c) const
{
    std::unordered_map<int, char>::const_iterator itr = _antiSpamReplace.find(c);

    if (itr == _antiSpamReplace.end())
        return c;

    return itr->second;
}

std::string Logon::NormalizeForAntiSpam(std::string str) const
{
    std::string normalized;

    uint16 special = 0;

    for (std::string::iterator itr = str.begin(); itr != str.end(); ++itr)
    {
        uint8 c = 0xFF & (*itr);

        if (c < 0x80 || special)
        {
            char newchar = GetReplaceChar(special | c);
            special = 0;

            if (isalnum(newchar))
                normalized += toupper(newchar);
        }
        else
            special = c << 8;
    }

    return normalized;
}

void Logon::InsertSpamMsg(std::string str)
{
    _antiSpam.insert(NormalizeForAntiSpam(str));
}

bool Logon::CheckForSpam(std::string data) const
{
    std::string str = NormalizeForAntiSpam(data);

    for (AntiSpam::iterator itr = _antiSpam.begin(); itr != _antiSpam.end(); ++itr)
    {
        std::size_t found = str.find(*itr);
        if (found != std::string::npos)
            return true;
    }

    return false;
}

void Logon::SetFakeMsg(const std::string& motd)
{
    _fakemsg = motd;
}

const char* Logon::GetFakeMsg() const
{
    return _fakemsg.c_str();
}

void Logon::Whisper(const std::string& text, uint32 language, Player* player)
{
    if (!player)
        return;
    WorldPacket data(SMSG_MESSAGECHAT, 200);
    data << uint8(CHAT_MSG_WHISPER_INFORM);
    data << uint32(language);
    data << uint64(657);
    data << uint32(language);                               //language 2.1.0 ?
    data << uint64(657);
    data << uint32(text.length() + 1);
    data << text;
    data << uint8(0);
    player->GetSession()->SendPacket(&data);
}
