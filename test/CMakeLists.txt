# Eggs.FlatMap
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ── Verify interface header sets ────────────────────────────────────────────

add_test(
    NAME eggs.flat_map.verify_headers
    COMMAND
        ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target
        _eggs_flat_map_verify_interface_header_sets
)
set_tests_properties(eggs.flat_map.verify_headers PROPERTIES LABEL "headers")

# ── One executable + one CTest per test source ───────────────────────────────
add_subdirectory(catch2)

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
    target_link_libraries(
        ${_target}
        PRIVATE Eggs::FlatMap _eggs_flat_map_catch2
    )
    set_target_properties(${_target} PROPERTIES FOLDER "test")

    add_test(NAME "eggs.flat_map.${_name}" COMMAND ${_target})
    set_tests_properties("eggs.flat_map.${_name}" PROPERTIES LABEL "unit")
endforeach()
