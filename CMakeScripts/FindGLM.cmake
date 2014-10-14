# Try to find GLM
# Once done this will define:
# GLM_FOUND - system has GLM
# GLM_INCLUDE_DIR - GLM include dir

FIND_PATH(GLM_INCLUDE_DIR glm/glm.hpp
    DOC "Path to GLFW include directory."
    HINTS
    PATH_SUFFIX include
    PATHS
    /usr/include/
    /usr/local/include/
    /usr/include/
    /usr/local/include/
    ${GLM_ROOT}/
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM REQUIRED_VARS 
                                  GLM_INCLUDE_DIR)