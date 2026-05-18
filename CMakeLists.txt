# Eggs.FlatMap
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.23...3.31)

project(Eggs.FlatMap CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
    option(BUILD_EXAMPLE "Build the examples" ON)
    option(ENABLE_INSTALL "Enable installing the library" ON)
else()
    option(EGGS_FLAT_MAP_BUILD_EXAMPLE "Build the examples" OFF)
    option(EGGS_FLAT_MAP_BUILD_TESTING "Build the testing tree" OFF)
    option(EGGS_FLAT_MAP_ENABLE_INSTALL "Enable installing the library" OFF)

    set(BUILD_EXAMPLE ${EGGS_FLAT_MAP_BUILD_EXAMPLE})
    set(BUILD_TESTING ${EGGS_FLAT_MAP_BUILD_TESTING})
    set(ENABLE_INSTALL ${EGGS_FLAT_MAP_ENABLE_INSTALL})
endif()

include(GNUInstallDirs)

# ============================================================================
# Library target
# ============================================================================

add_library(_eggs_flat_map INTERFACE)
target_sources(
    _eggs_flat_map
    INTERFACE FILE_SET HEADERS BASE_DIRS include FILES include/eggs/flat_map.hpp
)
target_compile_features(_eggs_flat_map INTERFACE cxx_std_23)
set_target_properties(_eggs_flat_map PROPERTIES EXPORT_NAME FlatMap)

add_library(Eggs::FlatMap ALIAS _eggs_flat_map)

# ============================================================================
# Example
# ============================================================================

if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

# ============================================================================
# Test
# ============================================================================

if(BUILD_TESTING)
    add_subdirectory(test)
endif()

# ============================================================================
# Install
# ============================================================================

if(ENABLE_INSTALL)
    install(
        TARGETS _eggs_flat_map
        EXPORT _targets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        EXPORT _targets
        NAMESPACE Eggs::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/eggs.flat_map
        FILE eggs.flat_map-config.cmake
    )
endif()
