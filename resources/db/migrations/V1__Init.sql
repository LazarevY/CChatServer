CREATE TABLE IF NOT EXISTS chats (
    id INTEGER PRIMARY KEY AUTOINCREMENT ,
    name VARCHAR(255),
    desc VARCHAR(255),
    created_at INTEGER NOT NULL,
    updated_AT INTEGER NOT NULL
);

CREATE TABLE messages (
                         id INTEGER PRIMARY KEY AUTOINCREMENT,
                         user_id INTEGER NOT NULL,
                         text TEXT NOT NULL,
                         timestamp INTEGER NOT NULL,
                         reply_id INTEGER
);