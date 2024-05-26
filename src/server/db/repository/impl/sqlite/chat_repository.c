//
// Created by yaroslav on 5/25/24.
//

#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "db/repository/chat_repository.h"
#include "db/connection/sqlite/sqlite_database.h"
#include "env.h"

#ifndef USE_SQLITE


static sqlite3 *db = NULL;

// Reading all chats from the database
Chat *chat_repo_read_all() {
    const char *sql = "SELECT * FROM chats;";
    QueryResult *result = db_query(db, sql);
    if (!result) {
        return NULL;
    }

    int num_chats = result->rows;
    Chat *chats = (Chat *) malloc((num_chats + 1) * sizeof(Chat)); // Extra slot for NULL terminator
    for (int i = 0; i < num_chats; ++i) {
        chats[i].id = atoll(result->data[i * result->columns]);
        chats[i].name = strdup(result->data[i * result->columns + 1]);
        chats[i].desc = strdup(result->data[i * result->columns + 2]);
        chats[i].created_at = atoll(result->data[i * result->columns + 3]);
        chats[i].updated_at = atoll(result->data[i * result->columns + 4]);
    }
    // NULL terminate the array
    chats[num_chats].id = 0;

    db_free_query_result(result);
    return chats;
}

// Getting a chat by its ID
Chat *chat_repo_get_by_id(long_id id) {
    const char *sql = "SELECT * FROM chats WHERE id = ?;";
    char id_str[20];
    snprintf(id_str, sizeof(id_str), "%lld", id);
    const char *params[] = {id_str};
    QueryResult *result = db_query_param(db, sql, 1, params);
    if (!result || result->rows == 0) {
        return NULL;
    }

    Chat *chat = (Chat *) malloc(sizeof(Chat));
    chat->id = atoll(result->data[0]);
    chat->name = strdup(result->data[1]);
    chat->desc = strdup(result->data[2]);
    chat->created_at = atoll(result->data[3]);
    chat->updated_at = atoll(result->data[4]);

    db_free_query_result(result);
    return chat;
}

// Inserting a new chat into the database
long long chat_repo_insert(Chat *chat) {
    const char *sql = "INSERT INTO chats (name, desc, created_at, updated_at) VALUES (?, ?, ?, ?);";
    long long current = utils_time_in_milliseconds();
    char created_at_str[32], updated_at_str[32];
    snprintf(created_at_str, sizeof(created_at_str), "%lld", current);
    snprintf(updated_at_str, sizeof(updated_at_str), "%lld", current);
    const char *params[] = {chat->name, chat->desc, created_at_str, created_at_str};
    if (db_execute_param(db, sql, 4, params) != SQLITE_OK) {
        return 0;
    }
    return sqlite3_last_insert_rowid(db);
}

// Updating an existing chat in the database
void chat_repo_update(Chat *chat) {
    const char *sql = "UPDATE chats SET name = ?, desc = ?, updated_at = ? WHERE id = ?;";
    long long current = utils_time_in_milliseconds();
    char id_str[32];
    char updated_at_str[32];
    snprintf(updated_at_str, sizeof(updated_at_str), "%lld", current);
    snprintf(id_str, sizeof(id_str), "%lld", chat->id);
    const char *params[] = {chat->name, chat->desc, updated_at_str, id_str};
    db_execute_param(db, sql, 4, params);
}

// Deleting a chat from the database by its ID
void chat_repo_delete(long_id id) {
    const char *sql = "DELETE FROM chats WHERE id = ?;";
    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", id);
    const char *params[] = {id_str};
    db_execute_param(db, sql, 1, params);
}

void chat_repo_close_repo() {
    db_close(db);
    printf("Closed chat_repo..\n");
}

void chat_repo_start_repo() {
    printf("Started chat repo..\n");
    db = db_open(SQLITE_PATH);
}

#endif
