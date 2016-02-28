/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table 335Logon.command
DROP TABLE IF EXISTS `command`;
CREATE TABLE IF NOT EXISTS `command` (
  `name` varchar(50) NOT NULL DEFAULT '',
  `security` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `help` longtext,
  PRIMARY KEY (`name`)
) ENGINE=Aria DEFAULT CHARSET=utf8 PAGE_CHECKSUM=1 ROW_FORMAT=PAGE COMMENT='Chat System';

-- Dumping data for table 335Logon.command: 41 rows
/*!40000 ALTER TABLE `command` DISABLE KEYS */;
INSERT INTO `command` (`name`, `security`, `help`) VALUES
        ('unmute', 2, 'Syntax: .unmute [$playerName]\r\n\r\nRestore chat messaging for any character from account of character $playerName (or selected). Character can be ofline.'),
        ('server shutdown cancel', 5, 'Syntax: .server shutdown cancel\r\n\r\nCancel the restart/shutdown timer if any.'),
        ('server shutdown', 5, 'Syntax: .server shutdown #delay [#exit_code]\r\n\r\nShut the server down after #delay seconds. Use #exit_code or 0 as program exit code.'),
        ('server', 0, 'Syntax: .server $subcommand\nType .server to see the list of possible subcommands or .help server $subcommand to see info on subcommands'),
        ('halt', 4, NULL),
        ('gm chat', 2, 'Syntax: .gm chat [on/off]\r\n\r\nEnable or disable chat GM MODE (show gm badge in messages) or show current state of on/off not provided.'),
        ('ownership', 2, NULL),
        ('gm visible', 2, 'Syntax: .gm visible on/off\r\n\r\nOutput current visibility state or make GM visible(on) and invisible(off) for other players.'),
        ('announce', 3, 'Syntax: .announce $MessageToBroadcast\r\n\r\nSend a global message to all players online in chat log.'),
        ('gm', 2, 'Syntax: .gm [on/off]\r\n\r\nEnable or Disable in game GM MODE or show current state of on/off not provided.'),
        ('kick', 2, 'Syntax: .kick [$charactername] [$reason]\r\n\r\nKick the given character name from the world with or without reason. If no character name is provided then the selected player (except for yourself) will be kicked. If no reason is provided, default is "No Reason".'),
        ('mute', 2, 'Syntax: .mute [$playerName] $timeInMinutes [$reason] Disable chat messaging for any character from account of character $playerName (or currently selected) at $timeInMinutes minutes. Player can be offline.'),
        ('antispam insert', 3, 'Syntax: "Schinken" "Nudeln"'),
        ('ticket assign', 2, 'Usage: .ticket assign $ticketid $gmname.\r\nAssigns the specified ticket to the specified Game Master.'),
        ('cheat cooldown', 2, 'Syntax: .cheat cooldown [on/off]\r\nEnables or disables your character\'s spell cooldowns.'),
        ('ticket close', 2, 'Usage: .ticket close $ticketid.\r\nCloses the specified ticket. Does not delete permanently.'),
        ('ticket closedlist', 2, 'Displays a list of closed GM tickets.'),
        ('ticket comment', 2, 'Usage: .ticket comment $ticketid $comment.\r\nAllows the adding or modifying of a comment to the specified ticket.'),
        ('ticket delete', 2, 'Usage: .ticket delete $ticketid.\r\nDeletes the specified ticket permanently. Ticket must be closed first.'),
        ('ticket list', 2, 'Displays a list of open GM tickets.'),
        ('ticket onlinelist', 2, 'Displays a list of open GM tickets whose owner is online.'),
        ('ticket unassign', 2, 'Usage: .ticket unassign $ticketid.\r\nUnassigns the specified ticket from the current assigned Game Master.'),
        ('ticket viewid', 2, 'Usage: .ticket viewid $ticketid.\r\nReturns details about specified ticket. Ticket must be open and not deleted.'),
        ('ticket viewname', 2, 'Usage: .ticket viewname $creatorname. \r\nReturns details about specified ticket. Ticket must be open and not deleted.'),
        ('ticket', 2, 'Syntax: .ticket $subcommand\nType .ticket to see the list of possible subcommands or .help ticket $subcommand to see info on subcommands'),
        ('account create', 5, 'Syntax: .account create $account $password\r\n\r\nCreate account and set password to it.'),
        ('ban account', 3, 'Syntax: .ban account $Name $bantime $reason\r\nBan account kick player.\r\n$bantime: negative value leads to permban, otherwise use a timestring like "4d20h3s".'),
        ('ban character', 3, 'Syntax: .ban character $Name $bantime $reason\nBan character and kick player.\n$bantime: negative value leads to permban, otherwise use a timestring like "4d20h3s".'),
        ('ban ip', 3, 'Syntax: .ban ip $Ip $bantime $reason\r\nBan IP.\r\n$bantime: negative value leads to permban, otherwise use a timestring like "4d20h3s".'),
        ('ban', 3, 'Syntax: .ban $subcommand\nType .ban to see the list of possible subcommands or .help ban $subcommand to see info on subcommands'),
        ('account', 5, 'Syntax: .account\r\n\r\nDisplay the access level of your account.'),
        ('baninfo account', 2, 'Syntax: .baninfo account $accountid\r\nWatch full information about a specific ban.'),
        ('baninfo character', 2, 'Syntax: .baninfo character $charactername \r\nWatch full information about a specific ban.'),
        ('baninfo ip', 2, 'Syntax: .baninfo ip $ip\r\nWatch full information about a specific ban.'),
        ('baninfo', 2, 'Syntax: .baninfo $subcommand\nType .baninfo to see the list of possible subcommands or .help baninfo $subcommand to see info on subcommands'),
        ('banlist account', 2, 'Syntax: .banlist account [$Name]\r\nSearches the banlist for a account name pattern or show full list account bans.'),
        ('banlist character', 2, 'Syntax: .banlist character $Name\r\nSearches the banlist for a character name pattern. Pattern required.'),
        ('banlist ip', 2, 'Syntax: .banlist ip [$Ip]\r\nSearches the banlist for a IP pattern or show full list of IP bans.'),
        ('banlist', 2, 'Syntax: .banlist $subcommand\nType .banlist to see the list of possible subcommands or .help banlist $subcommand to see info on subcommands'),
        ('server info', 0, 'Syntax: .server info\r\n\r\nDisplay server version and the number of connected players.'),
        ('whispers', 2, 'Syntax: .whispers on|off\r\nEnable/disable accepting whispers by GM from players. By default use trinityd.conf setting.');
