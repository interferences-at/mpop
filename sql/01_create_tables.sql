CREATE TABLE tag
(
    id BIGSERIAL PRIMARY KEY,
    rfid VARCHAR(256) NOT NULL UNIQUE
);

CREATE TABLE visitor
(
    id BIGSERIAL PRIMARY KEY,
    tag_id BIGINT -- Relation with the tag table
    gender ENUM (male, female, other),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
);

CREATE TABLE question
(
    id BIGSERIAL PRIMARY KEY,
    identifier VARCHAR(256) NOT NULL UNIQUE,
    text_fr VARCHAR(256) NOT NULL UNIQUE,
    text_en VARCHAR(256) NOT NULL UNIQUE
);

CREATE TABLE answer
(
    id BIGSERIAL PRIMARY KEY,
    visitor_id BIGINT, -- relation with the table visitor
    question_id BIGINT, -- relation with the table question
    value TINYINT UNSIGNED,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

