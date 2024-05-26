//
// Created by yaroslav on 5/25/24.
//
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "db/repository/message_repository.h"
#include "db/connection/sqlite/sqlite_database.h"
#include "env.h"

#ifndef USE_SQLITE


static sqlite3 *db = NULL;

Message *msg_repo_read_all() {
    const char *sql = "SELECT * FROM messages;";
    QueryResult *result = db_query(db, sql);
    if (!result) {
        return NULL;
    }

    int num_messages = result->rows;
    Message *messages = (Message*)malloc((num_messages + 1) * sizeof(Message)); // Extra slot for NULL terminator
    for (int i = 0; i < num_messages; ++i) {
        messages[i].id = atoll(result->data[i * result->columns]);
        messages[i].user_id = atoll(result->data[i * result->columns + 1]);
        messages[i].text = strdup(result->data[i * result->columns + 2]);
        messages[i].timestamp = atoll(result->data[i * result->columns + 3]);
        messages[i].reply_id = result->data[i * result->columns + 4] ? atoll(result->data[i * result->columns + 4]) : 0;
    }
    // NULL terminate the array
    messages[num_messages].id = 0;

    db_free_query_result(result);
    return messages;
}

// Getting a message by its ID
Message *msg_repo_get_by_id(long_id id) {
    const char *sql = "SELECT * FROM messages WHERE id = ?;";
    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", id);
    const char *params[] = {id_str};
    QueryResult *result = db_query_param(db, sql, 1, params);
    if (!result || result->rows == 0) {
        db_free_query_result(result);
        return NULL;
    }

    Message *message = (Message*)malloc(sizeof(Message));
    message->id = atoll(result->data[0]);
    message->user_id = atoll(result->data[1]);
    message->text = strdup(result->data[2]);
    message->timestamp = atoll(result->data[3]);
    message->reply_id = result->data[4] ? atoll(result->data[4]) : 0;

    db_free_query_result(result);
    return message;
}

// Inserting a new message into the database
long_id msg_repo_insert(Message *message) {
    const char *sql = "INSERT INTO messages (user_id, text, timestamp, reply_id) VALUES (?, ?, ?, ?);";
    char user_id_str[32], timestamp_str[32], reply_id_str[32];
    snprintf(user_id_str, sizeof(user_id_str), "%lld", message->user_id);
    snprintf(timestamp_str, sizeof(timestamp_str), "%lld", message->timestamp);
    snprintf(reply_id_str, sizeof(reply_id_str), "%lld", message->reply_id);
    const char *params[] = {user_id_str, message->text, timestamp_str, reply_id_str};
    if (db_execute_param(db, sql, 4, params) != SQLITE_OK) {
        return 0;
    }
    return sqlite3_last_insert_rowid(db);
}

void msg_repo_update(Message *message) {
    const char *sql = "UPDATE messages SET user_id = ?, text = ?, timestamp = ?, reply_id = ? WHERE id = ?;";
    char id_str[32], user_id_str[32], timestamp_str[32], reply_id_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", message->id);
    snprintf(user_id_str, sizeof(user_id_str), "%lld", message->user_id);
    snprintf(timestamp_str, sizeof(timestamp_str), "%lld", message->timestamp);
    snprintf(reply_id_str, sizeof(reply_id_str), "%lld", message->reply_id);
    const char *params[] = {user_id_str, message->text, timestamp_str, reply_id_str, id_str};
    db_execute_param(db, sql, 5, params);
}

void msg_repo_delete(long_id id) {
    const char *sql = "DELETE FROM messages WHERE id = ?;";
    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", id);
    const char *params[] = {id_str};
    db_execute_param(db, sql, 1, params);
}

void msg_repo_start_repo() {
    db_close(db);
    printf("Closed message repo..\n");
}

void msg_repo_close_repo() {
    printf("Started message repo..\n");
    db = db_open(SQLITE_PATH);
}

#endif