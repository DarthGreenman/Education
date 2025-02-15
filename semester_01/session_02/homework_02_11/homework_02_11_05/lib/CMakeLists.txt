# CMakeList.txt: проект CMake для homework_02_11_05; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
project(Geometric)

file(GLOB HEADER_INC "*.h")
file(GLOB SOURCE_INC "*.cpp")

# Добавьте источник в исполняемый файл этого проекта.
add_library(Geometric SHARED ${HEADER_INC} ${SOURCE_INC})
target_compile_definitions(Geometric PRIVATE GEOMETRIC_SHAPES_EXPORT)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Geometric PROPERTY CXX_STANDARD 20)
endif()