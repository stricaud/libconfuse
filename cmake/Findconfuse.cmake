# Locate Confuse include paths and libraries
#
# $Id$
# Copyright (C) Sebastien Tricaud 2012
#
# CONFUSE_FOUND        - If libconfuse is found
# CONFUSE_INCLUDE_DIRS - Where include/confuse is found
# CONFUSE_LIBRARIES    - List of libraries when using libconfuse
# CONFUSE_DEFINITIONS  - List of definitions to be added when using libconfuse
#

set(CONFUSE_DEFINITIONS "")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_definitions(-DMACOS=1)
endif()


if(CONFUSE_INLINE_BUILD)
	set(CONFUSE_FOUND true)
	set(CONFUSE_INCLUDE_DIR "${confuse_SOURCE_DIR}/src/include")
	set(CONFUSE_LIBRARY "${confuse_BINARY_DIR}/src/libconfuse.so")

	if(WIN32)
		set(CONFUSE_INCLUDE_DIR "${confuse_SOURCE_DIR}\\src\\include")
		set(CONFUSE_LIBRARY "${confuse_BINARY_DIR}\\src\\${CMAKE_BUILD_TYPE}\\confuse.lib")
	endif()
	if(APPLE)
		set(CONFUSE_LIBRARY "${confuse_BINARY_DIR}/src/libconfuse.dylib")
	endif()

else(CONFUSE_INLINE_BUILD)
	find_path(CONFUSE_INCLUDE_DIR confuse/confuse.h
	          HINTS "../src/include" ${CONFUSE_INCLUDEDIR}
	          PATH_SUFFIXES confuse )

	find_library(CONFUSE_LIBRARY NAMES confuse
	             HINTS "../src/" ${CONFUSE_LIBDIR} 
		     PATH_SUFFIXES src/ )
endif(CONFUSE_INLINE_BUILD)

set(CONFUSE_LIBRARIES ${CONFUSE_LIBRARY})
set(CONFUSE_INCLUDE_DIRS ${CONFUSE_INCLUDE_DIR})

mark_as_advanced(CONFUSE_INCLUDE_DIRS CONFUSE_LIBRARIES)

