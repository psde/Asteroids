# Try to find GLFW3
# Once done this will define:
# GLFW3_FOUND - system has GLFW3
# GLFW3_INCLUDE_DIR - GLFW3 include dir
# GLFW3_LIBRARY_DIR - GLFW3 library dir
# GLFW3_LIBRARY - GLFW3 library

FIND_PATH(GLFW3_INCLUDE_DIR GLFW/glfw3.h 
    DOC "Path to GLFW include directory."
    HINTS
    $ENV{GLFW_ROOT}
    PATH_SUFFIX include
    PATHS
    /usr/include/
    /usr/local/include/
    /usr/include/GLs
    /usr/local/include/GL
    ${GLFW3_ROOT}/include/
)

FIND_LIBRARY(GLFW3_LIBRARY 
    NAMES glfw3 libglfw.so libglfw3.a
    DOC "Absolute path to GLFW3 library."
    HINTS 
    $ENV{GLFW_ROOT}
    PATH_SUFFIXES lib/win32
    PATHS
    /usr/local/lib/
    /usr/lib/
    ${GLFW3_ROOT}/build/src/Release/
)


GET_FILENAME_COMPONENT(GLFW3_LIBRARY_DIR ${GLFW3_LIBRARY} PATH)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	# Mac OS X specific code
	find_library(COCOA_FRAMEWORK Cocoa)
	find_library(IOKIT_FRAMEWORK IOKit)
	find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
	find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	find_library(CARBON_FRAMEWORK Carbon)
	mark_as_advanced(COCOA_FRAMEWORK
		IOKIT_FRAMEWORK
		CORE_FOUNDATION_FRAMEWORK
		CORE_VIDEO_FRAMEWORK
		CARBON_FRAMEWORK)
	list(APPEND GLFW3_LIBRARY "${COCOA_FRAMEWORK}"
		"${IOKIT_FRAMEWORK}"
		"${CORE_FOUNDATION_FRAMEWORK}"
		"${CORE_VIDEO_FRAMEWORK}"
		"${CARBON_FRAMEWORK}")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW3 REQUIRED_VARS 
                                  GLFW3_LIBRARY GLFW3_INCLUDE_DIR GLFW3_LIBRARY_DIR)