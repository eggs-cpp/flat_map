// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("assign/copy", "[assign]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b;

    b = a;

    CHECK(b.size() == 1);
    CHECK(b.begin()->first == 1);
    CHECK(b.begin()->value<0>() == 1.f);
}

TEST_CASE("assign/move", "[assign]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b;

    b = std::move(a);

    CHECK(b.size() == 1);
    CHECK(b.begin()->first == 1);
    CHECK(a.empty());
}

TEST_CASE("assign/initializer_list", "[assign]")
{
    flat_map<int(float)> m;

    m = {{1, 1.f}, {2, 2.f}};

    CHECK(m.size() == 2);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
}
