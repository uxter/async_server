cmake_minimum_required(VERSION 2.6)
project(async_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(async_server main.cpp config_store.cpp config_store.h json_value.cpp json_value.h json.h json_parser.cpp json_parser.h server.cpp server.h event_loop.cpp event_loop.h event.h event_handler.cpp event_handler.h connection.cpp connection.h json.cpp http_parser.cpp http_parser.h)