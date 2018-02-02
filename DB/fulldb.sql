-- MySQL dump 10.13  Distrib 5.7.20, for Win64 (x86_64)
--
-- Host: localhost    Database: mud
-- ------------------------------------------------------
-- Server version	5.7.20

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `areas`
--

DROP TABLE IF EXISTS `areas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `areas` (
  `id` int(11) NOT NULL,
  `name` varchar(45) DEFAULT NULL,
  `level_range_low` int(11) DEFAULT NULL,
  `level_range_high` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `areas`
--

LOCK TABLES `areas` WRITE;
/*!40000 ALTER TABLE `areas` DISABLE KEYS */;
INSERT INTO `areas` VALUES (1,'Northshire',-842150451,-842150451),(2,'Elwynn Forest',-842150451,-842150451);
/*!40000 ALTER TABLE `areas` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `class_skills`
--

DROP TABLE IF EXISTS `class_skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `class_skills` (
  `class` int(11) NOT NULL,
  `skill` int(11) NOT NULL,
  `level` int(11) DEFAULT NULL,
  PRIMARY KEY (`skill`,`class`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `class_skills`
--

LOCK TABLES `class_skills` WRITE;
/*!40000 ALTER TABLE `class_skills` DISABLE KEYS */;
INSERT INTO `class_skills` VALUES (1,1,1),(2,1,1),(3,1,1),(4,1,1),(4,5,1),(3,15,1),(1,16,1),(2,21,1),(2,22,2),(4,26,2),(4,29,5),(3,32,2),(2,33,5),(1,34,2),(1,35,5),(3,36,5);
/*!40000 ALTER TABLE `class_skills` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `classes`
--

DROP TABLE IF EXISTS `classes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `classes` (
  `id` int(11) NOT NULL,
  `name` varchar(12) DEFAULT NULL,
  `color` varchar(5) DEFAULT NULL,
  `items` tinytext,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `classes`
--

LOCK TABLES `classes` WRITE;
/*!40000 ALTER TABLE `classes` DISABLE KEYS */;
INSERT INTO `classes` VALUES (1,'Warrior','|M','3;4;5;6;'),(2,'Rogue','|Y','11;12;13;14;'),(3,'Mage','|C','7;8;9;10;'),(4,'Cleric','|W','15;16;17;10;');
/*!40000 ALTER TABLE `classes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `exits`
--

DROP TABLE IF EXISTS `exits`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `exits` (
  `from` int(11) NOT NULL,
  `direction` int(11) NOT NULL,
  `to` int(11) DEFAULT NULL,
  PRIMARY KEY (`from`,`direction`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `exits`
--

LOCK TABLES `exits` WRITE;
/*!40000 ALTER TABLE `exits` DISABLE KEYS */;
INSERT INTO `exits` VALUES (2,0,3),(2,2,8),(2,4,9),(2,6,4),(3,1,11),(3,4,2),(3,7,10),(4,2,2),(4,4,18),(4,6,5),(5,0,6),(5,2,4),(5,4,24),(5,6,7),(6,0,34),(6,4,5),(6,6,32),(7,0,32),(7,2,5),(7,4,25),(7,6,31),(8,2,81),(8,4,64),(8,6,2),(9,0,2),(9,2,64),(9,4,19),(9,6,18),(10,0,13),(10,2,12),(10,3,3),(11,0,14),(11,5,3),(11,6,12),(12,0,15),(12,2,11),(12,6,10),(13,4,10),(14,4,11),(15,4,12),(15,8,16),(16,8,17),(16,9,15),(17,9,16),(18,0,4),(18,2,9),(18,4,26),(18,6,24),(19,0,9),(19,2,65),(19,4,20),(19,6,26),(20,0,19),(20,3,21),(20,5,22),(21,5,23),(21,7,20),(22,1,20),(22,3,23),(23,1,21),(23,7,22),(24,0,5),(24,2,18),(24,4,27),(24,6,25),(25,0,7),(25,2,24),(25,4,28),(25,6,29),(26,0,18),(26,2,19),(26,6,27),(27,0,24),(27,2,26),(27,6,28),(28,0,25),(28,2,27),(28,6,30),(29,0,31),(29,2,25),(29,4,30),(30,0,29),(30,2,28),(31,0,33),(31,2,7),(31,4,29),(32,0,35),(32,2,6),(32,4,7),(32,6,33),(33,0,36),(33,2,32),(33,4,31),(34,0,37),(34,4,6),(34,6,35),(35,0,38),(35,2,34),(35,4,32),(35,6,36),(36,0,39),(36,2,35),(36,4,33),(37,0,41),(37,4,34),(37,6,38),(38,0,42),(38,2,37),(38,4,35),(38,6,39),(39,0,43),(39,2,38),(39,4,36),(39,6,40),(40,0,44),(40,2,39),(41,0,45),(41,2,51),(41,4,37),(41,6,42),(42,0,46),(42,2,41),(42,4,38),(42,6,43),(43,0,47),(43,2,42),(43,4,39),(43,6,44),(44,0,48),(44,2,43),(44,4,40),(45,0,74),(45,2,72),(45,4,41),(45,6,46),(46,0,73),(46,2,45),(46,4,42),(46,6,47),(47,0,67),(47,2,46),(47,4,43),(47,6,48),(48,0,49),(48,2,47),(48,4,44),(49,0,50),(49,2,67),(49,4,48),(50,0,109),(50,4,49),(51,0,72),(51,2,52),(51,6,41),(52,0,71),(52,2,53),(52,6,51),(53,0,70),(53,2,54),(53,6,52),(54,0,69),(54,2,55),(54,4,84),(54,6,53),(55,0,68),(55,2,56),(55,4,87),(55,6,54),(56,0,66),(56,2,99),(56,4,57),(56,6,55),(57,0,56),(57,2,98),(57,4,58),(57,6,87),(58,0,57),(58,2,91),(58,4,59),(58,6,86),(59,0,58),(59,2,88),(59,4,60),(59,6,85),(60,0,59),(60,2,89),(60,4,61),(60,6,80),(61,0,60),(61,2,90),(61,6,62),(62,0,80),(62,2,61),(62,6,63),(63,0,81),(63,2,62),(63,6,64),(64,0,8),(64,2,63),(64,4,65),(64,6,9),(65,0,64),(65,6,19),(66,4,56),(66,6,68),(67,2,73),(67,4,47),(67,6,49),(68,0,79),(68,2,66),(68,4,55),(68,6,69),(69,0,78),(69,2,68),(69,4,54),(69,6,70),(70,0,77),(70,2,69),(70,4,53),(70,6,71),(71,0,76),(71,2,70),(71,4,52),(71,6,72),(72,0,75),(72,2,71),(72,4,51),(72,6,45),(73,2,74),(73,4,46),(73,6,67),(74,2,75),(74,4,45),(74,6,73),(75,2,76),(75,4,72),(75,6,74),(76,2,77),(76,4,71),(76,6,75),(77,2,78),(77,4,70),(77,6,76),(78,2,79),(78,4,69),(78,6,77),(79,4,68),(79,6,78),(80,0,85),(80,2,60),(80,4,62),(80,6,81),(81,0,82),(81,2,80),(81,4,63),(81,6,8),(82,0,83),(82,2,85),(82,4,81),(83,0,84),(83,2,86),(83,4,82),(84,0,54),(84,2,87),(84,4,83),(85,0,86),(85,2,59),(85,4,80),(85,6,82),(86,0,87),(86,2,58),(86,4,85),(86,6,83),(87,0,55),(87,2,57),(87,4,86),(87,6,84),(88,0,91),(88,2,107),(88,4,89),(88,6,59),(89,0,88),(89,2,105),(89,4,90),(89,6,60),(90,0,89),(90,2,100),(90,6,61),(91,0,98),(91,2,92),(91,4,88),(91,6,58),(92,0,96),(92,2,93),(92,4,107),(92,6,91),(93,0,95),(93,2,94),(93,4,108),(93,6,92),(94,4,104),(94,6,93),(95,4,93),(95,6,96),(96,0,97),(96,2,95),(96,4,92),(96,6,98),(97,4,96),(97,6,99),(98,0,99),(98,2,96),(98,4,91),(98,6,57),(99,2,97),(99,4,98),(99,6,56),(100,0,105),(100,2,101),(100,6,90),(101,0,106),(101,2,102),(101,6,100),(102,0,103),(102,6,101),(103,0,104),(103,4,102),(103,6,106),(104,0,94),(104,4,103),(104,6,108),(105,0,107),(105,2,106),(105,4,100),(105,6,89),(106,0,108),(106,2,103),(106,4,101),(106,6,105),(107,0,92),(107,2,108),(107,4,105),(107,6,88),(108,0,93),(108,2,104),(108,4,106),(108,6,107),(109,0,112),(109,4,50),(109,6,110),(110,2,109),(110,6,111),(111,2,110),(112,4,109),(112,7,113),(113,1,114),(113,3,112),(113,6,115),(114,5,113),(114,9,116),(115,0,117),(115,2,113),(116,8,114),(117,1,118),(117,4,115),(118,5,117),(118,9,119),(119,0,120),(119,8,118),(120,4,119);
/*!40000 ALTER TABLE `exits` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `help`
--

DROP TABLE IF EXISTS `help`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `help` (
  `id` int(11) NOT NULL,
  `title` varchar(45) DEFAULT NULL,
  `search_string` tinytext,
  `text` mediumtext,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `help`
--

LOCK TABLES `help` WRITE;
/*!40000 ALTER TABLE `help` DISABLE KEYS */;
/*!40000 ALTER TABLE `help` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `items`
--

DROP TABLE IF EXISTS `items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `items` (
  `id` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `keywords` tinytext NOT NULL,
  `item_level` int(11) NOT NULL,
  `char_level` int(11) NOT NULL,
  `equip_location` int(11) NOT NULL,
  `quality` int(11) NOT NULL,
  `binds` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `quest` int(11) NOT NULL,
  `armor` int(11) NOT NULL,
  `durability` int(11) NOT NULL,
  `unique` int(11) NOT NULL,
  `damage_low` int(11) NOT NULL,
  `damage_high` int(11) NOT NULL,
  `value` int(11) NOT NULL,
  `speed` decimal(10,1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `items`
--

LOCK TABLES `items` WRITE;
/*!40000 ALTER TABLE `items` DISABLE KEYS */;
INSERT INTO `items` VALUES (1,'The One Ring','one ring',15,10,11,2,2,13,0,0,0,0,0,0,140,0.0),(2,'Test Item of Power','test item power',5,2,1,4,1,0,0,100,100,1,0,0,1000,0.0),(3,'Worn Greatsword','',1,1,16,1,0,4,0,0,25,0,2,3,4,2.4),(4,'Recruit\'s Vest','',1,1,5,1,0,0,0,3,55,0,0,0,2,0.0),(5,'Recruit\'s Pants','',1,1,9,1,0,0,0,3,40,0,0,0,2,0.0),(6,'Recruit\'s Boots','',1,1,10,1,0,0,0,2,25,0,0,0,1,0.0),(7,'Apprentice\'s Boots','',1,1,10,1,0,0,0,2,25,0,0,0,2,0.0),(8,'Apprentice\'s Robe','',1,1,5,1,0,0,0,3,55,0,0,0,2,0.0),(9,'Apprentice\'s Pants','',1,1,9,1,0,0,0,3,40,0,0,0,2,0.0),(10,'Bent Staff','',1,1,16,1,0,9,0,0,25,0,2,3,4,2.3),(11,'Footpad\'s Pants','',1,1,9,1,0,0,0,2,40,0,0,0,2,0.0),(12,'Footpad\'s Vest','',1,1,5,1,0,0,0,3,55,0,0,0,2,0.0),(13,'Footpad\'s Shoes','',1,1,10,1,0,0,0,3,25,0,0,0,1,0.0),(14,'Worn Shortsword','',1,1,14,1,0,4,0,0,20,0,1,3,3,1.9),(15,'Neophyte\'s Pants','',1,1,9,1,0,0,0,3,40,0,0,0,2,0.0),(16,'Neophyte\'s Boots','',1,1,10,1,0,0,0,2,25,0,0,0,2,0.0),(17,'Neophyte\'s Robe','',1,1,5,1,0,0,0,3,55,0,0,0,2,0.0),(18,'Refreshing Spring Water','',0,0,0,0,0,13,0,0,0,0,0,0,0,0.0),(19,'Shiny Red Apple','',0,0,0,0,0,13,0,0,0,0,0,0,0,0.0),(20,'Tough Wolf Meat','',1,0,0,1,0,13,1,0,0,0,0,0,0,0.0),(21,'Red Burlap Bandana','',1,0,0,1,0,13,1,0,0,0,0,0,0,0.0),(22,'Milly\'s Harvest','',1,0,0,1,0,13,1,0,0,0,0,0,0,0.0),(23,'Grape Manifest','',1,1,0,1,1,13,1,0,0,1,0,0,0,0.0),(24,'Marshal McBride\'s Documents','',1,1,0,1,1,13,1,0,0,1,0,0,0,0.0),(25,'Garrick\'s Head','',1,1,0,1,1,13,1,0,0,1,0,0,0,0.0);
/*!40000 ALTER TABLE `items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `npc_drops`
--

DROP TABLE IF EXISTS `npc_drops`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `npc_drops` (
  `npc` int(11) NOT NULL,
  `items` varchar(45) NOT NULL,
  `percent` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `npc_drops`
--

LOCK TABLES `npc_drops` WRITE;
/*!40000 ALTER TABLE `npc_drops` DISABLE KEYS */;
INSERT INTO `npc_drops` VALUES (2,'20;',75),(3,'20;',75),(18,'25;',100),(22,'21;',80),(23,'21;',80);
/*!40000 ALTER TABLE `npc_drops` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `npc_skills`
--

DROP TABLE IF EXISTS `npc_skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `npc_skills` (
  `npc` int(11) NOT NULL,
  `skill` int(11) NOT NULL,
  PRIMARY KEY (`npc`,`skill`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `npc_skills`
--

LOCK TABLES `npc_skills` WRITE;
/*!40000 ALTER TABLE `npc_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `npc_skills` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `npcs`
--

DROP TABLE IF EXISTS `npcs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `npcs` (
  `id` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `keywords` tinytext NOT NULL,
  `level` int(11) NOT NULL,
  `gender` int(11) NOT NULL,
  `race` int(11) NOT NULL,
  `agility` int(11) NOT NULL,
  `intellect` int(11) NOT NULL,
  `strength` int(11) NOT NULL,
  `stamina` int(11) NOT NULL,
  `wisdom` int(11) NOT NULL,
  `health` int(11) NOT NULL,
  `mana` int(11) NOT NULL,
  `energy` int(11) NOT NULL,
  `rage` int(11) NOT NULL,
  `title` varchar(45) NOT NULL,
  `attack_speed` double NOT NULL,
  `damage_low` int(11) NOT NULL,
  `damage_high` int(11) NOT NULL,
  `flags` tinytext NOT NULL,
  `speechtext` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `npcs`
--

LOCK TABLES `npcs` WRITE;
/*!40000 ALTER TABLE `npcs` DISABLE KEYS */;
INSERT INTO `npcs` VALUES (1,'The Lurker Below','',240,1,0,200,200,200,200,200,5000,5000,0,0,'',2.5,100,200,'2;',''),(2,'Diseased Young Wolf','',1,1,0,5,0,5,4,0,50,0,0,0,'',2,2,3,'1;',''),(3,'Diseased Timber Wolf','',1,1,0,5,0,5,4,0,50,0,0,0,'',2,2,3,'1;',''),(4,'Marshal McBride','',20,1,1,10,10,10,10,10,1227,100,0,0,'',2,1,1,'0;','Hey, citizen! You look like a stout one. We guards are spread a little thin out here, and I could use your help...'),(5,'Eagan Peltskinner','',3,1,1,10,10,10,7,10,100,150,0,0,'',2,1,1,'0;',''),(6,'Deputy Willem','',18,1,1,10,10,10,41,10,100,150,0,0,'',2,1,1,'0;','Normally I\'d be out on the beat looking after the folk of Stormwind, but a lot of the Stormwind guards are fighting in the other lands. So here I am, deputized and offering bounties when I\'d rather be on patrol...'),(7,'Milly Osworth','',2,1,1,10,10,10,5,10,100,150,0,0,'',2,1,1,'0;',''),(8,'Kobold Vermin','',2,1,0,10,10,10,4,10,20,20,0,0,'',2,2,4,'1;',''),(9,'Kobold Worker','',3,1,0,10,10,10,5,10,25,20,0,0,'',1.75,2,5,'1;',''),(10,'Kobold Laborer','',5,1,0,10,10,10,5,10,25,20,0,0,'',1.75,2,5,'1;',''),(11,'Falkhaan Isenstrider','',10,1,1,10,10,10,19,10,100,150,0,0,'',2,1,1,'0;',''),(12,'Brother Neals','',50,1,1,100,290,100,276,200,100,150,0,0,'',2,1,1,'0;',''),(13,'Janos Hammerknuckle','',5,1,1,10,10,10,10,10,100,150,0,0,'Weaponsmith',2,1,1,'0;4;5;',''),(14,'Godric Rothgar','',5,1,1,10,10,10,10,10,100,150,0,0,'Armorer & Shieldcrafter',2,1,1,'0;4;5;',''),(15,'Brother Danil','',5,1,1,10,10,10,10,10,100,150,0,0,'General Supplies',2,1,1,'0;4;',''),(16,'Dermot Johns','',5,1,1,10,10,10,10,10,100,150,0,0,'Cloth & Leather Armor Merchant',2,1,1,'0;4;5;',''),(17,'Marshal Dughan','',1,1,0,10,10,10,10,10,50,100,100,0,'',2,1,1,'0;','Ach, it\'s hard enough keeping order around here without all these new troubles popping up! I hope you have good news, Sandrol...'),(18,'Garrick Padfoot','',5,1,0,10,10,10,10,10,101,100,100,0,'',2,12,14,'2;',''),(19,'Innkeeper Farley','',30,1,0,10,10,10,10,10,50,100,100,0,'',2,1,1,'0;','Welcome to my Inn, weary traveler. What can I do for you?'),(20,'Brother Paxton','',5,1,0,10,10,10,10,10,50,100,100,0,'',2,1,1,'0;',''),(21,'Brother Sammuel','',5,1,0,10,10,10,10,10,50,100,100,0,'',2,1,1,'0;',''),(22,'Defias Thug','',4,1,0,10,10,10,10,10,75,100,100,0,'',2,5,10,'2;',''),(23,'Defias Thug','',3,2,0,10,10,10,10,10,75,100,100,0,'',2,5,10,'2;','');
/*!40000 ALTER TABLE `npcs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_active_quests`
--

DROP TABLE IF EXISTS `player_active_quests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_active_quests` (
  `player` varchar(12) NOT NULL,
  `quest` int(11) NOT NULL,
  `objectives` tinytext,
  PRIMARY KEY (`player`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_active_quests`
--

LOCK TABLES `player_active_quests` WRITE;
/*!40000 ALTER TABLE `player_active_quests` DISABLE KEYS */;
INSERT INTO `player_active_quests` VALUES ('Bobo',1,'0');
/*!40000 ALTER TABLE `player_active_quests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_class_data`
--

DROP TABLE IF EXISTS `player_class_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_class_data` (
  `player` varchar(12) NOT NULL,
  `class` int(11) NOT NULL,
  `level` int(11) NOT NULL,
  PRIMARY KEY (`player`,`class`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_class_data`
--

LOCK TABLES `player_class_data` WRITE;
/*!40000 ALTER TABLE `player_class_data` DISABLE KEYS */;
INSERT INTO `player_class_data` VALUES ('Bob',1,40),('Bob',2,30),('Bob',3,81),('Bob',4,89),('Bobo',2,2),('Boris',1,59),('Boris',2,60),('Boris',3,60),('Boris',4,60);
/*!40000 ALTER TABLE `player_class_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_completed_quests`
--

DROP TABLE IF EXISTS `player_completed_quests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_completed_quests` (
  `player` varchar(12) NOT NULL,
  `quest` int(11) NOT NULL,
  PRIMARY KEY (`player`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_completed_quests`
--

LOCK TABLES `player_completed_quests` WRITE;
/*!40000 ALTER TABLE `player_completed_quests` DISABLE KEYS */;
INSERT INTO `player_completed_quests` VALUES ('Bobo',4);
/*!40000 ALTER TABLE `player_completed_quests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_cooldowns`
--

DROP TABLE IF EXISTS `player_cooldowns`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_cooldowns` (
  `player` varchar(12) NOT NULL,
  `skill` int(11) NOT NULL,
  `timestamp` double DEFAULT NULL,
  PRIMARY KEY (`player`,`skill`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_cooldowns`
--

LOCK TABLES `player_cooldowns` WRITE;
/*!40000 ALTER TABLE `player_cooldowns` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_cooldowns` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_inventory`
--

DROP TABLE IF EXISTS `player_inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_inventory` (
  `player` varchar(12) NOT NULL,
  `item` int(11) NOT NULL,
  `location` int(11) NOT NULL,
  KEY `player` (`player`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_inventory`
--

LOCK TABLES `player_inventory` WRITE;
/*!40000 ALTER TABLE `player_inventory` DISABLE KEYS */;
INSERT INTO `player_inventory` VALUES ('Bobo',12,0),('Bobo',11,0),('Bobo',13,0),('Bobo',14,0),('Bob',4,0),('Bob',5,0),('Bob',6,0),('Bob',3,0),('Boris',21,1),('Boris',20,1);
/*!40000 ALTER TABLE `player_inventory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_spell_affects`
--

DROP TABLE IF EXISTS `player_spell_affects`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_spell_affects` (
  `player` varchar(12) NOT NULL,
  `caster` varchar(12) NOT NULL,
  `skill` int(11) NOT NULL,
  `ticks` int(11) DEFAULT NULL,
  `duration` double DEFAULT NULL,
  `timeleft` double DEFAULT NULL,
  `stackable` int(11) DEFAULT NULL,
  `hidden` int(11) DEFAULT NULL,
  `debuff` tinyint(4) DEFAULT NULL,
  `category` int(11) DEFAULT NULL,
  `auras` tinytext,
  `data` tinytext,
  PRIMARY KEY (`player`,`skill`,`caster`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_spell_affects`
--

LOCK TABLES `player_spell_affects` WRITE;
/*!40000 ALTER TABLE `player_spell_affects` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_spell_affects` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `players`
--

DROP TABLE IF EXISTS `players`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `players` (
  `name` varchar(12) NOT NULL,
  `password` varchar(15) NOT NULL,
  `immlevel` int(11) NOT NULL,
  `title` varchar(45) NOT NULL,
  `experience` int(11) NOT NULL,
  `room` int(11) NOT NULL,
  `level` int(11) NOT NULL,
  `gender` int(11) NOT NULL,
  `race` int(11) NOT NULL,
  `agility` int(11) NOT NULL,
  `intellect` int(11) NOT NULL,
  `strength` int(11) NOT NULL,
  `stamina` int(11) NOT NULL,
  `wisdom` int(11) NOT NULL,
  `health` int(11) NOT NULL,
  `mana` int(11) NOT NULL,
  `class` int(11) NOT NULL,
  `recall` int(11) NOT NULL,
  `ghost` tinyint(4) NOT NULL,
  `stat_points` int(11) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `players`
--

LOCK TABLES `players` WRITE;
/*!40000 ALTER TABLE `players` DISABLE KEYS */;
INSERT INTO `players` VALUES ('Bob','\r\Z',0,'',16516800,2,240,1,0,139,512,118,446,269,2230,5120,4,0,0,0),('Bobo','\r\Z',0,'',862,5,2,1,6,10,10,10,10,10,50,100,2,2,0,6),('Boris','\r\Z',10,'',16516800,2,240,1,7,249,110,488,727,10,3635,1100,2,2,0,4208);
/*!40000 ALTER TABLE `players` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_objectives`
--

DROP TABLE IF EXISTS `quest_objectives`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_objectives` (
  `quest` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `id` int(11) NOT NULL,
  `count` int(11) DEFAULT NULL,
  `description` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`quest`,`type`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_objectives`
--

LOCK TABLES `quest_objectives` WRITE;
/*!40000 ALTER TABLE `quest_objectives` DISABLE KEYS */;
INSERT INTO `quest_objectives` VALUES (1,2,8,8,'Kobold Vermin'),(2,2,9,8,'Kobold Workers'),(3,2,10,8,'Kobold Laborers'),(4,3,20,8,'Tough Wolf Meat'),(5,3,21,8,'Red Burlap Bandana'),(9,3,22,8,'Milly\'s Harvest'),(11,2,18,1,'Garrick Padfoot slain'),(11,3,25,1,'Garrick\'s Head'),(12,3,23,1,'Grape Manifest'),(13,3,24,1,'Marshal McBride\'s Documents');
/*!40000 ALTER TABLE `quest_objectives` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quests`
--

DROP TABLE IF EXISTS `quests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quests` (
  `id` int(11) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `short_description` text,
  `long_description` text,
  `progress_message` text,
  `completion_message` text,
  `level` int(11) DEFAULT NULL,
  `quest_requirement` int(11) DEFAULT NULL,
  `start` int(11) DEFAULT NULL,
  `end` int(11) DEFAULT NULL,
  `exp_reward` int(11) DEFAULT NULL,
  `money_reward` int(11) DEFAULT NULL,
  `shareable` int(11) DEFAULT NULL,
  `level_requirement` int(11) DEFAULT NULL,
  `quest_restriction` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quests`
--

LOCK TABLES `quests` WRITE;
/*!40000 ALTER TABLE `quests` DISABLE KEYS */;
INSERT INTO `quests` VALUES (1,'Kobold Camp Cleanup','Kill 8 Kobold Vermin, then return to Marshal McBride.','Your first task is one of cleansing. A clan of kobolds have infested the \nwoods to the north. Go there and fight the Kobold Vermin you find. Reduce \ntheir numbers so that we may one day drive them from Northshire.','','Well done, citizen.  Those kobolds are thieves and cowards, but in large \nnumbers they pose a threat to us.  And the humans of Stormwind do not \nneed another threat.\n\nFor defeating them, you have my gratitude. ',2,6,4,4,170,10,1,1,0),(2,'Investigate Echo Ridge','Kill 8 Kobold Workers, then report back to Marshal McBride.','My scouts tell me that the kobold infestation is larger than we had thought. \nA group of Kobold Workers has camped near the Echo Ridge Mine to the north.\n\nGo to the mine and remove them. We know there are at least 8. Kill them, \nsee if there are more, then report back to me. ','','I don\'t like hearing of all these kobolds in our mine.  No good can come \nof this.  Here, take this as payment, and when you\'re ready, speak to me \nagain.  I would like you to go back to the mines one more time... ',3,1,4,4,250,20,1,1,0),(3,'Skirmish at Echo Ridge','Kill 8 Kobold Laborers, then return to Marshal McBride at Northshire Abbey.','Your previous investigations are proof that the Echo Ridge Mine needs \npurging. Return to the mine and help clear it of kobolds.\n\nWaste no time. The longer the kobolds are left unmolested in the mine, \nthe deeper a foothold they gain in Northshire. ','','Once again, you have earned my respect, and the gratitude of the \nStormwind Army.  There may yet be kobolds in the mine, but I will \nmarshal others against them.  We have further tasks for you. ',5,2,4,4,500,330,1,1,0),(4,'Wolves Across the Border','Bring 8 pieces of Tough Wolf Meat to Eagan Peltskinner outside Northshire Abbey.','I hate those nasty timber wolves! But I sure like eating wolf steaks... \nBring me tough wolf meat and I will exchange it for something you\'ll\n find useful.\n\nTough wolf meat is gathered from hunting the timber wolves and \nyoung wolves wandering the Northshire countryside.','I\'m getting hungry...did you get that tough wolf meat? ','You\'ve been busy!  I can\'t wait to cook up that wolf meat...\n\nI have some things here you might want - take your pick! ',2,6,5,5,170,0,1,1,0),(5,'Brotherhood of Thieves','Bring 8 Red Burlap Bandanas to Deputy Willem outside the Northshire Abbey. ','Recently, a new group of thieves has been hanging around \nNorthshire. They call themselves the Defias Brotherhood, and have\nbeen seen across the river to the east.\n\nI don\'t know what they\'re up to, but I\'m sure it\'s not good! Bring me \nthe bandanas they wear, and I\'ll reward you with a weapon. ','Have you gathered those bandanas for me yet? ','Back with some bandanas, I see.  The Stormwind Army appreciates your help. ',4,6,6,6,360,0,1,2,0),(6,'A Threat Within','Speak with Marshal McBride. ','I hope you strapped your belt on tight, because there is work \n\rto do here in Northshire.\n\r\n\rAnd I don\'t mean farming.\n\r\n\rThe Stormwind guards are hard pressed to keep the peace here, \n\rwith so many of us in distant lands and so many threats pressing \n\rclose. And so we\'re enlisting the aid of anyone willing to defend \n\rtheir home. And their alliance.\n\r\n\rIf you\'re here to answer the call, then speak with my superior, \n\rMarshal McBride. He\'s inside the abbey to the north.\n\r','','Ah, good.  Another volunteer.  We\'re getting a lot of you these days.\n\nI hope it\'s enough.\n\nOur lands are threatened from without, and so many of our forces\nhave been marshaled abroad.  This, in turn, leaves room for corrupt \nand lawless groups to thrive within our borders.\n\nIt is a many-fronted battle we wage.  Gird yourself for a long campaign. ',1,0,6,4,50,0,0,1,0),(7,'Milly Osworth','Speak with Milly Osworth. ','You\'ve shown yourself to be dependable. Dependable, and not \nafraid to get your hands dirty, eh?\n\nI have a friend, Milly Osworth, who\'s in some trouble. She\'s over \nwith her wagon on the other side of the abbey, near the stable. I\'m \nsure she could use a pair of hands like yours. ','','Oh, Deputy Willem told you to speak with me?  He\'s a brave \nman and always willing to help, but his duties keep him stuck \nat Northshire Abbey and I\'m afraid the problem I have today is \nbeyond him.\n\nPerhaps you can help me? ',4,5,6,7,50,0,1,2,0),(8,'Eagan Peltskinner','Speak with Eagan Peltskinner.','Eagan Peltskinner is looking for someone to hunt wolves for him. \n\rThat\'s good news, because we\'re seeing a lot more wolves in \n\rNorthshire Valley lately.\n\r\n\rIf you\'re interested then speak with Eagan. He\'s around the \n\rside of the abbey, to the west.\n\r','','That\'s true. I\'m looking for someone to hunt me some wolves! \nAre you that person?',1,6,6,5,100,0,1,1,4),(9,'Milly\'s Harvest','Bring 8 crates of Milly\'s Harvest to Milly Osworth at Northshire Abbey. ','A gang of brigands, the Defias, moved into the Northshire Vineyards while I was harvesting! I reported it to the Northshire guards and they assured me they\'d take care of things, but... I\'m afraid for my crop of grapes! If the Defias don\'t steal them then I fear our guards will trample them when they chase away the thugs.\n\n\nPlease, you must help me! I gathered most of my grapes into buckets, but I left them in the vineyards to the southeast.\n\n\nBring me those buckets! Save my harvest! \n\n','Do you have my harvest?\n\n','Oh thank you! You saved my harvest!  And I hope you showed a few of those Defias that they can\'t cause trouble around here.\n\n\nWe might be short on guards these days, but we\'re lucky to have heroes like you to protect us! \n\n',4,7,7,7,200,0,1,2,0),(10,'Rest and Relaxation','Speak with Innkeeper Farley at the Lion\'s Pride Inn.','Every adventurer should rest when exhaustion sets in - and there is no finer place to get rest and relaxation than at the Lion\'s Pride Inn!\n\r\n\rMy best friend, Innkeeper Farley, runs the Lion\'s Pride. If you tell him I sent you, he may give you the special discounted rates on food and drink.\n\r\n\rTo find the Lion\'s Pride Inn, travel south along the road from here -- you can\'t miss it! \n\r','','Rest and relaxation for the tired and cold -- that\'s our motto! Please, take a seat by the fire and rest your weary bones.\n\r\n\rWould you like to try a sampling of some of our fine food and drink? \n\r',5,0,11,19,100,0,1,1,0),(11,'Bounty on Garrick Padfoot','Kill Garrick Padfoot and bring his head to Deputy Willem at Northshire Abbey.','Garrick Padfoot - a cutthroat who\'s plagued our farmers and merchants for weeks - was seen at a shack near the vineyards, which lies east of the Abbey and across the bridge. Bring me the villain\'s head, and earn his bounty!\n\r\n\rBut be wary. Garrick has gathered a gang of thugs around him. He will not be an easy man to reach.\n\r','Did you find Garrick\'s shack? Are we finally free of that villain?\n\r','Hah - you caught him! You\'ve done Elwynn a great service, and earned a nice bounty!\n\r',5,5,6,6,350,0,1,3,0),(12,'Grape Manifest','Bring the Grape Manifest to Brother Neals in Northshire Abbey.','Now that my crop is saved, take this Grape Manifest to Brother Neals. He manages the store of food and drink in Northshire, and I\'m sure he\'ll be delighted to hear that he has fresh grapes.\n\r\n\rYou\'ll find Brother Neals in the abbey, in the bell tower... where he likes to taste his wine.\n\r','You look to be in fine spirits! Come! Have a seat, and have a drink!\n\r','Let\'s see here...\n\r\n\rOh my! Milly\'s grapes have been saved! When she told me that brigands overran her vineyards I nearly despaired, but my faith in the Light did not waver!\n\r\n\rAnd through your bravery, we now have grapes for more wine! May the Light bless you, and keep you safe!\n\r',4,9,7,12,360,0,0,3,0),(13,'Report to Goldshire','Take Marshal McBride\'s Documents to Marshal Dughan in Goldshire.','You have proven interest in the security of Northshire. You are now tasked with the protection of the surrounding Elwynn Forest.\n\r\n\rIf you accept this duty, then I have prepared papers that must be delivered to Marshal Dughan in Goldshire. Goldshire lies along the southern road, past the border gates.\n\r','You have word from McBride? Northshire is a garden compared to Elwynn Forest, but I wonder what Marshal McBride has to report.\n\r\n\rHere, let me have his papers...\n\r','Well, it says here that you\'ve been awarded Acting Deputy Status with the Stormwind Marshals. Congratulations.\n\r\n\rAnd good luck - keeping Elwynn safe is no picnic... what with most the army busy doing who knows what for who knows which noble!\n\r\n\rIt\'s hard to keep track of politics in these dark days...\n\r',5,3,4,17,230,0,1,3,14),(14,'The Fargodeep Mine','Explore the Fargodeep Mine, then return to Marshal Dughan in Goldshire.','The mine in Northshire isn\'t the only one with problems! I have reports that the Fargodeep Mine in Elwynn has also become a haven for Kobolds.\n\r\n\rExplore the mine and confirm these reports, then return to me. The mine is almost due south of Goldshire, between the Stonefield and Maclure homesteads.\n\r','What do you have to report? Have you been to the Fargodeep Mine?\n\r','This is bad news. What\'s next, dragons?!? We\'ll have to increase our patrols near that mine. Thanks for your efforts. And hold a moment... I might have another task for you.\n\r',7,0,17,17,480,75,1,4,0);
/*!40000 ALTER TABLE `quests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `resets`
--

DROP TABLE IF EXISTS `resets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `resets` (
  `room_id` int(11) NOT NULL,
  `id` int(11) NOT NULL,
  `type` int(11) DEFAULT NULL,
  `target_id` int(11) DEFAULT NULL,
  `wander_dist` int(11) DEFAULT NULL,
  `leash_dist` int(11) DEFAULT NULL,
  `interval` int(11) DEFAULT NULL,
  PRIMARY KEY (`room_id`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `resets`
--

LOCK TABLES `resets` WRITE;
/*!40000 ALTER TABLE `resets` DISABLE KEYS */;
INSERT INTO `resets` VALUES (2,1,1,6,0,0,30),(3,1,1,4,0,0,30),(4,1,1,16,0,0,30),(4,2,1,14,0,0,30),(7,1,1,2,0,0,120),(7,2,1,3,0,0,120),(13,1,1,20,0,0,30),(14,1,1,21,0,0,30),(17,1,1,12,0,0,30),(18,1,1,15,0,0,30),(18,2,1,13,0,0,30),(23,1,1,11,0,0,30),(27,1,1,2,0,0,120),(28,1,1,2,0,0,120),(29,1,1,3,0,0,120),(30,1,1,3,0,0,120),(32,1,1,3,0,0,120),(33,1,1,2,0,0,120),(34,1,1,5,0,0,30),(35,1,1,2,0,0,120),(35,2,1,8,0,0,120),(36,1,1,8,0,0,120),(38,1,1,3,0,0,120),(40,1,1,9,0,0,120),(42,1,1,8,0,0,120),(43,1,1,8,0,0,120),(43,2,1,8,0,0,120),(46,1,1,9,0,0,120),(47,1,1,9,0,0,120),(48,1,1,9,0,0,120),(48,2,1,9,0,0,120),(49,1,1,10,0,0,120),(50,1,1,10,0,0,120),(62,1,1,2,0,0,120),(63,1,1,2,0,0,120),(65,1,1,2,0,0,120),(65,2,1,3,0,0,120),(68,1,1,8,0,0,120),(70,1,1,8,0,0,120),(72,1,1,8,0,0,120),(72,2,1,8,0,0,120),(74,1,1,9,0,0,120),(74,2,1,9,0,0,120),(76,1,1,9,0,0,120),(80,1,1,3,0,0,120),(84,1,1,7,0,0,30),(85,1,1,2,0,0,120),(86,1,1,3,0,0,120),(87,1,1,3,0,0,120),(88,1,1,23,0,0,120),(92,1,1,23,0,0,120),(94,1,1,22,0,0,120),(95,1,1,18,0,0,120),(95,2,1,23,0,0,120),(96,1,1,22,0,0,120),(96,2,1,23,0,0,120),(97,1,1,22,0,0,120),(97,2,1,23,0,0,120),(98,1,1,22,0,0,120),(100,1,1,23,0,0,120),(102,1,1,22,0,0,120),(106,1,1,22,0,0,120),(108,1,1,22,0,0,120),(109,1,1,10,0,0,120),(110,1,1,10,0,0,120),(111,1,1,10,0,0,120),(112,1,1,10,0,0,120),(113,1,1,10,0,0,120),(115,1,1,10,0,0,120),(116,1,1,10,0,0,120),(117,1,1,10,0,0,120),(118,1,1,10,0,0,120),(120,1,1,10,0,0,120);
/*!40000 ALTER TABLE `resets` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rooms`
--

DROP TABLE IF EXISTS `rooms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rooms` (
  `id` int(11) NOT NULL,
  `name` varchar(50) DEFAULT NULL,
  `description` text,
  `area` int(11) DEFAULT NULL,
  `flags` tinytext,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rooms`
--

LOCK TABLES `rooms` WRITE;
/*!40000 ALTER TABLE `rooms` DISABLE KEYS */;
INSERT INTO `rooms` VALUES (1,'The One Room','One Room to rule them all.\n\r',0,''),(2,'On the Steps of Northshire Abbey','',1,'0;'),(3,'Northshire Abbey','',1,''),(4,'Dermot\'s Wagon','',1,''),(5,'Northshire Valley','',1,''),(6,'Northshire Valley','',1,''),(7,'Northshire Valley','',1,''),(8,'Northshire Valley','',1,''),(9,'Northshire Road','',1,''),(10,'Northshire Abbey','',1,''),(11,'Northshire Abbey','',1,''),(12,'Main Hall','',1,''),(13,'Library Wing','',1,''),(14,'Hall of Arms','',1,''),(15,'Main Hall','',1,''),(16,'Northshire Abbey','',1,''),(17,'Northshire Abbey Bell Tower','',1,''),(18,'Dermot\'s Wagon','',1,''),(19,'Northshire Road','',1,''),(20,'Northshire Road','',1,''),(21,'Northshire Valley Gate','',1,''),(22,'Northshire Valley Gate','',1,''),(23,'Elwynn Forest Road','',2,''),(24,'Northshire Valley','',1,''),(25,'Northshire Valley','',1,''),(26,'Northshire Valley','',1,''),(27,'Northshire Valley','',1,''),(28,'Northshire Valley','',1,''),(29,'Northshire Valley','',1,''),(30,'Northshire Valley','',1,''),(31,'Northshire Valley','',1,''),(32,'Northshire Valley','',1,''),(33,'Northshire Valley','',1,''),(34,'Northshire Valley','',1,''),(35,'Northshire Valley','',1,''),(36,'Northshire Valley','',1,''),(37,'Northshire Valley','',1,''),(38,'Northshire Valley','',1,''),(39,'Northshire Valley','',1,''),(40,'Northshire Valley','',1,''),(41,'Northshire Valley','',1,''),(42,'Northshire Valley','',1,''),(43,'Northshire Valley','',1,''),(44,'Northshire Valley','',1,''),(45,'Northshire Valley','',1,''),(46,'Northshire Valley','',1,''),(47,'Northshire Valley','',1,''),(48,'Northshire Valley','',1,''),(49,'Entrance to Echo Ridge Mine','',1,''),(50,'Echo Ridge Mine','',1,''),(51,'Behind Northshire Abbey','',1,''),(52,'Behind Northshire Abbey','',1,''),(53,'Behind Northshire Abbey','',1,''),(54,'Northshire Valley','',1,''),(55,'Northshire Valley','',1,''),(56,'Northshire River','',1,''),(57,'Northshire River','',1,''),(58,'Northshire River','',1,''),(59,'A Bridge Over Northshire River','',1,''),(60,'Northshire River','',1,''),(61,'Northshire River','',1,''),(62,'Northshire Valley','',1,''),(63,'Northshire Valley','',1,''),(64,'Northshire Valley','',1,''),(65,'Northshire Valley','',1,''),(66,'An Impassable Waterfall','',1,''),(67,'Northshire Valley','',1,''),(68,'Northshire Valley','',1,''),(69,'Northshire Valley','',1,''),(70,'Northshire Valley','',1,''),(71,'Northshire Valley','',1,''),(72,'Northshire Valley','',1,''),(73,'Northshire Valley','',1,''),(74,'Northshire Valley','',1,''),(75,'Northshire Valley','',1,''),(76,'Northshire Valley','',1,''),(77,'Northshire Valley','',1,''),(78,'Northshire Valley','',1,''),(79,'Northshire Valley','',1,''),(80,'Northshire Valley','',1,''),(81,'Northshire Valley','',1,''),(82,'Northshire Valley','',1,''),(83,'Northshire Valley','',1,''),(84,'Northshire Stables','',1,''),(85,'Northshire Valley','',1,''),(86,'Northshire Valley','',1,''),(87,'Northshire Valley','',1,''),(88,'Northshire Vineyards','',1,''),(89,'Northshire Vineyards','',1,''),(90,'Northshire Vineyards','',1,''),(91,'Northshire Vineyards','',1,''),(92,'Northshire Vineyards','',1,''),(93,'Northshire Vineyards','',1,''),(94,'Northshire Vineyards','',1,''),(95,'Defias Camp','',1,''),(96,'Defias Camp','',1,''),(97,'Defias Camp','',1,''),(98,'Northshire Valley','',1,''),(99,'Northshire Valley','',1,''),(100,'Northshire Vineyards','',1,''),(101,'Northshire Vineyards','',1,''),(102,'Northshire Vineyards','',1,''),(103,'Northshire Vineyards','',1,''),(104,'Northshire Vineyards','',1,''),(105,'Northshire Vineyards','',1,''),(106,'Northshire Vineyards','',1,''),(107,'Northshire Vineyards','',1,''),(108,'Northshire Vineyards','',1,''),(109,'Echo Ridge Mine','',1,''),(110,'Echo Ridge Mine','',1,''),(111,'Echo Ridge Mine','',1,''),(112,'Echo Ridge Mine','',1,''),(113,'Echo Ridge Mine','',1,''),(114,'Echo Ridge Mine','',1,''),(115,'Echo Ridge Mine','',1,''),(116,'Echo Ridge Mine','',1,''),(117,'Echo Ridge Mine','',1,''),(118,'Echo Ridge Mine','',1,''),(119,'Echo Ridge Mine','',1,''),(120,'Echo Ridge Mine','',1,'');
/*!40000 ALTER TABLE `rooms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skills`
--

DROP TABLE IF EXISTS `skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skills` (
  `id` int(11) NOT NULL,
  `long_name` varchar(100) NOT NULL,
  `name` varchar(100) NOT NULL,
  `cooldown` double DEFAULT NULL,
  `target_type` int(11) DEFAULT NULL,
  `description` tinytext,
  `cost_description` tinytext,
  `cast_time` double DEFAULT NULL,
  `interrupt_flags` varchar(15) DEFAULT NULL,
  `function_name` varchar(100) DEFAULT NULL,
  `cast_script` text,
  `apply_script` text,
  `tick_script` text,
  `remove_script` text,
  `cost_script` text,
  PRIMARY KEY (`id`),
  UNIQUE KEY `long_name_UNIQUE` (`long_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skills`
--

LOCK TABLES `skills` WRITE;
/*!40000 ALTER TABLE `skills` DISABLE KEYS */;
INSERT INTO `skills` VALUES (1,'Recall','recall',1800,0,'Return to a preset location. Set with \"recall\" command.','100 mana',10,'0;1;','skill_recall','function skill_recall_cast(caster, target, skill)\n\r    caster:Message(\"|WYou have completed your spell.|X\", 0, null);\n\r    caster:Message(\"|W\" .. caster:GetName() .. \" has completed \" .. caster:HisHer() ..\" spell.|X\", 3, null);\n\r    caster:SetCooldown(skill, \"\", false, -1);\n\r    caster:ConsumeMana(100)\n\r    caster:Message(\"|W\" .. caster:GetName() .. \" disappears in a flash of light.|X\", 3, null);\n\r    caster:ChangeRoomsID(caster:GetPlayer().recall);\n\r    caster:Message(\"|W\" .. caster:GetName() .. \" appears in a flash of light.|X\", 3, null);\n\r    cmd_look(caster, \"\");\n\rend\n\r','','','','function skill_recall_cost(caster, target, skill)\n\r    if(caster:GetPlayer().recall == 0) then\n\r        caster:Message(\"You must set your recall location first.\", 0, null);\n\r        return 0;\n\r    end\n\r\n\r    if(not caster:HasResource(RESOURCE_MANA, 100)) then\n\r        caster:Message(\"You don\'t have enough mana.\", 0, null);\n\r        return 0;\n\r    end\n\r\n\r    return 1;\n\rend\n\r'),(2,'Fireball','fireball',0,2,'none',NULL,2.5,NULL,'skill_fireball','function skill_fireball_cast(caster, target, skill)\r\n\r\n    caster:Message(\"|WYou have completed your spell.|X\", 0, null);\r\n\r\n    caster:Message(\"|W\" .. caster:GetName() .. \" has completed \" .. caster:HisHer() ..\" spell.|X\", 3, null);\r\n\r\n    caster:Send(\"Your fireball hits \" .. target:GetName() .. \" for 10 damage.\\n\\r\");\r\n\r\n    target:Send(caster:GetName() .. \"\'s fireball hits you for 10 damage.\\n\\r\");\r\n\r\n    caster:Message(caster:GetName() .. \"\'s fireball hits \" .. target:GetName() .. \" for 10 damage.\", 5, target);\r\n\r\n    caster:EnterCombat(target);\r\n\r\n\r\n    target:AdjustHealth(caster, -10);\r\n\r\n    caster:ConsumeMana(25)\r\n\r\n\r\n    caster:SetCooldown(skill, \"\", false, 0)\r\n\r\n\r\nend\r\n\r\n\r\n','','','','function skill_fireball_cost(caster, target, skill)\r\n\r\n    return 1;\r\n\r\n\r\nend\r\n\r\n'),(3,'Heal','heal',0,4,'none',NULL,3,NULL,'skill_heal','function skill_heal_cast(caster, target, skill)\r\n\r\n    caster:Message(\"|WYou have completed your spell.|X\", 0, null);\r\n\r\n    caster:Message(\"|W\" .. caster:GetName() .. \" has completed \" .. caster:HisHer() ..\" spell.|X\", 3, null);\r\n\r\n\r\n    if(caster:IsFighting(target)) then\r\n\r\n        caster:Send(\"You can\'t heal the target you\'re attacking.\\n\\r\")\r\n\r\n        return\r\n\r\n    end\r\n\r\n    if(caster:GetName() == target:GetName()) then\r\n\r\n        caster:Send(\"You heal yourself for 20 health.\\n\\r\");\r\n\r\n    else\r\n\r\n        caster:Send(\"You heal \" .. target:GetName() .. \" for 20 health.\\n\\r\");\r\n\r\n        target:Send(caster:GetName() .. \"\'s heal restores 20 health.\\n\\r\");\r\n\r\n    end\r\n\r\n\r\n    caster:Message(caster:GetName() .. \"\'s spell heals \" .. target:GetName() .. \" for 20 health.\", 5, target);\r\n\r\n    target:AdjustHealth(caster, 20);\r\n\r\n    caster:SetCooldown(skill, \"\", false, 0)\r\n\r\n    caster:AdjustMana(caster, -20)\r\n\r\nend\r\n\r\n\r\n','','','','function skill_heal_cost(caster, target, skill)\r\n\r\n    if(not caster:HasResource(2, 12 + math.ceil(2.53*caster:GetPlayer():GetClassLevel(4)))) then\r\n\r\n        caster:Send(\"You don\'t have enough mana.\\n\\r\");\r\n\r\n        return 0;\r\n\r\n    end\r\n\r\n    return 1;\r\n\r\nend'),(4,'Frost Nova','frost nova',25,2,'Frozen in place.',NULL,0,NULL,'skill_frost_nova','function skill_frost_nova_cast(caster, target, sk)\r\n\r\n    caster:Message(\"|WYou cast frost nova.|X\", 0, null);\r\n\r\n    target:AddSpellAffect(1, caster, \"Frost Nova\", false, false, 0, 8.0, 1, sk);\r\n\r\n    caster:EnterCombat(target);\r\n\r\n    caster:SetCooldown(sk, \"\", false, -1)\r\n\r\n    caster:AdjustMana(caster, -200)\r\n\r\n\r\n\r\nend\r\n\r\n\r\n','function skill_frost_nova_apply(caster, target, affect)\r\n\r\n    caster:Send(target:GetName() .. \" is affected by your frost nova.\\n\\r\");\r\n\r\n    target:Send(\"|WYou are affected by \" .. caster:GetName() .. \"\'s frost nova.|X\\n\\r\");\r\n\r\n    caster:Message(\"|W\" .. target:GetName() .. \" is affected by \" .. caster:GetName() .. \"\'s frost nova.|X\", 5, target);\r\n\r\n    affect:ApplyAura(1, -100); --Movement speed minus 100%\r\n\r\nend\r\n\r\n','','function skill_frost_nova_remove(caster, target, affect)\r\n\r\n    target:Send(\"Frost nova fades from you.\\n\\r\");\r\n\r\n    if(caster ~= nil) then\r\n\r\n        caster:Send(\"Your frost nova fades from \" .. target:GetName() .. \".\\n\\r\");\r\n\r\n\r\n\r\n        caster:Message(caster:GetName() .. \"\'s frost nova fades from \" .. target:GetName() .. \".\", 5, target);\r\n\r\n    else\r\n\r\n        target:Message(affect:GetCasterName() .. \"\'s frost nova fades from \" .. target:GetName() .. \".\", 5, nil)\r\n\r\n    end\r\n\r\nend\r\n\r\n','function skill_frost_nova_cost(caster, target, skill)\r\n\r\n    return 1;\r\n\r\n\r\nend\r\n\r\n'),(5,'Renew','renew',0,4,'Heals the target instantly for 62% of intellect, and then 310% of intellect over 15 seconds','2% of mana',0,'0;','skill_renew','function skill_renew_cast(caster, target, sk)\n\r    caster:ConsumeMana(math.ceil(caster:GetMaxMana() * .02))\n\r    local healvalue = math.ceil(0.62 * caster:GetIntellect())\n\r    if(caster:GetName() ~= target:GetName()) then\n\r        caster:Message(\"|WYou cast renew on \" .. target:GetName() .. \". Your renew heals \" .. target:GetName() .. \" for \" .. healvalue .. \" health.|X\", MSG_CHAR, null)\n\r        target:Send(\"|W\" .. caster:GetName() .. \" casts renew on you. \" .. caster:GetName() .. \"\'s renew restores \" .. healvalue .. \" health.|X\\n\\r\")\n\r    else\n\r        target:Send(\"|WYou cast renew. Your renew restores \" .. healvalue .. \" health.|X\\n\\r\")\n\r    end\n\r    caster:Message(\"|W\" .. caster:GetName() .. \" casts renew on \" .. target:GetName() .. \".|X\", MSG_ROOM_NOTCHARVICT, target)\n\r    target:AdjustHealth(caster, healvalue)\n\r    target:AddSpellAffect(0, caster, \"Renew\", false, false, 5, 15.0, 1, sk, \"Healing \" .. healvalue .. \" health every 3 seconds\")\n\r    caster:SetCooldown(sk, \"\", false, -1)\n\rend\n\r','function skill_renew_apply(caster, target, affect)\n\r    local healvalue = math.ceil(0.62 * caster:GetIntellect())\n\r    affect:SaveDataInt(\"healvalue\", healvalue)\n\r\n\r    if(caster:GetName() == target:GetName()) then\n\r        target:Send(\"|WYou are affected by renew.|X\\n\\r\")\n\r    else\n\r        caster:Send(\"|W\" .. target:GetName() .. \" is affected by your renew.|X\\n\\r\")\n\r        target:Send(\"|WYou are affected by \" .. caster:GetName() .. \"\'s renew.|X\\n\\r\")\n\r    end\n\rend\n\r','function skill_renew_tick(caster, target, affect)\n\r    local healvalue = affect:GetDataInt(\"healvalue\")\n\r    if(affect:GetCasterName() == target:GetName()) then\n\r        target:Send(\"|WYour renew restores \" .. healvalue .. \" health.|X\\n\\r\")\n\r    else\n\r        if(caster ~= nil) then\n\r            caster:Send(\"|WYour renew heals \" .. target:GetName() .. \" for \" .. healvalue .. \" health.|X\\n\\r\")\n\r        end\n\r        target:Send(\"|W\" .. affect:GetCasterName() .. \"\'s renew restores \" .. healvalue .. \" health.|X\\n\\r\")\n\r    end\n\r    target:Message(\"|W\" .. affect:GetCasterName() .. \"\'s renew heals \" .. target:GetName() .. \" for \" .. healvalue .. \" health.|X\", MSG_ROOM_NOTCHARVICT, caster)\n\r    target:AdjustHealth(caster, healvalue)\n\rend\n\r','function skill_renew_remove(caster, target, affect)\n\r    if(affect:GetCasterName() == target:GetName()) then\n\r        target:Send(\"|WYour renew fades.|X\\n\\r\")\n\r    else\n\r        if(caster ~= nil) then\n\r            caster:Send(\"|WYour renew fades from \" .. target:GetName() .. \".|X\\n\\r\")\n\r        end\n\r        target:Send(\"|W\" .. affect:GetCasterName() .. \"\'s renew fades from you.|X\\n\\r\")\n\r    end\n\r    target:Message(\"|W\" .. affect:GetCasterName() .. \"\'s renew fades from \" .. target:GetName() .. \".|X\", MSG_ROOM_NOTCHARVICT, caster)\n\rend\n\r','function skill_renew_cost(caster, target, skill)\n\r    if(not caster:HasResource(RESOURCE_MANA, math.ceil(caster:GetMaxMana() * .02))) then\n\r        caster:Send(\"You don\'t have enough mana.\\n\\r\")\n\r        return 0\n\r    end\n\r    return 1\n\rend\n\r'),(6,'Hamstring','hamstring',0,2,'Movement speed reduced by 50%',NULL,0,NULL,'skill_hamstring','function skill_hamstring_cast(caster, target, sk)\r\n\r\n    target:AddSpellAffect(1, caster, \"Hamstring\", false, false, 0, 15.0, 0, sk)\r\n\r\n    caster:EnterCombat(target)\r\n\r\n    caster:AdjustStamina(caster, -(10 + (2*caster:GetPlayer():GetClassLevel(1))))\r\n\r\n    caster:SetCooldown(sk, \"\", false, 0)\r\n\r\nend\r\n\r\n','function skill_hamstring_apply(caster, target, affect)\r\n\r\n    caster:Send(target:GetName() .. \" is affected by your hamstring.\\n\\r\")\r\n\r\n    target:Send(\"You are affected by \" .. caster:GetName() .. \"\'s hamstring.\\n\\r\")\r\n\r\n    caster:Message(\"\" .. target:GetName() .. \" is affected by \" .. caster:GetName() .. \"\'s hamstring.\", 5, target);\r\n\r\n    affect:ApplyAura(1, -50);\r\n\r\n\r\nend\r\n\r\n','','function skill_hamstring_remove(caster, target, affect)\r\n\r\n\r\n    target:Send(\"Hamstring fades from you.\\n\\r\");\r\n\r\n    caster:Send(\"Your hamstring fades from \" .. target:GetName() .. \".\\n\\r\");\r\n\r\n\r\n    caster:Message(caster:GetName() .. \"\'s hamstring fades from \" .. target:GetName() .. \".\", 5, target);\r\n\r\n\r\nend\r\n\r\n','function skill_hamstring_cost(caster, target, skill)\r\n\r\n    if(not caster:HasResource(3, 10 + (2*caster:GetPlayer():GetClassLevel(1)))) then\r\n\r\n\r\n        caster:Send(\"You don\'t have enough stamina.\\n\\r\");\r\n\r\n        return 0;\r\n\r\n    end\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n\r\n'),(7,'Greater Heal','greater heal',0,4,'none',NULL,2.5,NULL,'skill_greater_heal','function skill_greater_heal_cast(caster, target, skill)\r\n\r\n    local manacost = 20 + (4*caster:GetPlayer():GetClassLevel(4));\r\n\r\n    local healamount = 100 + (15 * caster:GetPlayer():GetClassLevel(4));\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n    caster:Message(\"You have completed your spell.\", 0, null);\r\n\r\n    caster:Message(\"\" .. caster:GetName() .. \" has completed \" .. caster:HisHer() ..\" spell.\", 3, null);\r\n\r\n\r\n\r\n    if(caster:GetName() == target:GetName()) then\r\n\r\n        caster:Send(\"You heal yourself for \" .. healamount .. \" health.\\n\\r\");\r\n\r\n\r\n    else\r\n\r\n        caster:Send(\"You heal \" .. target:GetName() .. \" for \" .. healamount .. \" health.\\n\\r\");\r\n\r\n        target:Send(caster:GetName() .. \"\'s heal restores \" .. healamount .. \" health.\\n\\r\");\r\n\r\n\r\n    end\r\n\r\n\r\n    caster:Message(caster:GetName() .. \"\'s spell heals \" .. target:GetName() .. \" for \" .. healamount .. \" health.\", 5, target);\r\n\r\n\r\n    target:AdjustHealth(caster, healamount);\r\n\r\n\r\n    caster:SetCooldown(skill, \"\", false, 0)\r\n\r\n    caster:ConsumeMana(manacost);\r\n\r\n\r\n\r\n\r\nend\r\n\r\n\r\n','','','','function skill_greater_heal_cost(caster, target, skill)\r\n\r\n    if(not caster:HasResource(2, 20 + (4*caster:GetPlayer():GetClassLevel(4)))) then\r\n\r\n        caster:Send(\"You don\'t have enough mana.\\n\\r\");\r\n\r\n        return 0;\r\n\r\n\r\n    end\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(8,'Zot','zot',0,3,'Health, Mana, Stamina = 1',NULL,0,NULL,'skill_zot','function skill_zot_cast(caster, target, skill)\r\n\r\n    caster:Send(\"|YYou send a bolt of lightning to strike \" .. target:GetName() .. \".|X\\n\\r\");\r\n\r\n\r\n    target:Send(\"|YZOT! You are struck by a bolt of lightning!|X\\n\\r\");\r\n\r\n    target:Message(\"|YZOT! \" .. target:GetName() .. \" is struck by a bolt of lightning!|X\", 3, null);\r\n\r\n\r\n    target:AdjustHealth(caster, -(target:GetHealth() - 1));\r\n\r\n    target:AdjustMana(caster, -(target:GetMana() - 1));\r\n\r\n    target:AdjustStamina(caster, -(target:GetStamina() - 1));\r\n\r\nend\r\n','','','','function skill_zot_cost(caster, target, skill)\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(9,'Sprint','sprint',60,0,'Increases your movement speed by 70% for 8 seconds',NULL,0,NULL,'skill_sprint','function skill_sprint_cast(caster, target, sk)\r\n\r\n    caster:Message(\"You cast sprint.\", 0, null);\r\n\r\n    caster:Message(\"\" .. caster:GetName() .. \" casts sprint.\", 3, caster);\r\n\r\n    caster:AddSpellAffect(0, caster, \"Sprint\", false, false, 0, 8, 0, sk);\r\n\r\n    caster:SetCooldown(sk, \"\", false, -1);\r\n\r\n\r\nend\r\n\r\n','function skill_sprint_apply(caster, target, affect)\r\n\r\n    affect:ApplyAura(1, 70);\r\n\r\nend\r\n\r\n','','function skill_sprint_remove(caster, target, affect)\r\n\r\n    target:Send(\"You are no longer sprinting.\\n\\r\");\r\n\r\n    target:Message(affect:GetCasterName() .. \"\'s sprint fades from \" .. affect:GetCasterName() .. \".\", 3, target)\r\n\r\n\r\nend\r\n\r\n','function skill_sprint_cost(caster, target, skill)\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(10,'Lucifron\'s Curse','lucifrons curse',0,2,'Spell and ability costs are increased by 100%',NULL,0,NULL,'skill_lucifrons_curse','function skill_lucifrons_curse_cast(caster, target, skill)\r\n\r\n    caster:Message(\"You cast Lucifron\'s Curse.\", 0, null);\r\n\r\n    target:AddSpellAffect(1, caster, \"Lucifron\'s Curse\", false, false, 0, 300, 2, skill);\r\n\r\nend\r\n\r\n','function skill_lucifrons_curse_apply(caster, target, affect)\r\n\r\n    caster:Send(target:GetName() .. \" is affected by Lucifron\'s curse.\\n\\r\")\r\n\r\n    target:Send(\"You are affected by Lucifron\'s curse.\\n\\r\")\r\n\r\n    caster:Message(\"\" .. target:GetName() .. \" is affected by Lucifron\'s curse.\", 5, target);\r\n\r\n    affect:ApplyAura(2, 100);\r\n\r\nend\r\n\r\n','','function skill_lucifrons_curse_remove(caster, target, affect)\r\n\r\n\r\n\r\n    target:Send(\"Lucifron\'s curse fades from you.\\n\\r\");\r\n\r\n    caster:Send(\"Lucifron\'s curse fades from \" .. target:GetName() .. \".\\n\\r\");\r\n\r\n    caster:Message(\"Lucifron\'s curse fades from \" .. target:GetName() .. \".\", 5, target);\r\n\r\nend\r\n\r\n','function skill_lucifrons_curse_cost(caster, target, skill)\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(11,'Impending Doom','impending doom',0,2,'2000 Shadow damage inflicted after 10 sec.',NULL,0,NULL,'skill_impending_doom','function skill_impending_doom_cast(caster, target, skill)\r\n\r\n    caster:EnterCombat(target)\r\n\r\n    caster:Message(\"You cast Impending Doom.\", 0, null);\r\n\r\n    target:AddSpellAffect(1, caster, \"Impending Doom\", false, false, 1, 10, 1, skill);\r\n\r\nend\r\n\r\n','function skill_impending_doom_apply(caster, target, affect)\r\n\r\n    caster:Send(target:GetName() .. \" is affected by Impending Doom.\\n\\r\")\r\n\r\n    target:Send(\"You are affected by Impending Doom.\\n\\r\")\r\n\r\n    caster:Message(\"\" .. target:GetName() .. \" is affected by Impending Doom.\", 5, target);\r\n\r\nend\r\n\r\n','function skill_impending_doom_tick(caster, target, affect)\r\n\r\n    target:AdjustHealth(caster, -2000)\r\n\r\nend\r\n\r\n','function skill_impending_doom_remove(caster, target, affect)\r\n\r\n    target:Send(\"Impending Doom fades from you.\\n\\r\");\r\n\r\n    target:Message(\"Impending Doom fades from \" .. target:GetName() .. \".\", 3, target);\r\n\r\nend\r\n\r\n','function skill_impending_doom_cost(caster, target, skill)\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(12,'Remove Curse','remove curse',5,4,'Removes all curses from a friendly target',NULL,0,NULL,'skill_remove_curse','function skill_remove_curse_cast(caster, target, skill)\r\n\r\n    local removed = target:CleanseSpellAffect(caster, 2, -1);\r\n\r\n    caster:Send(\"Removed \" .. removed .. \" curses.\\n\\r\");\r\n\r\n    if(caster:GetName() ~= target:GetName()) then\r\n\r\n        target:Send(\"Removed \" .. removed .. \" curses.\\n\\r\");\r\n\r\n    end\r\n\r\nend\r\n\r\n','','','','function skill_remove_curse_cost(caster, target, skill)\r\n\r\n    if(not caster:HasResource(2, 250 + math.ceil(1.5*caster:GetPlayer():GetClassLevel(3)))) then\r\n\r\n        caster:Send(\"You don\'t have enough mana.\\n\\r\");\r\n\r\n        return 0;\r\n\r\n    end\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(13,'Purify','purify',5,4,'Dispels harmful magic and disease affects.',NULL,0,NULL,'skill_purify','function skill_purify_cast(caster, target, skill)\r\n\r\n    local magic = target:CleanseSpellAffect(caster, 1, -1);\r\n\r\n    local disease = target:CleanseSpellAffect(caster, 4, -1);\r\n\r\n    caster:Send(\"Removed \" .. magic .. \" magic affects.\\n\\r\");\r\n\r\n    caster:Send(\"Removed \" .. disease .. \" diseases.\\n\\r\");\r\n\r\n    if(caster:GetName() ~= target:GetName()) then\r\n\r\n        target:Send(\"Removed \" .. magic .. \" magic affects.\\n\\r\");\r\n\r\n        target:Send(\"Removed \" .. disease .. \" diseases.\\n\\r\");\r\n\r\n    end\r\n\r\nend\r\n','','','','function skill_purify_cost(caster, target, skill)\r\n\r\n    if(not caster:HasResource(2, 150 + math.ceil(1.75*caster:GetPlayer():GetClassLevel(4)))) then\r\n\r\n        caster:Send(\"You don\'t have enough mana.\\n\\r\");\r\n\r\n        return 0;\r\n\r\n    end\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(14,'Slow','slow',0,2,'Movement speed decreased by 80%, gradually increasing over spell duration',NULL,0,NULL,'skill_slow','function skill_slow_cast(caster, target, sk)\r\n\r\n    caster:Message(\"You cast slow on \" .. target:GetName() .. \".\", 0, null);\r\n\r\n    caster:Message(\"\" .. caster:GetName() .. \" casts slow on \" .. target:GetName() .. \".\", 3, caster);\r\n\r\n    target:AddSpellAffect(1, caster, \"Slow\", false, false, 5, 10, 0, sk);\r\n\r\n    caster:SetCooldown(sk, \"\", false, -1);\r\n\r\n    target:Send(\"You are affected by \" .. caster:GetName() .. \"\'s slow.\");\r\n\r\n    caster:EnterCombat(target);\r\n\r\n    caster:ConsumeMana(25)\r\n\r\nend\r\n\r\n\r\n\r\n','function skill_slow_apply(caster, target, affect)\r\n\r\n    affect:ApplyAura(1, -80);\r\n\r\n\r\nend\r\n\r\n','function skill_slow_tick(caster, target, affect)\r\n\r\n    -- need a RemoveAura function in affect\r\n\r\n    --need a GetRemainingDuration function in affect\r\n\r\n    -- need a GetCurrentTick() function in affect\r\n\r\n    -- todo: remove any movespeed aura, apply a movespeed of -80 + (15 * GetCurrentTick())\r\n\r\nend\r\n\r\n','function skill_slow_remove(caster, target, affect)\r\n\r\nend\r\n\r\n','function skill_slow_cost(caster, target, skill)\r\n\r\n    return 1;\r\n\r\nend\r\n\r\n'),(15,'Frostbolt','frostbolt',0,2,'Launches a bolt of frost at the enemy, causing 200% of intellect damage and slowing movement speed by 50% for 10 seconds','2% of mana',2,'0;','skill_frostbolt','function skill_frostbolt_cast(caster, target, sk)\n\r    caster:ConsumeMana(math.ceil(caster:GetMaxMana() * .02))\n\r    local damage = math.ceil(2 * caster:GetIntellect())\n\r    caster:Message(\"|WYou cast frostbolt on \" .. target:GetName() .. \". Your frostbolt hits \" .. target:GetName() .. \" for \" .. damage .. \" damage.|X\", MSG_CHAR, null)\n\r    target:Send(\"|W\" .. caster:GetName() .. \" casts frostbolt on you. \" .. caster:GetName() .. \"\'s frostbolt hits you for \" .. damage .. \" damage.|X\\n\\r\")\n\r    caster:Message(\"|W\" .. caster:GetName() .. \" casts frostbolt on \" .. target:GetName() .. \". \" .. caster:GetName() .. \"\'s frostbolt hits \" .. target:GetName() .. \" for \" .. damage .. \" damage.|X\", MSG_ROOM_NOTCHARVICT, target)\n\r    target:AddSpellAffect(1, caster, \"Chilled\", false, false, 0, 10, AFFECT_MAGIC, sk, \"Movement speed reduced by 50%\")\n\r    caster:SetCooldown(sk, \"\", false, -1)\n\r    caster:EnterCombat(target)\n\r    target:AdjustHealth(caster, -damage)\n\rend\n\r','function skill_frostbolt_apply(caster, target, affect)\n\r        affect:ApplyAura(AURA_MOVE_SPEED, -50);\n\r        target:Send(\"|WYou are affected by Chilled.|X\\n\\r\")\n\r        target:Message(\"|W\" .. target:GetName() .. \" is affected by Chilled.|X\", MSG_ROOM_NOTCHAR, nil)\n\rend\n\r','','function skill_frostbolt_remove(caster, target, affect)\n\r    target:Send(\"|WChilled fades from you.|X\\n\\r\")\n\r    target:Message(\"|WChilled fades from \" .. target:GetName() .. \".|X\", MSG_ROOM_NOTCHAR, nil)\n\rend\n\r','function skill_frostbolt_cost(caster, target, skill)\n\r    if(not caster:HasResource(RESOURCE_MANA, math.ceil(caster:GetMaxMana() * .02))) then\n\r        caster:Send(\"You don\'t have enough mana.\\n\\r\")\n\r        return 0\n\r    end\n\r    return 1\n\rend\n\r'),(16,'Slam','slam',0,2,'Slams an opponent, causing Physical damage','20 rage',0,'0;','skill_slam','','','','',''),(17,'Charge','',0,0,'',NULL,0,NULL,'','','','','',''),(18,'Victory Rush','',0,0,'',NULL,0,NULL,'','','','','',''),(19,'Execute','',0,0,'',NULL,0,NULL,'','','','','',''),(20,'Mortal Strike','',0,0,'',NULL,0,NULL,'','','','','',''),(21,'Sinister Strike','sinister strike',0,2,'An instant strike that causes 420% Physical damage. Awards 1 combo point.','40 energy',0,'0;','skill_sinister_strike','function skill_sinister_strike_cast(caster, target, sk)\n\r    local damage = math.ceil(caster:GetMainhandDamagePerSecond() * 2 + (caster:GetStrength() * .3))\n\r    caster:EnterCombat(target)\n\r    caster:ConsumeEnergy(40)\n\r    caster:GenerateComboPoint(target)\n\r    caster:Send(\"|WYour Sinister Strike hits \" .. target:GetName() .. \" for \" .. damage .. \" damage.|X\\n\\r\")\n\r    target:Send(\"|W\" .. caster:GetName() .. \"\'s Sinister Strike hits you for \" .. damage .. \" damage.|X\\n\\r\")\n\r    caster:Message(\"|W\" .. caster:GetName() .. \"\'s Sinister Strike hits \" .. target:GetName() .. \" for \" .. damage .. \" damage.|X\", MSG_ROOM_NOTCHARVICT, target)\n\r    caster:SetCooldown(sk, \"\", false, -1)\n\r    target:AdjustHealth(caster, -damage)\n\rend\n\r','','','','function skill_sinister_strike_cost(caster, target, skill)\n\r\n\r    if(not caster:HasResource(RESOURCE_ENERGY, 40)) then\n\r        caster:Send(\"You don\'t have enough energy.\\n\\r\")\n\r\n\r        return 0\n\r\n\r    end\n\r    return 1\n\r\n\rend\n\r'),(22,'Eviscerate','eviscerate',0,2,'Finishing move causing weapon damage plus 30% of strength per combo point.','35 Energy / 1 to 5 Combo Points',0,'0;','skill_eviscerate','function skill_eviscerate_cast(caster, target, sk)\n\r    local combos = caster:SpendComboPoints(target)\n\r    if(combos == 0) then\n\r        caster:Send(\"You don\'t have combo points on that target.\\n\\r\")\n\r        return\n\r    end\n\r    local damage = math.ceil((caster:GetMainhandDamagePerSecond() + caster:GetOffhandDamagePerSecond()\n\r                   + (caster:GetStrength() * .3)) * combos)\n\r    caster:ConsumeEnergy(35)\n\r    caster:EnterCombat(target)\n\r    caster:Send(\"|WYour Eviscerate hits \" .. target:GetName() .. \" for \" .. damage .. \" damage.|X\\n\\r\")\n\r    target:Send(\"|W\" .. caster:GetName() .. \"\'s Eviscerate hits you for \" .. damage .. \" damage.|X\\n\\r\")\n\r    caster:Message(\"|W\" .. caster:GetName() .. \"\'s Eviscerate hits \" .. target:GetName() .. \" for \" .. damage .. \" damage.|X\", MSG_ROOM_NOTCHARVICT, target)\n\r    caster:SetCooldown(sk, \"\", false, -1)\n\r    target:AdjustHealth(caster, -damage)\n\rend\n\r','','','','function skill_eviscerate_cost(caster, target, skill)\n\r    if(not caster:HasResource(RESOURCE_ENERGY, 35)) then\n\r        caster:Send(\"You don\'t have enough energy.\\n\\r\")\n\r        return 0\n\r    end\n\r    if(not caster:HasResource(RESOURCE_COMBO, 1)) then\n\r        caster:Send(\"You don\'t have any combo points.\\n\\r\")\n\r        return 0\n\r    end\n\r    return 1\n\rend\n\r'),(23,'Stealth','',0,0,'',NULL,0,NULL,'','','','','',''),(24,'Cheap Shot','',0,0,'',NULL,0,NULL,'','','','','',''),(25,'Backstab','',0,0,'',NULL,0,NULL,'','','','','',''),(26,'Smite','smite',0,2,'','NULL',2,'0;','skill_smite','','','','',''),(27,'Pain','',0,0,'',NULL,0,NULL,'','','','','',''),(28,'Flash Heal','',0,0,'',NULL,0,NULL,'','','','','',''),(29,'Shield','shield',0,4,'','NULL',0,'0;','skill_shield','','','','',''),(30,'Penance','',0,0,'',NULL,0,NULL,'','','','','',''),(31,'Fire Blast','',0,0,'',NULL,0,NULL,'','','','','',''),(32,'Ice Lance','ice lance',0,2,'','NULL',0,'0;','skill_ice_lance','','','','',''),(33,'Garrote','garrote',15,2,'Garrote the enemy, causing 220% of strength as bleed damage over 18 sec. Awards 1 combo point','45 energy',0,'0;','skill_garrote','function skill_garrote_cast(caster, target, sk)\n\r    local damage = caster:GetStrength()\n\r    caster:EnterCombat(target)\n\r    caster:ConsumeEnergy(45)\n\r    caster:GenerateComboPoint(target)\n\r    target:AddSpellAffect(1, caster, \"Garrote\", false, false, 9, 18, AFFECT_BLEED, sk, \"Suffering \" .. damage .. \" damage every 2 seconds\")\n\r    caster:SetCooldown(sk, \"\", false, -1)\n\rend\n\r','function skill_garrote_apply(caster, target, affect)\n\r    local damage = caster:GetStrength()\n\r    affect:SaveDataInt(\"damage\", damage)\n\r\n\r    caster:Send(\"|W\" .. target:GetName() .. \" is affected by your Garrote.|X\\n\\r\")\n\r    target:Send(\"|WYou are affected by \" .. caster:GetName() .. \"\'s Garrote.|X\\n\\r\")\n\r    target:Message(\"|W\" .. target:GetName() .. \" is affected by \" .. caster:GetName() .. \"\'s Garrote.|X\", MSG_ROOM_NOTCHARVICT, caster)\n\rend\n\r','function skill_garrote_tick(caster, target, affect)\n\r    local damage = affect:GetDataInt(\"damage\")\n\r\n\r    if(caster ~= nil) then\n\r        caster:Send(\"|WYour Garrote damages \" .. target:GetName() .. \" for \" .. damage .. \" health.|X\\n\\r\")\n\r    end\n\r    target:Send(\"|W\" .. affect:GetCasterName() .. \"\'s Garrote damages you for \" .. damage .. \" health.|X\\n\\r\")\n\r\n\r    target:Message(\"|W\" .. affect:GetCasterName() .. \"\'s Garrote damages \" .. target:GetName() .. \" for \" .. damage .. \" health.|X\", MSG_ROOM_NOTCHARVICT, caster)\n\r    target:AdjustHealth(caster, -damage)\n\rend\n\r','function skill_garrote_remove(caster, target, affect)\n\r    if(caster ~= nil) then\n\r        caster:Send(\"|WYour Garrote fades from \" .. target:GetName() .. \".|X\\n\\r\")\n\r    end\n\r    target:Send(\"|W\" .. affect:GetCasterName() .. \"\'s Garrote fades from you.|X\\n\\r\")\n\r    target:Message(\"|W\" .. affect:GetCasterName() .. \"\'s Garrote fades from \" .. target:GetName() .. \".|X\", MSG_ROOM_NOTCHARVICT, caster)\n\rend\n\r','function skill_garrote_cost(caster, target, skill)\n\r    if(not caster:HasResource(RESOURCE_ENERGY, 45)) then\n\r        caster:Send(\"You don\'t have enough energy.\\n\\r\")\n\r        return 0\n\r    end\n\r    return 1\n\rend\n\r'),(34,'Bloodthirst','bloodthirst',4.5,2,'','Generates 20 rage',0,'0;','skill_bloodthirst','','','','',''),(35,'Rend','rend',0,2,'','30 rage',0,'0;','skill_rend','','','','',''),(36,'Mana Shield','mana shield',0,0,'','',0,'0;','','','','','','');
/*!40000 ALTER TABLE `skills` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `triggers`
--

DROP TABLE IF EXISTS `triggers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `triggers` (
  `parent_id` int(11) NOT NULL,
  `id` int(11) DEFAULT NULL,
  `parent_type` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `argument` varchar(45) DEFAULT NULL,
  `script` text,
  `function` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`parent_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `triggers`
--

LOCK TABLES `triggers` WRITE;
/*!40000 ALTER TABLE `triggers` DISABLE KEYS */;
/*!40000 ALTER TABLE `triggers` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-02-01 19:51:21
