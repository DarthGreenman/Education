# CMakeList.txt: проект CMake для homework_02_11_05; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Пути для заголовочных файлов
set(INCLUDES_PATH "${PROJECT_SOURCE_DIR}/lib")
include_directories(${INCLUDES_PATH})

# Добавьте источник в исполняемый файл этого проекта.
add_executable (${PROJECT_NAME} "homework_02_11_05.cpp" "Localisation.h")
target_link_libraries(${PROJECT_NAME} Geometric)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()