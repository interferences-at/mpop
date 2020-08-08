-- Create tables for the MPOP database


-- A physical RFID tag known by our system
CREATE TABLE IF NOT EXISTS tag
(
  `id` SERIAL,
  `rfid` VARCHAR(256) NOT NULL UNIQUE,
  `visitor_id` BIGINT UNSIGNED NULL, -- Each tag can be associated with a visitor (one at a time)
  `last_used_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) CHARACTER SET utf8 COLLATE utf8_general_ci;


-- A visitor in the exhibition
CREATE TABLE IF NOT EXISTS visitor
(
  `id` SERIAL,
  `gender` ENUM('male', 'female', 'other') DEFAULT NULL,
  `rfid` VARCHAR(256) DEFAULT NULL,
  `language` ENUM('fr', 'en') DEFAULT NULL,
  `nation` BIGINT UNSIGNED DEFAULT NULL, -- Relation with the nation table
  `age` TINYINT DEFAULT -1, -- number within the range [-1,127]
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) CHARACTER SET utf8 COLLATE utf8_general_ci;


-- An answer to a question from a visitor
--
-- NOTE: We don't need to remember which RFID tag a visited used.
CREATE TABLE IF NOT EXISTS answer
(
  `id` SERIAL,
  `visitor_id` BIGINT UNSIGNED NOT NULL,
  `question_id` BIGINT UNSIGNED NOT NULL,
  `answer_value` TINYINT UNSIGNED NOT NULL, -- number within the range [0,100]
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  -- maybe later: `is_latest` ENUM('yes', 'no') DEFAULT 'yes',
) CHARACTER SET utf8 COLLATE utf8_general_ci;


-- Questions that are answered by visitors.
--
-- These are stored in the database, so that we can do optimized JOIN queries,
-- but most of the information associated with each question - their text to display, etc.
-- is in QML models.
CREATE TABLE IF NOT EXISTS question
(
  `id` SERIAL,
  `identifier` VARCHAR(100) NOT NULL
) CHARACTER SET utf8 COLLATE utf8_general_ci;


-- Users have a nation/culture.
--
-- These are also stored in both the database and some QML models.
CREATE TABLE IF NOT EXISTS nation
(
  `id` SERIAL,
  `identifier` VARCHAR(100) NOT NULL
) CHARACTER SET utf8 COLLATE utf8_general_ci;

