// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Example: game entity database
//
// A flat_map<uint32_t(Vec3, float, std::string)> used as a cache-friendly SoA
// entity store keyed by entity ID, demonstrating the primary use cases:
//
//  - brace-init insertion
//  - structured bindings over rows
//  - per-element value access via value<I>()
//  - cache-friendly column iteration via values<I>()
//  - cross-column access via find + distance

#include <eggs/flat_map.hpp>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

using namespace eggs;

struct Vec3
{
    float x, y, z;
};

int main()
{
    // entities keyed by ID: position, health, name
    flat_map<uint32_t(Vec3, float, std::string)> entities;

    // insert via brace init — {key, v0, v1, v2}
    entities.insert({1u, Vec3{0.f, 0.f, 0.f}, 100.f, "Goblin"});
    entities.insert({2u, Vec3{5.f, 0.f, 3.f}, 80.f, "Troll"});
    entities.insert({3u, Vec3{1.f, 0.f, 1.f}, 60.f, "Orc"});

    // iterate all rows — use as_const to get const_iterator (proxy ref semantics)
    std::cout << "--- All entities ---\n";
    for (const auto& [id, pos, hp, name] : std::as_const(entities))
        std::cout << id << ": " << name << "  hp=" << hp << "\n";

    // find one entity, mutate a single value column
    if (auto it = entities.find(2); it != entities.end())
        it->value<1>() -= 25.f; // deal damage to Troll

    // regen tick — iterate health column directly, no key/name touched
    for (float& hp : entities.values<1>()) hp = std::min(hp + 5.f, 100.f);

    // find + distance: one lookup, then O(1) direct column writes
    auto it = entities.find(1);
    auto idx = std::distance(entities.begin(), it);
    entities.values<0>()[idx] = Vec3{2.f, 0.f, 2.f}; // reposition Goblin
    entities.values<2>()[idx] = "Goblin Chief";      // promote

    std::cout << "\n--- After updates ---\n";
    for (const auto& [id, pos, hp, name] : std::as_const(entities))
        std::cout << id << ": " << name << "  hp=" << hp << "\n";
}
