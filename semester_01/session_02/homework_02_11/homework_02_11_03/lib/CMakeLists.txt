# CMakeList.txt: проект CMake; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
project(Addressing_someone)

file(GLOB HEADER_INC "*.h")
file(GLOB SOURCE_INC "*.cpp")

# Добавьте источник в исполняемый файл этого проекта.
add_library(Addressing_someone SHARED ${HEADER_INC} ${SOURCE_INC})
target_compile_definitions(Addressing_someone PRIVATE ADDRESSING_SOMEONE_EXPORT)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Addressing_someone PROPERTY CXX_STANDARD 20)
endif()