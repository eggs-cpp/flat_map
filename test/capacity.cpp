// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("capacity/empty/true", "[capacity]")
{
    flat_map<int(float)> m;

    CHECK(m.empty());
}

TEST_CASE("capacity/empty/false", "[capacity]")
{
    flat_map<int(float)> m{{1, 1.f}};

    CHECK(!m.empty());
}

TEST_CASE("capacity/size", "[capacity]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    CHECK(m.size() == 2);
}

TEST_CASE("capacity/max_size", "[capacity]")
{
    flat_map<int(float)> m;

    CHECK(m.max_size() > 0);
}

TEST_CASE("capacity/reserve", "[capacity]")
{
    flat_map<int(float)> m;

    m.reserve(100);

    CHECK(m.empty());
    CHECK(m.keys().capacity() >= 100);
    CHECK(m.values<0>().capacity() >= 100);
}

TEST_CASE("capacity/shrink_to_fit", "[capacity]")
{
    flat_map<int(float)> m;

    m.reserve(100);
    m.shrink_to_fit();

    CHECK(m.empty());
}
