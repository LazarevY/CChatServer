//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_USER_H
#define CHATSERVER_USER_H

#include "utils.h"

typedef struct {
    long_id id;
    char *name;
    long long created_at;
    long long updated_at;
} User ;
#endif //CHATSERVER_USER_H
