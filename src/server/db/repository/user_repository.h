//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_USER_REPOSITORY_H
#define CHATSERVER_USER_REPOSITORY_H

#include "entity/user.h"

void user_repo_start_repo();
User *user_repo_read_all();
User *user_repo_get_by_id(long_id id);
long_id user_repo_insert(User *user);
void user_repo_update(User *user);
void user_repo_delete(long_id id);
void user_repo_close_repo();

#endif //CHATSERVER_USER_REPOSITORY_H
