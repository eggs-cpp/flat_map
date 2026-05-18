// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("insert_or_assign/new", "[insert_or_assign]")
{
    flat_map<int(float)> m;

    auto [it, ok] = m.insert_or_assign(1, 1.f);

    CHECK(ok);
    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
}

TEST_CASE("insert_or_assign/existing/overwrites", "[insert_or_assign]")
{
    flat_map<int(float)> m{flat_map<int(float)>::value_type{1, 1.f}};

    auto [it, ok] = m.insert_or_assign(1, 9.f);

    CHECK(!ok);
    CHECK(it->value<0>() == 9.f);
}

TEST_CASE("insert_or_assign/existing/size_unchanged", "[insert_or_assign]")
{
    flat_map<int(float)> m{flat_map<int(float)>::value_type{1, 1.f}};

    m.insert_or_assign(1, 9.f);

    CHECK(m.size() == 1);
}

TEST_CASE("insert_or_assign/multiple_values", "[insert_or_assign]")
{
    using FM = flat_map<int(float, std::string)>;
    FM m{FM::value_type{1, 1.f, "a"}};

    auto [it, ok] = m.insert_or_assign(1, 9.f, std::string{"b"});

    CHECK(!ok);
    CHECK(it->value<0>() == 9.f);
    CHECK(it->value<1>() == "b");
}

TEST_CASE("insert_or_assign/hint/iterator", "[insert_or_assign]")
{
    flat_map<int(float)> m;

    flat_map<int(float)>::iterator hint = m.end();
    auto it = m.insert_or_assign(hint, 1, 1.f);

    CHECK(it->first == 1);
}

TEST_CASE("insert_or_assign/hint/const_iterator", "[insert_or_assign]")
{
    flat_map<int(float)> m;

    flat_map<int(float)>::const_iterator hint = m.cend();
    auto it = m.insert_or_assign(hint, 1, 1.f);

    CHECK(it->first == 1);
}
