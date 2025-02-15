# CMakeList.txt: проект CMake для homework_04_04_01; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавляем указанные каталоги к тем, которые компилятор использует
# для поиска включаемых файлов (list.h)
set(INCLUDES_PATH "${PROJECT_SOURCE_DIR}./lib")
include_directories(${INCLUDES_PATH})

# Добавляем подкаталог Catch2 в сборку 
set(CATCH2_PATH "${PROJECT_SOURCE_DIR}../../../unit_testing_frameworks/Catch2")
add_subdirectory(${CATCH2_PATH} Catch2_build)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (${PROJECT_NAME} "homework_04_04_01.cpp" "homework_04_04_01.h")
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2) #Catch2::Catch2WithMain

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()