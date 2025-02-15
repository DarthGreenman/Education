# CMakeList.txt: проект CMake для homework_02_10_02; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Переменные
set(INCLUDES_PATH "${PROJECT_SOURCE_DIR}/includes")
set(HEADER_INC "${INCLUDES_PATH}/Localisation.h" "${INCLUDES_PATH}/Triangle.h"  "${INCLUDES_PATH}/Quadrangle.h" "${INCLUDES_PATH}/Shape.cpp")
set(SOURCE_INC "${INCLUDES_PATH}/Shape.cpp")

# Пути для заголовочных файлов
include_directories(${INCLUDES_PATH})

# Добавьте источник в исполняемый файл этого проекта.
add_executable (${PROJECT_NAME} "homework_02_10_02.cpp" ${HEADER_INC} ${SOURCE_INC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET homework_02_10_02 PROPERTY CXX_STANDARD 20)
endif()