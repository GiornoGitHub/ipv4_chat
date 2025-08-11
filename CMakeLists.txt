cmake_minimum_required(VERSION 3.10)
project(udp_chat VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(SOURCES
    src/main.c
)

add_executable(chat ${SOURCES})
target_link_libraries(chat pthread)

install(TARGETS chat DESTINATION bin)
install(PROGRAMS scripts/run_chat.sh DESTINATION bin)

option(ENABLE_DEBUG "Enable debug symbols" OFF)
if(ENABLE_DEBUG)
    target_compile_definitions(chat PRIVATE DEBUG)
    target_compile_options(chat PRIVATE -g)
endif()