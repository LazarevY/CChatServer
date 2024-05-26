//
// Created by yaroslav on 5/25/24.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "db/repository/user_repository.h"
#include "db/connection/sqlite/sqlite_database.h"

static sqlite3 *db = NULL;

User *user_repo_read_all() {
    const char *sql = "SELECT * FROM users;";
    QueryResult *result = db_query(db, sql);
    if (!result) {
        return NULL;
    }

    int num_users = result->rows;
    User *users = (User*)malloc((num_users + 1) * sizeof(User)); // Extra slot for NULL terminator
    for (int i = 0; i < num_users; ++i) {
        users[i].id = atoll(result->data[i * result->columns]);
        users[i].name = strdup(result->data[i * result->columns + 1]);
        users[i].created_at = atoll(result->data[i * result->columns + 2]);
        users[i].updated_at = atoll(result->data[i * result->columns + 3]);
    }
    // NULL terminate the array
    users[num_users].id = 0;

    db_free_query_result(result);
    return users;
}

// Getting a user by its ID
User *user_repo_get_by_id(long_id id) {
    const char *sql = "SELECT * FROM users WHERE ID = ?;";
    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", id);
    const char *params[] = {id_str};
    QueryResult *result = db_query_param(db, sql, 1, params);
    if (!result || result->rows == 0) {
        db_free_query_result(result);
        return NULL;
    }

    User *user = (User*)malloc(sizeof(User));
    user->id = atoll(result->data[0]);
    user->name = strdup(result->data[1]);
    user->created_at = atoll(result->data[2]);
    user->updated_at = atoll(result->data[3]);

    db_free_query_result(result);
    return user;
}

// Inserting a new user into the database
long_id user_repo_insert(User *user) {
    const char *sql = "INSERT INTO users (name, created_at, updated_at) VALUES (?, ?, ?);";
    long long current = utils_time_in_milliseconds();
    char created_at_str[32], updated_at_str[32];
    snprintf(created_at_str, sizeof(created_at_str), "%lld", current);
    snprintf(updated_at_str, sizeof(updated_at_str), "%lld", current);
    const char *params[] = {user->name, created_at_str, updated_at_str};
    if (db_execute_param(db, sql, 3, params) != SQLITE_OK) {
        return 0;
    }
    return sqlite3_last_insert_rowid(db);
}

// Updating an existing user in the database
void user_repo_update(User *user) {
    const char *sql = "UPDATE users SET name = ?,  = ? WHERE ID = ?;";
    long long current = utils_time_in_milliseconds();
    char id_str[32], created_at_str[32], updated_at_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", user->id);
    snprintf(updated_at_str, sizeof(updated_at_str), "%lld", current);
    const char *params[] = {user->name, created_at_str, updated_at_str, id_str};
    db_execute_param(db, sql, 4, params);
}

// Deleting a user from the database by its ID
void user_repo_delete(long_id id) {
    const char *sql = "DELETE FROM users WHERE ID = ?;";
    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", id);
    const char *params[] = {id_str};
    db_execute_param(db, sql, 1, params);
}