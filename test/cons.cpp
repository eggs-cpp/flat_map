// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("cons/default", "[cons]")
{
    flat_map<int(float, std::string)> m;

    CHECK(m.empty());
}

TEST_CASE("cons/compare", "[cons]")
{
    flat_map<int(float), std::greater<int>> m;

    CHECK(m.empty());
}

TEST_CASE("cons/compare/ordering", "[cons]")
{
    flat_map<int(float), std::greater<int>> m{{1, 1.f}, {2, 2.f}};

    CHECK(m.begin()->first == 2);
}

TEST_CASE("cons/range", "[cons]")
{
    std::vector<flat_map<int(float)>::value_type> v{{1, 1.f}, {2, 2.f}};

    flat_map<int(float)> m(v.begin(), v.end());

    CHECK(m.size() == 2);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
}

TEST_CASE("cons/range/duplicate_keys", "[cons]")
{
    std::vector<flat_map<int(float)>::value_type> v{{1, 1.f}, {1, 2.f}};

    flat_map<int(float)> m(v.begin(), v.end());

    CHECK(m.size() == 1);
}

TEST_CASE("cons/sorted_unique/range", "[cons]")
{
    std::vector<flat_map<int(float)>::value_type> v{{1, 1.f}, {2, 2.f}};

    flat_map<int(float)> m(sorted_unique, v.begin(), v.end());

    CHECK(m.size() == 2);
}

TEST_CASE("cons/initializer_list", "[cons]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}, {2, 2.f, "b"}};

    CHECK(m.size() == 2);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
}

TEST_CASE("cons/sorted_unique/ilist", "[cons]")
{
    flat_map<int(float)> m(sorted_unique, {{1, 1.f}, {2, 2.f}});

    CHECK(m.size() == 2);
}

TEST_CASE("cons/copy", "[cons]")
{
    flat_map<int(float)> a{{1, 1.f}};

    flat_map<int(float)> b(a);

    CHECK(b.size() == 1);
    CHECK(b.begin()->first == a.begin()->first);
    CHECK(b.begin()->value<0>() == a.begin()->value<0>());
}

TEST_CASE("cons/move", "[cons]")
{
    flat_map<int(float)> a{{1, 1.f}};

    flat_map<int(float)> b(std::move(a));

    CHECK(b.size() == 1);
    CHECK(a.empty());
}
