-- Create tables for the MPOP database

-- A physical RFID tag known by our system
CREATE TABLE IF NOT EXISTS tag
(
  `id` SERIAL,
  `rfid` VARCHAR(256) NOT NULL UNIQUE,
  `visitor_id` BIGINT UNSIGNED NULL UNIQUE,
  `last_used_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
);

-- A visitor in the exhibition
CREATE TABLE IF NOT EXISTS visitor
(
  `id` SERIAL,
  `gender` ENUM('male', 'female', 'other') DEFAULT NULL,
  `language` ENUM('fr', 'en') DEFAULT NULL,
  `nation` ENUM('quebecer', 'canadian', 'american', 'european', 'native', 'other') NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `age` TINYINT UNSIGNED DEFAULT NULL, -- number within the range [0,255]
  CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
);

-- An answer to a question from a visitor
CREATE TABLE IF NOT EXISTS answer
(
  `id` SERIAL,
  `visitor_id` BIGINT UNSIGNED NOT NULL,
  `question_identifier` VARCHAR(100) DEFAULT NULL,
  `answer_value` TINYINT UNSIGNED NOT NULL, -- number within the range [0,100]
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
);

