// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("swap/member", "[swap]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b{{2, 2.f}};

    a.swap(b);

    CHECK(a.begin()->first == 2);
    CHECK(b.begin()->first == 1);
}

TEST_CASE("swap/non_member", "[swap]")
{
    flat_map<int(float)> a{{1, 1.f}};
    flat_map<int(float)> b{{2, 2.f}};

    swap(a, b);

    CHECK(a.begin()->first == 2);
    CHECK(b.begin()->first == 1);
}

TEST_CASE("swap/columns_in_sync", "[swap]")
{
    flat_map<int(float, std::string)> a{{1, 1.f, "a"}};
    flat_map<int(float, std::string)> b{{2, 2.f, "b"}, {3, 3.f, "c"}};

    a.swap(b);

    CHECK(a.size() == 2);
    CHECK(b.size() == 1);
    CHECK(a.keys().size() == a.values<0>().size());
    CHECK(a.keys().size() == a.values<1>().size());
}
