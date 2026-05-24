# Eggs.FlatMap
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.25...4.2.3)

project(Eggs.FlatMap CXX)

include(GNUInstallDirs)

if(PROJECT_IS_TOP_LEVEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

    include(CTest) # option(BUILD_TESTING ...)
    option(BUILD_EXAMPLE "Build the examples" ON)
    option(ENABLE_INSTALL "Enable installing the library" ON)
    set(SYSTEM OFF)
else()
    option(EGGS_FLAT_MAP_BUILD_TESTING "Build the testing tree" OFF)
    option(EGGS_FLAT_MAP_BUILD_EXAMPLE "Build the examples" OFF)
    option(EGGS_FLAT_MAP_ENABLE_INSTALL "Enable installing the library" OFF)

    set(BUILD_EXAMPLE ${EGGS_FLAT_MAP_BUILD_EXAMPLE})
    set(BUILD_TESTING ${EGGS_FLAT_MAP_BUILD_TESTING})
    set(ENABLE_INSTALL ${EGGS_FLAT_MAP_ENABLE_INSTALL})
    set(SYSTEM ON)
endif()

# Library target (header-only INTERFACE library)
add_library(_eggs_flat_map INTERFACE)
target_sources(
    _eggs_flat_map
    INTERFACE FILE_SET HEADERS BASE_DIRS include FILES include/eggs/flat_map.hpp
)
set_target_properties(
    _eggs_flat_map
    PROPERTIES
        EXPORT_NAME FlatMap
        SYSTEM ${SYSTEM}
        VERIFY_INTERFACE_HEADER_SETS ${BUILD_TESTING}
)
target_compile_features(_eggs_flat_map INTERFACE cxx_std_23)

add_library(Eggs::FlatMap ALIAS _eggs_flat_map)

# Example
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

# Test
if(BUILD_TESTING)
    add_subdirectory(test)
endif()

# Install
if(ENABLE_INSTALL)
    include(CMakePackageConfigHelpers)

    install(
        TARGETS _eggs_flat_map
        EXPORT Eggs::FlatMap
        FILE_SET HEADERS
        INCLUDES DESTINATION include
    )

    install(
        EXPORT Eggs::FlatMap
        NAMESPACE Eggs::
        FILE Eggs.FlatMapTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Eggs.FlatMap
    )

    file(
        GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Eggs.FlatMapConfig.cmake"
        CONTENT
            [[include("${CMAKE_CURRENT_LIST_DIR}/Eggs.FlatMapTargets.cmake")]]
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/Eggs.FlatMapConfigVersion.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/Eggs.FlatMapConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/Eggs.FlatMapConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Eggs.FlatMap
    )
endif()
