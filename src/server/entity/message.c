//
// Created by yaroslav on 5/26/24.
//
#include "message.h"
#include <cjson/cJSON.h>
#include <malloc.h>
#include <string.h>

Message* message_from_json(const char *json_str) {
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL) {
        return NULL;
    }

    Message *message = malloc(sizeof(Message));
    if (!message) {
        cJSON_Delete(json);
        return NULL;
    }

    PARSE_JSON_FIELD_DOUBLE(message, id, json);
    PARSE_JSON_FIELD_DOUBLE(message, user_id, json);
    PARSE_JSON_FIELD_STRING(message, text, json);
    PARSE_JSON_FIELD_DOUBLE(message, timestamp, json);
    PARSE_JSON_FIELD_DOUBLE(message, reply_id, json);

    cJSON_Delete(json);
    return message;
}

char* message_to_json(const Message *message) {
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        return NULL;
    }

    ADD_JSON_NUMBER(json, id , message);
    ADD_JSON_NUMBER(json, user_id , message);
    ADD_JSON_STRING(json, text , message);
    ADD_JSON_NUMBER(json, timestamp , message);
    ADD_JSON_NUMBER(json, reply_id , message);

    char *json_str = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_str;
}

Message* messages_from_json_array(const char *json_str, size_t *count) {
    cJSON *json_array = cJSON_Parse(json_str);
    if (json_array == NULL || !cJSON_IsArray(json_array)) {
        return NULL;
    }

    size_t array_size = cJSON_GetArraySize(json_array);
    Message *messages = malloc(array_size * sizeof(Message));
    if (!messages) {
        cJSON_Delete(json_array);
        return NULL;
    }

    cJSON *json;
    size_t index = 0;
    cJSON_ArrayForEach(json, json_array) {
        if (index >= array_size) {
            break; // Prevent overflow
        }
        Message *message = message_from_json(json);
        if (!message) {
            continue; // Skip invalid messages
        }
        messages[index++] = *message;
        free(message); // Free allocated memory
    }

    *count = index;
    cJSON_Delete(json_array);
    return messages;
}

char* messages_to_json_array(const Message *messages, size_t count) {
    cJSON *json_array = cJSON_CreateArray();
    if (!json_array) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        char *message_json = message_to_json(&messages[i]);
        if (!message_json) {
            cJSON_Delete(json_array);
            return NULL;
        }
        cJSON *json = cJSON_Parse(message_json);
        cJSON_AddItemToArray(json_array, json);
        free(message_json);
    }

    char *json_str = cJSON_PrintUnformatted(json_array);
    cJSON_Delete(json_array);
    return json_str;
}