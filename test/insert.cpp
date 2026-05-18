// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_map.hpp>

#include "catch.hpp"

using namespace eggs;

using FM = flat_map<int(float)>;
using FMS = flat_map<int(float, std::string)>;

TEST_CASE("insert/value_type/new", "[insert]")
{
    FM m;

    auto [it, ok] = m.insert(FM::value_type{1, 1.f});

    CHECK(ok);
    CHECK(it->first == 1);
    CHECK(it->value<0>() == 1.f);
}

TEST_CASE("insert/value_type/existing", "[insert]")
{
    FM m{FM::value_type{1, 1.f}};

    auto [it, ok] = m.insert(FM::value_type{1, 2.f});

    CHECK(!ok);
    CHECK(it->value<0>() == 1.f);
}

TEST_CASE("insert/value_type/move", "[insert]")
{
    FMS m;

    FMS::value_type v{1, 1.f, "a"};
    auto [it, ok] = m.insert(std::move(v));

    CHECK(ok);
    CHECK(it->first == 1);
}

TEST_CASE("insert/hint/iterator", "[insert]")
{
    FM m;

    FM::iterator hint = m.end();
    auto it = m.insert(hint, FM::value_type{1, 1.f});

    CHECK(it->first == 1);
}

TEST_CASE("insert/hint/const_iterator", "[insert]")
{
    FM m;

    FM::const_iterator hint = m.cend();
    auto it = m.insert(hint, FM::value_type{1, 1.f});

    CHECK(it->first == 1);
}

TEST_CASE("insert/range", "[insert]")
{
    std::vector<FM::value_type> v{
        FM::value_type{1, 1.f}, FM::value_type{2, 2.f}
    };
    FM m;

    m.insert(v.begin(), v.end());

    CHECK(m.size() == 2);
}

TEST_CASE("insert/sorted_unique/range", "[insert]")
{
    std::vector<FM::value_type> v{
        FM::value_type{1, 1.f}, FM::value_type{2, 2.f}
    };
    FM m;

    m.insert(sorted_unique, v.begin(), v.end());

    CHECK(m.size() == 2);
}

TEST_CASE("insert/initializer_list", "[insert]")
{
    FM m;

    m.insert({FM::value_type{1, 1.f}, FM::value_type{2, 2.f}});

    CHECK(m.size() == 2);
}

TEST_CASE("insert/sorted_unique/ilist", "[insert]")
{
    FM m;

    m.insert(sorted_unique, {FM::value_type{1, 1.f}, FM::value_type{2, 2.f}});

    CHECK(m.size() == 2);
}

TEST_CASE("insert/sorted_order", "[insert]")
{
    FM m;

    m.insert(FM::value_type{3, 3.f});
    m.insert(FM::value_type{1, 1.f});
    m.insert(FM::value_type{2, 2.f});

    CHECK(m.begin()->first == 1);
    CHECK(std::next(m.begin())->first == 2);
    CHECK(std::next(m.begin(), 2)->first == 3);
}
