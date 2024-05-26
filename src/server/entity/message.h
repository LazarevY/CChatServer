//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_MESSAGE_H
#define CHATSERVER_MESSAGE_H

#include <stddef.h>
#include "utils.h"

typedef struct {
    long_id id;
    long_id user_id;
    char *text;
    long long timestamp;
    long_id reply_id;
} Message;


Message* message_from_json(const char *json_str);
char* message_to_json(const Message *message);
Message* messages_from_json_array(const char *json_str, size_t *count);
char* messages_to_json_array(const Message *messages, size_t count);

#endif //CHATSERVER_MESSAGE_H
