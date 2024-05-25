//
// Created by yaroslav on 5/25/24.
//
#include <malloc.h>
#include "chat.h"

void chat_free_object(Chat *chat) {
    if (chat) {
        free(chat->name);
        free(chat->desc);
    }
}

void chat_free_objects(Chat *chat) {
    for (int i = 0; chat[i].id != 0 ; ++i) {
        chat_free_object(&chat[i]);
    }
}

