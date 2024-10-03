# Install script for directory: C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/src/df012521d2-b9cbdf594c.clean/lib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/pkgs/libfort_x64-windows")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/x64-windows-rel/lib/fort.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/src/df012521d2-b9cbdf594c.clean/lib/fort.h"
    "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/src/df012521d2-b9cbdf594c.clean/lib/fort.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/x64-windows-rel/lib/libfort.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libfort" TYPE FILE FILES
    "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/x64-windows-rel/lib/libfort-config.cmake"
    "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/x64-windows-rel/lib/libfort-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libfort/libfort-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libfort/libfort-targets.cmake"
         "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/x64-windows-rel/lib/CMakeFiles/Export/bba6333106b5e1b5982ab34d57f28053/libfort-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libfort/libfort-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libfort/libfort-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libfort" TYPE FILE FILES "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/x64-windows-rel/lib/CMakeFiles/Export/bba6333106b5e1b5982ab34d57f28053/libfort-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libfort" TYPE FILE FILES "C:/Users/NROSARIO/Desktop/OSEmulator/vcpkg_installed/vcpkg/blds/libfort/x64-windows-rel/lib/CMakeFiles/Export/bba6333106b5e1b5982ab34d57f28053/libfort-targets-release.cmake")
  endif()
endif()

