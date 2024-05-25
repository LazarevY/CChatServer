//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_MESSAGE_REPOSITORY_H
#define CHATSERVER_MESSAGE_REPOSITORY_H

#include "entity/message.h"

void msg_repo_start_repo();

Message *msg_repo_read_all();

Message *msg_repo_get_by_id(long_id id);

long_id msg_repo_insert(Message *chat);

void msg_repo_update(Message *chat);

void msg_repo_delete(long_id id);

void msg_repo_close_repo();

#endif //CHATSERVER_MESSAGE_REPOSITORY_H
