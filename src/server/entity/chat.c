//
// Created by yaroslav on 5/25/24.
//
#include <malloc.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
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

Chat* chat_from_json(const char *json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL) {
        return NULL;
    }

    Chat *chat = malloc(sizeof(Chat));
    if (!chat) {
        cJSON_Delete(json);
        return NULL;
    }

    PARSE_JSON_FIELD_DOUBLE(chat, id, json)
    PARSE_JSON_FIELD_STRING(chat, name, json)
    PARSE_JSON_FIELD_STRING(chat, desc, json)
    PARSE_JSON_FIELD_DOUBLE(chat, created_at, json)
    PARSE_JSON_FIELD_DOUBLE(chat, updated_at, json)

    cJSON_Delete(json);
    return chat;
}

char* chat_to_json(const Chat *chat) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        return NULL;
    }

    cJSON_AddNumberToObject(json, "id", chat->id);
    cJSON_AddStringToObject(json, "name", chat->name);
    cJSON_AddStringToObject(json, "desc", chat->desc);
    cJSON_AddNumberToObject(json, "created_at", chat->created_at);
    cJSON_AddNumberToObject(json, "updated_at", chat->updated_at);

    char *json_str = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_str;
}

Chat* chats_from_json_array(const char *json_str, size_t *count) {
    cJSON *json_array = cJSON_Parse(json_str);
    if (json_array == NULL || !cJSON_IsArray(json_array)) {
        return NULL;
    }

    size_t array_size = cJSON_GetArraySize(json_array);
    Chat *chats = malloc(array_size * sizeof(Chat));
    if (!chats) {
        cJSON_Delete(json_array);
        return NULL;
    }

    cJSON *json;
    size_t index = 0;
    cJSON_ArrayForEach(json, json_array) {
        if (index >= array_size) {
            break; // Prevent overflow
        }
        Chat *chat = chat_from_json(json);
        if (!chat) {
            continue; // Skip invalid messages
        }
        chats[index++] = *chat;
        free(chat); // Free allocated memory
    }

    *count = index;
    cJSON_Delete(json_array);
    return chats;
}

char* chats_to_json_array(const Chat *chats) {
    cJSON *json_array = cJSON_CreateArray();
    if (!json_array) {
        return NULL;
    }

    for (size_t i = 0; chats[i].id > 0; ++i) {
        char *chat_json = chat_to_json(&chats[i]);
        if (!chat_json) {
            cJSON_Delete(json_array);
            return NULL;
        }
        cJSON *json = cJSON_Parse(chat_json);
        cJSON_AddItemToArray(json_array, json);
        free(chat_json);
    }

    char *json_str = cJSON_PrintUnformatted(json_array);
    cJSON_Delete(json_array);
    return json_str;
}
