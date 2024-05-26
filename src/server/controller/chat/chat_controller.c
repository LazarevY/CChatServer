//
// Created by yaroslav on 5/26/24.
//

#include "chat_controller.h"
#include "db/repository/chat_repository.h"

int chat_controller_handle_get_chats(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    Chat *chats = chat_repo_read_all();
    if (!chats) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error reading chats");
        return 0;
    }

    char *response = chats_to_json_array(chats);
    if (!response) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error creating JSON response");
        return 0;
    }

    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
    free(response);

    chat_free_objects(chats);
    return 0;
}

int chat_controller_handle_get_chat_by_id(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    long_id id = atoll(params[0]);
    Chat *chat = chat_repo_get_by_id(id);
    if (!chat) {
        mg_http_reply(nc, 404, "Content-Type: text/plain\r\n", "Chat not found");
        return 0;
    }

    char *response = chat_to_json(chat);
    if (!response) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error creating JSON response");
        return 0;
    }

    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
    free(response);

    chat_free_object(chat);
    return 0;
}

int chat_controller_handle_create_chat(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    // Extract the JSON body from the HTTP request
    char json_body[hm->body.len + 1];
    strncpy(json_body, hm->body.buf, hm->body.len);
    json_body[hm->body.len] = '\0';

    // Parse JSON to Chat object
    Chat *chat = chat_from_json(json_body);
    if (!chat) {
        mg_http_reply(nc, 400, "", "Invalid JSON\n");
        return 0;
    }

    // Insert Chat object into the database
    long_id chat_id = chat_repo_insert(chat);
    if (chat_id == 0) {
        mg_http_reply(nc, 500, "", "Failed to insert chat\n");
        chat_free_object(chat);
        return 0;
    }

    chat = chat_repo_get_by_id(chat_id);

    // Convert the Chat object back to JSON
    char *response_json = chat_to_json(chat);
    if (!response_json) {
        mg_http_reply(nc, 500, "", "Failed to serialize chat\n");
        chat_free_object(chat);
        return 0;
    }

    // Send the JSON response
    mg_http_reply(nc, 201, "Content-Type: application/json\r\n", "%s", response_json);

    // Clean up
    free(response_json);
    chat_free_object(chat);
    return 0;
}


void chat_controller_start() {
    chat_repo_start_repo();
}

void chat_controller_stop() {
    chat_repo_close_repo();
}
