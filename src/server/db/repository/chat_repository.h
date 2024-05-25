//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_CHAT_REPOSITORY_H
#define CHATSERVER_CHAT_REPOSITORY_H

#include "entity/chat.h"

void chat_repo_start_repo();

Chat *chat_repo_read_all();

Chat *chat_repo_get_by_id(long_id id);

long_id chat_repo_insert(Chat *chat);

void chat_repo_update(Chat *chat);

void chat_repo_delete(long_id id);

void chat_repo_close_repo();

#endif //CHATSERVER_CHAT_REPOSITORY_H
