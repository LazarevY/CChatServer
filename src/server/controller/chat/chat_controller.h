//
// Created by yaroslav on 5/26/24.
//

#ifndef CHATSERVER_CHAT_CONTROLLER_H
#define CHATSERVER_CHAT_CONTROLLER_H
#include "mongoose.h"


// Define your request handler functions here
void chat_controller_start();
void chat_controller_stop();

int chat_controller_handle_get_chats(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int chat_controller_handle_get_chat_by_id(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int chat_controller_handle_create_chat(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int chat_controller_handle_update_chat(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int chat_controller_handle_delete_chat_by_id(struct mg_connection *nc, struct mg_http_message *hm, char **params);

#endif //CHATSERVER_CHAT_CONTROLLER_H
