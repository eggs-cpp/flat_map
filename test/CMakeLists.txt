# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ── Vendored Catch2 v2 single-header ────────────────────────────────────────

add_library(_catch2 INTERFACE)
target_include_directories(
    _catch2
    SYSTEM
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)
# cuts per-TU overhead in v2
target_compile_definitions(_catch2 INTERFACE CATCH_CONFIG_FAST_COMPILE)

# ── Shared test main (compiled once, linked into every test executable) ──────

add_library(_eggs_flat_map_test_main OBJECT catch.cpp)
target_link_libraries(_eggs_flat_map_test_main PUBLIC Eggs::FlatMap _catch2)
set_target_properties(
    _eggs_flat_map_test_main
    PROPERTIES
        FOLDER "test"
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_SCAN_FOR_MODULES OFF
)

# ── One executable + one CTest per test source ───────────────────────────────

set(_test_sources
    access.cpp
    assign.cpp
    capacity.cpp
    columns.cpp
    comparison.cpp
    cons.cpp
    const_reference.cpp
    emplace.cpp
    erase.cpp
    erase_if.cpp
    extract.cpp
    insert.cpp
    insert_or_assign.cpp
    iterators.cpp
    lookup.cpp
    observers.cpp
    reference.cpp
    swap.cpp
    try_emplace.cpp
    value_type.cpp
)

foreach(_src IN LISTS _test_sources)
    get_filename_component(_name "${_src}" NAME_WLE)
    set(_target "_eggs_flat_map_test.${_name}")

    add_executable(${_target} "${_src}")
    target_link_libraries(${_target} PRIVATE _eggs_flat_map_test_main)
    set_target_properties(
        ${_target}
        PROPERTIES
            FOLDER "test"
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            CXX_SCAN_FOR_MODULES OFF
    )

    add_test(NAME "flat_map.${_name}" COMMAND ${_target})
endforeach()
