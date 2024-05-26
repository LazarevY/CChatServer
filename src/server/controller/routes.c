//
// Created by yaroslav on 5/25/24.
//

// routes.c

#include "routes.h"
#include <stdlib.h>
#include <string.h>

#define MAX_ROUTES 100
#define MAX_PARAMS 10

#include "chat/chat_controller.h"

typedef struct {
    const char *method;
    const char *pattern;
    RequestHandler handler;
} Route;

static Route routes[MAX_ROUTES];
static int route_count = 0;

static char* extract_http_info(struct mg_str method) {
    // Allocate memory for the method string with null-terminator
    char *method_str = (char *)malloc(method.len + 1);
    if (method_str == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy method to the new string and add null-terminator
    strncpy(method_str, method.buf, method.len);
    method_str[method.len] = '\0';

    return method_str;
}

void add_route(const char *method, const char *pattern, RequestHandler handler) {
    if (route_count < MAX_ROUTES) {
        routes[route_count].method = method;
        routes[route_count].pattern = pattern;
        routes[route_count].handler = handler;
        route_count++;
    }
}

static char **split_path(const char *path, int *count) {
    char *path_copy = strdup(path);
    char *token;
    char **tokens = NULL;
    int i = 0;

    token = strtok(path_copy, "/");
    while (token != NULL) {
        tokens = realloc(tokens, sizeof(char *) * (i + 1));
        tokens[i++] = strdup(token);
        token = strtok(NULL, "/");
    }
    free(path_copy);

    *count = i;
    return tokens;
}

static int match_route(const char *pattern, const char *url, char **params) {
    int pattern_count, url_count;
    char **pattern_parts = split_path(pattern, &pattern_count);
    char **url_parts = split_path(url, &url_count);

    if (pattern_count != url_count) {
        for (int i = 0; i < pattern_count; i++) free(pattern_parts[i]);
        free(pattern_parts);
        for (int i = 0; i < url_count; i++) free(url_parts[i]);
        free(url_parts);
        return 0;
    }

    int param_index = 0;
    for (int i = 0; i < pattern_count; ++i) {
        if (pattern_parts[i][0] == '{' && pattern_parts[i][strlen(pattern_parts[i]) - 1] == '}') {
            params[param_index++] = strdup(url_parts[i]);
        } else if (strcmp(pattern_parts[i], url_parts[i]) != 0) {
            for (int j = 0; j < pattern_count; j++) free(pattern_parts[j]);
            free(pattern_parts);
            for (int j = 0; j < url_count; j++) free(url_parts[j]);
            free(url_parts);
            return 0;
        }
    }

    for (int i = 0; i < pattern_count; i++) free(pattern_parts[i]);
    free(pattern_parts);
    for (int i = 0; i < url_count; i++) free(url_parts[i]);
    free(url_parts);
    return 1;
}

void route_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    char *params[MAX_PARAMS];

    if (ev == MG_EV_HTTP_MSG) {
        char *method_name = extract_http_info(hm->method);
        char *path = extract_http_info(hm->uri);
        for (int i = 0; i < route_count; ++i) {
            if (strcmp(routes[i].method, method_name) == 0 && match_route(routes[i].pattern, path, params)) {
                if (routes[i].handler(nc, hm, params) == 0) {
                    return;  // Successfully handled
                }
            }
        }
        mg_http_reply(nc, 404, 0, "Content-Type: text/plain");
        mg_printf(nc, "%s", "Not Found");
        free(path);
        free(method_name);
    }
}


void register_routes() {
    add_route("GET", "/chats", chat_controller_handle_get_chats);
    add_route("GET", "/chats/{id}", chat_controller_handle_get_chat_by_id);
    add_route("POST", "/chats", chat_controller_handle_create_chat);
}

void enable_controllers() {
    register_routes();
    chat_controller_start();
}

void free_controllers() {
    chat_controller_stop();
}