/*!40000 ALTER TABLE `command` ENABLE KEYS */;


-- Dumping structure for table 335Logon.logonlist
DROP TABLE IF EXISTS `logonlist`;
CREATE TABLE IF NOT EXISTS `logonlist` (
  `id` int(8) NOT NULL DEFAULT '0',
  `Address` varchar(32) NOT NULL,
  `Name` varchar(32) NOT NULL,
  `Pass` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=Aria DEFAULT CHARSET=latin1 PAGE_CHECKSUM=1;

-- Dumping data for table 335Logon.logonlist: 1 rows
/*!40000 ALTER TABLE `logonlist` DISABLE KEYS */;
INSERT INTO `logonlist` (`id`, `Address`, `Name`, `Pass`) VALUES
        (1, '127.0.0.1', 'Strawberry', '12345');
/*!40000 ALTER TABLE `logonlist` ENABLE KEYS */;


-- Dumping structure for table 335Logon.logonstates
DROP TABLE IF EXISTS `logonstates`;
CREATE TABLE IF NOT EXISTS `logonstates` (
  `realmid` int(11) NOT NULL,
  `entry` int(10) NOT NULL,
  `value` int(10) NOT NULL,
  PRIMARY KEY (`realmid`)
) ENGINE=Aria DEFAULT CHARSET=latin1 PAGE_CHECKSUM=1;

-- Dumping data for table 335Logon.logonstates: 0 rows
/*!40000 ALTER TABLE `logonstates` DISABLE KEYS */;
/*!40000 ALTER TABLE `logonstates` ENABLE KEYS */;


-- Dumping structure for table 335Logon.map_table
DROP TABLE IF EXISTS `map_table`;
CREATE TABLE IF NOT EXISTS `map_table` (
  `MapID` int(11) NOT NULL,
  `NodeID` int(11) NOT NULL,
  `BackupNodeID` int(11) NOT NULL,
  `SubNode` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`MapID`)
) ENGINE=Aria DEFAULT CHARSET=latin1 PAGE_CHECKSUM=1;

