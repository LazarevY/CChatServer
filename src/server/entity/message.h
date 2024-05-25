//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_MESSAGE_H
#define CHATSERVER_MESSAGE_H

#include "utils.h"

typedef struct {
    long_id id;
    long_id user_id;
    char *text;
    long long timestamp;
    long_id reply_id;
} Message;
#endif //CHATSERVER_MESSAGE_H
