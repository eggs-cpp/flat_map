# Eggs.FlatMap
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ── Vendored Catch2 v2 single-header ────────────────────────────────────────

add_library(_eggs_flat_map_catch2 OBJECT catch.cpp)
target_include_directories(
    _eggs_flat_map_catch2
    SYSTEM
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_compile_definitions(
    _eggs_flat_map_catch2
    INTERFACE CATCH_CONFIG_FAST_COMPILE
)
set_target_properties(_eggs_flat_map_catch2 PROPERTIES FOLDER "test/catch2")