-- Dumping data for table 335Logon.map_table: 135 rows
/*!40000 ALTER TABLE `map_table` DISABLE KEYS */;
INSERT INTO `map_table` (`MapID`, `NodeID`, `BackupNodeID`, `SubNode`) VALUES
        (0, 1, 0, 0),
        (1, 1, 0, 0),
        (13, 1, 0, 0),
        (25, 1, 0, 0),
        (30, 1, 0, 0),
        (33, 1, 0, 0),
        (34, 1, 0, 0),
        (35, 1, 0, 0),
        (36, 1, 0, 0),
        (37, 1, 0, 0),
        (42, 1, 0, 0),
        (43, 1, 0, 0),
        (44, 1, 0, 0),
        (47, 1, 0, 0),
        (48, 1, 0, 0),
        (70, 1, 0, 0),
        (90, 1, 0, 0),
        (109, 1, 0, 0),
        (129, 1, 0, 0),
        (169, 1, 0, 0),
        (189, 1, 0, 0),
        (209, 1, 0, 0),
        (229, 1, 0, 0),
        (230, 1, 0, 0),
        (249, 1, 0, 0),
        (269, 1, 0, 0),
        (289, 1, 0, 0),
        (309, 1, 0, 0),
        (329, 1, 0, 0),
        (349, 1, 0, 0),
        (369, 1, 0, 0),
        (389, 1, 0, 0),
        (409, 1, 0, 0),
        (429, 1, 0, 0),
        (449, 1, 0, 0),
        (450, 1, 0, 0),
        (451, 1, 0, 0),
        (469, 1, 0, 0),
        (489, 1, 0, 0),
        (509, 1, 0, 0),
        (529, 1, 0, 0),
        (530, 1, 0, 0),
        (531, 1, 0, 0),
        (532, 1, 0, 0),
        (533, 1, 0, 0),
        (534, 1, 0, 0),
        (540, 1, 0, 0),
        (542, 1, 0, 0),
        (543, 1, 0, 0),
        (544, 1, 0, 0),
        (545, 1, 0, 0),
        (546, 1, 0, 0),
        (547, 1, 0, 0),
        (548, 1, 0, 0),
        (550, 1, 0, 0),
        (552, 1, 0, 0),
        (553, 1, 0, 0),
        (554, 1, 0, 0),
        (555, 1, 0, 0),
        (556, 1, 0, 0),
        (557, 1, 0, 0),
        (558, 1, 0, 0),
        (559, 1, 0, 0),
        (560, 1, 0, 0),
        (562, 1, 0, 0),
        (564, 1, 0, 0),
        (565, 1, 0, 0),
        (566, 1, 0, 0),
        (568, 1, 0, 0),
        (571, 1, 0, 0),
        (572, 1, 0, 0),
        (573, 1, 0, 0),
        (574, 1, 0, 0),
        (575, 1, 0, 0),
        (576, 1, 0, 0),
        (580, 1, 0, 0),
        (578, 1, 0, 0),
        (582, 1, 0, 0),
        (584, 1, 0, 0),
        (585, 1, 0, 0),
        (586, 1, 0, 0),
        (587, 1, 0, 0),
        (588, 1, 0, 0),
        (589, 1, 0, 0),
        (590, 1, 0, 0),
        (591, 1, 0, 0),
        (592, 1, 0, 0),
        (593, 1, 0, 0),
        (594, 1, 0, 0),
        (595, 1, 0, 0),
        (596, 1, 0, 0),
        (597, 1, 0, 0),
        (598, 1, 0, 0),
        (599, 1, 0, 0),
        (600, 1, 0, 0),
        (601, 1, 0, 0),
        (603, 1, 0, 0),
        (602, 1, 0, 0),
        (604, 1, 0, 0),
        (605, 1, 0, 0),
        (606, 1, 0, 0),
        (607, 1, 0, 0),
        (608, 1, 0, 0),
        (609, 1, 0, 0),
        (610, 1, 0, 0),
        (612, 1, 0, 0),
        (613, 1, 0, 0),
        (614, 1, 0, 0),
        (615, 1, 0, 0),
        (616, 1, 0, 0),
        (617, 1, 0, 0),
        (618, 1, 0, 0),
        (619, 1, 0, 0),
        (620, 1, 0, 0),
        (621, 1, 0, 0),
        (622, 1, 0, 0),
        (623, 1, 0, 0),
        (624, 1, 0, 0),
        (673, 1, 0, 0),
        (628, 1, 0, 0),
        (631, 1, 0, 0),
        (632, 1, 0, 0),
        (641, 1, 0, 0),
        (642, 1, 0, 0),
        (647, 1, 0, 0),
        (649, 1, 0, 0),
        (650, 1, 0, 0),
        (658, 1, 0, 0),
        (668, 1, 0, 0),
        (672, 1, 0, 0),
        (712, 1, 0, 0),
        (713, 1, 0, 0),
        (718, 1, 0, 0),
        (723, 1, 0, 0),
        (724, 1, 0, 0);
