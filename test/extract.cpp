// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

TEST_CASE("extract/empties_source", "[extract]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto c = std::move(m).extract();

    CHECK(m.empty());
}

TEST_CASE("extract/keys_moved", "[extract]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    auto c = std::move(m).extract();

    CHECK(c.keys.size() == 2);
    CHECK(c.keys[0] == 1);
    CHECK(c.keys[1] == 2);
}

TEST_CASE("extract/values_moved", "[extract]")
{
    flat_map<int(float)> m{{1, 1.f}};

    auto c = std::move(m).extract();

    CHECK(c.values<0>().size() == 1);
    CHECK(c.values<0>()[0] == 1.f);
}

TEST_CASE("extract/replace_restores", "[extract]")
{
    flat_map<int(float)> m{{1, 1.f}, {2, 2.f}};

    auto c = std::move(m).extract();
    m.replace(std::move(c));

    CHECK(m.size() == 2);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
}

TEST_CASE("extract/structured_bindings", "[extract]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}};

    auto [ks, fs, ss] = std::move(m).extract();

    CHECK(ks.size() == 1);
    CHECK(fs.size() == 1);
    CHECK(ss.size() == 1);
    CHECK(ks[0] == 1);
    CHECK(fs[0] == 1.f);
    CHECK(ss[0] == "a");
}

TEST_CASE("extract/columns_in_sync", "[extract]")
{
    flat_map<int(float, std::string)> m{{1, 1.f, "a"}, {2, 2.f, "b"}};

    auto c = std::move(m).extract();

    CHECK(c.keys.size() == c.values<0>().size());
    CHECK(c.keys.size() == c.values<1>().size());
}
