# CMakeList.txt: проект CMake для homework_02_10_01; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (homework_02_10_01 "homework_02_10_01.cpp" "Localisation.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET homework_02_10_01 PROPERTY CXX_STANDARD 20)
endif()