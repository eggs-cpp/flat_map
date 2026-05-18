// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("reference/first", "[reference]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    CHECK(m.begin()->first == 1);
}

TEST_CASE("reference/value/mutable", "[reference]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    m.begin()->value<0>() = 9.f;
    m.begin()->value<1>() = "z";

    CHECK(m.begin()->value<0>() == 9.f);
    CHECK(m.begin()->value<1>() == "z");
}

TEST_CASE("reference/structured_bindings", "[reference]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    auto ref = *m.begin();
    auto& [k, f, s] = ref;

    CHECK(k == 1);
    CHECK(f == 1.f);
    CHECK(s == "a");
}

TEST_CASE("reference/assign_from_value_type/copy", "[reference]")
{
    flat_map<int(float)> m{{1, 1.f}};

    *m.begin() = {1, 9.f};

    CHECK(m.begin()->value<0>() == 9.f);
}

TEST_CASE("reference/assign_from_value_type/move", "[reference]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    flat_map<int(float, std::string)>::value_type v{1, 2.f, "b"};
    *m.begin() = std::move(v);

    CHECK(m.begin()->value<0>() == 2.f);
    CHECK(m.begin()->value<1>() == "b");
}

TEST_CASE("reference/refs_into_storage", "[reference]")
{
    // reference must borrow from the actual storage, not copies
    flat_map<int(float)> m{{1, 1.f}};

    auto ref = *m.begin();
    ref.value<0>() = 9.f;

    CHECK(m.values<0>()[0] == 9.f);
}
