cmake_minimum_required(VERSION 3.16)
project(airport_inspector LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Sql
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    cpp.hint
    main.cpp
    airport_inspector_viewer.ui
    airport_inspector_viewer.h
    airport_inspector_viewer.cpp
    airport_inspector_driver.h
    airport_inspector_driver.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

#set_target_properties(${PROJECT_NAME}
#    PROPERTIES
#        WIN32_EXECUTABLE TRUE
#)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
)

