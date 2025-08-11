cmake_minimum_required(VERSION 3.16)
project(tcp_server LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Network
)

set(PROJECT_SOURCES
        main.cpp
        tcpserver.h
        tcpserver.cpp
        cpp.hint
)

qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${COMMON_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Network
)
