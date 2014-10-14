# Try to find GLEW
# Once done this will define:
# GLEW_FOUND - system has GLEW
# GLEW_INCLUDE_DIR - GLEW include dir
# GLEW_LIBRARY_DIR - GLEW library dir
# GLEW_LIBRARY - GLEW library

FIND_PATH(GLEW_INCLUDE_DIR GL/glew.h 
    DOC "Path to GLEW include directory."
    HINTS
    $ENV{GLEW_ROOT}
    PATH_SUFFIX include
    PATHS
    /usr/include/
    /usr/local/include/
    /usr/include/GL
    /usr/local/include/GL
    ${GLEW_ROOT}/include/
)

FIND_LIBRARY(GLEW_LIBRARY 
    NAMES glew32s libGLEW.so
    DOC "Absolute path to GLEW library."
    HINTS 
    $ENV{GLEW_ROOT}
    PATH_SUFFIXES lib/win32
    PATHS
    /usr/local/lib
    /usr/lib
    ${GLEW_ROOT}/lib/Release/Win32
)

GET_FILENAME_COMPONENT(GLEW_LIBRARY_DIR ${GLEW_LIBRARY} PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW REQUIRED_VARS 
                                  GLEW_LIBRARY GLEW_INCLUDE_DIR GLEW_LIBRARY_DIR)