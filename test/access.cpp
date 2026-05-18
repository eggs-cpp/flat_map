// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("access/at/found", "[access]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    auto [f, s] = m.at(1);

    CHECK(f == 1.f);
    CHECK(s == "a");
}

TEST_CASE("access/at/not_found", "[access]")
{
    flat_map<int(float)> m;

    CHECK_THROWS_AS(m.at(1), std::out_of_range);
}

TEST_CASE("access/at/const", "[access]")
{
    const flat_map<int(float)> m{{1, 1.f}};

    CHECK(std::get<0>(m.at(1)) == 1.f);
}

TEST_CASE("access/operator[]/existing", "[access]")
{
    flat_map<int(float)> m{{1, 1.f}};

    std::get<0>(m[1]) = 2.f;

    CHECK(std::get<0>(m.at(1)) == 2.f);
}

TEST_CASE("access/operator[]/insert", "[access]")
{
    flat_map<int(float)> m;

    m[1];

    CHECK(m.size() == 1);
}

TEST_CASE("access/operator[]/assign", "[access]")
{
    flat_map<int(float, std::string)> m;

    std::get<0>(m[1]) = 2.f;
    std::get<1>(m[1]) = "hi";

    CHECK(std::get<0>(m.at(1)) == 2.f);
    CHECK(std::get<1>(m.at(1)) == "hi");
}
