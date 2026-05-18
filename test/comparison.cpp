// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("comparison/eq/equal", "[comparison]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b{{1, 1.f}};

    CHECK(a == b);
}

TEST_CASE("comparison/eq/not_equal/key", "[comparison]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b{{2, 1.f}};

    CHECK(a != b);
}

TEST_CASE("comparison/eq/not_equal/value", "[comparison]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b{{1, 2.f}};

    CHECK(a != b);
}

TEST_CASE("comparison/eq/not_equal/size", "[comparison]")
{
    flat_map<int(float)> a{{1, 1.f}, {2, 2.f}};
    flat_map<int(float)> b{{1, 1.f}};

    CHECK(a != b);
}

TEST_CASE("comparison/spaceship/less", "[comparison]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b{{2, 2.f}};

    CHECK(std::is_lt(a <=> b));
}

TEST_CASE("comparison/spaceship/greater", "[comparison]")
{
    flat_map<int(float)> a{{2, 2.f}};
    flat_map<int(float)> b{{1, 1.f}};

    CHECK(std::is_gt(a <=> b));
}

TEST_CASE("comparison/spaceship/equal", "[comparison]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b{{1, 1.f}};

    CHECK(std::is_eq(a <=> b));
}

TEST_CASE("comparison/spaceship/key_only", "[comparison]")
{
    // map with no value types — degenerate Key() case
    flat_map<int()> a{{1}};
    flat_map<int()> b{{2}};

    CHECK(std::is_lt(a <=> b));
}
