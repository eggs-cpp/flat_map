// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("columns/keys/size", "[columns]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    CHECK(m.keys().size() == 2);
}

TEST_CASE("columns/keys/sorted", "[columns]")
{
    flat_map<int(float)> m{{2, 2.f}, {1, 1.f}};

    CHECK(m.keys()[0] == 1);
    CHECK(m.keys()[1] == 2);
}

TEST_CASE("columns/values/read", "[columns]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    CHECK(m.values<0>()[0] == 1.f);
    CHECK(m.values<0>()[1] == 2.f);
}

TEST_CASE("columns/values/write", "[columns]")
{
    flat_map<int(float)> m{{1, 1.f}};

    m.values<0>()[0] = 9.f;

    CHECK(m.values<0>()[0] == 9.f);
}

TEST_CASE("columns/values/const", "[columns]")
{
    const flat_map<int(float)> m{{1, 1.f}};

    CHECK(m.values<0>()[0] == 1.f);
}

TEST_CASE("columns/values/iterate", "[columns]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    float sum = 0.f;
    for (float v : m.values<0>()) sum += v;

    CHECK(sum == 3.f);
}

TEST_CASE("columns/values/multiple_columns", "[columns]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}, {2, 2.f, "b"}};

    CHECK(m.values<0>().size() == 2);
    CHECK(m.values<1>().size() == 2);
    CHECK(m.values<0>()[0] == 1.f);
    CHECK(m.values<1>()[0] == "a");
}
