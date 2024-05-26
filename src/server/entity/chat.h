//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_CHAT_H
#define CHATSERVER_CHAT_H

#include <string.h>
#include "utils.h"

typedef struct {
    long_id id;
    char *name;
    char *desc;
    long long created_at; // Timestamp
    long long updated_at; // Timestamp
} Chat;

void chat_free_object(Chat *chat);
void chat_free_objects(Chat *chat);

Chat* chat_from_json(const char *json_str);
char* chat_to_json(const Chat *chat);
Chat* chats_from_json_array(const char *json_str, size_t *count);
char* chats_to_json_array(const Chat *chats);

#endif //CHATSERVER_CHAT_H
