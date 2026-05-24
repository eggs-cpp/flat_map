# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25)
project(Eggs::FlatMap-FetchContent CXX)

# -------------------------------------------------------------------------
# EGGS_FlatMap_SOURCE_DIR is injected by the CTest fixture so that this
# consumer always tests the local working tree, not a remote tag.
# -------------------------------------------------------------------------
if(NOT DEFINED EGGS_FLAT_MAP_SOURCE_DIR)
    message(
        FATAL_ERROR
        "EGGS_FLAT_MAP_SOURCE_DIR must be set. "
        "Pass -DEGGS_FLAT_MAP_SOURCE_DIR=<path> to cmake."
    )
endif()
file(TO_CMAKE_PATH "${EGGS_FLAT_MAP_SOURCE_DIR}" EGGS_FLAT_MAP_SOURCE_DIR)

include(FetchContent)
FetchContent_Declare(Eggs.FlatMap SOURCE_DIR "${EGGS_FLAT_MAP_SOURCE_DIR}")
FetchContent_MakeAvailable(Eggs.FlatMap)

# -------------------------------------------------------------------------
# Verification target — identical intent to the install consumer, but this
# time the target comes from FetchContent rather than an installed config.
# -------------------------------------------------------------------------
add_executable(consumer_verify ../../example/entity_db.cpp)
target_link_libraries(consumer_verify PRIVATE Eggs::FlatMap)

# -------------------------------------------------------------------------
# Confirm that the FetchContent target exposes the same properties as the
# installed one (no accidental in-tree-only paths).
# -------------------------------------------------------------------------
foreach(
    _prop
    IN
    ITEMS
        INTERFACE_INCLUDE_DIRECTORIES
        INTERFACE_COMPILE_DEFINITIONS
        INTERFACE_COMPILE_FEATURES
        INTERFACE_LINK_LIBRARIES
)
    get_target_property(_val Eggs::FlatMap ${_prop})
    message(STATUS "Eggs::FlatMap (FetchContent) ${_prop}: ${_val}")
endforeach()

# Make the verify binary a ctest too
enable_testing()
add_test(NAME verify COMMAND consumer_verify)
