// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("observers/key_comp/less", "[observers]")
{
    flat_map<int(float)> m;

    CHECK(m.key_comp()(1, 2));
    CHECK(!m.key_comp()(2, 1));
    CHECK(!m.key_comp()(1, 1));
}

TEST_CASE("observers/key_comp/custom", "[observers]")
{
    flat_map<int(float), std::greater<int>> m;

    CHECK(m.key_comp()(2, 1));
    CHECK(!m.key_comp()(1, 2));
}

TEST_CASE("observers/value_comp", "[observers]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    CHECK(m.value_comp()(*m.begin(), *std::next(m.begin())));
    CHECK(!m.value_comp()(*std::next(m.begin()), *m.begin()));
}
