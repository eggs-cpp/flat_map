// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("const_reference/first", "[const_reference]")
{
    const flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    CHECK(m.begin()->first == 1);
}

TEST_CASE("const_reference/value", "[const_reference]")
{
    const flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    CHECK(m.begin()->value<0>() == 1.f);
    CHECK(m.begin()->value<1>() == "a");
}

TEST_CASE("const_reference/structured_bindings", "[const_reference]")
{
    const flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    const auto& [k, f, s] = *m.cbegin();

    CHECK(k == 1);
    CHECK(f == 1.f);
    CHECK(s == "a");
}

TEST_CASE("const_reference/from_iterator", "[const_reference]")
{
    flat_map<int(float)> m{{1, 1.f}};

    // const_iterator implicitly constructed from iterator
    flat_map<int(float)>::const_iterator it = m.begin();

    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
}
