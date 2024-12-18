# CMakeList.txt: проект CMake для advanced_data_extraction; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(PROJECT_NAME advanced_data_extraction)
#set(EXECUTABLE_FILE_NAME phone_book_app)
project (${PROJECT_NAME} VERSION 1.0)

set(PostgreSQL_ADDITIONAL_VERSIONS 17)
#set(INCLUDES_PATH "${CMAKE_SOURCE_DIR}/includes/")
#set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/src/")
set(LIBPQXX_PATH "../../../../../lib/libpqxx-7.9.2")

#file(GLOB HEADER_INC ${INCLUDES_PATH} "*.h")
#file(GLOB SOURCE_INC ${SOURCE_PATH} "*.cpp")

# Добавьте источник в исполняемый файл этого проекта.
add_subdirectory(${LIBPQXX_PATH} libpqxx-build)
add_executable (${PROJECT_NAME} "advanced_data_extraction.cpp" #${HEADER_INC} ${SOURCE_INC})
	"includes/phone_book.h"
	"includes/contact.h"
	"includes/types.h"
	"includes/my_utilities.h"
	"includes/my_ascii.h"
	"includes/my_input.h"
	"includes/simple_db_viewer.h"
	"src/phone_book.cpp"
	"src/contact.cpp"
	"src/types.cpp"
	"src/my_utilities.cpp"
	"src/simple_db_viewer.cpp"
	)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE pqxx)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.