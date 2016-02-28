/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table 335Logs.gmq_accounts
DROP TABLE IF EXISTS `gmq_accounts`;
CREATE TABLE IF NOT EXISTS `gmq_accounts` (
  `id` int(11) NOT NULL,
  `accountname` text NOT NULL,
  `password` text NOT NULL,
  `permissions` tinyint(3) NOT NULL DEFAULT '-1',
  `comment` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Logs.gmq_accounts: ~0 rows (approximately)
/*!40000 ALTER TABLE `gmq_accounts` DISABLE KEYS */;
/*!40000 ALTER TABLE `gmq_accounts` ENABLE KEYS */;


-- Dumping structure for table 335Logs.gm_chat_log
DROP TABLE IF EXISTS `gm_chat_log`;
CREATE TABLE IF NOT EXISTS `gm_chat_log` (
  `timestamp` int(11) NOT NULL,
  `gm_acc_guid` int(10) NOT NULL,
  `gm_acc_name` varchar(255) NOT NULL,
  `securityLevel` tinyint(4) NOT NULL,
  `chat_type` tinyint(4) NOT NULL,
  `receiver` varchar(255) NOT NULL,
  `text` text NOT NULL,
  PRIMARY KEY (`timestamp`,`gm_acc_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 335Logs.gm_chat_log: ~0 rows (approximately)
/*!40000 ALTER TABLE `gm_chat_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_chat_log` ENABLE KEYS */;


-- Dumping structure for table 335Logs.gm_log
DROP TABLE IF EXISTS `gm_log`;
CREATE TABLE IF NOT EXISTS `gm_log` (
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

-- Dumping data for table 335Logs.gm_log: ~0 rows (approximately)
/*!40000 ALTER TABLE `gm_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_log` ENABLE KEYS */;


-- Dumping structure for table 335Logs.gm_ticket_log
DROP TABLE IF EXISTS `gm_ticket_log`;
CREATE TABLE IF NOT EXISTS `gm_ticket_log` (
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

-- Dumping data for table 335Logs.gm_ticket_log: ~0 rows (approximately)
/*!40000 ALTER TABLE `gm_ticket_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_ticket_log` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
 
