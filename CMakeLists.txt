cmake_minimum_required(VERSION 2.8)

project(asteroids)

# Variables
set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "1")
set(APPLICATION_VERSION_PATCH "0")
set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")
set(APPLICATION_NAME ${PROJECT_NAME})

# User config
include("UserDefinitions.cmake" OPTIONAL)

# CMakeScripts
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
link_directories(${GLEW_LIBRARY_DIR})

# GLFW3
find_package(GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})
link_directories(${GLFW3_LIBRARY_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Compiler flags
set(CPP_COMPILER_FLAGS "")
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Detected GNUCXX")
    set(CPP_COMPILER_FLAGS "-Wall -Wextra -Wpedantic -g -std=c++11")
elseif(MSVC)
    message(STATUS "Detected MSVC")
    #set(CPP_COMPILER_FLAGS "-D_CRT_SECURE_NO_WARNINGS")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Detected Clang++")
    set(CPP_COMPILER_FLAGS "-Wall -Wextra -Wno-c++98-compat -std=c++11 -Wpedantic")
else()
    message(STATUS "Unknown Compiler")
endif()
add_definitions("${CPP_COMPILER_FLAGS}")


# Add subdirs
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
#set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src" PROPERTY COMPILE_DEFINITIONS ${CPP_COMPILER_FLAGS})

if(MSVC)
    message(STATUS "Detected MSVC")
	set(MSVC_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
    file(MAKE_DIRECTORY  "${CMAKE_BINARY_DIR}/src/asteroids/")
    configure_file("${PROJECT_SOURCE_DIR}/cmake/VisualStudio/asteroids.vcxproj.user.in" "${CMAKE_BINARY_DIR}/src/asteroids.vcxproj.user")
endif()