cmake_minimum_required(VERSION 3.16)
project(client_server LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_SOURCES "common/")

add_subdirectory(tcp_client)
add_subdirectory(tcp_server)

add_custom_target(client_server)
add_dependencies(client_server tcp_client tcp_server)

