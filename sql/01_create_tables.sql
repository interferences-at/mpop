CREATE TABLE IF NOT EXISTS tag 
(
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    is_in_use BOOLEAN NOT NULL DEFAULT 0,
    last_use TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    rfid VARCHAR(256) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS visitor
(
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    tag_id BIGINT UNSIGNED, -- Relation with the tag table
    gender ENUM ('male', 'female', 'other'),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS answer
(
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    visitor_id BIGINT UNSIGNED, -- relation with the table visitor
    question_identifier VARCHAR(100), -- ID of the question (see the questions file)
    value TINYINT UNSIGNED,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    /*
    question_id BIGINT, -- relation with the table question
    */
);

/*
CREATE TABLE IF NOT EXISTS question
(
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    identifier VARCHAR(256) NOT NULL UNIQUE,
    text_fr VARCHAR(256) NOT NULL DEFAULT "",
    text_en VARCHAR(256) NOT NULL DEFAULT "",
    text_min_fr VARCHAR(256) NOT NULL DEFAULT "",
    text_max_fr VARCHAR(256) NOT NULL DEFAULT "",
    text_min_en VARCHAR(256) NOT NULL DEFAULT "",
    text_max_en VARCHAR(256) NOT NULL DEFAULT ""
);
*/

