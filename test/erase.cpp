// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("erase/by_iterator", "[erase]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    auto it = m.erase(m.begin());

    CHECK(m.size() == 1);
    CHECK(it->first == 2);
}

TEST_CASE("erase/by_const_iterator", "[erase]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    m.erase(m.cbegin());

    CHECK(m.size() == 1);
    CHECK(m.begin()->first == 2);
}

TEST_CASE("erase/by_range", "[erase]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}, {3, 3.f}};

    m.erase(m.begin(), std::next(m.begin(), 2));

    CHECK(m.size() == 1);
    CHECK(m.begin()->first == 3);
}

TEST_CASE("erase/by_key/found", "[erase]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto n = m.erase(1);

    CHECK(n == 1);
    CHECK(m.empty());
}

TEST_CASE("erase/by_key/not_found", "[erase]")
{
    flat_map<int(float)> m;

    auto n = m.erase(1);

    CHECK(n == 0);
}

TEST_CASE("erase/by_key/heterogeneous", "[erase]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto n = m.erase(1L);

    CHECK(n == 1);
    CHECK(m.empty());
}

TEST_CASE("erase/clear", "[erase]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    m.clear();

    CHECK(m.empty());
}

TEST_CASE("erase/columns_in_sync", "[erase]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}, {2, 2.f, "b"}};

    m.erase(1);

    CHECK(m.keys().size() == m.values<0>().size());
    CHECK(m.keys().size() == m.values<1>().size());
    CHECK(m.size() == 1);
}
