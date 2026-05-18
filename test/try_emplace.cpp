// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("try_emplace/new", "[try_emplace]")
{
    flat_map<int(float, std::string)> m;

    auto [it, ok] = m.try_emplace(1, 1.f, "a");

    CHECK(ok);
    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
    CHECK(it->value<1>() == "a");
}

TEST_CASE("try_emplace/existing/no_overwrite", "[try_emplace]")
{
    flat_map<int(float)> m{flat_map<int(float)>::value_type{1, 1.f}};

    auto [it, ok] = m.try_emplace(1, 9.f);

    CHECK(!ok);
    CHECK(it->value<0>() == 1.f);
}

TEST_CASE("try_emplace/existing/size_unchanged", "[try_emplace]")
{
    flat_map<int(float)> m{flat_map<int(float)>::value_type{1, 1.f}};

    m.try_emplace(1, 9.f);

    CHECK(m.size() == 1);
}

TEST_CASE("try_emplace/hint/iterator", "[try_emplace]")
{
    flat_map<int(float)> m;

    flat_map<int(float)>::iterator hint = m.end();
    auto it = m.try_emplace(hint, 1, 1.f);

    CHECK(it->first == 1);
}

TEST_CASE("try_emplace/hint/const_iterator", "[try_emplace]")
{
    flat_map<int(float)> m;

    flat_map<int(float)>::const_iterator hint = m.cend();
    auto it = m.try_emplace(hint, 1, 1.f);

    CHECK(it->first == 1);
}
