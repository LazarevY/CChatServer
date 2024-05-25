//
// Created by yaroslav on 5/25/24.
//

#include <stdio.h>
#include <malloc.h>
#include "server/db/repository/chat_repository.h"

void print_chat(Chat *chat) {
    if (chat) {
        printf("ID: %lld, Name: %s, Description: %s, Created: %lld, Updated: %lld\n", chat->id, chat->name, chat->desc, chat->created_at, chat->updated_at);
    } else {
        printf("Invalid chat\n");
    }
}

int main() {
    chat_repo_start_repo();

    Chat chat  = {0, "ada", "adsdd"};

    chat_repo_insert(&chat);

    Chat* chats = chat_repo_read_all();

    if (chats) {
        printf("All Chats:\n");
        for (int i = 0; chats[i].id != 0 ; ++i) {
            print_chat(&chats[i]);
        }
    } else {
        printf("Failed to read chats from the database\n");
    }

    chat_free_objects(chats);

    chat_repo_close_repo();
    return 0;
}