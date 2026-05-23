# Eggs.FlatMap
#
# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_executable(_eggs_flat_map_example.entity_db entity_db.cpp)
target_link_libraries(_eggs_flat_map_example.entity_db PRIVATE Eggs::FlatMap)
set_target_properties(
    _eggs_flat_map_example.entity_db
    PROPERTIES FOLDER "example"
)