/*!40000 ALTER TABLE `map_table` ENABLE KEYS */;


-- Dumping structure for table 335Logon.nodecharacters
DROP TABLE IF EXISTS `nodecharacters`;
CREATE TABLE IF NOT EXISTS `nodecharacters` (
  `realmid` int(11) unsigned NOT NULL DEFAULT '0',
  `acctid` bigint(20) unsigned NOT NULL,
  `numchars` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`realmid`,`acctid`),
  KEY `acctid` (`acctid`)
) ENGINE=Aria DEFAULT CHARSET=utf8 PAGE_CHECKSUM=1 ROW_FORMAT=DYNAMIC COMMENT='Realm Character Tracker';

-- Dumping data for table 335Logon.nodecharacters: 0 rows
/*!40000 ALTER TABLE `nodecharacters` DISABLE KEYS */;
/*!40000 ALTER TABLE `nodecharacters` ENABLE KEYS */;


-- Dumping structure for table 335Logon.nodelist
DROP TABLE IF EXISTS `nodelist`;
CREATE TABLE IF NOT EXISTS `nodelist` (
  `NodeID` int(11) NOT NULL,
  `Name` varchar(32) NOT NULL,
  `Address` varchar(32) NOT NULL,
  `Port` int(11) NOT NULL,
  `ControlPort` int(11) NOT NULL,
  `Online` tinyint(3) NOT NULL,
  `NodeType` tinyint(3) NOT NULL,
  `allowedSecurityLevel` tinyint(3) DEFAULT NULL,
  PRIMARY KEY (`NodeID`)
) ENGINE=Aria DEFAULT CHARSET=latin1 PAGE_CHECKSUM=1;

-- Dumping data for table 335Logon.nodelist: 1 rows
/*!40000 ALTER TABLE `nodelist` DISABLE KEYS */;
INSERT INTO `nodelist` (`NodeID`, `Name`, `Address`, `Port`, `ControlPort`, `Online`, `NodeType`, `allowedSecurityLevel`) VALUES
        (1, 'Alpha', '127.0.0.1', 8099, 8200, 0, 1, 0);
/*!40000 ALTER TABLE `nodelist` ENABLE KEYS */;


-- Dumping structure for table 335Logon.uptime
DROP TABLE IF EXISTS `uptime`;
CREATE TABLE IF NOT EXISTS `uptime` (
  `realmid` int(11) unsigned NOT NULL,
  `starttime` bigint(20) unsigned NOT NULL DEFAULT '0',
  `startstring` varchar(64) NOT NULL DEFAULT '',
  `uptime` bigint(20) unsigned NOT NULL DEFAULT '0',
  `maxplayers` smallint(5) unsigned NOT NULL DEFAULT '0',
  `revision` varchar(255) NOT NULL DEFAULT 'Trilliumcore',
  PRIMARY KEY (`realmid`,`starttime`)
) ENGINE=Aria DEFAULT CHARSET=utf8 PAGE_CHECKSUM=1 ROW_FORMAT=DYNAMIC COMMENT='Uptime system';

-- Dumping data for table 335Logon.uptime: 0 rows
/*!40000 ALTER TABLE `uptime` DISABLE KEYS */;
/*!40000 ALTER TABLE `uptime` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
 
