// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("lookup/find/found", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto it = m.find(1);

    CHECK(it != m.end());
    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
}

TEST_CASE("lookup/find/not_found", "[lookup]")
{
    flat_map<int(float)> m;

    CHECK(m.find(1) == m.end());
}

TEST_CASE("lookup/find/const", "[lookup]")
{
    const flat_map<int(float)> m{{1, 1.f}};

    auto it = m.find(1);

    CHECK(it != m.end());
    CHECK(it->first == 1);
}

TEST_CASE("lookup/find/heterogeneous", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}};

    CHECK(m.find(1L) != m.end());
}

TEST_CASE("lookup/contains/true", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}};

    CHECK(m.contains(1));
}

TEST_CASE("lookup/contains/false", "[lookup]")
{
    flat_map<int(float)> m;

    CHECK(!m.contains(1));
}

TEST_CASE("lookup/contains/heterogeneous", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}};

    CHECK(m.contains(1L));
}

TEST_CASE("lookup/count/found", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}};

    CHECK(m.count(1) == 1);
}

TEST_CASE("lookup/count/not_found", "[lookup]")
{
    flat_map<int(float)> m;

    CHECK(m.count(1) == 0);
}

TEST_CASE("lookup/lower_bound/exact", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}, {3, 3.f}};

    CHECK(m.lower_bound(1)->first == 1);
}

TEST_CASE("lookup/lower_bound/between", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}, {3, 3.f}};

    CHECK(m.lower_bound(2)->first == 3);
}

TEST_CASE("lookup/upper_bound", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}, {3, 3.f}};

    CHECK(m.upper_bound(1)->first == 3);
}

TEST_CASE("lookup/equal_range/found", "[lookup]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto [lo, hi] = m.equal_range(1);

    CHECK(lo->first == 1);
    CHECK(hi == m.end());
}

TEST_CASE("lookup/equal_range/not_found", "[lookup]")
{
    flat_map<int(float)> m{{2, 2.f}};

    auto [lo, hi] = m.equal_range(1);

    CHECK(lo == hi);
}
