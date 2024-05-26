//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_UTILS_H
#define CHATSERVER_UTILS_H

#include <sys/time.h>

#define PARSE_JSON_FIELD_DOUBLE(obj, field, json) \
        {cJSON *json_item = cJSON_GetObjectItemCaseSensitive(json, #field); \
        if (cJSON_IsNumber(json_item)) { \
            obj->field = json_item->valuedouble; \
        } else { \
            obj->field = 0; \
        }}

#define PARSE_JSON_FIELD_STRING(obj, field, json) \
        {cJSON *json_item = cJSON_GetObjectItemCaseSensitive(json, #field); \
        if (cJSON_IsString(json_item)) { \
            obj->field = strdup(json_item->valuestring); \
        } else { \
            obj->field = NULL; \
        }}

#define ADD_JSON_NUMBER(json, field, obj) \
    cJSON_AddNumberToObject(json, #field, obj->field)

#define ADD_JSON_STRING(json, field, obj) \
    cJSON_AddStringToObject(json, #field, obj->field)

long long utils_time_in_milliseconds();

typedef long long long_id;

#endif //CHATSERVER_UTILS_H
