cmake_minimum_required(VERSION 3.5)
project(08_PreDataBase VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Concurrent
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        db_connect.h
        db_connect.cpp
        db_connect.ui
        cpp.hint
)

add_executable(${PROJECT_NAME}	${PROJECT_SOURCES})

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
        Qt6::Concurrent
)
#target_link_libraries(${PROJECT_NAME} 
#    PRIVATE
#        libcrypto-3-x64
#        libiconv-2
#        libintl-9
#        libpq
#        libssl-3-x64
#)
#set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1")
#set(CMAKE_PREFIX_PATH "D:/Programs/Qt/6.9.1")


