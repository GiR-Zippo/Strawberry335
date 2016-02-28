/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table 335Chars.account_data
DROP TABLE IF EXISTS `account_data`;
CREATE TABLE IF NOT EXISTS `account_data` (
  `accountId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `data` blob NOT NULL,
  PRIMARY KEY (`accountId`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.account_data: ~0 rows (approximately)
/*!40000 ALTER TABLE `account_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_data` ENABLE KEYS */;


-- Dumping structure for table 335Chars.account_instance_times
DROP TABLE IF EXISTS `account_instance_times`;
CREATE TABLE IF NOT EXISTS `account_instance_times` (
  `accountId` int(10) unsigned NOT NULL,
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0',
  `releaseTime` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`accountId`,`instanceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.account_instance_times: ~0 rows (approximately)
/*!40000 ALTER TABLE `account_instance_times` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_instance_times` ENABLE KEYS */;


-- Dumping structure for table 335Chars.account_tutorial
DROP TABLE IF EXISTS `account_tutorial`;
CREATE TABLE IF NOT EXISTS `account_tutorial` (
  `accountId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `tut0` int(10) unsigned NOT NULL DEFAULT '0',
  `tut1` int(10) unsigned NOT NULL DEFAULT '0',
  `tut2` int(10) unsigned NOT NULL DEFAULT '0',
  `tut3` int(10) unsigned NOT NULL DEFAULT '0',
  `tut4` int(10) unsigned NOT NULL DEFAULT '0',
  `tut5` int(10) unsigned NOT NULL DEFAULT '0',
  `tut6` int(10) unsigned NOT NULL DEFAULT '0',
  `tut7` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`accountId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.account_tutorial: ~0 rows (approximately)
/*!40000 ALTER TABLE `account_tutorial` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_tutorial` ENABLE KEYS */;


-- Dumping structure for table 335Chars.addons
DROP TABLE IF EXISTS `addons`;
CREATE TABLE IF NOT EXISTS `addons` (
  `name` varchar(120) NOT NULL DEFAULT '',
  `crc` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Addons';

-- Dumping data for table 335Chars.addons: ~23 rows (approximately)
/*!40000 ALTER TABLE `addons` DISABLE KEYS */;
INSERT INTO `addons` (`name`, `crc`) VALUES
        ('Blizzard_AchievementUI', 1276933997),
        ('Blizzard_ArenaUI', 1276933997),
        ('Blizzard_AuctionUI', 1276933997),
        ('Blizzard_BarbershopUI', 1276933997),
        ('Blizzard_BattlefieldMinimap', 1276933997),
        ('Blizzard_BindingUI', 1276933997),
        ('Blizzard_Calendar', 1276933997),
        ('Blizzard_CombatLog', 1276933997),
        ('Blizzard_CombatText', 1276933997),
        ('Blizzard_DebugTools', 1276933997),
        ('Blizzard_GlyphUI', 1276933997),
        ('Blizzard_GMChatUI', 1276933997),
        ('Blizzard_GMSurveyUI', 1276933997),
        ('Blizzard_GuildBankUI', 1276933997),
        ('Blizzard_InspectUI', 1276933997),
        ('Blizzard_ItemSocketingUI', 1276933997),
        ('Blizzard_MacroUI', 1276933997),
        ('Blizzard_RaidUI', 1276933997),
        ('Blizzard_TalentUI', 1276933997),
        ('Blizzard_TimeManager', 1276933997),
        ('Blizzard_TokenUI', 1276933997),
        ('Blizzard_TradeSkillUI', 1276933997),
        ('Blizzard_TrainerUI', 1276933997);
/*!40000 ALTER TABLE `addons` ENABLE KEYS */;


-- Dumping structure for table 335Chars.arena_team
DROP TABLE IF EXISTS `arena_team`;
CREATE TABLE IF NOT EXISTS `arena_team` (
  `arenaTeamId` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(24) NOT NULL,
  `captainGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `rating` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weekGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weekWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `backgroundColor` int(10) unsigned NOT NULL DEFAULT '0',
  `emblemStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `emblemColor` int(10) unsigned NOT NULL DEFAULT '0',
  `borderStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `borderColor` int(10) unsigned NOT NULL DEFAULT '0',
  `inactivity` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`arenaTeamId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.arena_team: ~0 rows (approximately)
/*!40000 ALTER TABLE `arena_team` DISABLE KEYS */;
/*!40000 ALTER TABLE `arena_team` ENABLE KEYS */;


-- Dumping structure for table 335Chars.arena_team_member
DROP TABLE IF EXISTS `arena_team_member`;
CREATE TABLE IF NOT EXISTS `arena_team_member` (
  `arenaTeamId` int(10) unsigned NOT NULL DEFAULT '0',
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `weekGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weekWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `personalRating` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`arenaTeamId`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.arena_team_member: ~0 rows (approximately)
/*!40000 ALTER TABLE `arena_team_member` DISABLE KEYS */;
/*!40000 ALTER TABLE `arena_team_member` ENABLE KEYS */;


-- Dumping structure for table 335Chars.arena_team_member_end
DROP TABLE IF EXISTS `arena_team_member_end`;
CREATE TABLE IF NOT EXISTS `arena_team_member_end` (
  `arenaTeamId` int(10) NOT NULL DEFAULT '0',
  `guid` int(10) NOT NULL DEFAULT '0',
  `weekGames` smallint(5) DEFAULT NULL,
  `weekWins` smallint(5) DEFAULT NULL,
  `seasonGames` smallint(5) DEFAULT NULL,
  `seasonWins` smallint(5) DEFAULT NULL,
  `personalRating` smallint(5) DEFAULT NULL,
  `type` smallint(5) DEFAULT NULL,
  PRIMARY KEY (`arenaTeamId`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.arena_team_member_end: ~0 rows (approximately)
/*!40000 ALTER TABLE `arena_team_member_end` DISABLE KEYS */;
/*!40000 ALTER TABLE `arena_team_member_end` ENABLE KEYS */;


-- Dumping structure for table 335Chars.arena_team_member_start
DROP TABLE IF EXISTS `arena_team_member_start`;
CREATE TABLE IF NOT EXISTS `arena_team_member_start` (
  `arenaTeamId` int(10) NOT NULL DEFAULT '0',
  `guid` int(10) NOT NULL DEFAULT '0',
  `weekGames` smallint(5) DEFAULT NULL,
  `weekWins` smallint(5) DEFAULT NULL,
  `seasonGames` smallint(5) DEFAULT NULL,
  `seasonWins` smallint(5) DEFAULT NULL,
  `personalRating` smallint(5) DEFAULT NULL,
  `type` smallint(5) DEFAULT NULL,
  PRIMARY KEY (`arenaTeamId`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.arena_team_member_start: ~0 rows (approximately)
/*!40000 ALTER TABLE `arena_team_member_start` DISABLE KEYS */;
/*!40000 ALTER TABLE `arena_team_member_start` ENABLE KEYS */;


-- Dumping structure for table 335Chars.armory_character_stats
DROP TABLE IF EXISTS `armory_character_stats`;
CREATE TABLE IF NOT EXISTS `armory_character_stats` (
  `guid` int(11) NOT NULL,
  `data` longtext NOT NULL,
  `save_date` int(11) NOT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='World of Warcraft Armory table';

-- Dumping data for table 335Chars.armory_character_stats: ~0 rows (approximately)
/*!40000 ALTER TABLE `armory_character_stats` DISABLE KEYS */;
/*!40000 ALTER TABLE `armory_character_stats` ENABLE KEYS */;


-- Dumping structure for table 335Chars.armory_game_chart
DROP TABLE IF EXISTS `armory_game_chart`;
CREATE TABLE IF NOT EXISTS `armory_game_chart` (
  `gameid` int(11) NOT NULL,
  `teamid` int(11) NOT NULL,
  `guid` int(11) NOT NULL,
  `changeType` int(11) NOT NULL,
  `ratingChange` int(11) NOT NULL,
  `teamRating` int(11) NOT NULL,
  `damageDone` int(11) NOT NULL,
  `deaths` int(11) NOT NULL,
  `healingDone` int(11) NOT NULL,
  `damageTaken` int(11) NOT NULL,
  `healingTaken` int(11) NOT NULL,
  `killingBlows` int(11) NOT NULL,
  `mapId` int(11) NOT NULL,
  `start` int(11) NOT NULL,
  `end` int(11) NOT NULL,
  PRIMARY KEY (`gameid`,`teamid`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='WoWArmory Game Chart';

-- Dumping data for table 335Chars.armory_game_chart: ~0 rows (approximately)
/*!40000 ALTER TABLE `armory_game_chart` DISABLE KEYS */;
/*!40000 ALTER TABLE `armory_game_chart` ENABLE KEYS */;


-- Dumping structure for table 335Chars.auctionhouse
DROP TABLE IF EXISTS `auctionhouse`;
CREATE TABLE IF NOT EXISTS `auctionhouse` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `auctioneerguid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemguid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemowner` int(10) unsigned NOT NULL DEFAULT '0',
  `buyoutprice` int(10) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `buyguid` int(10) unsigned NOT NULL DEFAULT '0',
  `lastbid` int(10) unsigned NOT NULL DEFAULT '0',
  `startbid` int(10) unsigned NOT NULL DEFAULT '0',
  `deposit` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `item_guid` (`itemguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.auctionhouse: ~0 rows (approximately)
/*!40000 ALTER TABLE `auctionhouse` DISABLE KEYS */;
/*!40000 ALTER TABLE `auctionhouse` ENABLE KEYS */;


-- Dumping structure for table 335Chars.bg_status
DROP TABLE IF EXISTS `bg_status`;
CREATE TABLE IF NOT EXISTS `bg_status` (
  `guid` int(255) NOT NULL,
  `name` int(255) NOT NULL,
  `team` int(255) NOT NULL,
  `type` int(255) NOT NULL,
  `levelrange` int(255) NOT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.bg_status: ~0 rows (approximately)
/*!40000 ALTER TABLE `bg_status` DISABLE KEYS */;
/*!40000 ALTER TABLE `bg_status` ENABLE KEYS */;


-- Dumping structure for table 335Chars.bugreport
DROP TABLE IF EXISTS `bugreport`;
CREATE TABLE IF NOT EXISTS `bugreport` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `type` longtext NOT NULL,
  `content` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Debug System';

-- Dumping data for table 335Chars.bugreport: ~0 rows (approximately)
/*!40000 ALTER TABLE `bugreport` DISABLE KEYS */;
/*!40000 ALTER TABLE `bugreport` ENABLE KEYS */;


-- Dumping structure for table 335Chars.calendar_events
DROP TABLE IF EXISTS `calendar_events`;
CREATE TABLE IF NOT EXISTS `calendar_events` (
  `id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `creator` int(10) unsigned NOT NULL DEFAULT '0',
  `title` varchar(255) COLLATE utf8_bin NOT NULL DEFAULT '',
  `description` varchar(255) COLLATE utf8_bin NOT NULL DEFAULT '',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '4',
  `dungeon` int(10) NOT NULL DEFAULT '-1',
  `eventtime` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `time2` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- Dumping data for table 335Chars.calendar_events: ~0 rows (approximately)
/*!40000 ALTER TABLE `calendar_events` DISABLE KEYS */;
/*!40000 ALTER TABLE `calendar_events` ENABLE KEYS */;


-- Dumping structure for table 335Chars.calendar_invites
DROP TABLE IF EXISTS `calendar_invites`;
CREATE TABLE IF NOT EXISTS `calendar_invites` (
  `id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `event` bigint(20) unsigned NOT NULL DEFAULT '0',
  `invitee` int(10) unsigned NOT NULL DEFAULT '0',
  `sender` int(10) unsigned NOT NULL DEFAULT '0',
  `status` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `statustime` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `text` varchar(255) COLLATE utf8_bin NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- Dumping data for table 335Chars.calendar_invites: ~0 rows (approximately)
/*!40000 ALTER TABLE `calendar_invites` DISABLE KEYS */;
/*!40000 ALTER TABLE `calendar_invites` ENABLE KEYS */;


-- Dumping structure for table 335Chars.challenge_group
DROP TABLE IF EXISTS `challenge_group`;
CREATE TABLE IF NOT EXISTS `challenge_group` (
  `groupid` int(11) NOT NULL AUTO_INCREMENT,
  `challengeid` int(11) NOT NULL DEFAULT '0',
  `timeNeeded` bigint(20) NOT NULL,
  PRIMARY KEY (`groupid`,`challengeid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.challenge_group: ~0 rows (approximately)
/*!40000 ALTER TABLE `challenge_group` DISABLE KEYS */;
/*!40000 ALTER TABLE `challenge_group` ENABLE KEYS */;


-- Dumping structure for table 335Chars.challenge_group_chars
DROP TABLE IF EXISTS `challenge_group_chars`;
CREATE TABLE IF NOT EXISTS `challenge_group_chars` (
  `groupid` int(11) NOT NULL,
  `guid` int(11) NOT NULL,
  PRIMARY KEY (`groupid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.challenge_group_chars: ~0 rows (approximately)
/*!40000 ALTER TABLE `challenge_group_chars` DISABLE KEYS */;
/*!40000 ALTER TABLE `challenge_group_chars` ENABLE KEYS */;


-- Dumping structure for table 335Chars.channels
DROP TABLE IF EXISTS `channels`;
CREATE TABLE IF NOT EXISTS `channels` (
  `name` varchar(128) NOT NULL,
  `team` int(10) unsigned NOT NULL,
  `announce` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `ownership` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `password` varchar(32) DEFAULT NULL,
  `bannedList` text,
  `lastUsed` int(10) unsigned NOT NULL,
  PRIMARY KEY (`name`,`team`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Channel System';

-- Dumping data for table 335Chars.channels: ~0 rows (approximately)
/*!40000 ALTER TABLE `channels` DISABLE KEYS */;
/*!40000 ALTER TABLE `channels` ENABLE KEYS */;


-- Dumping structure for table 335Chars.characters
DROP TABLE IF EXISTS `characters`;
CREATE TABLE IF NOT EXISTS `characters` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `account` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `name` varchar(12) NOT NULL DEFAULT '',
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `xp` int(10) unsigned NOT NULL DEFAULT '0',
  `money` int(10) unsigned NOT NULL DEFAULT '0',
  `playerBytes` int(10) unsigned NOT NULL DEFAULT '0',
  `playerBytes2` int(10) unsigned NOT NULL DEFAULT '0',
  `playerFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `instance_id` int(10) unsigned NOT NULL DEFAULT '0',
  `instance_mode_mask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `taximask` text NOT NULL,
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `cinematic` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `totaltime` int(10) unsigned NOT NULL DEFAULT '0',
  `leveltime` int(10) unsigned NOT NULL DEFAULT '0',
  `logout_time` int(10) unsigned NOT NULL DEFAULT '0',
  `is_logout_resting` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `rest_bonus` float NOT NULL DEFAULT '0',
  `resettalents_cost` int(10) unsigned NOT NULL DEFAULT '0',
  `resettalents_time` int(10) unsigned NOT NULL DEFAULT '0',
  `trans_x` float NOT NULL DEFAULT '0',
  `trans_y` float NOT NULL DEFAULT '0',
  `trans_z` float NOT NULL DEFAULT '0',
  `trans_o` float NOT NULL DEFAULT '0',
  `transguid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `extra_flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `stable_slots` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `at_login` smallint(5) unsigned NOT NULL DEFAULT '0',
  `zone` smallint(5) unsigned NOT NULL DEFAULT '0',
  `death_expire_time` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_path` text,
  `arenaPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `totalHonorPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `todayHonorPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `yesterdayHonorPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `totalKills` int(10) unsigned NOT NULL DEFAULT '0',
  `todayKills` smallint(5) unsigned NOT NULL DEFAULT '0',
  `yesterdayKills` smallint(5) unsigned NOT NULL DEFAULT '0',
  `chosenTitle` int(10) unsigned NOT NULL DEFAULT '0',
  `knownCurrencies` bigint(20) unsigned NOT NULL DEFAULT '0',
  `watchedFaction` int(10) unsigned NOT NULL DEFAULT '0',
  `drunk` smallint(5) unsigned NOT NULL DEFAULT '0',
  `health` int(10) unsigned NOT NULL DEFAULT '0',
  `power1` int(10) unsigned NOT NULL DEFAULT '0',
  `power2` int(10) unsigned NOT NULL DEFAULT '0',
  `power3` int(10) unsigned NOT NULL DEFAULT '0',
  `power4` int(10) unsigned NOT NULL DEFAULT '0',
  `power5` int(10) unsigned NOT NULL DEFAULT '0',
  `power6` int(10) unsigned NOT NULL DEFAULT '0',
  `power7` int(10) unsigned NOT NULL DEFAULT '0',
  `latency` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `speccount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `activespec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `exploredZones` longtext,
  `equipmentCache` longtext,
  `ammoId` int(10) unsigned NOT NULL DEFAULT '0',
  `knownTitles` longtext,
  `actionBars` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `grantableLevels` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `deleteInfos_Account` int(10) unsigned DEFAULT NULL,
  `deleteInfos_Name` varchar(12) DEFAULT NULL,
  `deleteDate` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`guid`),
  KEY `idx_account` (`account`),
  KEY `idx_online` (`online`),
  KEY `idx_name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.characters: ~0 rows (approximately)
/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;


-- Dumping structure for table 335Chars.characters_addon
DROP TABLE IF EXISTS `characters_addon`;
CREATE TABLE IF NOT EXISTS `characters_addon` (
  `guid` int(10) DEFAULT NULL,
  `Name` text,
  `PW` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.characters_addon: ~0 rows (approximately)
/*!40000 ALTER TABLE `characters_addon` DISABLE KEYS */;
/*!40000 ALTER TABLE `characters_addon` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_account_data
DROP TABLE IF EXISTS `character_account_data`;
CREATE TABLE IF NOT EXISTS `character_account_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `data` blob NOT NULL,
  PRIMARY KEY (`guid`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_account_data: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_account_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_account_data` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_achievement
DROP TABLE IF EXISTS `character_achievement`;
CREATE TABLE IF NOT EXISTS `character_achievement` (
  `guid` int(10) unsigned NOT NULL,
  `achievement` smallint(5) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_achievement: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_achievement` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_achievement` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_achievement_progress
DROP TABLE IF EXISTS `character_achievement_progress`;
CREATE TABLE IF NOT EXISTS `character_achievement_progress` (
  `guid` int(10) unsigned NOT NULL,
  `criteria` smallint(5) unsigned NOT NULL,
  `counter` int(10) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`criteria`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_achievement_progress: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_achievement_progress` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_achievement_progress` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_action
DROP TABLE IF EXISTS `character_action`;
CREATE TABLE IF NOT EXISTS `character_action` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `button` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `action` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spec`,`button`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_action: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_action` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_action` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_arena_stats
DROP TABLE IF EXISTS `character_arena_stats`;
CREATE TABLE IF NOT EXISTS `character_arena_stats` (
  `guid` int(10) NOT NULL,
  `slot` tinyint(3) NOT NULL,
  `matchMakerRating` smallint(5) NOT NULL,
  PRIMARY KEY (`guid`,`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_arena_stats: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_arena_stats` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_arena_stats` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_aura
DROP TABLE IF EXISTS `character_aura`;
CREATE TABLE IF NOT EXISTS `character_aura` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Full Global Unique Identifier',
  `item_guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `effect_mask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `recalculate_mask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `stackcount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `amount0` int(11) NOT NULL DEFAULT '0',
  `amount1` int(11) NOT NULL DEFAULT '0',
  `amount2` int(11) NOT NULL DEFAULT '0',
  `base_amount0` int(11) NOT NULL DEFAULT '0',
  `base_amount1` int(11) NOT NULL DEFAULT '0',
  `base_amount2` int(11) NOT NULL DEFAULT '0',
  `maxduration` int(11) NOT NULL DEFAULT '0',
  `remaintime` int(11) NOT NULL DEFAULT '0',
  `remaincharges` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`,`effect_mask`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_aura: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_aura` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_aura` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_banned
DROP TABLE IF EXISTS `character_banned`;
CREATE TABLE IF NOT EXISTS `character_banned` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `bandate` int(10) unsigned NOT NULL DEFAULT '0',
  `unbandate` int(10) unsigned NOT NULL DEFAULT '0',
  `bannedby` varchar(50) NOT NULL,
  `banreason` varchar(255) NOT NULL,
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`guid`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Ban List';

-- Dumping data for table 335Chars.character_banned: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_banned` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_banned` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_battleground_data
DROP TABLE IF EXISTS `character_battleground_data`;
CREATE TABLE IF NOT EXISTS `character_battleground_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `instanceId` int(10) unsigned NOT NULL COMMENT 'Instance Identifier',
  `team` smallint(5) unsigned NOT NULL,
  `joinX` float NOT NULL DEFAULT '0',
  `joinY` float NOT NULL DEFAULT '0',
  `joinZ` float NOT NULL DEFAULT '0',
  `joinO` float NOT NULL DEFAULT '0',
  `joinMapId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `taxiStart` int(10) unsigned NOT NULL DEFAULT '0',
  `taxiEnd` int(10) unsigned NOT NULL DEFAULT '0',
  `mountSpell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_battleground_data: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_battleground_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_battleground_data` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_battleground_random
DROP TABLE IF EXISTS `character_battleground_random`;
CREATE TABLE IF NOT EXISTS `character_battleground_random` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_battleground_random: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_battleground_random` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_battleground_random` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_declinedname
DROP TABLE IF EXISTS `character_declinedname`;
CREATE TABLE IF NOT EXISTS `character_declinedname` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `genitive` varchar(15) NOT NULL DEFAULT '',
  `dative` varchar(15) NOT NULL DEFAULT '',
  `accusative` varchar(15) NOT NULL DEFAULT '',
  `instrumental` varchar(15) NOT NULL DEFAULT '',
  `prepositional` varchar(15) NOT NULL DEFAULT '',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_declinedname: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_declinedname` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_declinedname` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_equipmentsets
DROP TABLE IF EXISTS `character_equipmentsets`;
CREATE TABLE IF NOT EXISTS `character_equipmentsets` (
  `guid` int(10) NOT NULL DEFAULT '0',
  `setguid` bigint(20) NOT NULL AUTO_INCREMENT,
  `setindex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `name` varchar(31) NOT NULL,
  `iconname` varchar(100) NOT NULL,
  `item0` int(10) unsigned NOT NULL DEFAULT '0',
  `item1` int(10) unsigned NOT NULL DEFAULT '0',
  `item2` int(10) unsigned NOT NULL DEFAULT '0',
  `item3` int(10) unsigned NOT NULL DEFAULT '0',
  `item4` int(10) unsigned NOT NULL DEFAULT '0',
  `item5` int(10) unsigned NOT NULL DEFAULT '0',
  `item6` int(10) unsigned NOT NULL DEFAULT '0',
  `item7` int(10) unsigned NOT NULL DEFAULT '0',
  `item8` int(10) unsigned NOT NULL DEFAULT '0',
  `item9` int(10) unsigned NOT NULL DEFAULT '0',
  `item10` int(10) unsigned NOT NULL DEFAULT '0',
  `item11` int(10) unsigned NOT NULL DEFAULT '0',
  `item12` int(10) unsigned NOT NULL DEFAULT '0',
  `item13` int(10) unsigned NOT NULL DEFAULT '0',
  `item14` int(10) unsigned NOT NULL DEFAULT '0',
  `item15` int(10) unsigned NOT NULL DEFAULT '0',
  `item16` int(10) unsigned NOT NULL DEFAULT '0',
  `item17` int(10) unsigned NOT NULL DEFAULT '0',
  `item18` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`setguid`),
  UNIQUE KEY `idx_set` (`guid`,`setguid`,`setindex`),
  KEY `Idx_setindex` (`setindex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_equipmentsets: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_equipmentsets` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_equipmentsets` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_feed_log
DROP TABLE IF EXISTS `character_feed_log`;
CREATE TABLE IF NOT EXISTS `character_feed_log` (
  `guid` int(11) NOT NULL,
  `type` smallint(1) NOT NULL,
  `data` int(11) NOT NULL,
  `date` int(11) DEFAULT NULL,
  `counter` int(11) NOT NULL,
  `difficulty` smallint(6) DEFAULT '-1',
  `item_quality` smallint(6) NOT NULL,
  `item_guid` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_feed_log: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_feed_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_feed_log` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_gifts
DROP TABLE IF EXISTS `character_gifts`;
CREATE TABLE IF NOT EXISTS `character_gifts` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `item_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`item_guid`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_gifts: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_gifts` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_gifts` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_glyphs
DROP TABLE IF EXISTS `character_glyphs`;
CREATE TABLE IF NOT EXISTS `character_glyphs` (
  `guid` int(10) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `glyph1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `glyph2` smallint(5) unsigned DEFAULT '0',
  `glyph3` smallint(5) unsigned DEFAULT '0',
  `glyph4` smallint(5) unsigned DEFAULT '0',
  `glyph5` smallint(5) unsigned DEFAULT '0',
  `glyph6` smallint(5) unsigned DEFAULT '0',
  PRIMARY KEY (`guid`,`spec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_glyphs: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_glyphs` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_glyphs` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_homebind
DROP TABLE IF EXISTS `character_homebind`;
CREATE TABLE IF NOT EXISTS `character_homebind` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `zoneId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Zone Identifier',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_homebind: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_homebind` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_homebind` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_instance
DROP TABLE IF EXISTS `character_instance`;
CREATE TABLE IF NOT EXISTS `character_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `instance` int(10) unsigned NOT NULL DEFAULT '0',
  `permanent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `extended` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_instance: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_instance` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_instance_swap
DROP TABLE IF EXISTS `character_instance_swap`;
CREATE TABLE IF NOT EXISTS `character_instance_swap` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `instance` int(10) unsigned NOT NULL DEFAULT '0',
  `permanent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_instance_swap: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_instance_swap` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_instance_swap` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_inventory
DROP TABLE IF EXISTS `character_inventory`;
CREATE TABLE IF NOT EXISTS `character_inventory` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `bag` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Item Global Unique Identifier',
  PRIMARY KEY (`item`),
  UNIQUE KEY `guid` (`guid`,`bag`,`slot`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_inventory: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_inventory` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_joincount
DROP TABLE IF EXISTS `character_joincount`;
CREATE TABLE IF NOT EXISTS `character_joincount` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `count` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_joincount: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_joincount` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_joincount` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_kill
DROP TABLE IF EXISTS `character_kill`;
CREATE TABLE IF NOT EXISTS `character_kill` (
  `test` int(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_kill: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_kill` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_kill` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_npcbot
DROP TABLE IF EXISTS `character_npcbot`;
CREATE TABLE IF NOT EXISTS `character_npcbot` (
  `owner` int(11) DEFAULT NULL,
  `entry` int(11) DEFAULT NULL,
  `race` tinyint(4) DEFAULT NULL,
  `class` tinyint(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_npcbot: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_npcbot` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_npcbot` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_pet
DROP TABLE IF EXISTS `character_pet`;
CREATE TABLE IF NOT EXISTS `character_pet` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `owner` int(10) unsigned NOT NULL DEFAULT '0',
  `modelid` int(10) unsigned DEFAULT '0',
  `CreatedBySpell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `PetType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` smallint(5) unsigned NOT NULL DEFAULT '1',
  `exp` int(10) unsigned NOT NULL DEFAULT '0',
  `Reactstate` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `name` varchar(21) NOT NULL DEFAULT 'Pet',
  `renamed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `curhealth` int(10) unsigned NOT NULL DEFAULT '1',
  `curmana` int(10) unsigned NOT NULL DEFAULT '0',
  `curhappiness` int(10) unsigned NOT NULL DEFAULT '0',
  `savetime` int(10) unsigned NOT NULL DEFAULT '0',
  `abdata` text,
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`),
  KEY `idx_slot` (`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

-- Dumping data for table 335Chars.character_pet: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_pet` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_pet` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_pet_declinedname
DROP TABLE IF EXISTS `character_pet_declinedname`;
CREATE TABLE IF NOT EXISTS `character_pet_declinedname` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `owner` int(10) unsigned NOT NULL DEFAULT '0',
  `genitive` varchar(12) NOT NULL DEFAULT '',
  `dative` varchar(12) NOT NULL DEFAULT '',
  `accusative` varchar(12) NOT NULL DEFAULT '',
  `instrumental` varchar(12) NOT NULL DEFAULT '',
  `prepositional` varchar(12) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `owner_key` (`owner`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_pet_declinedname: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_pet_declinedname` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_pet_declinedname` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_queststatus
DROP TABLE IF EXISTS `character_queststatus`;
CREATE TABLE IF NOT EXISTS `character_queststatus` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  `status` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `explored` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `timer` int(10) unsigned NOT NULL DEFAULT '0',
  `mobcount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `mobcount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `mobcount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `mobcount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `itemcount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `itemcount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `itemcount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `itemcount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `playercount` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_queststatus: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_queststatus` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_queststatus_daily
DROP TABLE IF EXISTS `character_queststatus_daily`;
CREATE TABLE IF NOT EXISTS `character_queststatus_daily` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_queststatus_daily: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_queststatus_daily` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus_daily` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_queststatus_rewarded
DROP TABLE IF EXISTS `character_queststatus_rewarded`;
CREATE TABLE IF NOT EXISTS `character_queststatus_rewarded` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  `time` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_queststatus_rewarded: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_queststatus_rewarded` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus_rewarded` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_queststatus_rewarded_pl
DROP TABLE IF EXISTS `character_queststatus_rewarded_pl`;
CREATE TABLE IF NOT EXISTS `character_queststatus_rewarded_pl` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_queststatus_rewarded_pl: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_queststatus_rewarded_pl` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus_rewarded_pl` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_queststatus_seasonal
DROP TABLE IF EXISTS `character_queststatus_seasonal`;
CREATE TABLE IF NOT EXISTS `character_queststatus_seasonal` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  `event` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Event Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_queststatus_seasonal: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_queststatus_seasonal` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus_seasonal` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_queststatus_weekly
DROP TABLE IF EXISTS `character_queststatus_weekly`;
CREATE TABLE IF NOT EXISTS `character_queststatus_weekly` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_queststatus_weekly: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_queststatus_weekly` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_queststatus_weekly` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_rate_override
DROP TABLE IF EXISTS `character_rate_override`;
CREATE TABLE IF NOT EXISTS `character_rate_override` (
  `guid` int(10) NOT NULL,
  `rate` tinyint(3) NOT NULL,
  `value` int(10) NOT NULL,
  UNIQUE KEY `rate` (`guid`,`rate`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.character_rate_override: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_rate_override` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_rate_override` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_reputation
DROP TABLE IF EXISTS `character_reputation`;
CREATE TABLE IF NOT EXISTS `character_reputation` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `faction` smallint(5) unsigned NOT NULL DEFAULT '0',
  `standing` int(11) NOT NULL DEFAULT '0',
  `flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`faction`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_reputation: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_reputation` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_reputation` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_skills
DROP TABLE IF EXISTS `character_skills`;
CREATE TABLE IF NOT EXISTS `character_skills` (
  `guid` int(10) unsigned NOT NULL COMMENT 'Global Unique Identifier',
  `skill` smallint(5) unsigned NOT NULL,
  `value` smallint(5) unsigned NOT NULL,
  `max` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`skill`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_skills: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_skills` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_social
DROP TABLE IF EXISTS `character_social`;
CREATE TABLE IF NOT EXISTS `character_social` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `friend` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Friend Global Unique Identifier',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Friend Flags',
  `note` varchar(48) NOT NULL DEFAULT '' COMMENT 'Friend Note',
  PRIMARY KEY (`guid`,`friend`,`flags`),
  KEY `friend` (`friend`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_social: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_social` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_social` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_spell
DROP TABLE IF EXISTS `character_spell`;
CREATE TABLE IF NOT EXISTS `character_spell` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `disabled` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.character_spell: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_spell` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_spell` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_spell_cooldown
DROP TABLE IF EXISTS `character_spell_cooldown`;
CREATE TABLE IF NOT EXISTS `character_spell_cooldown` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `item` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Item Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_spell_cooldown: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_spell_cooldown` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_spell_cooldown` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_stats
DROP TABLE IF EXISTS `character_stats`;
CREATE TABLE IF NOT EXISTS `character_stats` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `maxhealth` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower1` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower2` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower3` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower4` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower5` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower6` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower7` int(10) unsigned NOT NULL DEFAULT '0',
  `strength` int(10) unsigned NOT NULL DEFAULT '0',
  `agility` int(10) unsigned NOT NULL DEFAULT '0',
  `stamina` int(10) unsigned NOT NULL DEFAULT '0',
  `intellect` int(10) unsigned NOT NULL DEFAULT '0',
  `spirit` int(10) unsigned NOT NULL DEFAULT '0',
  `armor` int(10) unsigned NOT NULL DEFAULT '0',
  `resHoly` int(10) unsigned NOT NULL DEFAULT '0',
  `resFire` int(10) unsigned NOT NULL DEFAULT '0',
  `resNature` int(10) unsigned NOT NULL DEFAULT '0',
  `resFrost` int(10) unsigned NOT NULL DEFAULT '0',
  `resShadow` int(10) unsigned NOT NULL DEFAULT '0',
  `resArcane` int(10) unsigned NOT NULL DEFAULT '0',
  `blockPct` float unsigned NOT NULL DEFAULT '0',
  `dodgePct` float unsigned NOT NULL DEFAULT '0',
  `parryPct` float unsigned NOT NULL DEFAULT '0',
  `critPct` float unsigned NOT NULL DEFAULT '0',
  `rangedCritPct` float unsigned NOT NULL DEFAULT '0',
  `spellCritPct` float unsigned NOT NULL DEFAULT '0',
  `attackPower` int(10) unsigned NOT NULL DEFAULT '0',
  `rangedAttackPower` int(10) unsigned NOT NULL DEFAULT '0',
  `spellPower` int(10) unsigned NOT NULL DEFAULT '0',
  `resilience` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_stats: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_stats` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_stats` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_survey_votes
DROP TABLE IF EXISTS `character_survey_votes`;
CREATE TABLE IF NOT EXISTS `character_survey_votes` (
  `account` int(11) DEFAULT NULL,
  `survey` int(11) DEFAULT NULL,
  `option` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.character_survey_votes: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_survey_votes` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_survey_votes` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_talent
DROP TABLE IF EXISTS `character_talent`;
CREATE TABLE IF NOT EXISTS `character_talent` (
  `guid` int(10) unsigned NOT NULL,
  `spell` mediumint(8) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`,`spec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.character_talent: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_talent` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_talent` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_transfer
DROP TABLE IF EXISTS `character_transfer`;
CREATE TABLE IF NOT EXISTS `character_transfer` (
  `acc_id` int(12) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `char_id` int(12) NOT NULL,
  `ip` varchar(30) NOT NULL DEFAULT '0.0.0.0',
  `time` int(10) NOT NULL,
  `payed` bit(1) NOT NULL DEFAULT b'0',
  PRIMARY KEY (`acc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='character_transfer';

-- Dumping data for table 335Chars.character_transfer: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_transfer` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_transfer` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_transmog_active
DROP TABLE IF EXISTS `character_transmog_active`;
CREATE TABLE IF NOT EXISTS `character_transmog_active` (
  `guid` int(10) unsigned NOT NULL,
  `item_guid` int(10) unsigned NOT NULL,
  `transmog_id` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`guid`,`item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.character_transmog_active: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_transmog_active` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_transmog_active` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_transmog_history
DROP TABLE IF EXISTS `character_transmog_history`;
CREATE TABLE IF NOT EXISTS `character_transmog_history` (
  `guid` int(10) unsigned NOT NULL,
  `transmog_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`transmog_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.character_transmog_history: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_transmog_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_transmog_history` ENABLE KEYS */;


-- Dumping structure for table 335Chars.character_transmog_unlocked
DROP TABLE IF EXISTS `character_transmog_unlocked`;
CREATE TABLE IF NOT EXISTS `character_transmog_unlocked` (
  `guid` int(10) unsigned NOT NULL,
  `transmog_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`transmog_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.character_transmog_unlocked: ~0 rows (approximately)
/*!40000 ALTER TABLE `character_transmog_unlocked` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_transmog_unlocked` ENABLE KEYS */;


-- Dumping structure for table 335Chars.cheaters
DROP TABLE IF EXISTS `cheaters`;
CREATE TABLE IF NOT EXISTS `cheaters` (
  `entry` bigint(20) NOT NULL AUTO_INCREMENT,
  `player` varchar(255) NOT NULL,
  `acctid` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL DEFAULT 'unknown',
  `speed` float NOT NULL DEFAULT '0',
  `Val1` float NOT NULL DEFAULT '0',
  `Val2` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(11) NOT NULL DEFAULT '0',
  `Map` smallint(5) NOT NULL DEFAULT '-1',
  `Pos` varchar(255) NOT NULL DEFAULT '0',
  `Level` mediumint(9) NOT NULL DEFAULT '0',
  `first_date` datetime NOT NULL,
  `last_date` datetime NOT NULL,
  `Op` varchar(255) NOT NULL DEFAULT 'unknown',
  PRIMARY KEY (`entry`),
  KEY `idx_Count` (`count`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.cheaters: ~0 rows (approximately)
/*!40000 ALTER TABLE `cheaters` DISABLE KEYS */;
/*!40000 ALTER TABLE `cheaters` ENABLE KEYS */;


-- Dumping structure for table 335Chars.corpse
DROP TABLE IF EXISTS `corpse`;
CREATE TABLE IF NOT EXISTS `corpse` (
  `corpseGuid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `phaseMask` smallint(5) unsigned NOT NULL DEFAULT '1',
  `displayId` int(10) unsigned NOT NULL DEFAULT '0',
  `itemCache` text NOT NULL,
  `bytes1` int(10) unsigned NOT NULL DEFAULT '0',
  `bytes2` int(10) unsigned NOT NULL DEFAULT '0',
  `guildId` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `dynFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `corpseType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  PRIMARY KEY (`corpseGuid`),
  KEY `idx_type` (`corpseType`),
  KEY `idx_instance` (`instanceId`),
  KEY `idx_player` (`guid`),
  KEY `idx_time` (`time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Death System';

-- Dumping data for table 335Chars.corpse: ~0 rows (approximately)
/*!40000 ALTER TABLE `corpse` DISABLE KEYS */;
/*!40000 ALTER TABLE `corpse` ENABLE KEYS */;


-- Dumping structure for table 335Chars.creature_respawn
DROP TABLE IF EXISTS `creature_respawn`;
CREATE TABLE IF NOT EXISTS `creature_respawn` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `respawnTime` int(10) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(10) unsigned NOT NULL DEFAULT '0',
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  PRIMARY KEY (`guid`,`instanceId`),
  KEY `idx_instance` (`instanceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Grid Loading System';

-- Dumping data for table 335Chars.creature_respawn: ~0 rows (approximately)
/*!40000 ALTER TABLE `creature_respawn` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_respawn` ENABLE KEYS */;


-- Dumping structure for table 335Chars.gameobject_respawn
DROP TABLE IF EXISTS `gameobject_respawn`;
CREATE TABLE IF NOT EXISTS `gameobject_respawn` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `respawnTime` int(10) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(10) unsigned NOT NULL DEFAULT '0',
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  PRIMARY KEY (`guid`,`instanceId`),
  KEY `idx_instance` (`instanceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Grid Loading System';

-- Dumping data for table 335Chars.gameobject_respawn: ~0 rows (approximately)
/*!40000 ALTER TABLE `gameobject_respawn` DISABLE KEYS */;
/*!40000 ALTER TABLE `gameobject_respawn` ENABLE KEYS */;


-- Dumping structure for table 335Chars.game_event_condition_save
DROP TABLE IF EXISTS `game_event_condition_save`;
CREATE TABLE IF NOT EXISTS `game_event_condition_save` (
  `eventEntry` tinyint(3) unsigned NOT NULL,
  `condition_id` int(10) unsigned NOT NULL DEFAULT '0',
  `done` float DEFAULT '0',
  PRIMARY KEY (`eventEntry`,`condition_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.game_event_condition_save: ~0 rows (approximately)
/*!40000 ALTER TABLE `game_event_condition_save` DISABLE KEYS */;
/*!40000 ALTER TABLE `game_event_condition_save` ENABLE KEYS */;


-- Dumping structure for table 335Chars.game_event_save
DROP TABLE IF EXISTS `game_event_save`;
CREATE TABLE IF NOT EXISTS `game_event_save` (
  `eventEntry` tinyint(3) unsigned NOT NULL,
  `state` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `next_start` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`eventEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.game_event_save: ~0 rows (approximately)
/*!40000 ALTER TABLE `game_event_save` DISABLE KEYS */;
/*!40000 ALTER TABLE `game_event_save` ENABLE KEYS */;


-- Dumping structure for table 335Chars.gm_subsurveys
DROP TABLE IF EXISTS `gm_subsurveys`;
CREATE TABLE IF NOT EXISTS `gm_subsurveys` (
  `surveyId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `subsurveyId` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `comment` text NOT NULL,
  PRIMARY KEY (`surveyId`,`subsurveyId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.gm_subsurveys: ~0 rows (approximately)
/*!40000 ALTER TABLE `gm_subsurveys` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_subsurveys` ENABLE KEYS */;


-- Dumping structure for table 335Chars.gm_surveys
DROP TABLE IF EXISTS `gm_surveys`;
CREATE TABLE IF NOT EXISTS `gm_surveys` (
  `surveyId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `mainSurvey` int(10) unsigned NOT NULL DEFAULT '0',
  `overallComment` longtext NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`surveyId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.gm_surveys: ~0 rows (approximately)
/*!40000 ALTER TABLE `gm_surveys` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_surveys` ENABLE KEYS */;


-- Dumping structure for table 335Chars.gm_tickets
DROP TABLE IF EXISTS `gm_tickets`;
CREATE TABLE IF NOT EXISTS `gm_tickets` (
  `ticketId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier of ticket creator',
  `name` varchar(12) NOT NULL COMMENT 'Name of ticket creator',
  `message` text NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `lastModifiedTime` int(10) unsigned NOT NULL DEFAULT '0',
  `closedBy` int(10) NOT NULL DEFAULT '0',
  `assignedTo` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'GUID of admin to whom ticket is assigned',
  `comment` text NOT NULL,
  `completed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `escalated` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `viewed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ticketId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.gm_tickets: ~0 rows (approximately)
/*!40000 ALTER TABLE `gm_tickets` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_tickets` ENABLE KEYS */;


-- Dumping structure for table 335Chars.groups
DROP TABLE IF EXISTS `groups`;
CREATE TABLE IF NOT EXISTS `groups` (
  `guid` int(10) unsigned NOT NULL,
  `leaderGuid` int(10) unsigned NOT NULL,
  `lootMethod` tinyint(3) unsigned NOT NULL,
  `looterGuid` int(10) unsigned NOT NULL,
  `lootThreshold` tinyint(3) unsigned NOT NULL,
  `icon1` int(10) unsigned NOT NULL,
  `icon2` int(10) unsigned NOT NULL,
  `icon3` int(10) unsigned NOT NULL,
  `icon4` int(10) unsigned NOT NULL,
  `icon5` int(10) unsigned NOT NULL,
  `icon6` int(10) unsigned NOT NULL,
  `icon7` int(10) unsigned NOT NULL,
  `icon8` int(10) unsigned NOT NULL,
  `groupType` tinyint(3) unsigned NOT NULL,
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `raiddifficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`),
  KEY `leaderGuid` (`leaderGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Groups';

-- Dumping data for table 335Chars.groups: ~0 rows (approximately)
/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;


-- Dumping structure for table 335Chars.group_instance
DROP TABLE IF EXISTS `group_instance`;
CREATE TABLE IF NOT EXISTS `group_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `instance` int(10) unsigned NOT NULL DEFAULT '0',
  `permanent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`guid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.group_instance: ~0 rows (approximately)
/*!40000 ALTER TABLE `group_instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_instance` ENABLE KEYS */;


-- Dumping structure for table 335Chars.group_member
DROP TABLE IF EXISTS `group_member`;
CREATE TABLE IF NOT EXISTS `group_member` (
  `guid` int(10) unsigned NOT NULL,
  `memberGuid` int(10) unsigned NOT NULL,
  `memberFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `subgroup` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `roles` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`memberGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Groups';

-- Dumping data for table 335Chars.group_member: ~0 rows (approximately)
/*!40000 ALTER TABLE `group_member` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_member` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild
DROP TABLE IF EXISTS `guild`;
CREATE TABLE IF NOT EXISTS `guild` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(24) NOT NULL DEFAULT '',
  `leaderguid` int(10) unsigned NOT NULL DEFAULT '0',
  `EmblemStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EmblemColor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BorderStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BorderColor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BackgroundColor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `info` text NOT NULL,
  `motd` varchar(128) NOT NULL DEFAULT '',
  `createdate` int(10) unsigned NOT NULL DEFAULT '0',
  `BankMoney` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Dumping data for table 335Chars.guild: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild_bank_eventlog
DROP TABLE IF EXISTS `guild_bank_eventlog`;
CREATE TABLE IF NOT EXISTS `guild_bank_eventlog` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Log record identificator - auxiliary column',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Guild bank TabId',
  `EventType` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Event type',
  `PlayerGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemOrMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemStackCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DestTabId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Destination Tab Id',
  `TimeStamp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`,`LogGuid`,`TabId`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_PlayerGuid` (`PlayerGuid`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.guild_bank_eventlog: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_bank_eventlog` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_bank_eventlog` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild_bank_item
DROP TABLE IF EXISTS `guild_bank_item`;
CREATE TABLE IF NOT EXISTS `guild_bank_item` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SlotId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item_guid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`TabId`,`SlotId`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_item_guid` (`item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.guild_bank_item: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_bank_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_bank_item` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild_bank_right
DROP TABLE IF EXISTS `guild_bank_right`;
CREATE TABLE IF NOT EXISTS `guild_bank_right` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `rid` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `gbright` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SlotPerDay` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`TabId`,`rid`),
  KEY `guildid_key` (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.guild_bank_right: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_bank_right` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_bank_right` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild_bank_tab
DROP TABLE IF EXISTS `guild_bank_tab`;
CREATE TABLE IF NOT EXISTS `guild_bank_tab` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TabName` varchar(16) NOT NULL DEFAULT '',
  `TabIcon` varchar(100) NOT NULL DEFAULT '',
  `TabText` varchar(500) DEFAULT NULL,
  PRIMARY KEY (`guildid`,`TabId`),
  KEY `guildid_key` (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.guild_bank_tab: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_bank_tab` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_bank_tab` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild_eventlog
DROP TABLE IF EXISTS `guild_eventlog`;
CREATE TABLE IF NOT EXISTS `guild_eventlog` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL COMMENT 'Log record identificator - auxiliary column',
  `EventType` tinyint(3) unsigned NOT NULL COMMENT 'Event type',
  `PlayerGuid1` int(10) unsigned NOT NULL COMMENT 'Player 1',
  `PlayerGuid2` int(10) unsigned NOT NULL COMMENT 'Player 2',
  `NewRank` tinyint(3) unsigned NOT NULL COMMENT 'New rank(in case promotion/demotion)',
  `TimeStamp` int(10) unsigned NOT NULL COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`,`LogGuid`),
  KEY `Idx_PlayerGuid1` (`PlayerGuid1`),
  KEY `Idx_PlayerGuid2` (`PlayerGuid2`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild Eventlog';

-- Dumping data for table 335Chars.guild_eventlog: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_eventlog` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_eventlog` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild_member
DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE IF NOT EXISTS `guild_member` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `guid` int(10) unsigned NOT NULL,
  `rank` tinyint(3) unsigned NOT NULL,
  `pnote` varchar(31) NOT NULL DEFAULT '',
  `offnote` varchar(31) NOT NULL DEFAULT '',
  `BankResetTimeMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab0` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab0` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab1` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab1` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab2` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab2` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab3` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab3` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab4` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab4` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab5` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab5` int(10) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `guid_key` (`guid`),
  KEY `guildid_key` (`guildid`),
  KEY `guildid_rank_key` (`guildid`,`rank`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Dumping data for table 335Chars.guild_member: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_member` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_member` ENABLE KEYS */;


-- Dumping structure for table 335Chars.guild_rank
DROP TABLE IF EXISTS `guild_rank`;
CREATE TABLE IF NOT EXISTS `guild_rank` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `rid` tinyint(3) unsigned NOT NULL,
  `rname` varchar(20) NOT NULL DEFAULT '',
  `rights` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BankMoneyPerDay` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`rid`),
  KEY `Idx_rid` (`rid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Dumping data for table 335Chars.guild_rank: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_rank` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_rank` ENABLE KEYS */;


-- Dumping structure for table 335Chars.honor
DROP TABLE IF EXISTS `honor`;
CREATE TABLE IF NOT EXISTS `honor` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `guid` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '0',
  `kills` int(11) NOT NULL DEFAULT '0',
  `honor` int(11) NOT NULL DEFAULT '0',
  `gender` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.honor: ~0 rows (approximately)
/*!40000 ALTER TABLE `honor` DISABLE KEYS */;
/*!40000 ALTER TABLE `honor` ENABLE KEYS */;


-- Dumping structure for table 335Chars.instance
DROP TABLE IF EXISTS `instance`;
CREATE TABLE IF NOT EXISTS `instance` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0',
  `resettime` int(10) unsigned NOT NULL DEFAULT '0',
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `completedEncounters` int(10) unsigned NOT NULL DEFAULT '0',
  `data` tinytext NOT NULL,
  `extended` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `map` (`map`),
  KEY `resettime` (`resettime`),
  KEY `difficulty` (`difficulty`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.instance: ~0 rows (approximately)
/*!40000 ALTER TABLE `instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `instance` ENABLE KEYS */;


-- Dumping structure for table 335Chars.instance_reset
DROP TABLE IF EXISTS `instance_reset`;
CREATE TABLE IF NOT EXISTS `instance_reset` (
  `mapid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `resettime` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`mapid`,`difficulty`),
  KEY `difficulty` (`difficulty`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.instance_reset: ~0 rows (approximately)
/*!40000 ALTER TABLE `instance_reset` DISABLE KEYS */;
/*!40000 ALTER TABLE `instance_reset` ENABLE KEYS */;


-- Dumping structure for table 335Chars.item_instance
DROP TABLE IF EXISTS `item_instance`;
CREATE TABLE IF NOT EXISTS `item_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemEntry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `owner_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `creatorGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `giftCreatorGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '1',
  `duration` int(10) NOT NULL DEFAULT '0',
  `charges` tinytext,
  `flags` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `enchantments` text NOT NULL,
  `randomPropertyId` smallint(5) NOT NULL DEFAULT '0',
  `durability` smallint(5) unsigned NOT NULL DEFAULT '0',
  `playedTime` int(10) unsigned NOT NULL DEFAULT '0',
  `text` text,
  PRIMARY KEY (`guid`),
  KEY `idx_owner_guid` (`owner_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item System';

-- Dumping data for table 335Chars.item_instance: ~0 rows (approximately)
/*!40000 ALTER TABLE `item_instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_instance` ENABLE KEYS */;


-- Dumping structure for table 335Chars.item_loot
DROP TABLE IF EXISTS `item_loot`;
CREATE TABLE IF NOT EXISTS `item_loot` (
  `guid` int(11) NOT NULL,
  `gold` int(11) NOT NULL,
  `item_entries` text,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.item_loot: ~0 rows (approximately)
/*!40000 ALTER TABLE `item_loot` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_loot` ENABLE KEYS */;


-- Dumping structure for table 335Chars.item_refund_instance
DROP TABLE IF EXISTS `item_refund_instance`;
CREATE TABLE IF NOT EXISTS `item_refund_instance` (
  `item_guid` int(10) unsigned NOT NULL COMMENT 'Item GUID',
  `player_guid` int(10) unsigned NOT NULL COMMENT 'Player GUID',
  `paidMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `paidExtendedCost` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`item_guid`,`player_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item Refund System';

-- Dumping data for table 335Chars.item_refund_instance: ~0 rows (approximately)
/*!40000 ALTER TABLE `item_refund_instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_refund_instance` ENABLE KEYS */;


-- Dumping structure for table 335Chars.item_soulbound_trade_data
DROP TABLE IF EXISTS `item_soulbound_trade_data`;
CREATE TABLE IF NOT EXISTS `item_soulbound_trade_data` (
  `itemGuid` int(10) unsigned NOT NULL COMMENT 'Item GUID',
  `allowedPlayers` text NOT NULL COMMENT 'Space separated GUID list of players who can receive this item in trade',
  PRIMARY KEY (`itemGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item Refund System';

-- Dumping data for table 335Chars.item_soulbound_trade_data: ~0 rows (approximately)
/*!40000 ALTER TABLE `item_soulbound_trade_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_soulbound_trade_data` ENABLE KEYS */;


-- Dumping structure for table 335Chars.item_text
DROP TABLE IF EXISTS `item_text`;
CREATE TABLE IF NOT EXISTS `item_text` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `text` longtext,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Item System';

-- Dumping data for table 335Chars.item_text: ~0 rows (approximately)
/*!40000 ALTER TABLE `item_text` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_text` ENABLE KEYS */;


-- Dumping structure for table 335Chars.jail
DROP TABLE IF EXISTS `jail`;
CREATE TABLE IF NOT EXISTS `jail` (
  `guid` int(11) unsigned NOT NULL COMMENT 'GUID of the jail brother',
  `char` varchar(13) NOT NULL COMMENT 'Jailed charname',
  `release` int(11) unsigned NOT NULL COMMENT 'Release time for the char',
  `amnestietime` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL COMMENT 'Reason for the jail',
  `times` int(11) unsigned NOT NULL COMMENT 'How many times this char already was jailed',
  `gmacc` int(11) unsigned NOT NULL COMMENT 'Used GM acc to jail this char',
  `gmchar` varchar(13) NOT NULL COMMENT 'Used GM char to jail this char',
  `lasttime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP COMMENT 'Last time jailed',
  `duration` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Duration of the jail',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Jail table for MaNGOS by WarHead';

-- Dumping data for table 335Chars.jail: ~0 rows (approximately)
/*!40000 ALTER TABLE `jail` DISABLE KEYS */;
/*!40000 ALTER TABLE `jail` ENABLE KEYS */;


-- Dumping structure for table 335Chars.jail_conf
DROP TABLE IF EXISTS `jail_conf`;
CREATE TABLE IF NOT EXISTS `jail_conf` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `obt` varchar(50) DEFAULT NULL,
  `jail_conf` int(11) DEFAULT NULL,
  `jail_tele` float DEFAULT NULL,
  `help_ger` varchar(255) CHARACTER SET latin1 DEFAULT '',
  `help_enq` varchar(255) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.jail_conf: ~0 rows (approximately)
/*!40000 ALTER TABLE `jail_conf` DISABLE KEYS */;
/*!40000 ALTER TABLE `jail_conf` ENABLE KEYS */;


-- Dumping structure for table 335Chars.lag_reports
DROP TABLE IF EXISTS `lag_reports`;
CREATE TABLE IF NOT EXISTS `lag_reports` (
  `reportId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `lagType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `latency` int(10) unsigned NOT NULL DEFAULT '0',
  `createTime` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`reportId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Dumping data for table 335Chars.lag_reports: ~0 rows (approximately)
/*!40000 ALTER TABLE `lag_reports` DISABLE KEYS */;
/*!40000 ALTER TABLE `lag_reports` ENABLE KEYS */;


-- Dumping structure for table 335Chars.lfg_data
DROP TABLE IF EXISTS `lfg_data`;
CREATE TABLE IF NOT EXISTS `lfg_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `dungeon` int(10) unsigned NOT NULL DEFAULT '0',
  `state` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LFG Data';

-- Dumping data for table 335Chars.lfg_data: ~0 rows (approximately)
/*!40000 ALTER TABLE `lfg_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `lfg_data` ENABLE KEYS */;


-- Dumping structure for table 335Chars.mail
DROP TABLE IF EXISTS `mail`;
CREATE TABLE IF NOT EXISTS `mail` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Identifier',
  `messageType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `stationery` tinyint(3) NOT NULL DEFAULT '41',
  `mailTemplateId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `sender` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `receiver` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `subject` longtext,
  `body` longtext,
  `has_items` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `expire_time` int(10) unsigned NOT NULL DEFAULT '0',
  `deliver_time` int(10) unsigned NOT NULL DEFAULT '0',
  `money` int(10) unsigned NOT NULL DEFAULT '0',
  `cod` int(10) unsigned NOT NULL DEFAULT '0',
  `checked` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `idx_receiver` (`receiver`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mail System';

-- Dumping data for table 335Chars.mail: ~0 rows (approximately)
/*!40000 ALTER TABLE `mail` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail` ENABLE KEYS */;


-- Dumping structure for table 335Chars.mail_items
DROP TABLE IF EXISTS `mail_items`;
CREATE TABLE IF NOT EXISTS `mail_items` (
  `mail_id` int(10) unsigned NOT NULL DEFAULT '0',
  `item_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `receiver` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  PRIMARY KEY (`item_guid`),
  KEY `idx_receiver` (`receiver`),
  KEY `idx_mail_id` (`mail_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Dumping data for table 335Chars.mail_items: ~0 rows (approximately)
/*!40000 ALTER TABLE `mail_items` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail_items` ENABLE KEYS */;


-- Dumping structure for table 335Chars.mm_account
DROP TABLE IF EXISTS `mm_account`;
CREATE TABLE IF NOT EXISTS `mm_account` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `username` varchar(32) NOT NULL DEFAULT '',
  `sha_pass_hash` varchar(40) NOT NULL DEFAULT '',
  `sessionkey` longtext,
  `v` longtext,
  `s` longtext,
  `email` text,
  `joindate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_ip` varchar(30) NOT NULL DEFAULT '127.0.0.1',
  `failed_logins` int(11) unsigned NOT NULL DEFAULT '0',
  `locked` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `last_login` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `online` tinyint(4) NOT NULL DEFAULT '0',
  `expansion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mutetime` bigint(40) unsigned NOT NULL DEFAULT '0',
  `locale` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `authkey` varchar(40) DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Account System';

-- Dumping data for table 335Chars.mm_account: ~0 rows (approximately)
/*!40000 ALTER TABLE `mm_account` DISABLE KEYS */;
/*!40000 ALTER TABLE `mm_account` ENABLE KEYS */;


-- Dumping structure for table 335Chars.mm_forum_posts
DROP TABLE IF EXISTS `mm_forum_posts`;
CREATE TABLE IF NOT EXISTS `mm_forum_posts` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `authorid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `authorname` varchar(16) NOT NULL DEFAULT '',
  `forum` bigint(20) unsigned NOT NULL DEFAULT '0',
  `topic` bigint(20) unsigned NOT NULL DEFAULT '0',
  `lastpost` bigint(20) unsigned NOT NULL DEFAULT '0',
  `name` varchar(50) NOT NULL DEFAULT '',
  `text` longtext,
  `time` varchar(255) NOT NULL,
  `annouced` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `sticked` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `closed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Dumping data for table 335Chars.mm_forum_posts: ~0 rows (approximately)
/*!40000 ALTER TABLE `mm_forum_posts` DISABLE KEYS */;
/*!40000 ALTER TABLE `mm_forum_posts` ENABLE KEYS */;


-- Dumping structure for table 335Chars.mm_motd
DROP TABLE IF EXISTS `mm_motd`;
CREATE TABLE IF NOT EXISTS `mm_motd` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `realmid` int(11) NOT NULL,
  `type` longtext NOT NULL,
  `content` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='MOTD System';

-- Dumping data for table 335Chars.mm_motd: ~0 rows (approximately)
/*!40000 ALTER TABLE `mm_motd` DISABLE KEYS */;
/*!40000 ALTER TABLE `mm_motd` ENABLE KEYS */;


-- Dumping structure for table 335Chars.mm_point_system_invites
DROP TABLE IF EXISTS `mm_point_system_invites`;
CREATE TABLE IF NOT EXISTS `mm_point_system_invites` (
  `entry` int(11) NOT NULL AUTO_INCREMENT,
  `PlayersAccount` char(50) DEFAULT NULL,
  `InvitedBy` char(50) DEFAULT NULL,
  `InviterAccount` char(50) DEFAULT NULL,
  `Treated` int(1) NOT NULL DEFAULT '0',
  `Rewarded` int(1) NOT NULL DEFAULT '0',
  UNIQUE KEY `entry` (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.mm_point_system_invites: ~0 rows (approximately)
/*!40000 ALTER TABLE `mm_point_system_invites` DISABLE KEYS */;
/*!40000 ALTER TABLE `mm_point_system_invites` ENABLE KEYS */;


-- Dumping structure for table 335Chars.old_gm_chat_log
DROP TABLE IF EXISTS `old_gm_chat_log`;
CREATE TABLE IF NOT EXISTS `old_gm_chat_log` (
  `timestamp` int(11) NOT NULL,
  `gm_acc_guid` int(10) NOT NULL,
  `gm_acc_name` varchar(255) NOT NULL,
  `securityLevel` tinyint(4) NOT NULL,
  `chat_type` tinyint(4) NOT NULL,
  `receiver` varchar(255) NOT NULL,
  `text` text NOT NULL,
  PRIMARY KEY (`timestamp`,`gm_acc_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.old_gm_chat_log: ~0 rows (approximately)
/*!40000 ALTER TABLE `old_gm_chat_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `old_gm_chat_log` ENABLE KEYS */;


-- Dumping structure for table 335Chars.old_gm_log
DROP TABLE IF EXISTS `old_gm_log`;
CREATE TABLE IF NOT EXISTS `old_gm_log` (
  `ticketId` int(10) unsigned NOT NULL,
  `gm_acc_guid` int(10) unsigned NOT NULL,
  `gm_acc_name` varchar(12) CHARACTER SET utf8 NOT NULL,
  `securityLevel` tinyint(3) unsigned NOT NULL,
  `char_acc_guid` int(10) unsigned NOT NULL,
  `char_acc_name` varchar(12) CHARACTER SET utf8 NOT NULL,
  `gm_text` text CHARACTER SET utf8,
  `char_text` text CHARACTER SET utf8,
  PRIMARY KEY (`ticketId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.old_gm_log: ~0 rows (approximately)
/*!40000 ALTER TABLE `old_gm_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `old_gm_log` ENABLE KEYS */;


-- Dumping structure for table 335Chars.old_gm_ticket_log
DROP TABLE IF EXISTS `old_gm_ticket_log`;
CREATE TABLE IF NOT EXISTS `old_gm_ticket_log` (
  `ticketId` int(10) unsigned NOT NULL,
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier of ticket creator',
  `name` varchar(12) CHARACTER SET utf8 NOT NULL COMMENT 'Name of ticket creator',
  `message` text CHARACTER SET utf8 NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `lastModifiedTime` int(10) unsigned NOT NULL DEFAULT '0',
  `closedBy` int(10) NOT NULL DEFAULT '0',
  `assignedTo` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'GUID of admin to whom ticket is assigned',
  `comment` text CHARACTER SET utf8,
  `completed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `escalated` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `viewed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ticketId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Chars.old_gm_ticket_log: ~0 rows (approximately)
/*!40000 ALTER TABLE `old_gm_ticket_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `old_gm_ticket_log` ENABLE KEYS */;


-- Dumping structure for table 335Chars.petition
DROP TABLE IF EXISTS `petition`;
CREATE TABLE IF NOT EXISTS `petition` (
  `ownerguid` int(10) unsigned NOT NULL,
  `petitionguid` int(10) unsigned DEFAULT '0',
  `name` varchar(24) NOT NULL,
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ownerguid`,`type`),
  UNIQUE KEY `index_ownerguid_petitionguid` (`ownerguid`,`petitionguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Dumping data for table 335Chars.petition: ~0 rows (approximately)
/*!40000 ALTER TABLE `petition` DISABLE KEYS */;
/*!40000 ALTER TABLE `petition` ENABLE KEYS */;


-- Dumping structure for table 335Chars.petition_sign
DROP TABLE IF EXISTS `petition_sign`;
CREATE TABLE IF NOT EXISTS `petition_sign` (
  `ownerguid` int(10) unsigned NOT NULL,
  `petitionguid` int(10) unsigned NOT NULL DEFAULT '0',
  `playerguid` int(10) unsigned NOT NULL DEFAULT '0',
  `player_account` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`petitionguid`,`playerguid`),
  KEY `Idx_playerguid` (`playerguid`),
  KEY `Idx_ownerguid` (`ownerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Dumping data for table 335Chars.petition_sign: ~0 rows (approximately)
/*!40000 ALTER TABLE `petition_sign` DISABLE KEYS */;
/*!40000 ALTER TABLE `petition_sign` ENABLE KEYS */;


-- Dumping structure for table 335Chars.pet_aura
DROP TABLE IF EXISTS `pet_aura`;
CREATE TABLE IF NOT EXISTS `pet_aura` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Full Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `effect_mask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `recalculate_mask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `stackcount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `amount0` mediumint(8) NOT NULL,
  `amount1` mediumint(8) NOT NULL,
  `amount2` mediumint(8) NOT NULL,
  `base_amount0` mediumint(8) NOT NULL,
  `base_amount1` mediumint(8) NOT NULL,
  `base_amount2` mediumint(8) NOT NULL,
  `maxduration` int(11) NOT NULL DEFAULT '0',
  `remaintime` int(11) NOT NULL DEFAULT '0',
  `remaincharges` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`,`effect_mask`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

-- Dumping data for table 335Chars.pet_aura: ~0 rows (approximately)
/*!40000 ALTER TABLE `pet_aura` DISABLE KEYS */;
/*!40000 ALTER TABLE `pet_aura` ENABLE KEYS */;


-- Dumping structure for table 335Chars.pet_spell
DROP TABLE IF EXISTS `pet_spell`;
CREATE TABLE IF NOT EXISTS `pet_spell` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `active` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

-- Dumping data for table 335Chars.pet_spell: ~0 rows (approximately)
/*!40000 ALTER TABLE `pet_spell` DISABLE KEYS */;
/*!40000 ALTER TABLE `pet_spell` ENABLE KEYS */;


-- Dumping structure for table 335Chars.pet_spell_cooldown
DROP TABLE IF EXISTS `pet_spell_cooldown`;
CREATE TABLE IF NOT EXISTS `pet_spell_cooldown` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.pet_spell_cooldown: ~0 rows (approximately)
/*!40000 ALTER TABLE `pet_spell_cooldown` DISABLE KEYS */;
/*!40000 ALTER TABLE `pet_spell_cooldown` ENABLE KEYS */;


-- Dumping structure for table 335Chars.pool_quest_save
DROP TABLE IF EXISTS `pool_quest_save`;
CREATE TABLE IF NOT EXISTS `pool_quest_save` (
  `pool_id` int(10) unsigned NOT NULL DEFAULT '0',
  `quest_id` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`pool_id`,`quest_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.pool_quest_save: ~0 rows (approximately)
/*!40000 ALTER TABLE `pool_quest_save` DISABLE KEYS */;
/*!40000 ALTER TABLE `pool_quest_save` ENABLE KEYS */;


-- Dumping structure for table 335Chars.reserved_name
DROP TABLE IF EXISTS `reserved_name`;
CREATE TABLE IF NOT EXISTS `reserved_name` (
  `name` varchar(12) NOT NULL DEFAULT '',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player Reserved Names';

-- Dumping data for table 335Chars.reserved_name: ~0 rows (approximately)
/*!40000 ALTER TABLE `reserved_name` DISABLE KEYS */;
/*!40000 ALTER TABLE `reserved_name` ENABLE KEYS */;


-- Dumping structure for table 335Chars.transfer_chars
DROP TABLE IF EXISTS `transfer_chars`;
CREATE TABLE IF NOT EXISTS `transfer_chars` (
  `CHAR_OLD_ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CHAR_NEW_ID` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 335Chars.transfer_chars: ~0 rows (approximately)
/*!40000 ALTER TABLE `transfer_chars` DISABLE KEYS */;
/*!40000 ALTER TABLE `transfer_chars` ENABLE KEYS */;


-- Dumping structure for table 335Chars.transports
DROP TABLE IF EXISTS `transports`;
CREATE TABLE IF NOT EXISTS `transports` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `name` text,
  `period` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Transports';

-- Dumping data for table 335Chars.transports: ~0 rows (approximately)
/*!40000 ALTER TABLE `transports` DISABLE KEYS */;
/*!40000 ALTER TABLE `transports` ENABLE KEYS */;


-- Dumping structure for table 335Chars.worldstates
DROP TABLE IF EXISTS `worldstates`;
CREATE TABLE IF NOT EXISTS `worldstates` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `value` int(10) unsigned NOT NULL DEFAULT '0',
  `comment` tinytext,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Variable Saves';

-- Dumping data for table 335Chars.worldstates: ~0 rows (approximately)
/*!40000 ALTER TABLE `worldstates` DISABLE KEYS */;
/*!40000 ALTER TABLE `worldstates` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
