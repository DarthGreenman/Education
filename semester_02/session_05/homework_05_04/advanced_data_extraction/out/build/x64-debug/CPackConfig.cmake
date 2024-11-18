# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction;D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug")
set(CPACK_CMAKE_GENERATOR "Ninja")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.29/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "advanced_data_extraction built using CMake")
set(CPACK_GENERATOR "TGZ")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALL_CMAKE_PROJECTS "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug;advanced_data_extraction;ALL;/")
set(CPACK_INSTALL_PREFIX "d:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/install/x64-debug")
set(CPACK_MODULE_PATH "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/lib/libpqxx-7.9.2/cmake")
set(CPACK_NSIS_DISPLAY_NAME "advanced_data_extraction 7.9.2")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_PACKAGE_NAME "advanced_data_extraction 7.9.2")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.29/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "advanced_data_extraction built using CMake")
set(CPACK_PACKAGE_FILE_NAME "advanced_data_extraction-7.9.2-win64")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "advanced_data_extraction 7.9.2")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "advanced_data_extraction 7.9.2")
set(CPACK_PACKAGE_NAME "advanced_data_extraction")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "7.9.2")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_RESOURCE_FILE_LICENSE "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.29/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.29/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.29/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "ON")
set(CPACK_SOURCE_GENERATOR "7Z;ZIP")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/CPackSourceConfig.cmake")
set(CPACK_SOURCE_ZIP "ON")
set(CPACK_SYSTEM_NAME "win64")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "win64")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "D:/Users/Darth Greenman/Documents/Visual Studio 2022/source/repos/sessions/semester_02/session_05/homework_05_04/advanced_data_extraction/out/build/x64-debug/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
