# CMakeList.txt: проект CMake для advanced_data_extraction; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(PostgreSQL_ADDITIONAL_VERSIONS 17)
project ("advanced_data_extraction")

# Добавьте источник в исполняемый файл этого проекта.

add_subdirectory("d:\\Users\\Darth Greenman\\Documents\\Visual Studio 2022\\source\\repos\\lib\\libpqxx-7.9.2" libpqxx-build)
add_executable (advanced_data_extraction "advanced_data_extraction.cpp" "includes/phone_book.h" "src/phone_book.cpp" "includes/contact.h" "src/contact.cpp")
target_compile_features(advanced_data_extraction PRIVATE cxx_std_20)
target_link_libraries(advanced_data_extraction PRIVATE pqxx)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET advanced_data_extraction PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.