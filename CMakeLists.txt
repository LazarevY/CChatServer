cmake_minimum_required(VERSION 3.27)
project(ChatServer C)

set(CMAKE_C_STANDARD 23)

include_directories(src/server)
include_directories(src)

link_directories(/usr/lib /usr/lib/x86_64-linux-gnu)

add_executable(ChatServer src/main.c
        src/server/entity/chat.h
        src/server/db/repository/chat_repository.h
        src/server/utils.h
        src/server/db/repository/impl/sqlite/chat_repository.c
        src/server/db/connection/sqlite/sqlite_database.h
        src/server/db/connection/sqlite/sqlite_database.c
        src/server/env.h
        src/server/entity/message.h
        src/server/entity/chat.c
        src/server/utils.c
        src/server/db/repository/message_repository.h
        src/server/db/repository/impl/sqlite/msg_repository.c
        src/server/entity/user.h
        src/server/db/repository/user_repository.h
        src/server/db/repository/impl/sqlite/user_repository.c
)

target_link_libraries(ChatServer -lsqlite3)
