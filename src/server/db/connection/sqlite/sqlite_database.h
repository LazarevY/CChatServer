//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_SQLITE_DATABASE_H
#define CHATSERVER_SQLITE_DATABASE_H

#include <sqlite3.h>

typedef struct {
    char **data;
    int rows;
    int columns;
} QueryResult;

sqlite3* db_open(const char *filename);
void db_close(sqlite3 *db);
int db_execute(sqlite3 *db, const char *sql);
int db_execute_param(sqlite3 *db, const char *sql, int param_count, const char **param_values);
QueryResult* db_query(sqlite3 *db, const char *sql);
QueryResult* db_query_param(sqlite3 *db, const char *sql, int param_count, const char **param_values);
void db_free_query_result(QueryResult *result);

#endif //CHATSERVER_SQLITE_DATABASE_H
