# Install script for directory: D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/lib/libpqxx-7.9.2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "d:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/install/x64-debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libpqxx" TYPE FILE FILES
    "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/lib/libpqxx-7.9.2/cmake/libpqxx-config.cmake"
    "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/libpqxx-build/libpqxx-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libpqxx/libpqxx-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libpqxx/libpqxx-targets.cmake"
         "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/libpqxx-build/CMakeFiles/Export/d6b66e93f8212dd1f00debf9f6654819/libpqxx-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libpqxx/libpqxx-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libpqxx/libpqxx-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libpqxx" TYPE FILE FILES "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/libpqxx-build/CMakeFiles/Export/d6b66e93f8212dd1f00debf9f6654819/libpqxx-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libpqxx" TYPE FILE FILES "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/libpqxx-build/CMakeFiles/Export/d6b66e93f8212dd1f00debf9f6654819/libpqxx-targets-debug.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/libpqxx-build/src/cmake_install.cmake")
  include("D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/libpqxx-build/include/cmake_install.cmake")
  include("D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/libpqxx-build/test/cmake_install.cmake")

endif()

