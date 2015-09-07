# Copyright (C) 2015 Shang Yuanchun <idealities@gmail.com>

project(bitzer)

cmake_minimum_required(VERSION 2.6)

message(STATUS "Running cmake version ${CMAKE_VERSION}")

set(BITZER_VERSION "0.0.1")
set(CMAKE_COLOR_MAKEFILE   ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# whether bz_log_debug() is available
if (CMAKE_BUILD_TYPE)
    string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
endif (CMAKE_BUILD_TYPE)
if (BUILD_TYPE STREQUAL "DEBUG" OR DEBUG_LOG)
    set(BZ_DEBUG_LOG TRUE)
endif (BUILD_TYPE STREQUAL "DEBUG" OR DEBUG_LOG)

if (LOG_PATH)
    set(BZ_LOG_PATH TRUE)
endif (LOG_PATH)
if (CONF_PATH)
    set(BZ_CONF_PATH TRUE)
endif (CONF_PATH)
if (PID_PATH)
    set(BZ_PID_PATH TRUE)
endif (PID_PATH)

# check header files
include(CheckIncludeFiles)

# check functions
include(CheckFunctionExists)
check_function_exists(backtrace HAVE_BACKTRACE)
check_function_exists(backtrace_symbols HAVE_BACKTRACE_SYMBOLS)
check_function_exists(posix_memalign HAVE_POSIX_MEMALIGN)
check_function_exists(memalign HAVE_MEMALIGN)
check_function_exists(getopt_long HAVE_GETOPT_LONG)
check_function_exists(getifaddrs HAVE_GETIFADDRS)
check_function_exists(pselect HAVE_PSELECT)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/src
                   )

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-D_GNU_SOURCE)
endif (CMAKE_COMPILER_IS_GNUCC)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(src)

