//
// Created by yaroslav on 5/25/24.
//

#ifndef CHATSERVER_ROUTES_H
#define CHATSERVER_ROUTES_H

#include "mongoose.h"

typedef int (*RequestHandler)(struct mg_connection *, struct mg_http_message *, char **);

void add_route(const char *method, const char *pattern, RequestHandler handler);

void route_handler(struct mg_connection *nc, int ev, void *ev_data);

void enable_controllers();
void free_controllers();

void register_routes();

#endif //CHATSERVER_ROUTES_H
