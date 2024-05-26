//
// Created by yaroslav on 5/25/24.
//

#include <stdio.h>
#include <malloc.h>
#include "controller/routes.h"

//void print_chat(Chat *chat) {
//    if (chat) {
//        printf("ID: %lld, Name: %s, Description: %s, Created: %lld, Updated: %lld\n", chat->id, chat->name, chat->desc, chat->created_at, chat->updated_at);
//    } else {
//        printf("Invalid chat\n");
//    }
//}

static int s_debug_level = MG_LL_INFO;
static const char *s_root_dir = ".";
static const char *s_listening_address = "http://0.0.0.0:8000";
static const char *s_enable_hexdump = "no";
static const char *s_ssi_pattern = "#.html";
static const char *s_upload_dir = NULL;  // File uploads disabled by default

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
    s_signo = signo;
}

// Event handler for the listening connection.
// Simply serve static files from `s_root_dir`
static void cb(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = ev_data;

        if (mg_match(hm->uri, mg_str("/upload"), NULL)) {
            // Serve file upload
            if (s_upload_dir == NULL) {
                mg_http_reply(c, 403, "", "Denied: file upload directory not set\n");
            } else {
                struct mg_http_part part;
                size_t pos = 0, total_bytes = 0, num_files = 0;
                while ((pos = mg_http_next_multipart(hm->body, pos, &part)) > 0) {
                    char path[MG_PATH_MAX];
                    MG_INFO(("Chunk name: [%.*s] filename: [%.*s] length: %lu bytes",
                            part.name.len, part.name.buf, part.filename.len,
                            part.filename.buf, part.body.len));
                    mg_snprintf(path, sizeof(path), "%s/%.*s", s_upload_dir,
                                part.filename.len, part.filename.buf);
                    if (mg_path_is_sane(mg_str(path))) {
                        mg_file_write(&mg_fs_posix, path, part.body.buf, part.body.len);
                        total_bytes += part.body.len;
                        num_files++;
                    } else {
                        MG_ERROR(("Rejecting dangerous path %s", path));
                    }
                }
                mg_http_reply(c, 200, "", "Uploaded %lu files, %lu bytes\n", num_files,
                              total_bytes);
            }
        } else {
            // Serve web root directory
            struct mg_http_serve_opts opts = {0};
            opts.root_dir = s_root_dir;
            opts.ssi_pattern = s_ssi_pattern;
            mg_http_serve_dir(c, hm, &opts);
        }

        // Log request
        MG_INFO(("%.*s %.*s %lu -> %.*s %lu", hm->method.len, hm->method.buf,
                hm->uri.len, hm->uri.buf, hm->body.len, 3, c->send.buf + 9,
                c->send.len));
    }
}



int main(void) {
    struct mg_mgr mgr;
    struct mg_connection *c;

    mg_mgr_init(&mgr);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    mg_log_set(s_debug_level);

    if ((c = mg_http_listen(&mgr, s_listening_address, route_handler, &mgr)) == NULL) {
        MG_ERROR(("Cannot listen on %s. Use http://ADDR:PORT or :PORT",
                s_listening_address));
        exit(EXIT_FAILURE);
    }

    printf("Starting web server on port 8000\n");

    enable_controllers();

    while (s_signo == 0) mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
    MG_INFO(("Exiting on signal %d", s_signo));
    free_controllers();
    return 0;
}
//
//int main() {
//    chat_repo_start_repo();
//
//    Chat chat  = {0, "ada", "adsdd"};
//
//    chat_repo_insert(&chat);
//
//    Chat* chats = chat_repo_read_all();
//
//    if (chats) {
//        printf("All Chats:\n");
//        for (int i = 0; chats[i].id != 0 ; ++i) {
//            print_chat(&chats[i]);
//        }
//    } else {
//        printf("Failed to read chats from the database\n");
//    }
//
//    chat_free_objects(chats);
//
//    chat_repo_close_repo();
//    return 0;
//}