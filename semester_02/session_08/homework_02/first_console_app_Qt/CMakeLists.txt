cmake_minimum_required(VERSION 3.16)

project(first_console_app_qt LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Network)


add_executable(first_console_app_qt
  main.cpp
)

target_link_libraries(first_console_app_qt PRIVATE Qt6::Core)
target_link_libraries(first_console_app_qt PRIVATE Qt6::Sql)
target_link_libraries(first_console_app_qt PRIVATE Qt6::Network)

include(GNUInstallDirs)
install(TARGETS first_console_app_qt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
