// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("erase_if/erases_matching", "[erase_if]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}, {3, 3.f}};

    auto n = erase_if(m, [](const auto& e) { return e.first % 2 == 0; });

    CHECK(n == 1);
    CHECK(m.size() == 2);
    CHECK(!m.contains(2));
}

TEST_CASE("erase_if/erases_none", "[erase_if]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto n = erase_if(m, [](const auto&) { return false; });

    CHECK(n == 0);
    CHECK(m.size() == 1);
}

TEST_CASE("erase_if/erases_all", "[erase_if]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    auto n = erase_if(m, [](const auto&) { return true; });

    CHECK(n == 2);
    CHECK(m.empty());
}

TEST_CASE("erase_if/predicate_sees_values", "[erase_if]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}, {3, 3.f}};

    erase_if(m, [](const auto& e) { return e.template value<0>() > 1.5f; });

    CHECK(m.size() == 1);
    CHECK(m.contains(1));
}

TEST_CASE("erase_if/columns_in_sync", "[erase_if]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}, {2, 2.f, "b"}};

    erase_if(m, [](const auto& e) { return e.first == 1; });

    CHECK(m.keys().size() == m.values<0>().size());
    CHECK(m.keys().size() == m.values<1>().size());
}
