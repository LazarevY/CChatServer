//
// Created by yaroslav on 5/25/24.
//


#include "sqlite_database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sqlite3 *db_open(const char *filename) {
    sqlite3 *db;
    int rc = sqlite3_open(filename, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

void db_close(sqlite3 *db) {
    sqlite3_close(db);
}

int db_execute(sqlite3 *db, const char *sql) {
    char *errMsg = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return SQLITE_OK;
}


int db_execute_param(sqlite3 *db, const char *sql, int param_count, const char **param_values) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    for (int i = 0; i < param_count; ++i) {
        rc = sqlite3_bind_text(stmt, i + 1, param_values[i], -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Parameter binding error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(db));
    }

    rc = sqlite3_finalize(stmt);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Finalize error: %s\n", sqlite3_errmsg(db));
    }
    return rc;
}

static int query_callback(void *data, int argc, char **argv, char **azColName) {
    QueryResult *result = (QueryResult *) data;
    if (result->columns == 0) {
        result->columns = argc;
        result->data = (char **) malloc(argc * sizeof(char *));
        for (int i = 0; i < argc; i++) {
            result->data[i] = strdup(azColName[i]);
        }
    }
    result->rows++;
    result->data = (char **) realloc(result->data, (result->rows * result->columns + result->columns) * sizeof(char *));
    for (int i = 0; i < argc; i++) {
        result->data[result->rows * result->columns + i] = argv[i] ? strdup(argv[i]) : NULL;
    }
    return 0;
}

QueryResult *db_query(sqlite3 *db, const char *sql) {
    QueryResult *result = (QueryResult*)malloc(sizeof(QueryResult));
    result->data = NULL;
    result->rows = 0;
    result->columns = 0;

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        free(result);
        return NULL;
    }

    result->columns = sqlite3_column_count(stmt);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        result->rows++;
        result->data = (char**)realloc(result->data, result->rows * result->columns * sizeof(char*));
        for (int i = 0; i < result->columns; ++i) {
            const char *col_data = (const char*)sqlite3_column_text(stmt, i);
            result->data[(result->rows - 1) * result->columns + i] = col_data ? strdup(col_data) : NULL;
        }
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return result;
}

QueryResult *db_query_param(sqlite3 *db, const char *sql, int param_count, const char **param_values) {
    QueryResult *result = (QueryResult*)malloc(sizeof(QueryResult));
    result->data = NULL;
    result->rows = 0;
    result->columns = 0;

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        free(result);
        return NULL;
    }

    for (int i = 0; i < param_count; ++i) {
        rc = sqlite3_bind_text(stmt, i + 1, param_values[i], -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Parameter binding error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            free(result);
            return NULL;
        }
    }

    result->columns = sqlite3_column_count(stmt);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        result->rows++;
        result->data = (char**)realloc(result->data, result->rows * result->columns * sizeof(char*));
        for (int i = 0; i < result->columns; ++i) {
            const char *col_data = (const char*)sqlite3_column_text(stmt, i);
            result->data[(result->rows - 1) * result->columns + i] = col_data ? strdup(col_data) : NULL;
        }
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return result;
}


void db_free_query_result(QueryResult *result) {
    if (result) {
        for (int i = 0; i < (result->rows) * result->columns; i++) {
            free(result->data[i]);
        }
        free(result->data);
        free(result);
    }
}
