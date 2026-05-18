# **Eggs.FlatMap**

## Introduction

**Eggs.FlatMap** is a C++23 header-only implementation of a variadic `flat_map`
— a sorted associative container with a Structure of Arrays (SoA) layout.

It extends the `std::flat_map` design from C++23 to support multiple value types
per key, each stored in its own contiguous array for cache-friendly column-wise
access.

```cpp
#include <eggs/flat_map.hpp>
using namespace eggs::flat_map;

// entity database: ID → (position, health, name)
flat_map<uint32_t(Vec3, float, std::string)> entities;

entities.insert({1, Vec3{0, 0, 0}, 100.f, "Goblin"});

// iterate all rows — flat structured bindings
for (auto [id, pos, hp, name] : entities) { ... }

// access a specific value column by index
entities.find(1)->value<1>() -= 25.f;

// iterate entire health column — cache-friendly, no indirection
for (float& hp : entities.values<1>())
    hp = std::min(hp + 5.f, 100.f);
```

## Design

The container is parameterized via a function-type signature:

```cpp
flat_map<Key(V0, V1, ..., Vn), Compare>
```

This mirrors the familiar `std::function<R(Args...)>` syntax. `Compare`
defaults to `std::less<Key>`. Internally, keys and each value type occupy
separate `std::vector`s kept in parallel sorted order.

| Operation | Complexity |
|-----------|------------|
| Lookup    | O(log n)   |
| Insert    | O(n)       |
| Iteration | O(n), cache-friendly per-column |

The interface follows `std::flat_map` as closely as possible. Notable
extensions:

- `it->value<I>()` — access the I-th value of an iterator element
- `m.values<I>()` — direct access to the I-th value column (`std::vector`)
- `auto [k, v0, v1, ...] = *it` — flat structured bindings across all columns
- `containers` struct supports flat structured bindings for `extract()`/`replace()`

## Requirements

- C++23 compiler (GCC 14+, Clang 17+, MSVC 19.38+)
- CMake 3.23 or later

## Usage

### Single header

Copy `include/eggs/flat_map.hpp` into your project.

### CMake (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(eggs.flat_map
    GIT_REPOSITORY https://github.com/eggs-cpp/flat_map
    GIT_TAG        main)
FetchContent_MakeAvailable(eggs.flat_map)

target_link_libraries(my_target PRIVATE Eggs::FlatMap)
```

### CMake (installed)

```cmake
find_package(eggs.flat_map REQUIRED)
target_link_libraries(my_target PRIVATE Eggs::FlatMap)
```

## License

> Copyright *Agustín Bergé*, *Fusion Fenix* 2026
>
> Distributed under the Boost Software License, Version 1.0. (See accompanying
> file LICENSE.txt or copy at <http://www.boost.org/LICENSE_1_0.txt>)
