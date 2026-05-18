// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("emplace/new", "[emplace]")
{
    flat_map<int(float, std::string)> m;

    auto [it, ok] = m.emplace(1, 1.f, "a");

    CHECK(ok);
    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
    CHECK(it->value<1>() == "a");
}

TEST_CASE("emplace/existing", "[emplace]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto [it, ok] = m.emplace(1, 2.f);

    CHECK(!ok);
    CHECK(it->value<0>() == 1.f);
}

TEST_CASE("emplace/sorted_order", "[emplace]")
{
    flat_map<int(float)> m;

    m.emplace(3, 3.f);
    m.emplace(1, 1.f);
    m.emplace(2, 2.f);

    CHECK(m.begin()->first == 1);
    CHECK(std::next(m.begin())->first == 2);
    CHECK(std::next(m.begin(), 2)->first == 3);
}

TEST_CASE("emplace/hint/iterator", "[emplace]")
{
    flat_map<int(float)> m;

    flat_map<int(float)>::iterator hint = m.end();
    auto it = m.emplace_hint(hint, 1, 1.f);

    CHECK(it->first == 1);
}

TEST_CASE("emplace/hint/const_iterator", "[emplace]")
{
    flat_map<int(float)> m;

    flat_map<int(float)>::const_iterator hint = m.cend();
    auto it = m.emplace_hint(hint, 1, 1.f);

    CHECK(it->first == 1);
}

TEST_CASE("emplace/piecewise/new", "[emplace]")
{
    flat_map<int(float, std::string)> m;

    auto [it, ok] = m.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(1),
        std::forward_as_tuple(1.f),
        std::forward_as_tuple("a")
    );

    CHECK(ok);
    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
    CHECK(it->value<1>() == "a");
}

TEST_CASE("emplace/piecewise/existing", "[emplace]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto [it, ok] = m.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(1),
        std::forward_as_tuple(2.f)
    );

    CHECK(!ok);
    CHECK(it->value<0>() == 1.f);
}
