cmake_minimum_required(VERSION 2.8)

project(confuse)

include(CMakeOptions.txt)
set(CONFUSE_INLINE_BUILD true)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${confuse_SOURCE_DIR}/cmake)

set(CMAKE_VERBOSE_MAKEFILE true)

# set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

if (DEBUG_MODE)
set(CMAKE_BUILD_TYPE "Debug")
        if(NOT WIN32)
                add_definitions(-g)
        endif()
else()
        if(NOT WIN32)
                add_definitions(-O3)
        endif()
endif()
if (NOT WIN32)
	add_definitions(-fPIC)
endif()

include(CTest)

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckLibraryExists)

include(FindPkgConfig)

find_package(FLEX)

find_package(confuse)
if(CONFUSE_FOUND)
  message("Confuse library found")
endif(CONFUSE_FOUND)

# Gettext stuff
include(GNUInstallDirs)
find_package(Gettext)
if(GETTEXT_FOUND)
  set(HAVE_GETTEXT 1)
  # set(LOCALEDIR ${CMAKE_INSTALL_FULL_LOCALEDIR})
  add_definitions(-DLOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")
  message("Locale directory: " ${CMAKE_INSTALL_FULL_LOCALEDIR})
  # add_subdirectory(po)
endif(GETTEXT_FOUND)

# Manage the version
file(STRINGS VERSION VERSION)
set(CONFUSE_VERSION ${VERSION})
configure_file("src/include/confuse/version.h.cmake" "src/include/confuse/version.h")

# Mac OS Specifics
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_definitions(-DMACOS=1)
endif()

# Mac OS Specifics
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-DWIN32=1)
endif()

set(CONFUSE_INCLUDE_DIRS "${confuse_SOURCE_DIR}/src/include/")

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
